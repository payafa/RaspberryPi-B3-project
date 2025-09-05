#!/bin/bash

# 树莓派B3项目Web界面部署脚本
# 作者: GitHub Copilot
# 日期: 2025-09-05

set -e  # 遇到错误立即退出

echo "========================================"
echo "  树莓派B3硬件控制系统 - Web界面部署"
echo "========================================"
echo ""

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 打印彩色消息
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 检查是否在树莓派上运行
check_platform() {
    print_status "检查运行平台..."
    
    if [[ $(uname -m) == "arm"* ]] || [[ $(uname -m) == "aarch64" ]]; then
        print_success "检测到ARM架构，继续部署"
    else
        print_warning "当前不在ARM架构上，某些硬件功能可能不可用"
        read -p "是否继续? (y/n): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    fi
}

# 检查权限
check_permissions() {
    print_status "检查运行权限..."
    
    if [[ $EUID -eq 0 ]]; then
        print_warning "建议使用普通用户运行此脚本"
    fi
    
    # 检查sudo权限
    if ! sudo -v > /dev/null 2>&1; then
        print_error "需要sudo权限来安装依赖"
        exit 1
    fi
    
    print_success "权限检查通过"
}

# 更新系统
update_system() {
    print_status "更新系统包..."
    
    sudo apt update
    if [[ $? -eq 0 ]]; then
        print_success "系统包列表更新完成"
    else
        print_error "系统包更新失败"
        exit 1
    fi
}

# 安装依赖
install_dependencies() {
    print_status "安装项目依赖..."
    
    # 基础编译工具
    print_status "安装编译工具..."
    sudo apt install -y build-essential
    
    # WiringPi库
    print_status "安装WiringPi库..."
    if ! dpkg -l | grep -q wiringpi; then
        sudo apt install -y wiringpi
    else
        print_success "WiringPi已安装"
    fi
    
    # cJSON库
    print_status "安装cJSON库..."
    sudo apt install -y libcjson-dev
    
    # 其他可能需要的库
    sudo apt install -y libpthread-stubs0-dev
    
    print_success "所有依赖安装完成"
}

# 验证依赖
verify_dependencies() {
    print_status "验证依赖安装..."
    
    # 检查gcc
    if command -v gcc > /dev/null 2>&1; then
        print_success "GCC: $(gcc --version | head -n1)"
    else
        print_error "GCC未安装"
        exit 1
    fi
    
    # 检查WiringPi
    if command -v gpio > /dev/null 2>&1; then
        print_success "WiringPi: $(gpio -v | head -n1)"
    else
        print_error "WiringPi未安装或不可用"
        exit 1
    fi
    
    # 检查cJSON
    if pkg-config --exists libcjson; then
        print_success "cJSON库验证通过"
    else
        print_warning "cJSON库可能未正确安装"
    fi
}

# 编译项目
compile_project() {
    print_status "编译项目..."
    
    # 清理之前的编译
    if [[ -f "Makefile.web" ]]; then
        print_status "清理之前的编译文件..."
        make -f Makefile.web clean > /dev/null 2>&1 || true
    fi
    
    # 编译Web版本
    print_status "编译Web服务器版本..."
    if make -f Makefile.web web-only; then
        print_success "Web版本编译成功"
    else
        print_error "Web版本编译失败"
        exit 1
    fi
    
    # 编译命令行版本
    print_status "编译命令行版本..."
    if make -f Makefile.web main-only; then
        print_success "命令行版本编译成功"
    else
        print_warning "命令行版本编译失败，但Web版本可用"
    fi
}

# 设置服务
setup_service() {
    print_status "设置系统服务..."
    
    # 创建服务文件
    sudo tee /etc/systemd/system/raspberry-web.service > /dev/null << EOF
[Unit]
Description=Raspberry Pi B3 Web Controller
After=network.target

[Service]
Type=simple
User=pi
WorkingDirectory=$(pwd)
ExecStart=$(pwd)/web_main
Restart=always
RestartSec=3

[Install]
WantedBy=multi-user.target
EOF

    # 重新加载systemd
    sudo systemctl daemon-reload
    
    print_success "系统服务设置完成"
    print_status "可以使用以下命令管理服务:"
    echo "  启动服务: sudo systemctl start raspberry-web"
    echo "  停止服务: sudo systemctl stop raspberry-web"
    echo "  开机启动: sudo systemctl enable raspberry-web"
    echo "  查看状态: sudo systemctl status raspberry-web"
}

# 创建快捷启动脚本
create_shortcuts() {
    print_status "创建快捷启动脚本..."
    
    # Web服务器启动脚本
    cat > start_web.sh << 'EOF'
#!/bin/bash
echo "启动树莓派B3 Web控制服务器..."
echo "访问地址: http://localhost:8080"
echo "按Ctrl+C停止服务器"
echo ""
sudo ./web_main
EOF
    chmod +x start_web.sh
    
    # 命令行版本启动脚本
    if [[ -f "main" ]]; then
        cat > start_cli.sh << 'EOF'
#!/bin/bash
echo "启动树莓派B3命令行控制程序..."
echo ""
sudo ./main
EOF
        chmod +x start_cli.sh
    fi
    
    print_success "快捷启动脚本创建完成"
}

# 显示使用说明
show_usage() {
    print_success "部署完成！"
    echo ""
    echo "========================================"
    echo "  使用说明"
    echo "========================================"
    echo ""
    echo "🌐 Web界面控制:"
    echo "   1. 运行: ./start_web.sh 或 sudo ./web_main"
    echo "   2. 浏览器访问: http://localhost:8080"
    echo "   3. 手机访问: http://$(hostname -I | awk '{print $1}'):8080"
    echo ""
    
    if [[ -f "main" ]]; then
        echo "💻 命令行控制:"
        echo "   运行: ./start_cli.sh 或 sudo ./main"
        echo ""
    fi
    
    echo "🔧 系统服务:"
    echo "   启动: sudo systemctl start raspberry-web"
    echo "   开机自启: sudo systemctl enable raspberry-web"
    echo ""
    echo "📚 API文档:"
    echo "   http://localhost:8080/api/ (启动服务器后访问)"
    echo ""
    echo "🚀 快速测试:"
    echo "   curl http://localhost:8080/api/status"
    echo ""
    echo "========================================"
    
    # 检测IP地址
    local ip=$(hostname -I | awk '{print $1}')
    if [[ -n "$ip" ]]; then
        print_status "检测到IP地址: $ip"
        print_status "其他设备可通过 http://$ip:8080 访问"
    fi
}

# 主函数
main() {
    echo "开始部署流程..."
    echo ""
    
    check_platform
    check_permissions
    update_system
    install_dependencies
    verify_dependencies
    compile_project
    setup_service
    create_shortcuts
    
    echo ""
    show_usage
    
    echo ""
    print_success "部署脚本执行完成！"
    
    # 询问是否立即启动
    echo ""
    read -p "是否立即启动Web服务器? (y/n): " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        print_status "启动Web服务器..."
        ./start_web.sh
    fi
}

# 脚本入口
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
