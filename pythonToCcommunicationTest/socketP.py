import socket
import subprocess
import os
import time


current_directory = os.getcwd()
print(current_directory)

socket_path = "/tmp/demo_socket"

# Attempt to unlink the existing socket file
try:
    os.unlink(socket_path)
    print("Unlinked socket")
except OSError:
    print("Socket file does not exist or failed to unlink")
    pass

# Create a new socket
s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Bind the socket to the specified path
try:
    s.bind(socket_path)
    print(f"Server listening on {socket_path}")
except OSError as e:
    print(f"Failed to bind socket: {e}")
    # Handle the error appropriately, possibly by exiting the script


# #calls C function
# c_program_path = os.path.join(current_directory, "pythonToCcommunicationTest", "socketC")
# subprocess.call(c_program_path)
# Listen for incoming connections
s.listen(5)
print("Server listening on", s)


# c_program_path = os.path.join(current_directory, "pythonToCcommunicationTest","helloPython", "hellopython")


exitCode="exit"
connection=True


while True:
    client, client_address = s.accept()
    print("Accepted connection from", client_address)

    while connection:
        # print("looping!")
        # Receive and send back data
        data = client.recv(1024)
        # print(f"Connection state: ",connection)
        if data:
            decoded = data.decode()
            print("Received data:", decoded)
            if(decoded != exitCode):
                print("Received data:", decoded)            
            else:
                connection = False
    client.close()
