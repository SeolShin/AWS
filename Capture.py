# coding: utf-8

# An example using startStreams

import threading
from threading import Thread, Lock
import numpy as np
import cv2
import sys
from pylibfreenect2 import Freenect2, SyncMultiFrameListener
from pylibfreenect2 import FrameType, Registration, Frame
from pylibfreenect2 import CpuPacketPipeline

class Test(threading.Thread):

	def makename(self,filetype, count):
		dir_name = "/home/snow/Workspace/FCN/result/"
		filename = filetype + "/" + filetype + str(count) + "jpg"
		path = dir_name + filename
		return path

	def run(self):
		pipeline = CpuPacketPipeline()
		print("Packet pipeline:", type(pipeline).__name__)

		enable_rgb = True
		enable_depth = True

		fn = Freenect2()
		num_devices = fn.enumerateDevices()
		if num_devices == 0:
		    print("No device connected!")
		    sys.exit(1)

		serial = fn.getDeviceSerialNumber(0)
		device = fn.openDevice(serial, pipeline=pipeline)

		types = 0
		if enable_rgb:
		    types |= FrameType.Color
		if enable_depth:
		    types |= (FrameType.Ir | FrameType.Depth)
		listener = SyncMultiFrameListener(types)

		# Register listeners
		device.setColorFrameListener(listener)
		device.setIrAndDepthFrameListener(listener)

		if enable_rgb and enable_depth:
		    device.start()
		else:
		    device.startStreams(rgb=enable_rgb, depth=enable_depth)

		# NOTE: must be called after device.start()
		if enable_depth:
		    registration = Registration(device.getIrCameraParams(),
				                device.getColorCameraParams())

		undistorted = Frame(512, 424, 4)
		registered = Frame(512, 424, 4)
		
		count = 0		
		while True:
		    mutex = Lock()
		    mutex.acquire()
		    frames = listener.waitForNewFrame()

		    if enable_rgb:
			color = frames["color"]
		    if enable_depth:
			ir = frames["ir"]
			depth = frames["depth"]

		    if enable_rgb and enable_depth:
			registration.apply(color, depth, undistorted, registered)
		    elif enable_depth:
			registration.undistortDepth(depth, undistorted)

		    if enable_depth:
			cv2.imshow("ir", ir.asarray() / 65535.)
			nameir = self.makename("ir", count)
			#cv2.imwrite(nameir, ir.asarray() / 65535.)

			cv2.imshow("depth", depth.asarray() / 4500.)
			named = self.makename("depth", count)
			#cv2.imwrite(named, depth.asarray() / 4500.)

			cv2.imshow("undistorted", undistorted.asarray(np.float32) / 4500.)
			nameu = self.makename("undistorted", count)
			#cv2.imwrite(nameu, undistorted.asarray(np.float32) / 4500.)
		    if enable_rgb:
			cv2.imshow("color", cv2.resize(color.asarray(),(int(1920 / 3), int(1080 / 3))))
			namec = self.makename("color", count)
			#cv2.imwrite(namec,cv2.resize(color.asarray(),(int(1920 / 3), int(1080 / 3))))
		    if enable_rgb and enable_depth:
			cv2.imshow("registered", registered.asarray(np.uint8))
			namer = self.makename("registered", count)
			#cv2.imwrite(namer,registered.asarray(np.uint8)) 
    		    mutex.release()

		    count = count + 1

		    listener.release(frames)

		    key = cv2.waitKey(delay=1)
		    if key == ord('q'):
			break


		device.stop()
		device.close()
		#cv2.destroyAllwindows()


#send = Test()
#send.start()

