import cv2

def main():
    # Открываем веб-камеру (0 - это индекс камеры, обычно 0 для встроенной камеры)
    cap = cv2.VideoCapture(0)
    cv2.imshow('Webcam', frame)
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break


