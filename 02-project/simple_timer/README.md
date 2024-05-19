> tldr
> 
> 最后项目放在 `https://github.com/Yang-Shiqin/simple_timer` 这个仓库了


动机: 项目计时，周报分析效率瓶颈

期望实现功能：
- 支持任务/子任务命名开辟计时任务
- 开始某个任务将暂停其他所有任务计时(一心一用)
- 一键全部暂停
- 数据导出
- 简洁美观的应用界面
- 统计
- ctrl可选择多个任务同时开始(并行作业)
- 设置统计周期则可定时导出每个周期的结果
- 结果可视化
- 成品易获取(打包)
- github readme

打算用的图形化界面库:pysider2
pip install pyside2 

和za聊了，他推荐了我一个pyside的接口，还说js实现更常见，electron

pip install PySide2-Fluent-Widgets -i https://pypi.org/simple/
