import picamera
import time
import sys

camera = picamera.PiCamera()

delay = int(sys.argv[1])
input_name = sys.argv[2]

time.sleep(delay)
camera.start_recording("videos/%s.h264" % input_name)
camera.stop_recording()
