# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'db_page.ui'
##
## Created by: Qt User Interface Compiler version 5.15.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *
from PySide2.QtWebEngineWidgets import QWebEngineView
from stdb import STDB
from pyecharts.charts import Pie, Line
from pyecharts import options as opts
from pyecharts.commons.utils import JsCode
from datetime import datetime

class DBPage(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.tab_widget = []
        self.view_func = [self.table_html, self.bar_html, self.pie_html]
        self.setupUi()
        self.tabWidget.currentChanged['int'].connect(self.show_db_html)
        self.db = STDB()
        self.show_db_html(0)

    def setupUi(self):
        self.horizontalLayout = QHBoxLayout(self)   # 加self才会显示出来
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        # tab组件
        self.tabWidget = QTabWidget()
        self.tabWidget.setObjectName(u"tabWidget")
        ## 添加页1
        tab1_table = QWebEngineView()
        tab1_table.setObjectName(u"table")
        self.tabWidget.addTab(tab1_table, "")
        self.tab_widget.append(tab1_table)
        ## 添加页2
        tab2_bar = QWebEngineView()
        tab2_bar.setObjectName(u"bar")
        self.tabWidget.addTab(tab2_bar, "")
        self.tab_widget.append(tab2_bar)
        ## 添加页3
        tab3_pie = QWebEngineView()
        tab3_pie.setObjectName(u"pie")
        self.tabWidget.addTab(tab3_pie, "")
        self.tab_widget.append(tab3_pie)

        self.horizontalLayout.addWidget(self.tabWidget, 2)

        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.comboBox = QComboBox()
        self.comboBox.setObjectName(u"comboBox")

        self.verticalLayout.addWidget(self.comboBox)

        self.horizontalSlider = QSlider()
        self.horizontalSlider.setObjectName(u"horizontalSlider")
        self.horizontalSlider.setOrientation(Qt.Horizontal)

        self.verticalLayout.addWidget(self.horizontalSlider)

        self.horizontalLayout.addLayout(self.verticalLayout, 1)

        self.tabWidget.setTabText(self.tabWidget.indexOf(tab1_table), QCoreApplication.translate("Dialog", u"table", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(tab2_bar), QCoreApplication.translate("Dialog", u"bar", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(tab3_pie), QCoreApplication.translate("Dialog", u"pie", None))

        self.tabWidget.setCurrentIndex(0)

        # QMetaObject.connectSlotsByName()
    # setupUi

    # 展示tab视图（先展示所有，后面再加条件筛选）
    def show_db_html(self, index):
        data = self.db.query_all()
        html = self.view_func[index](data)
        self.tab_widget[index].setHtml(html)

    def table_html(self, data):
        html = '''
        <table border="1">
            <tr>
                <th>id</th>
                <th>name</th>
                <th>create_time</th>
                <th>duration</th>
            </tr>
        '''
        for row in data:
            html += '<tr>'
            for col in row:
                html += f'<td>{col}</td>'
            html += '</tr>'
        html += '</table>'
        return html
    
    # 每个任务一条线，横坐标为日期，纵坐标为时长（TODO: 目前就考虑开始日期，不考虑跨天）
    def bar_html(self, data):
        # 获取每个任务每天的时长
        each_job = {}
        for item in data:
            _, name, create_time, duration = item
            create_day = create_time.split(' ')[0]
            if name in each_job:
                if create_day in each_job[name]:
                    each_job[name][create_day] += duration
                else:
                    each_job[name][create_day] = duration
            else:
                each_job[name] = {create_day:duration}
        names = list(each_job.keys())   # 任务名
        days = set(day for job in each_job.values() for day in job.keys())  # 日期
        date_objects = [datetime.strptime(day, '%Y-%m-%d') for day in days]
        # 对日期进行排序
        sorted_dates = sorted(date_objects)
        days = [date.strftime('%Y-%m-%d') for date in sorted_dates]
        # 根据任务和日期构建纵坐标数据
        y_data = [[job.get(day, 0) for day in days] for job in each_job.values()]

        line=(
            Line(init_opts=opts.InitOpts(width="400px", height="300px"))
            .add_xaxis(xaxis_data=list(days))
            .set_global_opts(title_opts=opts.TitleOpts(title="每日任务时长"))
        )
        for name, data in zip(names, y_data):   # 每个任务一条线
            line.add_yaxis(series_name=name, y_axis=data)
        return line.render_embed()
    
    # 每个任务的时长占比
    def pie_html(self, data):
        # 统计data中每个任务的总时长
        sums = {}
        for item in data:
            _, name, _, duration = item
            if name in sums:
                sums[name] += duration
            else:
                sums[name] = duration
        content = [[key, value] for key, value in sums.items()]
        pie = (
            Pie(init_opts=opts.InitOpts(width="400px", height="300px"))
            .add("", content)
            .set_global_opts(title_opts=opts.TitleOpts(title="单任务总时长"))
            .set_series_opts(label_opts=opts.LabelOpts(formatter=JsCode("""
            function(params){
                var seconds = params.value;
                var hours = Math.floor(seconds / 3600);
                var minutes = Math.floor((seconds % 3600) / 60);
                var seconds = seconds % 60;
                var timeStr = hours+'h'+minutes+'m'+seconds+'s';
                return params.name+': '+timeStr;
            }
        """)
            ))
        )
        return pie.render_embed()