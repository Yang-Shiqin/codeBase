#!/usr/bin/env python
# coding=utf-8
import sys
from PySide2.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QVBoxLayout

class MyWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("PySide2 示例")
        self.setGeometry(100, 100, 300, 200)

        self.label = QLabel("欢迎使用 PySide2", self)
        self.button = QPushButton("点击我", self)
        self.button.clicked.connect(self.display_message)

        layout = QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(self.button)

        self.setLayout(layout)

    def display_message(self):
        self.label.setText("你点击了按钮！")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MyWindow()
    window.show()
    sys.exit(app.exec_())

