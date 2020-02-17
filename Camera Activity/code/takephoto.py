import picamera
import time
import sys

camera = picamera.PiCamera()
#delay = 3
#input_name = "test"
delay = int(sys.argv[1])
input_name = sys.argv[2]
time.sleep(delay)
#camera.capture(output_file)
#camera.start_preview()
#camera.capture("test2.jpg")
camera.capture("photos/%s.jpg" % input_name)
#camera.stop_preview()

