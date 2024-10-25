import math

class Robot:
   
 #  def __init__(self):
 #     print("hello")
      
   # ----------- Move ---------
   
   def move_theta(self, mag=10, theta=30, delay=10):
      cmd = '{"mag":' + str(int(mag)) + ',"theta":' + str(int(theta)) +  ',"delay":' + str(delay) + '}<'
      print(cmd)
      return cmd
   
   def move_rad(self, mag=10, rad=0.1, delay=20):
      cmd = '{"mag":' + str(int(mag)) + ',"theta":' + str(int(math.degrees(rad))) +  ',"delay":' + str(delay) + '}<'
      print(cmd)
      return cmd
   
   def spin_rad(self, rad=0.1):
      cmd = '{"YawEnable":1}<'
      print(cmd)
      cmd = '{"YawGo":' + str(int(math.degrees(rad))) + '}<'
      print(cmd)
      cmd = '{"YawEnable":0}<'
      print(cmd)
      return cmd
      
