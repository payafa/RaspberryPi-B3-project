#!/bin/bash

# 树莓派B3项目Web界面测试脚本
# 作者: GitHub Copilot

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[TEST]${NC} $1"; }
print_success() { echo -e "${GREEN}[PASS]${NC} $1"; }
print_error() { echo -e "${RED}[FAIL]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARN]${NC} $1"; }

# 配置
API_BASE="http://localhost:8080"
TIMEOUT=10

echo "========================================"
echo "  树莓派B3 Web界面功能测试"
echo "========================================"
echo ""

# 检查服务器是否运行
check_server() {
    print_status "检查Web服务器状态..."
    
    if curl -s --connect-timeout $TIMEOUT "$API_BASE/api/status" > /dev/null; then
        print_success "Web服务器正在运行"
        return 0
    else
        print_error "Web服务器未运行或无法连接"
        echo "请先启动服务器: sudo ./web_main"
        return 1
    fi
}

# 测试API接口
test_api_status() {
    print_status "测试系统状态API..."
    
    local response=$(curl -s --connect-timeout $TIMEOUT "$API_BASE/api/status")
    if echo "$response" | grep -q "online"; then
        print_success "系统状态API正常"
        echo "响应: $response"
    else
        print_error "系统状态API异常"
        echo "响应: $response"
        return 1
    fi
}

test_api_sensors() {
    print_status "测试传感器数据API..."
    
    local response=$(curl -s --connect-timeout $TIMEOUT "$API_BASE/api/sensors")
    if echo "$response" | grep -q "sensors"; then
        print_success "传感器数据API正常"
        echo "温湿度数据: $(echo "$response" | grep -o '"dht11":[^}]*}')"
        echo "距离数据: $(echo "$response" | grep -o '"ultrasonic":[^}]*}')"
    else
        print_error "传感器数据API异常"
        echo "响应: $response"
        return 1
    fi
}

test_api_rgb() {
    print_status "测试RGB LED控制API..."
    
    # 测试开启红色
    local response=$(curl -s --connect-timeout $TIMEOUT \
        -X POST -H "Content-Type: application/json" \
        -d '{"action":"on","color":"red"}' \
        "$API_BASE/api/rgb")
    
    if echo "$response" | grep -q "success"; then
        print_success "RGB LED控制API正常"
        echo "红色LED已开启"
        
        sleep 2
        
        # 测试关闭
        curl -s --connect-timeout $TIMEOUT \
            -X POST -H "Content-Type: application/json" \
            -d '{"action":"off"}' \
            "$API_BASE/api/rgb" > /dev/null
        
        print_success "RGB LED已关闭"
    else
        print_error "RGB LED控制API异常"
        echo "响应: $response"
        return 1
    fi
}

test_api_beep() {
    print_status "测试蜂鸣器控制API..."
    
    local response=$(curl -s --connect-timeout $TIMEOUT \
        -X POST -H "Content-Type: application/json" \
        -d '{"action":"beep","duration":500}' \
        "$API_BASE/api/beep")
    
    if echo "$response" | grep -q "success"; then
        print_success "蜂鸣器控制API正常"
        echo "蜂鸣器已响0.5秒"
    else
        print_error "蜂鸣器控制API异常"
        echo "响应: $response"
        return 1
    fi
}

test_api_servo() {
    print_status "测试舵机控制API..."
    
    # 测试设置角度
    local response=$(curl -s --connect-timeout $TIMEOUT \
        -X POST -H "Content-Type: application/json" \
        -d '{"angle":45}' \
        "$API_BASE/api/servo")
    
    if echo "$response" | grep -q "success"; then
        print_success "舵机控制API正常"
        echo "舵机角度已设置为45°"
        
        sleep 2
        
        # 回到中心位置
        curl -s --connect-timeout $TIMEOUT \
            -X POST -H "Content-Type: application/json" \
            -d '{"angle":90}' \
            "$API_BASE/api/servo" > /dev/null
        
        print_success "舵机已回到中心位置"
    else
        print_error "舵机控制API异常"
        echo "响应: $response"
        return 1
    fi
}

test_api_distance() {
    print_status "测试距离传感器API..."
    
    local response=$(curl -s --connect-timeout $TIMEOUT "$API_BASE/api/distance")
    if echo "$response" | grep -q "distance"; then
        print_success "距离传感器API正常"
        echo "距离数据: $(echo "$response" | grep -o '"distance":[0-9]*')"
    else
        print_error "距离传感器API异常"
        echo "响应: $response"
        return 1
    fi
}

test_web_interface() {
    print_status "测试Web界面..."
    
    # 测试主页
    if curl -s --connect-timeout $TIMEOUT "$API_BASE/" | grep -q "树莓派B3硬件控制系统"; then
        print_success "主页面加载正常"
    else
        print_error "主页面加载失败"
        return 1
    fi
    
    # 测试静态资源
    if curl -s --connect-timeout $TIMEOUT "$API_BASE/style.css" | grep -q "body"; then
        print_success "CSS样式文件加载正常"
    else
        print_warning "CSS样式文件可能有问题"
    fi
    
    if curl -s --connect-timeout $TIMEOUT "$API_BASE/script.js" | grep -q "API_BASE_URL"; then
        print_success "JavaScript文件加载正常"
    else
        print_warning "JavaScript文件可能有问题"
    fi
}

# 性能测试
test_performance() {
    print_status "进行性能测试..."
    
    # 并发请求测试
    print_status "测试并发请求处理..."
    for i in {1..5}; do
        curl -s --connect-timeout 5 "$API_BASE/api/status" > /dev/null &
    done
    wait
    
    print_success "并发请求测试完成"
    
    # 响应时间测试
    print_status "测试API响应时间..."
    local start_time=$(date +%s%N)
    curl -s --connect-timeout $TIMEOUT "$API_BASE/api/status" > /dev/null
    local end_time=$(date +%s%N)
    local duration=$((($end_time - $start_time) / 1000000))
    
    if [ $duration -lt 1000 ]; then
        print_success "API响应时间: ${duration}ms (优秀)"
    elif [ $duration -lt 3000 ]; then
        print_success "API响应时间: ${duration}ms (良好)"
    else
        print_warning "API响应时间: ${duration}ms (较慢)"
    fi
}

# 安全测试
test_security() {
    print_status "进行基础安全测试..."
    
    # 测试无效请求
    local response=$(curl -s -w "%{http_code}" \
        --connect-timeout $TIMEOUT \
        "$API_BASE/api/nonexistent" \
        -o /dev/null)
    
    if [ "$response" = "404" ]; then
        print_success "404错误处理正常"
    else
        print_warning "错误处理可能有问题"
    fi
    
    # 测试无效JSON
    local response=$(curl -s --connect-timeout $TIMEOUT \
        -X POST -H "Content-Type: application/json" \
        -d '{"invalid json"}' \
        "$API_BASE/api/rgb")
    
    if echo "$response" | grep -q "error"; then
        print_success "无效JSON处理正常"
    else
        print_warning "JSON验证可能需要改进"
    fi
}

# 主测试流程
main() {
    local tests_passed=0
    local tests_total=0
    
    # 基础连接测试
    if check_server; then
        ((tests_passed++))
    fi
    ((tests_total++))
    
    # API功能测试
    for test_func in test_api_status test_api_sensors test_api_rgb test_api_beep test_api_servo test_api_distance; do
        if $test_func; then
            ((tests_passed++))
        fi
        ((tests_total++))
        sleep 1
    done
    
    # Web界面测试
    if test_web_interface; then
        ((tests_passed++))
    fi
    ((tests_total++))
    
    # 性能测试
    test_performance
    
    # 安全测试
    test_security
    
    # 测试结果总结
    echo ""
    echo "========================================"
    echo "  测试结果总结"
    echo "========================================"
    echo ""
    
    if [ $tests_passed -eq $tests_total ]; then
        print_success "所有核心测试通过! ($tests_passed/$tests_total)"
        echo ""
        echo "🎉 恭喜！您的树莓派B3 Web控制系统运行正常"
        echo ""
        echo "📱 访问地址:"
        echo "   本地: http://localhost:8080"
        
        # 显示局域网IP
        local ip=$(hostname -I | awk '{print $1}')
        if [[ -n "$ip" ]]; then
            echo "   局域网: http://$ip:8080"
        fi
        
        echo ""
        echo "🔗 API端点:"
        echo "   http://localhost:8080/api/"
        echo ""
        echo "📖 演示页面:"
        echo "   http://localhost:8080/demo.html"
        
    else
        print_error "部分测试失败 ($tests_passed/$tests_total)"
        echo ""
        echo "❌ 请检查以下问题:"
        echo "   1. 确保硬件连接正确"
        echo "   2. 确保WiringPi库已安装"
        echo "   3. 确保以root权限运行服务器"
        echo "   4. 检查GPIO权限设置"
    fi
    
    echo ""
    echo "========================================"
}

# 运行测试
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
