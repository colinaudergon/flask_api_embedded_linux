from flask import Flask, send_from_directory
from flask_socketio import SocketIO
import time
import threading

# import gpioController

app = Flask(__name__)
socketio = SocketIO(app)
CLIENT_PAGES_PATH = "client/"



gameIsRunning = False

@app.route("/")
def index():
    try:
        return send_from_directory(CLIENT_PAGES_PATH, "index.html")
    except Exception as e:
        app.logger.error(f"Error serving index.html: {e}")
        raise
# Serve static files (e.g., test.js) from the 'client' folder
@app.route("/client/<path:filename>")
def serve_static(filename):
    try:
        return send_from_directory("client", filename)
    except Exception as e:
        app.logger.error(f"Error serving static file {filename}: {e}")
        raise


def wait_for_user_input():
    global gameIsRunning
    while gameIsRunning:
        # Simulate waiting for user input (replace this with your actual logic)
        user_input = input("Enter a command: ")
        data = {"command": user_input}
        socketio.emit("command_input", data)
        time.sleep(1)  # Simulate some processing time


def send_data_to_client():
    while True:
        if gameIsRunning:
            #wait for user input
            command = "start"  # Set your desired command
            data = {"command": command}
            socketio.emit("command_input", data)
        time.sleep(1)


@socketio.on('start_game')
def handle_start_game():
    global gameIsRunning
    gameIsRunning = True
    send_data_to_client()

@socketio.on("connect")
def handle_connect():
    print("Client connected")
    socketio.start_background_task(target=send_data_to_client)


@socketio.on("my event")
def handle_message(data):
    print("received message:", data)


@socketio.on("gameLaunched")
def handle_game_launched(data):
    global gameIsRunning
    game_name = data["gameName"]
    print(f"Game {game_name} has been launched!")
    gameIsRunning = True
    # You can add additional logic here, such as broadcasting the message to other clients.

@socketio.on("gameClose")
def handle_game_closed(data):
    global gameIsRunning
    print("Game has been closed!")
    gameIsRunning = False
    socketio.emit("command_input", data)



if __name__ == "__main__":
    externalAccesGranted = True
    if externalAccesGranted:
        # app.run(debug=True,host="0.0.0.0")
        socketio.run(app, debug=True, host="0.0.0.0")
    else:
        # app.run(debug=True)
        socketio.run(app, debug=True)
