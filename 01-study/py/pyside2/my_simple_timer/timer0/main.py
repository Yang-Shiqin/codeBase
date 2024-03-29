#!/usr/bin/env python
# coding=utf-8

import sys
from PySide2.QtWidgets import QApplication, QDialog, QMainWindow, QPushButton, QHBoxLayout, QSizePolicy
from line import Line

class Window(QMainWindow):
    """Main window."""
    def __init__(self, parent=None):
        """Initializer."""
        super().__init__(parent)
        widget = Line(self)
        self.setCentralWidget(widget)

    


# Create the application
app = QApplication(sys.argv)
# Create and show the application's main window
w = Window()
# w = Line()
w.show()
# Run the application's main loop
app.exec_()
