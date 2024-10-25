import math

class GPS_Coordinate:
   Radius = 6378137
 
   
   def __init__(self, lat_ref, lon_ref):
      self.lat_ref, self.lon_ref = lat_ref, lon_ref
      self.X_scale = self.Radius * math.cos(math.radians(self.lat_ref))
      
      
   def get_X_Y(self,lat,lon):
      del_lat, del_lon = lat - self.lat_ref, lon - self.lon_ref
      del_X = self.X_scale * math.sin(math.radians(del_lon))
      del_Y = self.Radius * math.sin(math.radians(del_lat))
      return del_X, del_Y
   
   def get_Lat_Lon_from_X_Y(self, x=0, y=0):
      lat = math.degrees(math.asin(y / self.Radius)) + self.lat_ref
      lon = math.degrees(math.asin(x/self.X_scale))  + self.lon_ref
     # print("lat, Lon: ", lat, lon)
      return lat, lon
   
if __name__ == "__main__":
   
      coord_obj = GPS_Coordinate(lat_ref=44.747035800, lon_ref=-93.1937129634583)
      print(coord_obj.get_X_Y(lat=44.74713533018441, lon=-93.19377913074761))
      #print(coord_obj.X_scale)
     # print(math.radians(180))