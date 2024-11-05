import wifi_class

class gps_class:
   
   def __init__(self, host="192.168.11.201", port=8000):
      self.wifi_obj = wifi_class.wifi_class(host, port)
      
   def get_GPS(self):
      self.wifi_obj.send_Message('{"cmd":"get_gps"}')
      if self.wifi_obj.listen_Wifi(64): 
         in_json = self.wifi_obj.read_json
         self.lat, self.lon, self.prec = in_json['lat'], in_json['lon'], in_json['prec']
         return True
      return False
   
   def get_RTK(self):
      self.wifi_obj.send_Message('{"cmd":"get_rtk"}')
      if self.wifi_obj.listen_Wifi(16): 
         in_json = self.wifi_obj.read_json
         self.rtk = in_json['rtk']
         if self.rtk == 0: 
            self.rtk_type = "No Solution"
         elif self.rtk == 1:
            self.rtk_type = "Floating fix"
         else:
            self.rtk_type = "High precision fix"
         return True
      return False
   
   def close_Connection(self):
      self.wifi_obj.close_Connection()
      
# ==================== Testing ====================
if __name__ == "__main__":
   gps_obj = gps_class("192.168.11.201", 8000)
   
   for i in range(10):
      if gps_obj.get_GPS():
         #in_json = gps_obj.read_json
         print(i, gps_obj.lat, gps_obj.lon, gps_obj.prec)
         #lat, lon, prec = in_json['lat'], in_json['lon'], in_json['prec']
   if gps_obj.get_RTK():
      print("RTK: ", gps_obj.rtk, " type: ", gps_obj.rtk_type)
      
   gps_obj.close_Connection()
