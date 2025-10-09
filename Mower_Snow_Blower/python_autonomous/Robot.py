import math, wifi_class, time

class Robot:
   Yaw, Offset_Yaw, mag, theta, delay = 0, 0, 0, 0, 200
   cmd = ''
   
   def __init__(self, host="192.168.11.200", port=8000):
      self.wifi_obj = wifi_class.wifi_class(host=host, port=port)
      
   def normalize_angle(self, angle):
      return (int(angle) + 180) % 360 - 180
   
   def rad_to_angle(self, rad):
      return self.normalize_angle(math.degrees(rad))
      
   # ----------- Move ---------
   
   def move_theta(self, mag=10, theta=30, delay=100, is_Simulation=True):
      self.cmd = '{"mag":' + str(int(mag)) + ',"theta":' + str(self.normalize_angle(theta)) +  ',"delay":' + str(delay) + '}<'
      if not is_Simulation:
         print("Cmd: ", self.cmd)
         return self.send_Command(cmd=self.cmd, timeout=delay/1000.0)
         #time.sleep(delay/1000.0)
      return True
   
   # -------------------- input mag, theta, and delay to move robot ---------------------
   
   def move_Robot(self):
      self.cmd = '{"mag":' + str(int(self.mag)) + ',"theta":' + str(self.normalize_angle(self.theta)) +  ',"delay":' + str(self.delay) + '}<'
      return self.send_Command(cmd=self.cmd, timeout=self.delay/1000.0)
   
   def move_rad(self, mag=10, rad=0.1, delay=20):
      cmd = '{"mag":' + str(int(mag)) + ',"theta":' + str(self.rad_to_angle(rad)) +  ',"delay":' + str(delay) + '}<'
      return cmd
   
   def spin_rad(self, rad=0.1):
      cmd = '{"YawEnable":1}<'
      print(cmd)
      cmd = '{"YawGo":' + str(self.rad_to_angle(rad)) + '}<'
      print(cmd)
      cmd = '{"YawEnable":0}<'
      print(cmd)
      return cmd
   
   # ------------------------------------------------------------------------
   
   def spin_Angle_Target(self, theta=0, is_Simulation=True):
      if not is_Simulation:
         self.send_Command(cmd='{"YawEnable":1}<') 
         cmd = '{"YawGo":' + str(self.normalize_angle(-theta)) + '}<'
         for i in range(10):
            if not self.send_Command(cmd):
             #  print("Yaw: ", self.Yaw)
            #else:
               break
            #print("JSON returned: ", self.in_json)
            time.sleep(1)
      
        # time.sleep(10)

         self.send_Command(cmd='{"YawEnable":0}<')
         
      return '{"YawGo":' + str(self.normalize_angle(theta)) + '}<'
   
   # -----------------------------------------------------------------------
         
   def yaw_Offset(self, theta=0, is_Simulation=True):
      if not is_Simulation:
         self.Offset_Yaw = theta - self.Yaw
         self.send_Command(cmd='{"YawOffset":' + str(theta) + '}<')

   # ------------------------------------------------------------------------
   
   def send_Command(self, cmd='', timeout=1.0):
      self.wifi_obj.send_Message(cmd)
      #self.wifi_obj.set_Timeout(timeout)
      if self.wifi_obj.listen_Wifi(16): 
         self.in_json = self.wifi_obj.read_json
         #print(self.in_json)
         try:
            self.Yaw = -self.in_json['yaw']    # yaw is the opposite direction
#            print("Yaw: ", self.Yaw)
            return True
         except:
            return False
         
   # ------------------------------------------------------------------------
   
   def get_Yaw(self):  # only to get Yaw, send stop command
      self.send_Command(cmd='{"cmd":"getyaw"}') # motorstop"}')
      return self.Yaw
      
      
# ==================== Testing ====================
#if __name__ == "__main__":
   #robot_obj = Robot("192.168.11.200", 8000)
   #robot_obj.spin_Theta(theta=60, is_Simulation=False)
   
   
  
   #robot_obj.move_theta(mag=30, theta=-90, delay=1000, is_Simulation=False)
   
   #Outstr  = '{"mag":20, "theta": 90, "delay": 10}<'
   #cmd = robot_obj.move_theta(mag=20, theta=0, delay=1000, is_Simulation=False)
   
   #if robot_obj.send_Command(cmd):
   #   print("Yaw: ", robot_obj.Yaw)
