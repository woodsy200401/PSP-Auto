from http.server import BaseHTTPRequestHandler, HTTPServer
from datetime import datetime
import subprocess

def battery():
    try:
        output = subprocess.check_output(["termux-battery-status"], text=True, timeout=3)
        import json
        return str(json.loads(output).get("percentage", -1))
    except Exception:
        return "-1"

class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path != "/status":
            self.send_error(404)
            return
        data = ("time=" + datetime.now().strftime("%H:%M") + "\n"
                + "battery=" + battery() + "\n"
                + "track=PSP AUTO TEST\n"
                + "artist=ANDROID BRIDGE\n").encode()
        self.send_response(200)
        self.send_header("Content-Type","text/plain; charset=utf-8")
        self.send_header("Content-Length",str(len(data)))
        self.end_headers()
        self.wfile.write(data)

print("PSP Auto bridge: http://127.0.0.1:8765/status")
HTTPServer(("127.0.0.1",8765),Handler).serve_forever()
