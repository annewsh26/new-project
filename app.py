from flask import Flask, render_template, request
import subprocess

app = Flask(__name__)

def run_c(input_text):
    p = subprocess.Popen(
        ["event.exe"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        shell=True
    )
    out, _ = p.communicate(input_text)
    return out.strip()


@app.route("/")
def home():
    return render_template("index.html", sidebar_data=None)

@app.route("/insert", methods=["POST"])
def insert():
    eid = request.form["event_id"].replace("EVT-", "")
    name = request.form["name"]
    loc = request.form["loc"]
    date = request.form["date"]
    seats = request.form["seats"]
    upcoming = "1" if request.form.get("upcoming") else "0"

    output = run_c(f"INSERT {eid} {name} {loc} {date} {seats} {upcoming}\n")

    return render_template("index.html", sidebar_data=[output])

@app.route("/search", methods=["POST"])
def search():
    eid = request.form["event_id"]
    output = run_c(f"SEARCH {eid}\n")

    rows = []
    if "|" in output:
        p = output.split("|")
        rows.append(p)

    return render_template("index.html", sidebar_data=rows)

@app.route("/sort")
def sort_events():
    output = run_c("DISPLAY_ALL\n")
    rows = [line.split("|") for line in output.splitlines()]
    return render_template("index.html", sidebar_data=rows)

if __name__ == "__main__":
    app.run(debug=True)
