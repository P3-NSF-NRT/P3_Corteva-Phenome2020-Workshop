import picamera
import time
import sys

camera = picamera.PiCamera()

num = int(sys.argv[1])
input_name = sys.argv[2]
output = "photos/" + input_name + "%s.jpg"
for i in range(num):
    time.sleep(1)
    camera.capture(output % i)
