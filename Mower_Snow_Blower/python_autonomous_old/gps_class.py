import wifi_class, math

class gps:
   Radius   = 6378137    # Earth radius a given local geographic location
   rtk, rtk_type, prec = 0, "No Solution", 0.0
   
   def __init__(self, host="192.168.11.201", port=8000,lat_ref=0.0, lon_ref=0.0):
      self.wifi_obj = wifi_class.wifi_class(host, port)
      self.lat_ref, self.lon_ref, self.lat, self.lon = lat_ref, lon_ref, lat_ref, lon_ref  # reference GPS for X,Y=0,0
      self.X_scale = self.Radius * math.cos(math.radians(self.lat_ref))
      
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
      
   # --------------- Conversion X,Y and Lattitude, Longitude   
   
   def get_X_Y(self,lat,lon):
      del_lat, del_lon = lat - self.lat_ref, lon - self.lon_ref
      del_X = self.X_scale * math.sin(math.radians(del_lon))
      del_Y = self.Radius * math.sin(math.radians(del_lat))
      return del_X, del_Y

   def get_Lat_Lon_from_X_Y(self, x=0, y=0):
      lat = math.degrees(math.asin(y / self.Radius)) + self.lat_ref
      lon = math.degrees(math.asin(x/self.X_scale))  + self.lon_ref
      return lat, lon
      
# ==================== Testing ====================
if __name__ == "__main__":
   gps_obj = gps("192.168.11.201", 8000)
   
   for i in range(10):
      if gps_obj.get_GPS():
         #in_json = gps_obj.read_json
         print(i, gps_obj.lat, gps_obj.lon, gps_obj.prec/10, "mm")
         #lat, lon, prec = in_json['lat'], in_json['lon'], in_json['prec']
   if gps_obj.get_RTK():
      print("RTK: ", gps_obj.rtk, " type: ", gps_obj.rtk_type)
      
   gps_obj.close_Connection()
