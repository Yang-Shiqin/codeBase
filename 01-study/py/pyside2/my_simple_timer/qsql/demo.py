import sys
from PySide2.QtWidgets import QMainWindow,QWidget,QApplication,QTableView,QHBoxLayout
from PySide2.QtSql import QSqlDatabase,QSqlQuery,QSqlTableModel,QSqlQueryModel
from PySide2.QtCore import Qt
class sqlDemo(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
    def initUI(self):
        self.setWindowTitle('demo')
        self.resize(600,600)
        self.layout=QHBoxLayout()
        self.setLayout(self.layout)
 
        #------数据库操作------
        #1连接数据库
        self.connect_db()
        #操作数据库
        self.opreate_db()
        #在Pyqt界面显示数据库内容 （相当于查询）
        self.show_db()
 
    def connect_db(self):
        self.db=QSqlDatabase.addDatabase('QSQLITE')
        self.db.setDatabaseName('./test.db')
        d=self.db.open()
        # print(d)       #显示True 表示连接成功
    
    def opreate_db(self):
        query=QSqlQuery()
        #建立表
        query.exec_("create table test(ID int primary key,name varchar(20),url varchar(100))")
        #插入数据
        query.exec_("insert into test values(1000,'tom','http://www.xx.com')")
        query.exec_("insert into test values(1001,'marray','http://www.xx.com')")
        query.exec_("insert into test values(1002,'jack','http://www.xx.com')")
   
    def show_db(self):
        tableview=QTableView()
        self.layout.addWidget(tableview) #用来在界面显示查询的数据库
 
        #【Qsqlquerymodel】 相对于原生的Qsqlquery更方便
        querymodel=QSqlQueryModel()
        querymodel.setQuery("select * from test")
        querymodel.setHeaderData(0,Qt.Horizontal,'id')
        querymodel.setHeaderData(1,Qt.Horizontal,'姓名')
        querymodel.setHeaderData(2,Qt.Horizontal,'地址')
 
        tableview.setModel(querymodel)
 
        #在后台显示 第一种方法
        for i in range(querymodel.rowCount()):
            id=querymodel.record(i).value('id')
            name=querymodel.record(i).value(1)
            print(str(id)+name)
 
        for i in range(querymodel.rowCount()):
            id=querymodel.data(querymodel.index(i,0))
            print('显示出所有的id'+str(id))
 
 
 
    #关闭数据库
    def close_db(self):
        self.db.close()
 
 
if __name__=='__main__':
    app=QApplication(sys.argv)
    demo=sqlDemo()
    demo.show()
    sys.exit(app.exec_())