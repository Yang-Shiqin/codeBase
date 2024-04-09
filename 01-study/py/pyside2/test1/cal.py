import sys
from PySide2.QtWidgets import QApplication, QMainWindow, QCalendarWidget
from PySide2.QtCore import QDate, Qt

class MyCalendarWidget(QCalendarWidget):
    def __init__(self):
        super().__init__()

        # 设置突出显示的日期范围
        start_date = QDate(2024, 4, 1)
        end_date = QDate(2024, 4, 15)

        # 设置日期文本格式
        date_format = self.dateTextFormat(start_date)
        date_format.setBackground(Qt.yellow)
        self.setDateTextFormat(start_date, date_format)

        # 将日期范围内的日期设置为突出显示的格式
        current_date = start_date
        while current_date < end_date:
            current_date = current_date.addDays(1)
            date_format = self.dateTextFormat(current_date)
            date_format.setBackground(Qt.yellow)
            self.setDateTextFormat(current_date, date_format)

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Calendar Example")
        self.setGeometry(100, 100, 400, 300)

        self.calendar_widget = MyCalendarWidget()
        self.setCentralWidget(self.calendar_widget)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
