# pyside2学习

## 目录结构
```bash
.
├── README.md               # this file
├── my_simple_timer         ######## 制作我的simple timer小应用的过程
│   ├── pyechart            ### 学习pyechart, 一个基于js的可交互的画图库(交互好, 但画图不太好)
│   │   ├── render.html     # test.py生成
│   │   └── test.py         # 生成一个bar并在qt中展示
│   ├── qsql                ### 学习qt封装的sql(但我最后应该还是用的sqlite3, 自己写)
│   ├── timer0              ### simple timer阶段1: 计时行组件的搭建和使用
│   │   ├── line.py         # 计时行组件
│   │   ├── main.py         # 展示计时行组件
│   │   └── main2.py        # 在主窗口添加计时行组件
│   ├── timer1              ### simple timer阶段2: 数据库的搭建(记录计时数据)
│   │   ├── line.py         # 记录到数据库的代码插入line.py, 增加删除line组件
│   │   ├── main.py
│   │   ├── st.db
│   │   └── stdb.py         # sqlite封装
│   └── timer2              ### simple timer阶段2: 数据展示
│       ├── db_page.py      # 展示页, 三个视图: table, line, pie
│       ├── line.py
│       ├── main.py
│       ├── st.db
│       └── stdb.py
├── pyside2_learning        ######## pyside2学习的过程
│   ├── 00__frame.py
│   ├── 01__button_window.py
│   ├── 02__qmainwindow.py
│   ├── 03__qmainwindow2.py
│   ├── 04__signal_slot.py
│   ├── 05__signal_slot2.py
│   ├── 06__signal_slot3.py
│   ├── 07__signal_slot_2widget.py
│   ├── 08__event.py
│   ├── 09__widgets.py
│   ├── 10__qt_designer_as_main.py
│   ├── 11__qt_designer_as_dialog.py
│   ├── 12__qt_designer_static_load
│   │   ├── __pycache__
│   │   │   └── employee_dlg.cpython-310.pyc
│   │   ├── employee.ui
│   │   ├── employee_dlg.py
│   │   └── main.py
│   ├── 13__qresource_sys
│   │   ├── __pycache__
│   │   │   └── resources.cpython-310.pyc
│   │   ├── animal-monkey.png
│   │   ├── animal-penguin.png
│   │   ├── main_direct.py
│   │   ├── main_qrc.py
│   │   ├── resources.py
│   │   └── resources.qrc
│   └── dialog.ui
├── qt-fluent               # qt-fluent学习的过程(美化的qt)
│   ├── 00__qfluent_button
│   │   ├── __pycache__
│   │   │   └── a.cpython-310.pyc
│   │   ├── a.py
│   │   ├── button.ui
│   │   └── main.py
│   └── test.py
└── test1                   # 没用的, 临时的代码
    ├── a.ui
    ├── a_layout.png
    ├── cal.py
    └── test1.py

18 directories, 58 files
```

## my_simple_timer项目介绍
#TODO:
- [ ] 美化
- [ ] 任务条件筛选
- [ ] 数据库删除
- [ ] 数据库导出
- [ ] github文档
- [ ] 边缘收缩
    - https://blog.csdn.net/wisdomroc/article/details/135977423

pyinstaller与pyechart不兼容, 不能通过 `pyinstaller -F -w main.py` 打包
https://www.cnblogs.com/LoveOpenSourceBoy/p/14192260.html

### 功能介绍
- 所有任务都和其他任务冲突(不能同时计时)
- 点击完成按钮才能保存到数据库
