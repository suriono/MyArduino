import cv2, math, json, GPS_coordinate, Robot, time, gps_class
import tkinter as tk
from tkinter import PhotoImage
from numpy import sign

class GUI:
   image_path  = r'C:\Users\UZMN\OneDrive - SkyWater Technology\Documents\GitHub\MyArduino\Mower_4wheels\House-google-map.png'
   
   # ---- PID ---------------
   PID_rad_D_coef, PID_rad_I_coef, PID_rad_I = 0.2, 0.01, 0.0
   delta_angle = 0.0
   robot_obj, gps_obj = Robot.Robot(), gps_class.gps_class()
   Data = {}
   
   # ---- Frames ------------
   root = tk.Tk()
   leftframe, rightframe, simframe,robotcmdframe = tk.Frame(root), tk.Frame(root), tk.Frame(root), tk.Frame(root)
   coordframe, checkframe, moveframe = tk.Frame(rightframe), tk.Frame(rightframe), tk.Frame(rightframe) 
   angleframe = tk.Frame(rightframe)
   leftframe.pack(side=tk.LEFT)
   rightframe.pack(side=tk.TOP)
   coordframe.pack(side=tk.TOP)
   checkframe.pack(side=tk.TOP)
   moveframe.pack(side=tk.TOP)
   simframe.pack(side=tk.TOP)
   robotcmdframe.pack(side=tk.TOP)
   angleframe.pack(side=tk.TOP)
   
   # ---- widget variables ---------
   entryLat, entryLon, entrySpeed, entryAngle, labelCoord, is_Simulation = tk.DoubleVar(), tk.DoubleVar(), tk.IntVar(), tk.IntVar(), tk.StringVar(), tk.BooleanVar()
   labelCommand, labelDistance, entryTarget_angle, labelYaw, entryOffsetYaw = tk.StringVar(), tk.StringVar(), tk.IntVar(), tk.StringVar(), tk.IntVar()
   
   # ----------------------------- init ---------------
   
   def __init__(self, img_width=440, img_height=600, ref_Xpixel=100, ref_Ypixel=400, pixel_scale=19.8, lat_ref=0.0, lon_ref=0.0, waypoints=[]):
      self.img_width, self.img_height, self.ref_Xpixel, self.ref_Ypixel, self.pixel_scale = img_width, img_height, ref_Xpixel, ref_Ypixel, pixel_scale
      self.lat_ref, self.lon_ref, self.lat, self.lon, self.angle, self.X, self.Y = lat_ref, lon_ref, lat_ref, lon_ref, 0.0, 0.0, 0.0
      self.waypoints, self.waypoint_count = waypoints, 0
      
      self.GPS_obj = GPS_coordinate.GPS_Coordinate(lat_ref=lat_ref, lon_ref=lon_ref)
      
      # ---- canvas for the map -------
      self.canvas = tk.Canvas(self.leftframe, width=img_width, height=img_height)
      self.canvas.pack()
      self.image = PhotoImage(file=self.image_path)  # Replace with the path to your image file
      self.canvas.create_image(img_width, img_height, image=self.image, anchor='se')  # Position the image at the center of the canvas
      
      # ---- coordinates ---------------
      tk.Label(self.coordframe,textvariable=self.labelCoord).pack(side=tk.LEFT)
      
      tk.Label (self.checkframe,text="Test Lattitude").pack(side=tk.LEFT)
      tk.Entry (self.checkframe, textvariable=self.entryLat, width=18).pack(side=tk.LEFT)
      tk.Label (self.checkframe,text="Test Longitude").pack(side=tk.LEFT)
      tk.Entry (self.checkframe, textvariable=self.entryLon, width=18).pack(side=tk.LEFT)
      tk.Button(self.checkframe, text ="View test location", command=self.check_Location).pack(side=tk.LEFT)
      
      # ---- speed and initial direction ------------
      tk.Label (self.moveframe,text="Set Speed").pack(side=tk.LEFT)
      tk.Entry (self.moveframe, textvariable=self.entrySpeed, width=6).pack(side=tk.LEFT)
      tk.Label (self.moveframe,text="Rotate").pack(side=tk.LEFT)
      tk.Entry (self.moveframe, textvariable=self.entryAngle, width=6).pack(side=tk.LEFT)
      tk.Button(self.moveframe, text ="Test Move",  command = self.move_Test).pack(side=tk.LEFT)
      tk.Button(self.moveframe, text ="Step to test location", command=self.step_to_Location).pack(side=tk.LEFT)
      tk.Button(self.moveframe, text="Next Waypoint", command=self.next_WayPoint).pack(side=tk.LEFT)
      
      # ----- simulation to waypoints -----------------
      tk.Button     (self.simframe, text="Get Coordinates", command= self.get_Coordinates).pack(side=tk.LEFT)
      tk.Checkbutton(self.simframe, text='Simulation',variable=self.is_Simulation, onvalue=1, offvalue=0).pack(side=tk.LEFT)
      tk.Button     (self.simframe, text="Go to waypoints", command= self.sim_WayPoints).pack(side=tk.LEFT)
      tk.Button     (self.simframe, text="Get RTK Mode", command= self.get_RTK).pack(side=tk.LEFT)
      tk.Label      (self.simframe, textvariable=self.labelDistance).pack(side=tk.LEFT)
   
      # ----- Robot command ---------------------------
      tk.Label(self.robotcmdframe, text="Command sent to robot: ").pack(side=tk.LEFT)
      tk.Label(self.robotcmdframe, textvariable=self.labelCommand).pack(side=tk.LEFT)
      
      # ----- Angle command ----------------------------
      tk.Button(self.angleframe, text="Set Current Angle:", command= lambda: self.set_Current_Angle(self.entryOffsetYaw.get())).pack(side=tk.LEFT)
      tk.Entry (self.angleframe, textvariable=self.entryOffsetYaw, width=4).pack(side=tk.LEFT)
      tk.Label (self.angleframe, textvariable=self.labelYaw). pack(side=tk.LEFT)
      tk.Label (self.angleframe, text="Target angle"). pack(side=tk.LEFT)
      tk.Entry (self.angleframe, textvariable=self.entryTarget_angle, width=4).pack(side=tk.LEFT)
      tk.Button(self.angleframe, text="GO", command= lambda: self.spin_Target(self.entryTarget_angle.get())).pack(side=tk.LEFT)
      tk.Button(self.angleframe, text="Spin Right", command= lambda: self.spin_Robot_Simulation(10)).pack(side=tk.LEFT)
      
      self.show_Current_Location()
      
      # ------ read previously save state file: data.json -----
      try:
         with open('data.json', 'r') as file:
            self.Data = json.load(file)
            self.set_Current_Angle(self.Data['offset_yaw'])
      except:  # no file, create and initialize
         self.Data = {'angle': 0, 'offset_yaw': 0}
         with open('data.json', 'w') as f:
            json.dump(self.Data, f)
      
      # ---- default entry values --------------
      self.labelCoord.set('Current (Lattitude,Longitude): ' + str(self.lat_ref) + ' , ' + str(self.lon_ref))
      self.entryLat.set(44.74713533018441)
      self.entryLon.set(-93.19377913074761)
      self.entrySpeed.set(20)
      self.entryAngle.set(170)
      self.entryTarget_angle.set(45)
      self.is_Simulation.set(True)
      self.entryOffsetYaw.set(self.Data['offset_yaw'])
      
     
   
   # ---------- End of __init__ ----------------------------------------------------------
      
   # ---------- Radian rounding ----------------------------------------------------------
   
   def normalize_angle(self, angle):
      return (int(angle) + 180) % 360 - 180
   
   def rad_to_angle(self, rad):
      return self.normalize_angle(math.degrees(rad))
   
   def radian_rounding(self, rad):
      if abs(rad) > math.pi: rad = rad - sign(rad) * math.pi * 2.0
      return rad
      
   # ---------- Simulation ---------------------------------------------------------------
   
   def sim_WayPoints(self):
      #if self.is_Simulation.get():
      #   for index, wpoint in enumerate(self.waypoints):
      #      print(wpoint)
      #      self.sim_single_WayPoint()
      #      self.next_WayPoint()
      #   self.waypoint_count = 0
       
      for index, wpoint in enumerate(self.waypoints):
         print(wpoint)
         self.sim_single_WayPoint()
         self.next_WayPoint()
      self.waypoint_count = 0
   
   def sim_single_WayPoint(self): # (lat, lon)):
      lat, lon = self.waypoints[self.waypoint_count]
      print("=====================waypoint", lat, lon)
      for i in range(1000):
         if self.step_to_Location() < 0.7: break
         self.root.update()
         time.sleep(0.01)
         print("-----", i)
   
   def next_WayPoint(self):
      self.waypoint_count = (self.waypoint_count + 1) % len(self.waypoints)
      print(self.waypoints, self.waypoint_count)
      
   # ---------- Robot --------------------------------------------------------------------
   
   def step_to_Location(self):
      lat, lon = self.waypoints[self.waypoint_count]    
      X_dest, Y_dest = self.GPS_obj.get_X_Y(lat=float(lat), lon=float(lon))  # relative to the reference in meters
      pixelX_dest, pixelY_dest = self.X_Y_to_Pixel(X_dest,Y_dest)  # destination waypoints
      if self.is_Simulation.get():
                             
         del_X, del_Y   = X_dest - self.X, Y_dest - self.Y 
         self.move_Robot(del_X, del_Y, self.entrySpeed.get()) 
         self.draw_Cicle(pixelX_dest, pixelY_dest)
         self.canvas.itemconfig(self.new_canvas,image=self.image)   
      
         del_X, del_Y   = X_dest - self.X, Y_dest - self.Y 
         distance = math.sqrt(del_X*del_X+del_Y*del_Y)
         dest_theta = self.rad_to_angle(math.atan2(del_X,del_Y))
         self.labelDistance.set("Distance: " + '{0:.2f}'.format(distance) + ", Angle: " + '{0:.0f}'.format(dest_theta))
         return distance
      else:
         self.get_Coordinates()
         del_X, del_Y   = X_dest - self.X, Y_dest - self.Y 
         self.move_Robot(del_X, del_Y, self.entrySpeed.get()) 
         self.draw_Cicle(pixelX_dest, pixelY_dest)
         self.canvas.itemconfig(self.new_canvas,image=self.image)   
         
         self.get_Coordinates()
         del_X, del_Y   = X_dest - self.X, Y_dest - self.Y 
         distance = math.sqrt(del_X*del_X+del_Y*del_Y)
         dest_theta = self.rad_to_angle(math.atan2(del_X,del_Y))
         self.labelDistance.set("Distance: " + '{0:.2f}'.format(distance) + ", Angle: " + '{0:.0f}'.format(dest_theta))
         
         self.draw_Cicle(pixelX_dest, pixelY_dest)
         self.canvas.itemconfig(self.new_canvas,image=self.image)   
         
         return distance
         
         #self.X, self.Y = self.GPS_obj.get_X_Y(lat=float(lat), lon=float(lon))  # relative to the reference in meters
         
         return 0
      
   # --------------------------------------------------------------
      
   def move_Robot(self, del_X, del_Y, speed): # del_rad=0, speed=0):
      dest_angle = self.normalize_angle( self.rad_to_angle(math.atan2(del_X, del_Y)))
      del_angle = self.normalize_angle( self.rad_to_angle(math.atan2(del_X, del_Y)) - self.angle) 
      del_rad   = math.radians(del_angle)
      if self.is_Simulation.get():
         
         if math.cos(del_rad) < 0 :  # more than 90-degree then spin first
            if self.is_Simulation:
               self.spin_Robot_Simulation(sign(math.sin(del_rad)) * 90 )
         elif abs(del_angle) > 45:       # rotate 45-degree
            #self.delta_angle = sign(math.sin(del_rad)) * 45 # spin 45-degree first 
            if self.is_Simulation:
               self.spin_Robot_Simulation(sign(math.sin(del_rad)) * 45)
         else:
            self.PID(del_X, del_Y) 
            self.angle = self.normalize_angle(self.angle + self.delta_angle)
            if self.is_Simulation:
               self.X += 0.01*speed*math.sin(math.radians(self.angle))
               self.Y += 0.01*speed*math.cos(math.radians(self.angle))
        
         #cmd = self.robot_obj.move_rad(mag=speed*100, rad=self.delta_rad, delay=10)
            cmd = self.robot_obj.move_theta(mag=speed, theta=self.delta_angle, delay=10)
            self.labelCommand.set(cmd)
            self.show_Current_Location()
      else:
         print("Delta angle: ", self.delta_angle, "Dest Angle: ", dest_angle, "Cur Angle: ", self.angle )
         if abs(del_angle) > 75:       # rotate 45-degree
            self.robot_obj.move_theta(mag=30, theta=90*sign(del_angle), delay=1000, is_Simulation=False)
         elif abs(del_angle) > 10:
            self.spin_Target(dest_angle)
         else:
            self.PID(del_X, del_Y) 
            
            self.robot_obj.move_theta(mag=30, theta=self.delta_angle, delay=2000, is_Simulation=False)
         self.angle = self.normalize_angle( self.robot_obj.Yaw + self.robot_obj.Offset_Yaw)
        # print("Delta angle:   ", self.delta_angle)
         
      
   # -------------------------------------------------------------
      
   def spin_Robot_Simulation(self, spangle):
      self.delta_angle = spangle
      self.angle = self.normalize_angle(self.angle + self.delta_angle)
      self.PID_rad_I = 0
      cmd = self.robot_obj.spin_Angle_Target(theta=self.angle, is_Simulation=self.is_Simulation.get())
      self.labelCommand.set(cmd)
      #time.sleep(0.2)
      self.show_Current_Location()
      time.sleep(0.2)
      return cmd
   
   # ---------------- Spin to target ----------------------------
   
   def spin_Target(self, target_theta):
      if self.is_Simulation:
         self.spin_Robot_Simulation(target_theta-self.angle)
      else:
         self.robot_obj.spin_Angle_Target(theta=target_theta, is_Simulation=False)
      print("Target angle: ", target_theta)   
   
   # ---------------- PID ---------------------------------------
      
   def PID(self, dX, dY):
      #drad = math.fmod( math.atan2(dX, dY) - self.radian + 2.0*math.pi, 2.0*math.pi)
      dangle = self.normalize_angle( self.rad_to_angle(math.atan2(dX, dY)) - self.angle)
      #if abs(drad) > math.pi:
       #  drad = drad - sign(drad) * math.pi * 2.0
      if math.sin(math.radians(self.delta_angle)) * math.sin(math.radians(dangle)) < 0: self.PID_rad_I = 0.0 # when change direction to prevent oscillation
      self.PID_rad_I += self.delta_angle*self.PID_rad_I_coef
      self.delta_angle = self.normalize_angle( dangle * self.PID_rad_D_coef + self.PID_rad_I)
      
   # --------- GPS stuff --------------
   
   def get_RTK(self):
      if self.gps_obj.get_RTK():
         print("RTK: ", self.gps_obj.rtk, " type: ", self.gps_obj.rtk_type)
         self.labelDistance.set(self.gps_obj.rtk_type)
         
   def get_Coordinates(self):            # coordinates and yaw
      if self.gps_obj.get_GPS():
         #in_json = gps_obj.read_json
         if self.robot_obj.get_Yaw():
            self.labelYaw.set("Yaw:" + str(self.robot_obj.Yaw))
            self.angle = self.normalize_angle( self.robot_obj.Yaw + self.robot_obj.Offset_Yaw)
         
         self.X, self.Y = self.GPS_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
         #yaw = self.robot_obj.Yaw*math.pi/180.0 if self.robot_obj.get_Yaw() else 0
         self.draw_Arrow_Location_Meters(self.angle, self.X, self.Y)
         self.canvas.itemconfig(self.new_canvas,image=self.image)
#         print("Lat, Lon, Prec(mm), Yaw:", self.gps_obj.lat, self.gps_obj.lon, self.gps_obj.prec, self.angle)
         print("Prec(mm), Yaw, Angle:", self.gps_obj.prec, self.robot_obj.Yaw, self.angle)
    
   def set_Current_Angle(self, offset_theta):
      self.robot_obj.yaw_Offset(theta=offset_theta, is_Simulation=False)
      
      print("offset yaw: ", offset_theta)
      self.Data['angle'], self.Data['offset_yaw'] = offset_theta, offset_theta
      with open('data.json', 'w') as f:
         json.dump(self.Data, f)
   #def get_Yaw(self):
    #  self.labelYaw.set()
   
   # ----------- Test -------------------
   
   def move_Test(self):
      self.angle = self.entryAngle.get()
      if self.is_Simulation:
         self.X += 0.01*self.entrySpeed.get()*math.sin(math.radians(self.angle))
         self.Y += 0.01*self.entrySpeed.get()*math.cos(math.radians(self.angle))
         self.show_Current_Location()
         self.canvas.itemconfig(self.new_canvas,image=self.image)
         print("Move test", self.X, self.Y)
      
      
   # -------------------------------- Show Location -------------------
   
   def show_Current_Location(self):
      self.draw_Arrow_Location_Meters(self.angle, self.X, self.Y)
      self.lat, self.lon = self.GPS_obj.get_Lat_Lon_from_X_Y(self.X, self.Y)
      self.labelCoord.set('Current (Lattitude,Longitude): ' + str(self.lat) + ' , ' + str(self.lon))
#      print("Location: ", self.lat, self.lon)
   
   def check_Location(self):
      new_lat, new_lon = self.entryLat.get(), self.entryLon.get()
      X, Y = self.GPS_obj.get_X_Y(lat=float(new_lat), lon=float(new_lon))
      self.draw_Arrow_Location_Meters(0, X, Y)
      self.canvas.itemconfig(self.new_canvas,image=self.image)
      
   def X_Y_to_Pixel(self, X, Y):
      return int(self.ref_Xpixel+X*self.pixel_scale), int(self.ref_Ypixel-Y*self.pixel_scale)
   
   def draw_Arrow_Location_Meters(self, angle, X, Y):   # X and Y are meter from the reference
      pixelX, pixelY = self.X_Y_to_Pixel(X,Y)
      self.draw_Arrow(angle=angle, origX=pixelX, origY=pixelY)
    
   # ----------------------------------- Drawings ------------------------------
   
   def draw_Arrow(self, angle=10, origX=150, origY=120):
     radian = math.radians(angle)
     arrow_length = self.img_width / 5
     #center = (X,Y) # (int(self.img_width-arrow_length*0.6),int(self.img_height-arrow_length*0.6))
     x2   = int( arrow_length*math.sin(radian) + origX)
     y2   = int(-arrow_length*math.cos(radian) + origY)
     tipx1 = int(arrow_length/2 * math.sin(-radian -0.4) + x2)
     tipy1 = int(arrow_length/2 * math.cos(-radian -0.4) + y2)
     tipx2 = int(arrow_length/2 * math.sin(-radian +0.4) + x2)
     tipy2 = int(arrow_length/2 * math.cos(-radian +0.4) + y2)
     
     self.new_canvas =self.canvas.create_image(0,0, anchor="nw",image=self.image)
     self.canvas.create_line(origX, origY, x2,y2,fill="red",width=10)
     self.canvas.create_line(tipx1,tipy1, x2,y2,fill="yellow",width=10)
     self.canvas.create_line(tipx2,tipy2, x2,y2,fill="yellow",width=10)
     
   def create_Circle(self, x, y, r, **kwargs):
     return self.canvas.create_oval(x-r, y-r, x+r, y+r, **kwargs)
   def draw_Cicle(self, X, Y):
     #new_canvas =self.canvas.create_image(0,0, anchor="nw",image=self.image)
     self.create_Circle(X, Y, 20, fill="", outline="red", width=4)
    
# ==================== Testing ====================
if __name__ == "__main__":
   #current_loc = []
#   lat_ref, lon_ref = 44.747035800, -93.1937129634583
   lat_ref, lon_ref = 44.7470, -93.1937
#   img_width, img_height, ref_Xpixel, ref_Ypixel, pixel_scale =560, 500, 260, 375, 19.0
   img_width, img_height, ref_Xpixel, ref_Ypixel, pixel_scale =560, 500, 240, 390, 21.6
   #WayPoints = [(44.74713533018441,-93.19377913074761), (44.747154, -93.19373), (44.74711,-93.19366), (44.747065, -93.19362), (44.747035800, -93.1937129634583)]
   #WayPoints = [(44.74711,-93.19366), (44.74713533018441,-93.19377913074761), (44.747065, -93.19362), (44.747154, -93.19373), (44.747035800, -93.1937129634583)]
  # WayPoints = [(44.74705,-93.193656), (44.747035800, -93.1937129634583)]
   #WayPoints = [(44.74705,-93.193656)]
   #WayPoints = [(44.747035800, -93.19369)]
   WayPoints = [(44.74705,-93.193656), (44.747078,-93.19367),(44.747078,-93.19372),(44.747035800, -93.19369)]

   gui_obj = GUI(img_width=img_width, img_height=img_height, ref_Xpixel=ref_Xpixel, ref_Ypixel=ref_Ypixel, pixel_scale=pixel_scale, lat_ref=lat_ref,lon_ref=lon_ref, waypoints=WayPoints)
   gui_obj.root.mainloop()
