#!/usr/bin/env python
# coding=utf-8
import socket

def client():
    # 创建socket对象
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # 获取本地机器名
    host = socket.gethostname()
    
    # 设置端口号
    port = 9999
    
    # 连接服务，指定主机和端口
    client_socket.connect((host, port))
    
    # 发送数据
    message = "Hello, server!"
    client_socket.send(message.encode('utf-8'))
    
    # 接收小于 1024 字节的数据
    data = client_socket.recv(1024)
    
    print("从服务器接收的消息：'%s'" % data.decode('utf-8'))
    
    client_socket.close()

if __name__ == '__main__':
    client()

