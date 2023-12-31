from flask import Flask, send_from_directory
from flask_socketio import SocketIO
import time
import threading
from unix_socket_server import unix_socket_server

# import gpioController

app = Flask(__name__)
socketio = SocketIO(app)
CLIENT_PAGES_PATH = "client/"

gameIsRunning = False
socket_server_thread = None

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

def send_data_to_client(value_received):
    global gameIsRunning
    start_code = 0x80
    select_code = 0x40
    up_code =  0x08
    down_code = 0x04
    right_code = 0x02
    left_code = 0x01
    a_code = 0x20
    b_code = 0x10
    
    start_pressed = (value_received & start_code) != 0
    select_pressed = (value_received & select_code) != 0
    a_pressed = (value_received & a_code) != 0
    b_pressed = (value_received & b_code) != 0
    joystick_up = (value_received & up_code) != 0
    joystick_down = (value_received & down_code) != 0
    joystick_right = (value_received & right_code) != 0
    joystick_left = (value_received & left_code) != 0
    
    if gameIsRunning:
        if(start_pressed):
            command = "start"
            data = {"command": command}
            socketio.emit("command_input", data)
        if(select_pressed):
            command= "select"
            data = {"command": command}
            socketio.emit("command_input", data)
        if(a_pressed):
            command="a"
            data = {"command": command}
            socketio.emit("command_input", data)
        if(b_pressed):
            command="b"
            data = {"command": command}
            socketio.emit("command_input", data)
        if(joystick_up):
            command="up"
            data = {"command": command}
            socketio.emit("command_input", data)
        if(joystick_down):
            command="down"
            data = {"command": command}
            socketio.emit("command_input", data)
        if(joystick_left):
            command="left"
            data = {"command": command}
            socketio.emit("command_input", data)
        if(joystick_right):    
            command="right"
            data = {"command": command}
            socketio.emit("command_input", data)
            
        # Print the results
        print("Joystick Up:", joystick_up)
        print("Joystick Down:", joystick_down)
        print("Joystick Right:", joystick_right)
        print("Joystick Left:", joystick_left)
        print("Start Pressed:", start_pressed)
        print("Select Pressed:", select_pressed)
        print("A Pressed:", a_pressed)
        print("B Pressed:", b_pressed)


@socketio.on('start_game')
def handle_start_game():
    global gameIsRunning
    gameIsRunning = True

@socketio.on("connect")
def handle_connect():
    print("Client connected")
    # socketio.start_background_task(target=send_data_to_client)

@socketio.on("my event")
def handle_message(data):
    print("received message:", data)

@socketio.on("gameLaunched")
def handle_game_launched(data):
    global gameIsRunning
    game_name = data["gameName"]
    print(f"Game {game_name} has been launched!")
    gameIsRunning = True 
    global socket_server_thread
    print("About to create thread")
    socket_server_thread = threading.Thread(target=unix_socket_server(eventCallback=send_data_to_client))
    socket_server_thread.start()

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