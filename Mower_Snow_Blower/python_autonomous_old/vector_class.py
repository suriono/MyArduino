import gps_class
import math

class VECTOR:

    X,Y,distance = 0,0,10

    def __init__(self, waypoints=[], lat_ref=0, lon_ref=0):
        self.gps_obj = gps_class.gps(lat_ref=lat_ref, lon_ref=lon_ref)
        self.waypoints = waypoints
        self.get_Destination()

    # ------------------------------------------------------------

    def set_Origin(self, waypoint_index=0): #,x_orig=0, y_orig=0):
        #if waypoint_index == 0: # going to the first waypoint
        self.X_orig, self.Y_orig = self.X, self.Y # set current location x_orig, y_orig
        #else:
        #    self.X_orig, self.Y_orig = self.X_dest, self.Y_dest   # previous destination

    # ------------------------------------------------------------

    def get_Destination(self, index=0):
        self.dest_lat, self.dest_lon = self.waypoints[index]
        self.X_dest, self.Y_dest = self.gps_obj.get_X_Y(lat=float(self.dest_lat), lon=float(self.dest_lon))

    # ------------------------------------------------------------

    def update_Location(self, x, y):
        self.X, self.Y = x, y
        XY_to_dest = [self.X_dest-self.X, self.Y_dest-self.Y]
        self.distance = math.sqrt(XY_to_dest[0]*XY_to_dest[0] + XY_to_dest[1]*XY_to_dest[1])
        orig_dest  = self.normalize_vector(self.X_dest-self.X_orig, self.Y_dest-self.Y_orig)
        self.waypoint_angle_rad = math.atan2(orig_dest[0], orig_dest[1])
        self.waypoint_angle_deg = self.normalize_angle(math.degrees(self.waypoint_angle_rad))
        self.path_distance = self.cross_product(orig_dest,XY_to_dest)  # distance to the waypoint path

    # -------------------- Math ----------------------------------------

    def normalize_vector(self, x, y):
        magnitude = math.sqrt(x ** 2 + y ** 2)
        if magnitude == 0:
            return (0, 0)  # Avoid division by zero
        return (x / magnitude, y / magnitude)

    def cross_product(self,a, b):
        return a[0] * b[1] - a[1] * b[0]

    def rad_to_angle(self, rad):
        return self.normalize_angle(math.degrees(rad))

    def normalize_angle(self, angle):
        return (int(angle) + 180) % 360 - 180
