#!/usr/bin/env python
# coding=utf-8

import sys
from PySide2.QtWidgets import QApplication, QDialog, QMainWindow, QPushButton
from employee_dlg import Ui_Dialog

class Window(QMainWindow):
    """Main window."""
    def __init__(self, parent=None):
        """Initializer."""
        super().__init__(parent)
        # Use a QPushButton for the central widget
        self.centralWidget = QPushButton("Employee...")
        # Connect the .clicked() signal with the .onEmployeeBtnClicked() slot
        self.centralWidget.clicked.connect(self.onEmployeeBtnClicked)
        self.setCentralWidget(self.centralWidget)

    # Create a slot for launching the employee dialog
    def onEmployeeBtnClicked(self):
        """Launch the employee dialog."""
        dlg = EmployeeDlg(self)
        dlg.exec()


class EmployeeDlg(Ui_Dialog, QDialog):
    """Employee dialog."""
    def __init__(self, parent=None):
        super().__init__(parent)
        # Run the .setupUi() method to show the GUI
        self.setupUi(self)


# Create the application
app = QApplication(sys.argv)
# Create and show the application's main window
w = Window()
w.show()
# Run the application's main loop
app.exec_()
