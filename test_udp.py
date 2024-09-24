import socket

def send_data(ip, port, data):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((ip, port))
        s.sendall(data)
        response = s.recv(1024)
        print("Received:", response.decode())

ip = "192.168.4.1"  
port = 80
data = '1234' 

send_data(ip, port, data)
