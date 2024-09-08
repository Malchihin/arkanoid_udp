import cv2
import numpy as np
import cv2.aruco
import socket
from time import sleep
import keyboard

camera = cv2.VideoCapture(0)
arucoDict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_250)
arucoParam = cv2.aruco.DetectorParameters()
arucoDetect = cv2.aruco.ArucoDetector(arucoDict,arucoParam)

points = [[0,0]]
x2,y2 = 0,0
x1,y1 = 0,0
y3 = 0

x3 = 0
y4 = 0
x4 = 0
x5 = 0

cornes_y = 480
cornes_x = 0
robotID = 8

motor = 0
hit = 0

flag = False

#udp
UDP_IP = "192.168.1.100" 
UDP_PORT = 8888

def send_command(motor, hit):
    message = f"{motor:03d}{hit}"
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(message.encode(), (UDP_IP, UDP_PORT))

if keyboard.is_pressed("d"):
        motor = -255
        flag = True
        print('D')

while True:
    if keyboard.is_pressed("a"):
        motor = 255
        flag = True
        print('A')
    
    if keyboard.is_pressed("c"):
        motor = 0
        flag = True
        print('C')
    if keyboard.is_pressed("s"):
        hit = 1
        flag = True
        print('S')
    if keyboard.is_pressed("x"):
        hit = 0
        flag = True
        print('X')

    _,img = camera.read(1)
    hsv = cv2.inRange(cv2.cvtColor(img,cv2.COLOR_BGR2HSV), (0,48,87),(12, 255, 153))#gfj gb  gddgt gfhbatfghf  gdfgwhytl ;gfhtfhcfsdeq;fhghbdtn rfg rdffhhuqpgfhjqpdgw
    mom = cv2.moments(hsv, 255)

    try:
        markerCornes, markerID, markerNo = arucoDetect.detectMarkers(cv2.cvtColor(img,cv2.COLOR_BGR2GRAY))
        if markerID is not None:
            if markerID == robotID:
                ID = str(markerID)
                cornes = markerCornes[0][0]
                cornes_x = int((cornes[0][0]+cornes[1][0]+cornes[2][0]+cornes[3][0]) / 4)
                cornes_y = int((cornes[0][1]+cornes[1][1]+cornes[2][1]+cornes[3][1]) / 4)
                #print(cornes_y)
                #print(cornes_x)
                #print(str(markerID))


                cv2.putText(img, ID,(90,70),cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,255), 2)
                cv2.circle(img, (int(cornes[0][0]),int(cornes[0][1])), 20, (250,3,5), -1)
                cv2.circle(img, (int(cornes[1][0]),int(cornes[1][1])), 20, (250,3,5), -1)
                cv2.circle(img, (int(cornes[2][0]),int(cornes[2][1])), 20, (250,3,5), -1)
                cv2.circle(img, (int(cornes[3][0]),int(cornes[3][1])), 20, (250,3,5), -1)
                cv2.circle(img, (int(cornes_x),int(cornes_y)), 20, (250,3,5), -1)


        x_m = mom['m10']
        y_m = mom['m01']
        area = mom['m00']
        x = int(x_m / area)
        y = int(y_m / area)

        if len(points) <=50:
            points.append([x, y])
            if len(points) > 1:
                for i in range(1,len(points)):
                    cv2.line(img,(points[i-1][0],points[i-1][1]),(points[i][0],points[i][1]),(0,0,0),3)
                if len(points) == 11:
                    x1 = points[10][0]
                    y1 = points[10][1]
                    #y = k*x + b ; k = (y2-y1)/(x2-x1) ; b = y1 - k*x1
                    delta_y = points[10][1]-points[0][1]
                    delta_x = points[10][0]-points[0][0]
                    k = delta_y/delta_x
                    b = -k*points[0][0]+points[0][1]
                    if points[10][0] > points[0][0]:
                        x2 = 640
                    else:
                        x2 = 0
                    y2 = int(k * x2 + b)
                    a = y2 - y1
                    y3 = y2 + a
                    x3 = x1
                    delta_y2 = y3-y2
                    delta_x2 = x3-x2
                    k2 = delta_y2/delta_x2
                    b2 = -k2 * x2 + y2
                    x4 = int((y3 - b2) / k2)
                    #x5 = int((cornes_y-b)/k
                    error = cornes_x - x3
                cv2.line(img, (x1, y1), (x2, y2), (250, 0, 0), 5)
                cv2.line(img,(x2,y2), (x4, cornes_y), (250, 250, 0), 5)
                # cv2.putText(img, error,(90,70),cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,255), 2)



        else:
            points.clear()
            x1,x2,y1,y2,y3,x4,x5,cornes_x,cornes_y= 0,0,0,0,0,0,0,0,0

    except:
        points.clear()

    if len(points) >10:
        points.pop(0)
    cv2.imshow("IMG", img)
    cv2.imshow("IMG2", hsv)

    if cv2.waitKey(1) == ord("q"):
        break


