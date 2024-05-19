import sqlite3

class STDB: # simple timer database
    def __init__(self):
        self.conn = sqlite3.connect('st.db')
        self.cur = self.conn.cursor()
        create_table = '''
            CREATE TABLE IF NOT EXISTS time_data (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT,
            create_time TIMESTAMP,
            duration INTEGER
        );'''
        self.cur.execute(create_table) 

    def insert(self, data):
        self.cur.execute('''
            INSERT INTO time_data (name, create_time, duration)
            VALUES (:name, :create_time, :duration)
        ''', data)
        self.conn.commit()
        
    def __del__(self):
        self.cur.close()
        self.conn.close()
    
    def query_by_names(self, names):
        self.cur.execute('''
            SELECT * FROM time_data WHERE name IN ({})
        ''' .format(','.join(['?']*len(names))), names)
        return self.cur.fetchall()
    
    def query_by_time(self, start, end):
        self.cur.execute('''
            SELECT * FROM time_data WHERE create_time BETWEEN :start AND :end
        ''', {'start': start, 'end': end})
        return self.cur.fetchall()
    
    def query_by_names_and_time(self, names, start, end):
        self.cur.execute('''
            SELECT * FROM time_data WHERE name IN ({}) AND create_time BETWEEN :start AND :end
        ''' .format(','.join(['?']*len(names))), names)
        return self.cur.fetchall()
    
    def query_all(self):
        self.cur.execute('''
            SELECT * FROM time_data
        ''')
        return self.cur.fetchall()
    
    def get_title(self):
        self.cur.execute('''
            PRAGMA table_info(time_data)
        ''')
        return [row[1] for row in self.cur.fetchall()]

if __name__ == '__main__':
    db = STDB()
    # db_data = {}
    # db_data['name'] = "task1"
    # # 设置为now的前一天
    # db_data['create_time'] = QDateTime.currentDateTime().addDays(-1).toString('yyyy-MM-dd hh:mm:ss')
    # db_data['duration'] = QTime(0,0,0).msecsTo(QTime(0,3,0))
    # db.insert(db_data)
    print(db.query_all())
    del db