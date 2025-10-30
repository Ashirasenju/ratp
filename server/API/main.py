from flask import *
import sqlite3
from werkzeug.utils import secure_filename
import os
import hashlib
app = Flask(__name__)


@app.route("/ratp/")
def root():
    return "<p>Welcome </p>"


@app.route("/ratp/isLatest")
def update():   
    connection = sqlite3.connect("../db/db")
    cursor = connection.cursor()

    package_name = request.args.get("package_name")
    version = request.args.get("version")

    query = "SELECT latest_update FROM packages WHERE package_name=?"
    latest_realease = float(cursor.execute(query, (package_name,)).fetchall()[0])
    if latest_realease > float(version):
        return jsonify({"has_to_update": 1})
    else:
        return jsonify({"has_to_update": 0})


@app.route("/ratp/download_latest")
def get_mirror_link():
    package_name = request.args.get("package_name")

    # Build the path of the string TODO : when creating the installer for server side make sure to modify path_to_storage
    package_name = secure_filename(package_name)
    path_to_storage = "~/Documents/DevPerso/C/ratp/server/storage/"
    hashedpackage=hashlib.sha512(package_name.encode('utf-8')).hexdigest()
    file_name = f"{package_name}.tar.xz"
    path_to_latest_version = os.path.join(path_to_storage, hashedpackage, "latest_release")
    try:
        version = open(path_to_latest_version , "r").readline()
    except:
        return "Something went wrong."
    
    path_to_file = os.path.join(path_to_storage, hashedpackage, version,file_name)
    
    return send_file(path_to_file)

@app.route("/ratp/download")
def download_specific_version():
    # TODO : fonction that take two parameters package_name and version (normalized see notes for more info)
    return ""