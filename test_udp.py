import socket

HOST = '192.168.4.1' 
PORT = 80           
while True:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    
    speed = 12 
    relay = 1   
    
    data = [speed // 100, (speed % 100) // 10, speed % 10, relay]
    
    s.sendall(bytes(data))
