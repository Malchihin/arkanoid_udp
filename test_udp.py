import socket

def nope(x):
    pass

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_name = ('192.168.4.1', 80)
server_socket.connect(server_name)

speed = 1
solenoid_state = 1

data = f"{speed},{solenoid_state}\n"

server_socket.send(data.encode("utf-8"))

server_socket.close()
