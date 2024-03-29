#!/usr/bin/env python
# coding=utf-8

import sys
import resources

from PySide2 import QtGui, QtWidgets

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Hello World")
        self.button = QtWidgets.QPushButton("My button")

        self.icon_choose = 0
        self.icon = [
            ":/icons/penguin.png",
            ":/icons/monkey.png",
        ]
        icon = QtGui.QIcon(self.icon[self.icon_choose])
        self.button.setIcon(icon)
        self.button.clicked.connect(self.change_icon)

        self.setCentralWidget(self.button)

        self.show()

    def change_icon(self):
        self.icon_choose = self.icon_choose^1
        icon = QtGui.QIcon(self.icon[self.icon_choose])
        self.button.setIcon(icon)



app = QtWidgets.QApplication(sys.argv)
w = MainWindow()
app.exec_()
