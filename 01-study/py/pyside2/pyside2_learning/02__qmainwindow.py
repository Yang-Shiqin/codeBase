#!/usr/bin/env python
# coding=utf-8
import sys
from PySide2.QtWidgets import QApplication, QMainWindow

app = QApplication(sys.argv)

window = QMainWindow()
window.show()

# Start the event loop.
app.exec_()
