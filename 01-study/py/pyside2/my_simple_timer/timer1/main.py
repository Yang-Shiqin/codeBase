#!/usr/bin/env python
# coding=utf-8

import sys
from PySide2.QtWidgets import QApplication, QDialog, QMainWindow, QPushButton, QHBoxLayout, QSizePolicy, QVBoxLayout, QWidget
from line import TimerLine

class Window(QMainWindow):
    """Main window."""
    def __init__(self, parent=None):
        """Initializer."""
        super().__init__(parent)
        self.hLayout = QHBoxLayout()
        self.vLayout = QVBoxLayout()
        self.create_button = QPushButton('Create timer')
        self.pause_button = QPushButton('Pause all')
        self.hLayout.addWidget(self.create_button)
        self.hLayout.addWidget(self.pause_button)
        self.vLayout.addLayout(self.hLayout)
        self.create_button.clicked.connect(self.create_timer)
        container = QWidget()
        container.setLayout(self.vLayout)
        self.setCentralWidget(container)
        self.timer_lines = []
        self.id = 0

    def create_timer(self):
        widget = TimerLine(self, self.id)
        self.id += 1
        self.vLayout.addWidget(widget)
        self.timer_lines.append(widget)
        self.pause_button.clicked.connect(lambda: widget.pause_time(-1))   # pause_button发送-1(携带额外数据用lambda就行)
        widget.pause.clicked.connect(lambda: self.pause(widget.id))    # pause_button发送对应id
        
    def pause(self, id):    # 模拟pause_button点击，但发送的是对应id
        for widget in self.timer_lines:
            widget.pause_time(id)

# Create the application
app = QApplication(sys.argv)
w = Window()
w.show()
app.exec_()
