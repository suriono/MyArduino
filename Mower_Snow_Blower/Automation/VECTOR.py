import GPS
import math
from numpy import sign

class VECTOR_class:

    X,Y,Speed,Distance,Angle,WP_index = 0,0,0,10,0,0

    def __init__(self, waypoints=[], lat_ref=0, lon_ref=0):
        self.gps_obj = GPS.GPS_class(lat_ref=lat_ref, lon_ref=lon_ref)
        self.waypoints = waypoints
        self.get_Destination()

    # ------------------------------------------------------------

    def set_Origin(self, waypoint_index=0): # waypoint origin
        self.WP_Xorig, self.WP_Yorig = self.X, self.Y #  waypoint origin

    # ------------------------------------------------------------

    def get_Destination(self, index=0):
        self.dest_lat, self.dest_lon = self.waypoints[index]
        self.X_dest, self.Y_dest = self.gps_obj.get_X_Y(lat=float(self.dest_lat), lon=float(self.dest_lon))

    # ------------------------------------------------------------

    def get_X_Y(self, lat, lon):
        return self.gps_obj.get_X_Y(lat=lat, lon=lon)

    # ------------------------------------------------------------

    def update_Location(self): # , x, y):
        #self.X, self.Y = x, y
        XY_to_dest = [self.X_dest-self.X, self.Y_dest-self.Y]                     # X,Y to destination vector
        self.Distance = math.sqrt(XY_to_dest[0]*XY_to_dest[0] + XY_to_dest[1]*XY_to_dest[1])
        self.XY_to_dest_rad = math.atan2(XY_to_dest[0], XY_to_dest[1])
        self.XY_to_dest_deg = self.normalize_angle(math.degrees(self.XY_to_dest_rad))  # current location to destination angle
        orig_dest  = self.normalize_vector(self.X_dest-self.WP_Xorig, self.Y_dest-self.WP_Yorig)
        waypoint_angle_rad = math.atan2(orig_dest[0], orig_dest[1])
        self.waypoint_angle_deg = self.normalize_angle(math.degrees(waypoint_angle_rad))
        self.path_distance = self.cross_product(orig_dest,XY_to_dest)  # distance to the waypoint path
        self.delta_Angle_and_waypoint_angle = self.normalize_angle(self.waypoint_angle_deg-self.Angle)
        self.delta_Angle_and_dest_deg = self.normalize_angle(self.XY_to_dest_deg-self.Angle)

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

    # -------------------- WayPoints ------------------------------------

    def WayPoint_Go(self, speed=0):
        self.Speed = speed
        print("------------- WayPoint_Go index:", self.WP_index, " speed:", self.Speed)
        self.set_Origin()
        self.get_Destination(index=self.WP_index)
        self.update_Location()

    def step_to_Location(self, is_Simulation=True):
        min_mag, max_mag = 12, self.Speed
        simulation_max_angle = 13
        
        self.update_Location()

        if is_Simulation:
            if abs(self.delta_Angle_and_dest_deg) > 45:  # more than 45-degree then spin first
                del_deg = sign(math.sin(self.XY_to_dest_rad)) * simulation_max_angle
                print("====== spin to destination ===:", self.XY_to_dest_rad, self.XY_to_dest_deg)
                self.Angle = self.normalize_angle(self.Angle + del_deg)
            else:
                PID_D, PID_I = 0.5, 0.1
                #if abs(self.Distance) > 0.5:     # farther
                del_deg = self.normalize_angle(self.delta_Angle_and_waypoint_angle*PID_D - 10 * sign(self.path_distance))
                #    print("===== distance > 0.5")
                #else:
                #    del_deg = 0    # temporary

                if abs(del_deg) > 30: del_deg = 30 * sign(del_deg)    # to limit the turn to stay on path, so it does not turn too much

                self.Angle = self.normalize_angle(self.Angle + del_deg)
                self.X += 0.08 * self.Speed * math.sin(math.radians(self.Angle))
                self.Y += 0.08 * self.Speed * math.cos(math.radians(self.Angle))
            print("Angle:",self.Angle, "D:",self.Distance)

        else:     # move robot step to waypoint

            print("Angle:", self.Angle, "D:", self.Distance)