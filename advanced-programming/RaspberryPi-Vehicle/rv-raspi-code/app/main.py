#!/usr/bin/env python
# coding: utf-8

import threading
import time
import numpy as np
import cv2

shouldRun = [0] * 5

def color_identification():
    global shouldRun
    shouldRun[1] = 1
    red_lower = np.array([0, 43, 46])
    red_upper = np.array([10, 255, 255])
    green_lower = np.array([35, 43, 46])
    green_upper = np.array([77, 255, 255])
    cap = cv2.VideoCapture(0)
    cap.set(3, 1280)
    cap.set(4, 720)

    def chestRed():
        ret, frame = cap.read()
        frame = cv2.GaussianBlur(frame, (5, 5), 0)
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, red_lower, red_upper)
        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.GaussianBlur(mask, (3, 3), 0)
        cnts = cv2.findContours(
            mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
        if 20 < len(cnts) < 30:
            print("Red!")
            return True
        return False

    def chestGreen():
        ret, frame = cap.read()
        frame = cv2.GaussianBlur(frame, (5, 5), 0)
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, green_lower, green_upper)
        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.GaussianBlur(mask, (3, 3), 0)
        cnts = cv2.findContours(
            mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
        if 25 < len(cnts) < 29:
            print("Green!")
            return True
        return False

    while (shouldRun[1]):
        res_red = chestRed()
        res_green = chestGreen()
        if (res_red and (not res_green)): print("execute stop_speed 0")
        else: print("execute stop_speed 1")
    cap.release()


def main():
    color_identification_thread = threading.Thread(
        target=color_identification, name='color_identification', daemon=True)

    while True:
        args = input().split()
        if (args[0] == 'color_identification'):
            if (int(args[1])):
                color_identification_thread.start()
            else:
                global shouldRun
                shouldRun[1] = 0


if __name__ == "__main__":
    main()
