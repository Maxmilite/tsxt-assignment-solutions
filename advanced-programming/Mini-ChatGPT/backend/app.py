import os
import sys
import time
from flask import session
import asyncio
import flask
import schedule
import _thread
import nest_asyncio
from concurrent.futures import ThreadPoolExecutor
from flask import Flask, jsonify
from flask import request
import MySQLdb
from werkzeug.security import generate_password_hash, check_password_hash
from gevent import monkey
from nlp import getNLP
monkey.patch_all()

# Flask
client = 'http://127.0.0.1:5173'
app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret_key'

# Async IO
nest_asyncio.apply()
loop = asyncio.get_event_loop()

# Thread Pool
executor = ThreadPoolExecutor()

# Login and Session
app.secret_key = b'a*DMp_V%q_sQ27~5'
sessionList = {}
currentConnection = 0
maxConnection = 3


def querySession(username):
    if not username in sessionList:
        return -1
    return sessionList[username]


def addSession(username):
    global currentConnection
    if currentConnection >= maxConnection:
        return 0
    if querySession(username) != -1:
        sessionList[username] = time.time()
        return 1
    currentConnection += 1
    sessionList[username] = time.time()
    print("[Info] Session added from " + username + ".", file=sys.stderr)
    return 1


def removeSession(username):
    global currentConnection
    if querySession(username) == -1:
        return 1
    currentConnection -= 1
    sessionList.pop(username)
    print("[Info] Session removed from " + username + ".", file=sys.stderr)
    return 1


def renewSession(username):
    if querySession(username) == -1:
        return 0
    sessionList[username] = time.time()
    print("[Info] Session renewed from " + username + ".", file=sys.stderr)
    return 1


def checkAllSessions():
    def job():
        print("[Info] Running sessions check schedule.", file=sys.stderr)
        global currentConnection
        ts = time.time()
        for i in sessionList:
            if sessionList[i] == -1:
                continue
            if ts - sessionList[i] >= 120:
                sessionList[i] = -1
                print("[Info] Session removed from " +
                      i + ".", file=sys.stderr)
                currentConnection -= 1

    schedule.every(2).minutes.do(job)
    while True:
        schedule.run_pending()
        time.sleep(1)


_thread.start_new_thread(checkAllSessions)

# MySQL Connection

address = "localhost"
port = 3306
nameMessageDatabase = "MessageTemplate"
nameMessageTable = "message_table"
username = "guest"
password = "dnf@#YU328!4#NYVJ"
unknownMessage = "Unknown message."

db = MySQLdb.connect(address, username, password,
                     nameMessageDatabase, charset='utf8')
cursor = db.cursor()

# Internal Function


def updatePopularity(message):
    cursor.execute(
        "SELECT id,popularity FROM message_table WHERE question=\"" + message + "\"")
    data = cursor.fetchone()
    val = int(data[1]) + 1
    id = int(data[0])
    cursor.execute("UPDATE `messagetemplate`.`message_table` SET `popularity` = \'" +
                   str(val) + "\' WHERE (`id` = \'" + str(id) + "\')")
    db.commit()


def getUserPassword(username):
    cursor.execute(
        "SELECT password FROM `userdata`.`user_pass` WHERE username=\"" + username + "\"")
    res = cursor.fetchone()
    if res == None:
        return ""
    else:
        return res[0]


def createUser(username, password):
    cursor.execute(
        "INSERT INTO `userdata`.`user_pass` (`username`, `password`) VALUES ('%s', '%s')" % (username, password))
    db.commit()


def getUserID(username):
    cursor.execute(
        "SELECT id FROM `userdata`.`user_pass` WHERE username=\"" + username + "\"")
    res = cursor.fetchone()
    return res[0]


def storeMessage(userID, messageSent, messageReceived):
    cursor.execute(
        "INSERT INTO `userdata`.`chat_history` (`sender`, `message_sent`, `message_received`, `category`) VALUES ('%s', '%s', '%s', '%s')" % (userID, messageSent, messageReceived, "Default"))
    db.commit()


def getNLPMessage(e):
    NLPList = getNLP(e)
    res = "According to your question, I have found something similar with keywords: "
    for nlp in NLPList:
        res += "For %s: " % nlp
        cursor.execute(
            "SELECT answer FROM `MessageTemplate`.`message_table` WHERE question like \"%%" + nlp + "%%\"")
        data = cursor.fetchall()
        for datum in data:
            res += "    %s; " % datum
    if res == "According to your question, I have found something similar with keywords: ":
        return "I'm sorry but I cannot find anything similar with your keywords currently."
    return res


# API


@app.route('/')
def main():
    return "Invalid arguments"


@app.route('/message', methods=['GET'])
def getMessage():
    if not (session.get("username")):
        flask.abort(401)
    if querySession(session['username']) == -1:
        flask.abort(403)
    message = request.args.get('message', '').replace(
        "?", "").replace(".", "").replace("？", "")
    cursor.execute(
        "SELECT answer FROM message_table WHERE question=\"" + message + "\"")
    data = cursor.fetchone()
    if (data != None):
        updatePopularity(message)
        storeMessage(getUserID(session.get("username")), message, data[0])
        return data[0]
    else:
        return getNLPMessage(message)


@app.route('/message/file', methods=['POST'])
def getFileMessage():
    if not (session.get("username")):
        flask.abort(401)
    if querySession(session['username']) == -1:
        flask.abort(403)
    file = request.files.get('file')
    if file is None:
        return 'File is not uploaded.'
    file.save("Text.txt")
    message = ""
    with open("Text.txt", "r+", encoding="UTF-8") as f:
        list = f.readlines()
        for i in list:
            message += i
    os.remove("Text.txt")
    message = message.replace(
        "?", "").replace(".", "").replace("？", "")
    cursor.execute(
        "SELECT answer FROM message_table WHERE question=\"" + message + "\"")
    data = cursor.fetchone()
    if (data != None):
        updatePopularity(message)
        storeMessage(getUserID(session.get("username")), message, data[0])
        return data[0]
    else:
        return getNLPMessage(message)


@app.route('/autocomplete', methods=['GET'])
def getACList():
    if not (session.get("username")):
        flask.abort(401)
    cursor.execute(
        "SELECT question FROM message_table")
    data = cursor.fetchall()
    return jsonify([{"value": i[0]} for i in data])


@app.route('/hotspot', methods=['GET'])
def getHotSpot():
    cursor.execute(
        "SELECT question,answer,popularity FROM message_table ORDER BY popularity DESC")
    data = cursor.fetchall()
    return jsonify([{"question": i[0], "answer": i[1], "popularity": i[2], "rank": data.index(i) + 1} for i in data])


@app.route('/history/get', methods=['GET'])
def getChatHistory():
    if not (session.get("username")):
        flask.abort(401)
    userID = getUserID(session.get("username"))
    category = request.args.get('category', '')
    if not category:
        cursor.execute(
            "SELECT date,message_sent,message_received,category,id FROM `userdata`.`chat_history` WHERE `sender`=\'%s\' ORDER BY date DESC" % userID)
        data = cursor.fetchall()
        return jsonify([{"date": i[0], "question": i[1], "answer": i[2], "category": i[3], "id": i[4]} for i in data])
    else:
        cursor.execute(
            "SELECT date,message_sent,message_received,category,id FROM `userdata`.`chat_history` WHERE `sender`=\'%s\' AND `category`=\'%s\'" % (userID, category))
        data = cursor.fetchall()
        return jsonify([{"date": i[0], "question": i[1], "answer": i[2], "category": i[3], "id": i[4]} for i in data])


@app.route('/history/set', methods=['POST'])
def setChatHistory():
    if not (session.get("username")):
        flask.abort(401)
    try:
        setJson = request.get_json()
    except Exception:
        setJson = {}
    if not setJson:
        setJson = {}
    id = setJson.get("id")
    category = setJson.get("category")
    cursor.execute(
        "UPDATE `userdata`.`chat_history` SET `category` = '%s' WHERE (`id` = '%s')" % (category, id))
    db.commit()
    return jsonify(code=200, msg="Success")


@app.route('/history/categories', methods=['GET'])
def getHistoryCategories():
    if not (session.get("username")):
        flask.abort(401)
    userID = getUserID(session.get("username"))
    cursor.execute(
        "SELECT DISTINCT category FROM `userdata`.`chat_history` WHERE `sender`=\'%s\'" % userID)
    data = cursor.fetchall()
    return jsonify([i[0] for i in data])


@app.route('/login', methods=['POST'])
def login():
    try:
        loginJson = request.get_json()
    except Exception:
        loginJson = {}
    if not loginJson:
        loginJson = {}
    username = loginJson.get("username")
    password = loginJson.get("password")
    if not all([username, password]):
        return jsonify(code=400, msg="Invalid argument")
    curPass = getUserPassword(username=username)
    if curPass == "":
        createUser(username, generate_password_hash(password))
        session["username"] = username
        return jsonify(code=200, msg="Login success")
    elif check_password_hash(curPass, password):
        session["username"] = username
        return jsonify(code=200, msg="Login success")
    else:
        return jsonify(code=401, msg="Incorrect username or password")


@app.route("/session", methods=["GET"])
def check_session():
    username = session.get("username")
    if username:
        return jsonify(code=200, msg=username)
    else:
        return jsonify(code=401, msg="Not logged in")


@app.route("/logout")
def logout():
    if not (session.get("username")):
        flask.abort(401)
    session.clear()
    return jsonify(code=200, msg="Success")


@app.route("/session/renew")
def renew():
    if not (session.get("username")):
        flask.abort(401)
    if renewSession(session['username']) == 0:
        return jsonify(code=403, msg="You are not logged in.")
    return jsonify(code=200, msg="Success")


@app.route("/session/register")
def register():
    if not (session.get("username")):
        flask.abort(401)
    if addSession(session['username']) == 0:
        return jsonify(code=403, msg="Server is full, please try again later.")
    return jsonify(code=200, msg="Success")


@app.route("/session/remove")
def remove():
    if not (session.get("username")):
        flask.abort(401)
    removeSession(session['username'])
    return jsonify(code=200, msg="Success")

# Main


if __name__ == "__main__":
    from werkzeug.debug import DebuggedApplication
    from gevent import pywsgi
    dapp = DebuggedApplication(app, evalex=True)
    server = pywsgi.WSGIServer(('127.0.0.1', 5000), app)
    server.serve_forever()
