import socket
import RPi.GPIO as GPIO
import ctypes
import platform
import time

#加载共享库
controlLib = ctypes.CDLL('./lib/control.so')

# 创建 TCP Socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 绑定到所有网络接口端口
server_socket.bind(('0.0.0.0', 25500))

# 开始监听，最多允许一个客户端连接
server_socket.listen(1)
print("树莓派TCP服务器正在所有网口的25500端口监听中")

try:
    # 等待客户端连接
    client_socket, client_address = server_socket.accept()
    print(f"已连接IP ： {client_address}")

    controlLib.init()

    while True:
        # 接收客户端发来的数据
        data = client_socket.recv(1024).decode('utf-8').strip()
        if not data:
            print("停止接受数据")
            break
        print(f"收到命令 ： {data}")

        # # 解析并执行命令
        if data == "forward":
            controlLib.forward()
        elif data == "back":
            controlLib.back()
        elif data == "stop":
            controlLib.stop()
        # elif data == "":
        #     response = ""
        #     client_socket.send(response.encode('utf-8'))
        # else:
        #     response = f"未知命令 ： {data}"

finally:
    print("正在清理GPIO端口")
    GPIO.cleanup()
    server_socket.close()
