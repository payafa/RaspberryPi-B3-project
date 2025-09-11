# socket网络编程
import socket
# 树莓派导入包
import RPi.GPIO as GPIO
# 导入c库并使用
import ctypes
# 使用time.sleep
import time
# 使用cv2摄像头
import cv2
import struct
# 使用多线程
import threading

# 加载共享库
controlLib = ctypes.CDLL('./lib/control.so')
distanceLib = ctypes.CDLL('./lib/distance.so')

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server_socket.bind(('0.0.0.0', 25500))
server_socket.listen(1)
print("树莓派TCP服务器正在所有网口的25500端口监听中")

# 等待客户端连接
client_socket, client_address = server_socket.accept()
print(f"已连接IP ： {client_address}")

controlLib.init()
distanceLib.init()

# 全局变量
running = True
current_mode = "handDriving"  # 默认手动驾驶模式

def selfDrivingThread():
    """自动驾驶线程函数"""
    global running
    while running and current_mode == "selfDriving":
        try:
            dist = distanceLib.readDist()
            if dist <= 30:
                # 1. 先停止
                controlLib.stop()
                time.sleep(0.2)

                # 2. 后退一小段距离，增加安全空间
                controlLib.backward(100)
                time.sleep(0.5)
                controlLib.stop()
                time.sleep(0.2)

                # 3. 原地左转一个足够大的角度
                controlLib.spinleft(50)
                time.sleep(1.0)
                controlLib.stop()
                time.sleep(0.2)

                # 4. 转弯完成后，重新开始探测
                continue 

            elif dist > 30: 
                controlLib.forward(30)

            time.sleep(0.5)
        except Exception as e:
            print(f"自动驾驶线程错误: {e}")
            break
    print("自动驾驶线程结束")

def captureThread():
    """摄像头线程函数"""
    global running
    cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
    if not cap.isOpened():
        print("无法打开摄像头")
        return
        
    while running:
        try:
            ret, frame = cap.read()
            if not ret:
                break

            _, buffer = cv2.imencode('.jpg', frame)
            photo = buffer.tobytes()

            size = len(photo)
            client_socket.sendall(struct.pack("!I", size))
            client_socket.sendall(photo)

            time.sleep(0.1)  # 减少延迟以提高帧率
        except Exception as e:
            print(f"摄像头线程错误: {e}")
            break
            
    cap.release()
    print("摄像头线程结束")

# 创建并启动摄像头线程
t_camera = threading.Thread(target=captureThread)
t_camera.daemon = True
t_camera.start()

# 自动驾驶线程（初始不启动）
t_auto = None

try:
    while running:
        try:
            # 接收命令
            data = client_socket.recv(1024).decode('utf-8').strip()
            if not data:
                print("连接已关闭")
                break
                
            print(f"收到命令: {data}")

            # 处理模式切换命令
            if data == "handDriving":
                if current_mode != "handDriving":
                    print("切换到手动模式")
                    current_mode = "handDriving"
                    # 停止自动驾驶线程
                    if t_auto and t_auto.is_alive():
                        running = False
                        time.sleep(0.1)
                        running = True
                        t_auto = None
                    controlLib.stop()
                    
            elif data == "selfDriving":
                if current_mode != "selfDriving":
                    print("切换到自动模式")
                    current_mode = "selfDriving"
                    controlLib.stop()
                    # 启动自动驾驶线程
                    if not t_auto or not t_auto.is_alive():
                        t_auto = threading.Thread(target=selfDrivingThread)
                        t_auto.daemon = True
                        t_auto.start()

            # 处理方向命令（仅在手动模式下有效）
            if current_mode == "handDriving":
                if data == "forward":
                    controlLib.forward(100)
                elif data == "backward":
                    controlLib.backward(100)
                elif data == "spinleft":
                    controlLib.spinleft(50)
                elif data == "spinright":
                    controlLib.spinright(50)
                elif data == "stop":
                    controlLib.stop()
                    
        except Exception as e:
            print(f"主循环错误: {e}")
            break

except KeyboardInterrupt:
    print("程序被用户中断")

finally:
    print("正在清理资源...")
    running = False
    
    # 等待线程结束
    if t_auto and t_auto.is_alive():
        t_auto.join(timeout=1.0)
        
    if t_camera.is_alive():
        t_camera.join(timeout=1.0)
    
    # 关闭socket连接
    try:
        client_socket.close()
    except:
        pass
        
    try:
        server_socket.close()
    except:
        pass

    controlLib.stop()
    GPIO.cleanup()

    print("资源清理完成")