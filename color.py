import cv2
import numpy as np
import cv2.aruco
import socket
from time import sleep


camera = cv2.VideoCapture(0)
arucoDict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_250)
arucoParam = cv2.aruco.DetectorParameters()
arucoDetect = cv2.aruco.ArucoDetector(arucoDict,arucoParam)

min_rgb_color = 0
max_rgb_color = 255

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

kp = 0.7
kd = 1.1
k = 0.8
result = 0
motor = 0
hit = 1

flag = False


def nope():
    pass

cv2.namedWindow("Trackbars")
cv2.createTrackbar("H_min","Trackbars",75,max_rgb_color,nope)
cv2.createTrackbar("H_max","Trackbars",216,max_rgb_color,nope)
cv2.createTrackbar("S_min","Trackbars",138,max_rgb_color,nope)
cv2.createTrackbar("S_max","Trackbars",255,max_rgb_color,nope)
cv2.createTrackbar("V_min","Trackbars",0,max_rgb_color,nope)
cv2.createTrackbar("V_max","Trackbars",23,max_rgb_color,nope)

#udp
UDP_IP = " 192.168.4.1" 
UDP_PORT = 80
r = 25
h = 1

def send_command(r, h):
    message = f"{result:03d}{h}"
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(message.encode(), (UDP_IP, UDP_PORT))


while True:
    r_min = cv2.getTrackbarPos("H_min","Trackbars")
    r_max = cv2.getTrackbarPos("H_max", "Trackbars")
    g_min = cv2.getTrackbarPos("S_min", "Trackbars")
    g_max = cv2.getTrackbarPos("S_max", "Trackbars")
    b_min = cv2.getTrackbarPos("V_min", "Trackbars")
    b_max = cv2.getTrackbarPos("V_max", "Trackbars")

    _,img = camera.read(0)
    #img = cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE)

    hsv = cv2.inRange(cv2.cvtColor(img,cv2.COLOR_BGR2HSV), np.array([b_min,g_min,r_min]), np.array([b_max,g_max,r_max]))
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
                cv2.circle(img, (int(cornes[0][0]),int(cornes[0][1])), 5, (250,3,5), -1)
                cv2.circle(img, (int(cornes[1][0]),int(cornes[1][1])), 5, (250,3,5), -1)
                cv2.circle(img, (int(cornes[2][0]),int(cornes[2][1])), 5, (250,3,5), -1)
                cv2.circle(img, (int(cornes[3][0]),int(cornes[3][1])), 5, (250,3,5), -1)
                cv2.circle(img, (int(cornes_x),int(cornes_y)), 5, (250,3,5), -1)


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
                    x5 = int((cornes_y-b)/k)

                    error = cornes_x - x3 

                    p = error * kp
                    d = error * kd
                    result = int (p + d)

                    if error <= 22 and error >= -22:
                       error = 0
                    
                    if error <=0 and motor // 10 > error and motor > -255:
                        motor -= 255
                    elif 0 <=error and motor // 10 < error and motor < 255:
                        motor += 255
                    flag = True

                    hit = 0
                    errorY = y3 - cornes_y
                    if y3 - cornes_y <= 750:
                        hit = 1

                    print(errorY)
                    print(hit)

                    #(error)    
                cv2.line(img, (x1, y1), (x2, y2), (250, 0, 0), 5)
                cv2.line(img,(x2,y2), (x4, cornes_y), (250, 250, 0), 5)
                # cv2.putText(img, error,(90,70),cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,255), 2)

        else:
            points.clear()
            x1,x2,y1,y2,y3,x4,x5,cornes_x,cornes_y,error,p,d,result,hit   = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

    except:points.clear()

    if len(points) > 10:
        points.pop(0)
    cv2.imshow("IMG", img)
    cv2.imshow("IMG2", hsv)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        result = 0
        break


