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
      #del_Y = self.Radius * math.sin(del_lat)
      #print("Delta: ", del_lat, del_lon)
      return del_X, del_Y
      
   
if __name__ == "__main__":
   
      coord_obj = GPS_Coordinate(lat_ref=44.747035800, lon_ref=-93.1937129634583)
      print(coord_obj.get_X_Y(lat=44.74713533018441, lon=-93.19377913074761))
      #print(coord_obj.X_scale)
     # print(math.radians(180))