import socket
import os
import struct

error_code = 255


# Create a new Unix domain socket
s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Specify the socket file path
socket_path = "/tmp/socket"

# Attempt to unlink the existing socket file
try:
    os.unlink(socket_path)
    print("Unlinked socket")
except OSError:
    print("Socket file does not exist or failed to unlink")
    pass

# Bind the socket to the specified path
try:
    s.bind(socket_path)
    print(f"Server listening on {socket_path}")
except OSError as e:
    print(f"Failed to bind socket: {e}")
    # Handle the error appropriately, possibly by exiting the script

# Listen for incoming connections
s.listen(5)
print("Server listening on", s)

clientIsRunning = False

while True:
    client, client_address = s.accept()
    print("Accepted connection from", client_address)
    clientIsRunning = True
    while clientIsRunning:
        data = client.recv(1)  # Assuming the C client sends 1 byte at a time
        if data:
            value_received = struct.unpack('B', data)[0]
            print("Received data:", value_received)
            if value_received == error_code:
                print("Error handling data")
                # clientIsRunning = False
                #Add a program break here, close all threads and exit application
