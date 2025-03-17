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
import sqlite3
from werkzeug.security import generate_password_hash, check_password_hash
from gevent import monkey
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
# sessionList = {}
# currentConnection = 0
# maxConnection = 3


# def querySession(username):
#     if not username in sessionList:
#         return -1
#     return sessionList[username]


# def addSession(username):
#     global currentConnection
#     if currentConnection >= maxConnection:
#         return 0
#     if querySession(username) != -1:
#         sessionList[username] = time.time()
#         return 1
#     currentConnection += 1
#     sessionList[username] = time.time()
#     print("[Info] Session added from " + username + ".", file=sys.stderr)
#     return 1


# def removeSession(username):
#     global currentConnection
#     if querySession(username) == -1:
#         return 1
#     currentConnection -= 1
#     sessionList.pop(username)
#     print("[Info] Session removed from " + username + ".", file=sys.stderr)
#     return 1


# def renewSession(username):
#     if querySession(username) == -1:
#         return 0
#     sessionList[username] = time.time()
#     print("[Info] Session renewed from " + username + ".", file=sys.stderr)
#     return 1


# def checkAllSessions():
#     def job():
#         print("[Info] Running sessions check schedule.", file=sys.stderr)
#         global currentConnection
#         ts = time.time()
#         for i in sessionList:
#             if sessionList[i] == -1:
#                 continue
#             if ts - sessionList[i] >= 120:
#                 sessionList[i] = -1
#                 print("[Info] Session removed from " +
#                       i + ".", file=sys.stderr)
#                 currentConnection -= 1

#     schedule.every(2).minutes.do(job)
#     while True:
#         schedule.run_pending()
#         time.sleep(1)


# _thread.start_new_thread(checkAllSessions)

# SQLite Connection

address = 'db.sqlite3'

db = sqlite3.connect(address)
cursor = db.cursor()

# Internal Function


def getUserPassword(username):
    cursor.execute(
        "SELECT password FROM `users` WHERE username=\"" + username + "\"")
    res = cursor.fetchone()
    if res == None:
        return ""
    else:
        return res[0]


def createUser(username, password):
    cursor.execute(
        "INSERT INTO `users` (`username`, `password`, `admin`) VALUES ('%s', '%s', '0')" % (username, password))
    db.commit()


def getUserID(username):
    cursor.execute(
        "SELECT id FROM `users` WHERE username=\"" + username + "\"")
    res = cursor.fetchone()
    return res[0]

def checkAdmin(username):
    cursor.execute(
        "SELECT admin FROM `users` WHERE username=\"" + username + "\"")
    res = cursor.fetchone()
    return res[0] == 1

'''
PrivilegeNode => Tree Structure
OWNER MODERATOR ADMIN MEMBER
'''

@app.route("/session", methods=["GET"])
def checkSession():
    username = session.get("username")
    if username:
        return jsonify(code=200, msg=username)
    else:
        return jsonify(code=401, msg="Not logged in")


# API


@app.route('/')
def main():
    return "Invalid arguments"


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

@app.route("/logout")
def logout():
    if not (session.get("username")):
        flask.abort(401)
    session.clear()
    return jsonify(code=200, msg="Success")


@app.route('/contest/list', methods=['GET'])
def getContestList():
    if not (session.get("username")):
        flask.abort(401)
    cursor.execute(
        "SELECT cid, name, start_time, end_time FROM `contests`")
    data = cursor.fetchall()
    return jsonify([{"cid": i[0], "name": i[1], "startTime": i[2], "endTime": i[3]} for i in data])

@app.route('/contest/info', methods=['GET'])
def getContestInfo():
    if not (session.get("username")):
        flask.abort(401)
    cursor.execute(
        f"SELECT cid, name, start_time, end_time FROM `contests` WHERE cid={request.args.get('id', '0')}")
    i = cursor.fetchone()
    return jsonify({"cid": i[0], "name": i[1], "startTime": i[2], "endTime": i[3]})


@app.route('/record', methods=['GET'])
def getRecords():
    if not (session.get("username")):
        flask.abort(401)
    cursor.execute(
        f"SELECT id, uid, pid, record_time, score FROM `records` WHERE cid={request.args.get('id', '0')}")
    data = cursor.fetchall()
    cursor.execute(
        f"SELECT id, username FROM `users`")
    namelist = cursor.fetchall()
    namelist = dict(zip([i[0] for i in namelist], [i[1] for i in namelist]))
    res = [{"id": i[0], "name": namelist[i[1]], "pid": i[2], "time": i[3] ,"score": i[4]} for i in data]
    return jsonify(res)

@app.route('/contest/problems', methods=['GET'])
def getProblems():
    if not (session.get("username")):
        flask.abort(401)
    cursor.execute(
        f"SELECT pid, name, total_score FROM `problems`")
    namelist = cursor.fetchall()
    namelist = dict(zip([i[0] for i in namelist], [(i[1], i[2]) for i in namelist]))
    cursor.execute(
        f"SELECT pid score FROM `cp_mapping` WHERE cid={request.args.get('id', '0')}")
    data = cursor.fetchall()
    res = [{"pid": i[0], "name": namelist[i[0]][0], "totalScore": namelist[i[0]][1]} for i in data]
    return jsonify(res)

@app.route('/record/add', methods=['POST'])
def addRecords():
    if not (session.get("username")):
        flask.abort(401)
    if not (checkAdmin(session.get("username"))): flask.abort(403)
    try: setJson = request.get_json()
    except: setJson = {}
    if not setJson: setJson = {}
    cursor.execute(
        "INSERT INTO `records` (uid, cid, pid, score) VALUES ('%s', '%s', '%s', '%s')" 
            % (setJson['uid'], setJson['cid'], setJson['pid'], setJson['score']))
    db.commit()
    return jsonify(code=200, msg="Success")
    
@app.route('/record/modify', methods=['POST'])
def modifyRecords():
    if not (session.get("username")): flask.abort(401)
    if not (checkAdmin(session.get("username"))): flask.abort(403)
    try: setJson = request.get_json()
    except: setJson = {}
    if not setJson: setJson = {}
    cursor.execute(
        "UPDATE `records` SET `score` = '%s' WHERE (`id` = '%s')" % (setJson['score'], setJson['id']))
    db.commit()
    return jsonify(code=200, msg="Success")

@app.route('/record/delete', methods=['POST'])
def deleteRecords():
    if not (session.get("username")): flask.abort(401)
    if not (checkAdmin(session.get("username"))): flask.abort(403)
    try: setJson = request.get_json()
    except: setJson = {}
    if not setJson: setJson = {}
    cursor.execute(
        "DELETE FROM `records` WHERE (`id` = '%s')" % (setJson['id']))
    db.commit()
    return jsonify(code=200, msg="Success")

# Main


if __name__ == "__main__":
    from werkzeug.debug import DebuggedApplication
    from gevent import pywsgi
    dapp = DebuggedApplication(app, evalex=True)
    server = pywsgi.WSGIServer(('127.0.0.1', 5174), app)
    server.serve_forever()
