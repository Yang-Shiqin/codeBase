# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'line.ui'
##
## Created by: Qt User Interface Compiler version 5.15.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *

from stdb import STDB

# 一行计时器组件
class TimerLine(QWidget):
    def __init__(self, parent=None, id=0):
        super().__init__(parent)
        self.db = STDB()
        self.db_data = {}
        self.id = id    # 任务id
        self.time = QTime(0,0,0)
        self.end_time = QTime(0,0,0)
        self.state = 0  # 0: 暂停, 1: 启动
        self.pause_icon = [u'▶️', u'⏸️']
        self.timer_ = QTimer()
        self.timer_.timeout.connect(self.show_time)
        self.timer_.start(1000)     # 每隔1秒触发一次

        self.horizontalLayout = QHBoxLayout(self)   # 加self才会显示出来
        self.horizontalLayout.setObjectName(u"horizontalLayout")

        # 用户输入job name框
        self.job_name = QLineEdit()
        self.job_name.setObjectName(u"job_name")
        self.horizontalLayout.addWidget(self.job_name)
        self.job_name.returnPressed.connect(self.on_return_pressed)

        # 显示时间
        self.timer = QLabel()
        self.timer.setObjectName(u"timer")
        self.horizontalLayout.addWidget(self.timer)

        # 完成按钮
        self.finish = QPushButton()                     # 定义组件
        self.finish.setObjectName(u"finish")            # 设置组件名称
        self.finish.setFixedWidth(35)
        self.horizontalLayout.addWidget(self.finish)    # 添加组件到布局
        self.finish.clicked.connect(self.clear_time)    # 连接信号和槽

        # 暂停/启动按钮
        self.pause = QPushButton()
        self.pause.setObjectName(u"pause")
        self.pause.setFixedWidth(35)
        self.horizontalLayout.addWidget(self.pause)
        self.pause.clicked.connect(self.pause_resume_time)    # 连接信号和槽
    
        # 删除按钮
        self.delete = QPushButton()
        self.delete.setObjectName(u"delete")
        self.delete.setFixedWidth(35)
        self.horizontalLayout.addWidget(self.delete)
        self.horizontalLayout.setMargin(0)
        self.delete.clicked.connect(self.delete_all)    # 连接信号和槽

        self.timer.setText(u"00:00:00")
        self.finish.setText(u"✅")
        self.pause.setText(u"▶️")
        self.delete.setText(u"🗑️")


    # 完成，时间归零
    def clear_time(self):
        self.pause_time(-1)
        self.db_data['duration'] = QTime(0,0,0).msecsTo(self.time)
        self.db.insert(self.db_data)
        self.time = QTime(0,0,0)
        self.timer.setText(self.time.toString('hh:mm:ss'))

    # 暂停/启动计时
    def pause_resume_time(self):
        self.on_return_pressed()
        self.state = self.state^1
        if self.state == 0: # 暂停
            self.time = self.time.addMSecs(self.end_time.msecsTo(QDateTime.currentDateTime()))
        else:   # 启动
            self.end_time = QDateTime.currentDateTime()
        self.pause.setText(self.pause_icon[self.state])

    # 总按钮暂停计时
    def pause_time(self, id):
        if id == self.id:   # id相等则不暂停
            return
        if self.state == 1:
            self.state = 0
            self.time = self.time.addMSecs(self.end_time.msecsTo(QDateTime.currentDateTime()))
            self.pause.setText(self.pause_icon[self.state])

    def show_time(self):
        if self.state == 1:
            self.timer.setText(self.time.addMSecs(
                self.end_time.msecsTo(QDateTime.currentDateTime())).toString('hh:mm:ss'))

    def on_return_pressed(self):
        self.job_name.setEnabled(False)
        self.db_data['name'] = self.job_name.text()
        self.db_data['create_time'] = QDateTime.currentDateTime().toString('yyyy-MM-dd hh:mm:ss')

    def delete_all(self):
        self.timer_.stop()
        self.deleteLater()