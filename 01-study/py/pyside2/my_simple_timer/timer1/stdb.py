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

    def query(self):
        self.cur.execute('''
            SELECT * FROM time_data
        ''')
        return self.cur.fetchall()
    
if __name__ == '__main__':
    db = STDB()
    print(db.query())
    del db