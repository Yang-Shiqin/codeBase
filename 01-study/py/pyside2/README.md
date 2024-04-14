# pyside2学习
- 笔记在：`notee\tec\qt_study\PyQt-Fluent-Widgets入门-pyside2.md`
- 最终形成的项目在 `codeBase/02-project/simple_timer`

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
│   ├── 00__frame.py                    # pyqt最简单的应用框架
│   ├── 01__button_window.py            # 最简单的一个按钮窗口
│   ├── 02__qmainwindow.py              # 主窗口
│   ├── 03__qmainwindow2.py             # 自定义的主窗口
│   ├── 04__signal_slot.py              # 信号与槽1: 打印
│   ├── 05__signal_slot2.py             # 信号与槽2: 改变组件
│   ├── 06__signal_slot3.py             # 信号与槽3: 改变组件
│   ├── 07__signal_slot_2widget.py      # 信号与槽4: 组件间连接
│   ├── 08__event.py                    # 重写event(event是比信号更底层的，暂时没用)
│   ├── 09__widgets.py                  # 组件介绍
│   ├── 10__qt_designer_as_main.py      # qt-designer设计的.ui文件作为主窗口(动态加载)
│   ├── 11__qt_designer_as_dialog.py    # qt-designer设计的.ui文件作为对话框(动态加载)
│   ├── 12__qt_designer_static_load     ### qt-designer设计的.ui文件作为对话框(动态加载)
│   │   ├── employee.ui
│   │   ├── employee_dlg.py             # pyside2-uic employee.ui > employee_dlg.py生成的
│   │   └── main.py                     # 入口
│   ├── 13__qresource_sys               ### qresource介绍
│   │   ├── animal-monkey.png           # 具体资源1
│   │   ├── animal-penguin.png
│   │   ├── main_direct.py              # 入口: 直接调用资源
│   │   ├── main_qrc.py                 # 入口: qrc方式调用资源
│   │   ├── resources.py                # pyside2-rcc resources.qrc -o resources.py生成的
│   │   └── resources.qrc               # 资源如何组织(可以由qdesigner生成)
│   └── dialog.ui
├── qt-fluent               # qt-fluent学习的过程(美化的qt)
│   ├── 00__qfluent_button
│   │   ├── a.py
│   │   ├── button.ui
│   │   └── main.py
│   └── test.py
└── test1                   # 没用的, 临时的代码

18 directories, 58 files
```

## my_simple_timer项目介绍
#TODO:
- [ ] 美化
- [ ] 展示时间(纵坐标变成h为单位，label展示时分秒)
    - https://blog.csdn.net/weixin_45366499/article/details/105167985
    - 不会搞
- [ ] 任务/子任务
- [ ] 数据库删除
- [x] version
- [ ] qresource
    - [ ] 图标
    - [ ] 按钮
- [ ] 软件介绍/作者链接
- [ ] 数据库导出/导入
- [ ] github文档
- [ ] line的textedit可从已有任务中生成
- [ ] 边缘收缩
    - https://blog.csdn.net/wisdomroc/article/details/135977423

pyinstaller与pyechart不兼容, 不能通过 `pyinstaller -F -w main.py` 打包
https://www.cnblogs.com/LoveOpenSourceBoy/p/14192260.html

### 功能介绍
- 所有任务都和其他任务冲突(不能同时计时)
- 点击完成按钮才能保存到数据库
