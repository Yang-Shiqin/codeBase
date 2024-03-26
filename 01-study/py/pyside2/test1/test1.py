#!/usr/bin/env python
# coding=utf-8
from PySide2.QtUiTools import QUiLoader   # 模块下只有一个类
from PySide2.QtCore import QFile, QObject
from PySide2.QtWidgets import QApplication, QMainWindow, QWidget, QPushButton, QLabel

class MyWindow():
    def __init__(self):
        # 封装文件对象
        qfile = QFile("a.ui")
        # 打开文件
        qfile.open(QFile.ReadOnly)  # 只读方式打开
        # 读取数据(字节串)
        # l = qfile.readLine()
        # print("读取的一行数据:", l)

        # 关闭文件
        qfile.close()

        # 加载所有的文件内容，返回父窗口对象
        # 子控件 成为父控件的属性
        self.win = QUiLoader().load(qfile)
        print("加载所有的文件:", self.win, type(self.win), self.win.inherits("QWidget"))

    def show(self):
        self.win.pushButton.clicked.connect(lambda :print("点击了清空......"))
        self.win.show()


if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)

    win = MyWindow()
    win.show()

    exit_code = app.exec_()
    print("退出的状态码:", exit_code)
    sys.exit(exit_code)
