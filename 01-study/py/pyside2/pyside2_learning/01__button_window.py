#!/usr/bin/env python
# coding=utf-8
import sys
from PySide2.QtWidgets import QApplication, QPushButton

app = QApplication(sys.argv)

window = QPushButton("Push Me")
window.show()

app.exec_()
