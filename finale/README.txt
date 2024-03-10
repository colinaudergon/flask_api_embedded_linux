File structure description:

rock5b webBoy
server:
    - app.py | main server application, provides static html web page, opens sockets with web client, handles socket connection from gpiohandler C programm
    - /tmp/ folder containig socket location
    - unix_socket_server.py | unix socket connection with compiled programm for gpio processing
    - image_fetcher.py: image fetcher from github API, no need to be used anymore

    - frameBufferHandler: precompiled C code, copies image to screen
    - inputReader: precompiled C code, handles user inputs on the rock5b board

- client: 
    - index.html: index of webpage
    - test.js: js file containig function for socket connection with server as well as call to nostalgist.js, and fetch games from github
    - style.css: simple css style for index.html

Dependency:
python:
    server:
        flask:
        pip install Flask
        flask_socketio:
        pip install Flask-SocketIO
    UI Image processing:
    Pillow:
        python3 -m pip install --upgrade Pillow
    Numpy:
        pip install numpy


runApp.sh is the entry point, run:
chmod +x runApp
Then:
./runApp.sh

It will install the required dependencies and start the application.
The rock5b screen will display its IP address, on the desired screen/client type this IP address on your browser navigation bar.
The games available should be displayed. Select a game and enjoy!   