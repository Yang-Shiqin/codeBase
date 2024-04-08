from PySide2.QtCore import QUrl
from PySide2.QtWidgets import QMainWindow, QApplication
from PySide2.QtWebEngineWidgets import QWebEngineView

import os
import sys


class MainWindow(QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        self.browser = QWebEngineView()
        self.browser.setUrl(QUrl("file:///render.html"))
        self.setCentralWidget(self.browser)


app = QApplication(sys.argv)
window = MainWindow()
window.show()

app.exec_()