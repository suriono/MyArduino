import socket, json

class wifi_class:
   
   def __init__(self, host="192.168.11.200", port=8000):
      self.host, self.port = host, port
   
   def connect_Wifi(self):
      self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
      self.sock.connect((self.host, self.port))  
   
   def send_Message(self, msg='hello'):
      self.connect_Wifi()
      self.sock.send(bytes(msg + '<', 'utf-8'))
      
   def listen_Wifi(self, maxBytes=64):
      data = self.sock.recv(maxBytes)
      try:
         self.read_json = json.loads(data.decode('utf-8').replace('<',''))
         return True
      except:
         return False
      
   def close_Connection(self):
      self.sock.close()
   
# ==================== Testing ====================
if __name__ == "__main__":
   wifi_obj = wifi_class("192.168.0.27", 8000)
   for i in range(100):
      wifi_obj.send_Message('{"cmd":"get_gps"}')
      if wifi_obj.listen_Wifi(64): 
         in_json = wifi_obj.read_json
         print(in_json)
         lat, lon, prec = in_json['lat'], in_json['lon'], in_json['prec']
         print(lat, lon, prec)
   wifi_obj.close_Connection()