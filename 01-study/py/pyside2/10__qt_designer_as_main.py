#!/usr/bin/env python
# coding=utf-8

import sys
from PySide2 import QtCore, QtGui, QtWidgets
from PySide2.QtUiTools import QUiLoader

loader = QUiLoader()
app = QtWidgets.QApplication(sys.argv)
dialog = loader.load("dialog.ui", None)
dialog.show()
app.exec_()
