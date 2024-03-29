#!/usr/bin/env python
# coding=utf-8

import sys
from PySide2.QtWidgets import QApplication, QMainWindow
from line import TimerLine

class Window(QMainWindow):
    """Main window."""
    def __init__(self, parent=None):
        """Initializer."""
        super().__init__(parent)
        widget = TimerLine(self)
        self.setCentralWidget(widget)

app = QApplication(sys.argv)
w = Window()
w.show()
app.exec_()
