from flask import *
import sqlite3
from werkzeug.utils import secure_filename
import os
import hashlib
app = Flask(__name__)

# A changer par une variable universel
path_to_storage = "~/Documents/DevPerso/C/ratp/server/storage/"


@app.route("/ratp/")
def root():
    return "<p>Welcome </p>"


@app.route("/ratp/packageExist")
def packageExist():
    connection = sqlite3.connect(
        "/home/ashira/Documents/DevPerso/C/ratp/server/db/db")
    cursor = connection.cursor()

    package_name = request.args.get("package_name")
    package_name = secure_filename(package_name)
    query = "SELECT latest_update FROM packages WHERE package_name=?"
    latest_realease = cursor.execute(
        query, (package_name,)).fetchall()
    if latest_realease == []:
        return jsonify([0])
    else:
        return jsonify([1])




@app.route("/ratp/isLatest")
def update():

    connection = sqlite3.connect(
        "/home/ashira/Documents/DevPerso/C/ratp/server/db/db")
    cursor = connection.cursor()

    package_name = request.args.get("package_name")
    package_name = secure_filename(package_name)

    version = request.args.get("version")
    query = "SELECT latest_update FROM packages WHERE package_name=?"
    latest_realease = cursor.execute(
        query, (package_name,)).fetchall()
    if latest_realease == []:
        return jsonify({"has_to_update": 4})
    version = version[0] + "." + version[2] + version[4]
    latest_realease = latest_realease[0][0]
    latest_realease = latest_realease[0] + "." + \
        latest_realease[2] + latest_realease[4]
    latest_realease = float(latest_realease)
    if latest_realease > float(version):
        return jsonify({"has_to_update": 1})
    else:
        return jsonify({"has_to_update": 0})


@app.route("/ratp/download_latest")
def get_mirror_link():
    package_name = request.args.get("package_name")

    # Build the path of the string TODO : when creating the installer for server side make sure to modify path_to_storage
    package_name = secure_filename(package_name)
    path_to_storage = "/home/ashira/Documents/DevPerso/C/ratp/server/storage/"
    file_name = f"{package_name}.tar.xz"
    path_to_latest_version = os.path.join(
        path_to_storage, package_name, "latest_release")
    print(path_to_latest_version)
    try:
        version = open(path_to_latest_version, "r").readline().split("\n")[0]
    except:
        return "Something went wrong."

    path_to_file = os.path.join(
        path_to_storage, package_name, version, file_name)
    try:
        return send_file(path_to_file)
    except FileNotFoundError:
        return "Package Not Found"


@app.route("/ratp/download")
def download_specific_version():
    package_name = request.args.get("package_name")
    version = "v" + request.args.get("version")

    version = hashlib.sha512(version.encode('utf-8')).hexdigest()
    file_name = f"{package_name}.tar.xz"

    path_to_file = os.path.join(
        path_to_storage, package_name, version, file_name)
    try:
        return send_file(path_to_file)
    except FileNotFoundError:
        return "Package Not Found"


if __name__ == "__main__":
    app.run(port=66)
