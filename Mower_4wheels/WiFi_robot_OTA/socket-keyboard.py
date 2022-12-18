import socket, time, select, os
from pynput import keyboard
#socket.setdefaulttimeout(1)
HOST = "192.168.0.33"

  

def send_json(msg):
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
   s.connect((HOST, 8000))   
   s.send(bytes(msg, 'utf-8'))
   data = s.recv(2)
   #intby = int.from_bytes(data, "little")
   print(data)
   
def check_ping():
   response = os.system("ping -c 1 192.168.0.33")
   print(response)

# ================ Keyboard press ===================
def on_press(key):

   mag   = 20
   global count
   try:
      print(count,' ialphanumeric key {0} pressed'.format(key.char))
      count = count + 1
   except AttributeError:
      print('special key {0} pressed'.format(key))
      if key == keyboard.Key.up:
         #print ("     ----- Up button pressed")
         send_json('{"mag":' + str(mag) + ',"theta": 0}<')
      elif key == keyboard.Key.left:
         #print ("     ----- Up left pressed")
         send_json('{"mag":' + str(mag) + ',"theta": 270}<') 
      elif key == keyboard.Key.right:
         #print ("     ----- Up right pressed")
         send_json('{"mag":' + str(mag) + ',"theta": 90}<') 
      elif key == keyboard.Key.down:
         #print ("     ----- Up down pressed")
         send_json('{"mag":' + str(mag) + ',"theta": 180}<') 
def on_release(key):
   print('{0} released'.format(key))
   if key == keyboard.Key.esc:
      # Stop listener
      return False
# ================ End of Keyboard press ===================

# Collect events until released
count = 0
with keyboard.Listener(on_press=on_press,on_release=on_release) as listener:
   listener.join(1.0)


