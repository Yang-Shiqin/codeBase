#!/usr/bin/env python
# coding=utf-8
import socket

def server():
    # 创建socket对象
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # 获取本地机器名
    host = socket.gethostname()
    
    # 设置一个端口
    port = 9999
    
    # 绑定端口号
    server_socket.bind((host, port))
    
    # 设置最大连接数，超过后排队
    server_socket.listen(5)
    
    print("服务器启动，等待连接...")
    
    # 建立客户端连接
    client_socket, addr = server_socket.accept()
    
    print("连接地址: %s" % str(addr))
    
    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        print("收到消息：'%s'" % data.decode('utf-8'))
        client_socket.send(data)
    
    client_socket.close()

if __name__ == '__main__':
    server()

