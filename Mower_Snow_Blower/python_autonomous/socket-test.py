import socket, time, select, os
#socket.setdefaulttimeout(1)
HOST = "192.168.11.200"
def test_run():
  
   
   for i in range(3):
      s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
      s.connect((HOST, 8000))       
      #s.settimeout(1)             
      #message = b"\"cmd\":\"test\""
      #print(message)
   
      s.send(bytes("hello world:" + str(i),'utf-8'))
      #data = s.recv(10,socket.
      #print(data)
      #s.setblocking(0)
      #print(ready)
      #time.sleep(0.1)
      data = s.recv(2)
      if data != b'OK':
         time.sleep(1)
         print("sleep for 1 sec")
      #time.sleep(0.1)
      print(data)
   ##s.listen()
   #s.close()
  # data = s.recv(2)
  # print(data)
  
def send_json(msg):
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
   s.connect((HOST, 8000))   
   s.send(bytes(msg, 'utf-8'))
   data = s.recv(2)
   #intby = int.from_bytes(data, "little")
   print(data)
   
def run_for_loop():
   for i in range(10):
      theta = (10*i)%360
      mag   = 20
      send_json('{"mag":' + str(mag) + ',"theta": ' + str(theta) + ',"delay":1000}<')
      print(i ,"  mag:" , mag, " , theta:" , theta)
      time.sleep(0.2)
def check_ping():
   response = os.system("ping -c 1 192.168.0.33")
   print(response)



theta = 240
mag   = 40

run_for_loop()

