import socket, time, select, os
from pynput import keyboard
#socket.setdefaulttimeout(1)
HOST = "192.168.4.1"

  
class Motor:
   last_msg    = 0
   elapsed_min = 0.4
   motor_delay = 200
   
   def __init__(self):
      self.count = 0
      with keyboard.Listener(on_press=self.onpress, on_release=self.onrelease) as listener:
         listener.join()
   
   # ------- send message/command to the motor
   def send_json(self,msg):
      s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
      s.connect((HOST, 8000))   
      s.send(bytes(msg, 'utf-8'))
      data = s.recv(2)
   
   def check_ping(self):
      response = os.system("ping -c 1 192.168.0.33")
      print(response)

   # ================ Keyboard press ===================
   def onpress(self, key):
      mag   = 20
      
      try:
         print(self.count,' ialphanumeric key {0} pressed'.format(key.char))
         self.count += 1
      except AttributeError:
         
         time_elapsed = time.time() - self.last_msg
         
         # ------- only runs after specific minimum interval -------
         if time_elapsed > self.elapsed_min:
            self.last_msg = time.time()
            print("last time: ", self.last_msg, ", Elapsed: ", time_elapsed)
            
            print('special key {0} pressed'.format(key))
            if key == keyboard.Key.up:
               #print ("     ----- Up button pressed")
               self.send_json('{"mag":' + str(mag) + ',"theta": 0,"delay":' + str(self.motor_delay) + '}<')
            elif key == keyboard.Key.left:
               #print ("     ----- Up left pressed")
               self.send_json('{"mag":' + str(mag) + ',"theta": 270,"delay":' + str(self.motor_delay) + '}<')
            elif key == keyboard.Key.right:
               #print ("     ----- Up right pressed")
               self.send_json('{"mag":' + str(mag) + ',"theta": 90,"delay":' + str(self.motor_delay) + '}<')
            elif key == keyboard.Key.down:
               #print ("     ----- Up down pressed")
               self.send_json('{"mag":' + str(mag) + ',"theta": 180,"delay":' + str(self.motor_delay) + '}<')
   def onrelease(self, key):
      print('{0} released'.format(key))
      if key == keyboard.Key.esc:
         # Stop listener
         return False
      else:
         self.send_json('{"cmd":"motorstop"}<')
# ================ End of Keyboard press ===================

motor_obj = Motor()



