import sqlite3

def main():
    address = 'db.sqlite3'
    db = sqlite3.connect(address)
    cursor = db.cursor()
    cursor.execute('''CREATE TABLE users
       (id INTEGER PRIMARY KEY AUTOINCREMENT,
       username TEXT NOT NULL,
       password TEXT NOT NULL
       admin INTEGER NOT NULL);'''
    )
    cursor.execute('''CREATE TABLE records
       (id INTEGER PRIMARY KEY AUTOINCREMENT,
       uid INTEGER NOT NULL,
       cid INTEGER NOT NULL,
       pid INTEGER NOT NULL,
       record_time BIGINT NOT NULL,
       score INTEGER NOT NULL);'''
    )
    cursor.execute('''CREATE TABLE problems
       (pid INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
       total_score INTEGER NOT NULL);'''
    )
    cursor.execute('''CREATE TABLE contests
       (cid INTEGER PRIMARY KEY AUTOINCREMENT,
       name TEXT NOT NULL,
         start_time BIGINT NOT NULL,
         end_time BIGINT NOT NULL);'''
    )

    # contest & problem mapping
    cursor.execute('''CREATE TABLE cp_mapping
       (id INTEGER PRIMARY KEY AUTOINCREMENT,
        cid INTEGER NOT NULL,
        pid INTEGER NOT NULL
        );'''
    )

if __name__ == "__main__":
    main()