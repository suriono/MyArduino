import cv2, math, json, Robot, time, gps_class, threading
import tkinter as tk
from tkinter import PhotoImage
from numpy import sign

class GUI (threading.Thread):
   image_path  = r'C:\Users\surio\Documents\github\MyArduino\Mower_4wheels\python_autonomous\house.png'
   
   # ---- PID ---------------
   PID_rad_D_coef, PID_rad_I_coef, PID_rad_I = 0.2, 0.01, 0.0
   delta_angle, gps_count, distance, dest_theta, run_Mode = 0.0, 0, 100.0, 0, False

   robot_obj = Robot.Robot()
   Data = {}
   prev_X, prev_Y = 0,0
   
   # ---- Frames ------------
   root = tk.Tk()
   leftframe, rightframe = tk.Frame(root), tk.Frame(root)
   checkframe, moveframe, simframe, robotcmdframe = tk.Frame(rightframe), tk.Frame(rightframe), tk.Frame(rightframe), tk.Frame(rightframe)
   angleframe, dataframe = tk.Frame(rightframe), tk.Frame(rightframe)
   leftframe.pack(side=tk.LEFT)
   rightframe.pack(side=tk.LEFT)
  # coordframe.pack(side=tk.TOP)
   checkframe.pack(side=tk.TOP, anchor=tk.W)
   moveframe.pack(side=tk.TOP, anchor=tk.W)
   simframe.pack(side=tk.TOP, anchor=tk.W)
   robotcmdframe.pack(side=tk.TOP, anchor=tk.W)
   angleframe.pack(side=tk.TOP, anchor=tk.W)
   dataframe.pack(side=tk.TOP, anchor=tk.W)
   
   # ---- widget variables ---------
   entryLat, entryLon, entrySpeed, entryAngle, labelCoord, is_Simulation = tk.DoubleVar(), tk.DoubleVar(), tk.IntVar(), tk.IntVar(), tk.StringVar(), tk.BooleanVar()
   labelCommand, entryTarget_angle, labelYaw, entryOffsetYaw = tk.StringVar(), tk.IntVar(), tk.StringVar(), tk.IntVar()
   #labelAngleDiff = tk.StringVar()
   
   # ----------------------------- init ---------------
   
   def __init__(self, img_width=440, img_height=600, ref_Xpixel=100, ref_Ypixel=400, scale_xpix=19.8, scale_ypix=20, lat_ref=0.0, lon_ref=0.0, waypoints=[]):
      self.img_width, self.img_height, self.ref_Xpixel, self.ref_Ypixel, self.scale_xpix, self.scale_ypix = img_width, img_height, ref_Xpixel, ref_Ypixel, scale_xpix, scale_ypix
      self.lat_ref, self.lon_ref, self.angle, self.X, self.Y = lat_ref, lon_ref, 0.0, 0.0, 0.0
      self.waypoints, self.waypoint_count = waypoints, 0
      
      self.gps_obj   = gps_class.gps(lat_ref=lat_ref, lon_ref=lon_ref)
      
      # ---- canvas for the map -------
      self.canvas = tk.Canvas(self.leftframe, width=img_width, height=img_height)
      self.canvas.pack()
      self.image = PhotoImage(file=self.image_path)  # Replace with the path to your image file
      self.canvas.create_image(img_width, img_height, image=self.image, anchor='se')  # Position the image at the center of the canvas
      
      # ---- coordinates ---------------
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
      tk.Button(self.moveframe, text ="Step to Way Point", command=self.step_to_Location).pack(side=tk.LEFT)
      tk.Button(self.moveframe, text="Next Waypoint", command=self.next_WayPoint).pack(side=tk.LEFT)
      
      # ----- simulation to waypoints -----------------
      tk.Checkbutton(self.simframe, text='Simulation',variable=self.is_Simulation, onvalue=1, offvalue=0).pack(side=tk.LEFT)
      tk.Button     (self.simframe, text="Go to waypoints", command= self.WayPoints).pack(side=tk.LEFT)
      tk.Button     (self.simframe, text="Check GPS", command= self.check_GPS).pack(side=tk.LEFT)
      tk.Button     (self.simframe, text="Check Motors", command= self.check_Motor).pack(side=tk.LEFT)
      tk.Button     (self.simframe, text="Manual Stop", command= self.stop_Manual).pack(side=tk.LEFT)
      #tk.Label      (self.simframe, textvariable=self.labelDistance).pack(side=tk.LEFT)
   
      # ----- Robot command ---------------------------
      tk.Label(self.robotcmdframe, text="Command sent to robot: ").pack(side=tk.LEFT)
      tk.Label(self.robotcmdframe, textvariable=self.labelCommand).pack(side=tk.LEFT)
      
      # ----- Angle command ----------------------------
      tk.Button(self.angleframe, text="Set Current Angle:", command= lambda: self.set_Current_Angle(self.entryOffsetYaw.get())).pack(side=tk.LEFT)
      tk.Entry (self.angleframe, textvariable=self.entryOffsetYaw, width=4).pack(side=tk.LEFT)
      tk.Button(self.angleframe, text="Move & Get GPS Angle:", command=self.move_get_GPS_Angle).pack(side=tk.LEFT)
      tk.Label (self.angleframe, textvariable=self.labelYaw). pack(side=tk.LEFT)
      tk.Label (self.angleframe, text="Target angle"). pack(side=tk.LEFT)
      tk.Entry (self.angleframe, textvariable=self.entryTarget_angle, width=4).pack(side=tk.LEFT)
      tk.Button(self.angleframe, text="GO", command= lambda: self.spin_Target(self.entryTarget_angle.get())).pack(side=tk.LEFT)
      tk.Button(self.angleframe, text="Spin Right", command= lambda: self.spin_Robot(10)).pack(side=tk.LEFT)
      
      # ----- Data -----------------------------------
      tk.Label(self.dataframe,textvariable=self.labelCoord).pack(side=tk.LEFT, anchor="w")
      
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
      #self.labelCoord.set("Current (Lattitude ======== Longitude): " + str(self.lat_ref) + ' , ' + str(self.lon_ref))
      self.entryLat.set(44.74713533018441)
      self.entryLon.set(-93.19377913074761)
      self.entrySpeed.set(30)
      self.entryAngle.set(170)
      self.entryTarget_angle.set(45)
      self.is_Simulation.set(True)
      self.entryOffsetYaw.set(self.Data['offset_yaw'])
    #  self.labelAngleDiff.set("Angle dest-robot:")
      
      self.show_Current_Location()
      
      self.loop_GPS()
      self.loop_Move_Robot()
      
      threading.Thread.__init__(self)
      
   # ================================= Multi Threadings ===================================
      
   def loop_GPS(self):
      dest_lat, dest_lon = self.waypoints[self.waypoint_count]
      X_dest, Y_dest = self.gps_obj.get_X_Y(lat=float(dest_lat), lon=float(dest_lon))  # relative to the reference in meters
      
      if not self.is_Simulation.get():
         if self.gps_obj.get_GPS():
            if self.gps_count % 10 == 0:
               self.gps_obj.get_RTK()
               print("     RTK: ", self.gps_obj.rtk_type)
               
            self.gps_count += 1      
         
      del_X, del_Y   = X_dest - self.X, Y_dest - self.Y
      self.distance = math.sqrt(del_X*del_X+del_Y*del_Y)
         
      self.show_Current_Location()    # ==================== remove later to save graphic resources 
      
      #print("GPS: ", self.gps_obj.lat, self.gps_obj.lon, self.gps_obj.prec, self.gps_count)
      self.root.after(250, self.loop_GPS)  # reschedule event in 2 seconds

   # ---------------------------- Move Robot Commands -------------------------------------------
   def loop_Move_Robot(self):
      dest_lat, dest_lon = self.waypoints[self.waypoint_count]
      X_dest, Y_dest = self.gps_obj.get_X_Y(lat=float(dest_lat), lon=float(dest_lon))  # relative to the reference in meters
      
      if not self.is_Simulation.get():
         self.robot_obj.move_Robot()               # automatically move given robot_obj.mag, theta, and delay
         
         self.angle = self.normalize_angle( self.robot_obj.Yaw + self.robot_obj.Offset_Yaw)
         
      del_X, del_Y   = X_dest - self.X, Y_dest - self.Y 
      self.dest_theta = self.rad_to_angle(math.atan2(del_X,del_Y))   # destination theta from 0-degree
            
      self.root.after(200, self.loop_Move_Robot)  # reschedule event in 2 seconds
      
   # -------------------------- Run Motors loop ----------------------------------
      
   def loop_Motor_Until_Tolerance(self):
      #print("loop_Motor_Until_Tolerance")
      if self.is_Simulation.get():
         print("Runnning in simulation mode, distance:  ", self.distance)
         if self.run_Mode and self.distance > 0.7:
            
            self.step_to_Location()
            #self.root.update()
            self.root.after(100, self.loop_Motor_Until_Tolerance)  # reschedule event in 100mS
         else:
            print("==== Tolerance reached", self.distance, self.run_Mode)
            self.next_WayPoint()
            if self.waypoint_count > 0:
               self.distance = 10.0
               self.root.after(200, self.loop_Motor_Until_Tolerance)
            else:
               self.run_Mode = False
            
      else:  # NOT simulation, actual robot
         #self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = 20, 0, 200
         print("Distance: ", self.distance)
         if self.run_Mode and self.distance > 2.0:
            self.step_to_Location()
            #print("   Yaw:  ", self.robot_obj.Yaw, ", Offset:  ", self.robot_obj.Offset_Yaw)
            self.root.after(200, self.loop_Motor_Until_Tolerance)  # reschedule event in 2 seconds
         else:
            print("==== Tolerance reached", self.distance, self.run_Mode)
            self.next_WayPoint()
            if self.waypoint_count > 0:
               self.distance = 10.0
               self.root.after(200, self.loop_Motor_Until_Tolerance)
            else:
               self.run_Mode = False
               self.stop_Motor()
   
   # ---------- End of __init__ ----------------------------------------------------------
      
   # ---------- Radian rounding ----------------------------------------------------------
   
   def normalize_angle(self, angle):
      return (int(angle) + 180) % 360 - 180
   
   def rad_to_angle(self, rad):
      return self.normalize_angle(math.degrees(rad))
   
   def radian_rounding(self, rad):
      if abs(rad) > math.pi: rad = rad - sign(rad) * math.pi * 2.0
      return rad
      
   # ============================== Way Points =====================================

   def WayPoints(self):
      self.run_Mode, self.distance, self.waypoint_count = True, 10.0, 0
      self.root.after(100, self.loop_Motor_Until_Tolerance)
      #self.is_Simulation.set(True)
   
   def last_WayPoints(self):
      #self.run_Mode, self.distance, self.waypoint_count = True, 10.0, 1
   #   self.root.after(100, self.loop_Motor_Until_Tolerance)
      for index, waypoint in enumerate(self.waypoints): # iterate through waypoints
         self.run_Mode, self.distance, self.waypoint_count = True, 10.0, index
#         self.run_Mode, self.distance = True, 10.0
         print(" Way point number:  ", index, self.distance)
         self.root.after(100, self.loop_Motor_Until_Tolerance)
         #while self.run_Mode == True:

         if index == 0:
            self.root.after(100, self.loop_Motor_Until_Tolerance)
         #self.next_WayPoint()
      #self.waypoint_count = 0
      self.is_Simulation.set(True)
   
   def WayPoints_old(self):
      for index, wpoint in enumerate(self.waypoints): # iterate waypoints
         self.run_Mode = True
         self.distance = 10.0
         print(" Way point number:  ", index)
         print("=====================waypoint", self.waypoints[self.waypoint_count], ", Distance:  ", self.distance)
         self.loop_Motor_Until_Tolerance()
         #self.single_WayPoint()
         self.next_WayPoint()
      self.waypoint_count = 0
   
   def single_WayPoint(self): # (lat, lon)):
      #self.run_Mode = True
      print("=====================waypoint", self.waypoints[self.waypoint_count], ", Distance:  ", self.distance)
      self.loop_Motor_Until_Tolerance()
   
   def next_WayPoint(self):
      self.waypoint_count = (self.waypoint_count + 1) % len(self.waypoints)
      
   # ================================= Robot ===================================================
   
   def step_to_Location(self):
      lat, lon = self.waypoints[self.waypoint_count]    
      X_dest, Y_dest = self.gps_obj.get_X_Y(lat=float(lat), lon=float(lon))  # relative to the reference in meters
      #pixelX_dest, pixelY_dest = self.X_Y_to_Pixel(X_dest,Y_dest)            # destination waypoints in pixels
      del_X, del_Y   = X_dest - self.X, Y_dest - self.Y
      self.move_Robot(del_X, del_Y, self.entrySpeed.get())

   # ----------------------------------------
         
   def stop_Motor(self):
      self.robot_obj.mag = 0
      
   # --------------------------------------------------------------
   
   def check_Motor(self):
      self.robot_obj.mag, self.robot_obj.theta = self.entrySpeed.get(), self.entryAngle.get()
      #self.run_Mode = True
      #self.root.after(100, self.loop_Motor_Until_Tolerance) 
   
   def stop_Manual(self):
      self.stop_Motor()
      self.run_Mode = False
      self.X, self.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
      print("Stop at (X,Y):", self.prev_X, self.prev_Y)
      print("Angle from previous point: ", self.normalize_angle( self.rad_to_angle(math.atan2(self.X-self.prev_X, self.Y-self.prev_Y)))
)

   # ------------------------------
   def move_Robot(self, del_X, del_Y, speed): # del_rad=0, speed=0):
      dest_angle = self.normalize_angle( self.rad_to_angle(math.atan2(del_X, del_Y)))
      del_angle = self.normalize_angle( self.rad_to_angle(math.atan2(del_X, del_Y)) - self.angle)
      del_rad   = math.radians(del_angle)
      if self.is_Simulation.get():               # in simulation mode
         if math.cos(del_rad) < 0 :  # more than 90-degree then spin first
            #if self.is_Simulation:
            self.spin_Robot(sign(math.sin(del_rad)) * 90 )
         elif abs(del_angle) > 45:       # rotate 45-degree
            #if self.is_Simulation:
            self.spin_Robot(sign(math.sin(del_rad)) * 45)
         else:
            self.PID(del_X, del_Y) 
            self.angle = self.normalize_angle(self.angle + self.delta_angle)
            if self.is_Simulation:
               self.X += 0.01*speed*math.sin(math.radians(self.angle))
               self.Y += 0.01*speed*math.cos(math.radians(self.angle))
            cmd = self.robot_obj.move_theta(mag=speed, theta=self.delta_angle, delay=10)
            self.labelCommand.set(cmd)
         #self.show_Current_Location()

      else:                          # actually moving the robot
         #print("Delta angle: ", self.delta_angle, "Dest Angle: ", dest_angle, "Cur Angle: ", self.angle )
         if abs(del_angle) > 45:       # rotate 45-degree
            print("Spin when angle greater than 45 degrees, currently:  ", del_angle)
            self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = 20, 90 * sign(del_angle), 1000 # one way
         #elif abs(del_angle) > 30:
         #   print("Spin, more than 10 degrees")
            self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = 20, 90 * sign(del_angle), 200  # one way
            #self.spin_Target(dest_angle)
            #self.robot_obj.move_theta(mag=25, theta=90*sign(del_angle), delay=1000, is_Simulation=False)
         #elif abs(del_angle) > 10:
         #   print("Sping when angle greater than 10 degrees")
         #   self.spin_Target(dest_angle)
         #   self.PID_rad_D_coef, self.PID_rad_I_coef = 1.0, 1
         #   self.PID(del_X, del_Y)
#            double_angle = self.normalize_angle( (self.delta_angle *2 ))
         #   self.robot_obj.move_theta(mag=speed, theta=self.delta_angle, delay=1000, is_Simulation=False)
#            self.robot_obj.move_theta(mag=speed, theta=double_angle, delay=2000, is_Simulation=False)
         else:
            print("Distance: ", self.distance, ", Angle to destination: ", del_angle)
            self.PID_rad_D_coef, self.PID_rad_I_coef = 0.005, 0.0
            self.PID(del_X, del_Y)
            #            double_angle = self.normalize_angle( (self.delta_angle *2 ))
            self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = speed, del_angle, 1  # one way
          #  self.robot_obj.move_theta(mag=speed, theta=self.delta_angle, delay=1000, is_Simulation=False)
         #

        # self.angle = self.normalize_angle( self.robot_obj.Yaw + self.robot_obj.Offset_Yaw)
     # self.show_Current_Location()  # is this needed ?
        # print("Delta angle:   ", self.delta_angle)
         
      
   # -------------------------------------------------------------
      
   def spin_Robot(self, spangle):
      self.delta_angle = spangle
      self.angle = self.normalize_angle(self.angle + self.delta_angle)
      self.PID_rad_I = 0
      cmd = self.robot_obj.spin_Angle_Target(theta=self.angle, is_Simulation=self.is_Simulation.get())
      self.labelCommand.set(cmd)
      #self.show_Current_Location()
      time.sleep(0.2)
      return cmd
   
   # ---------------- Spin to target ----------------------------
   
   def spin_Target(self, target_theta):
      if self.is_Simulation:
         self.spin_Robot(target_theta-self.angle)
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
      self.PID_rad_I += self.PID_rad_I_coef # self.delta_angle*self.PID_rad_I_coef
      self.delta_angle = self.normalize_angle( dangle * self.PID_rad_D_coef + self.PID_rad_I)
      
   # --------- GPS stuff --------------
   
   def check_GPS(self):            # coordinates and yaw
      if self.gps_obj.get_GPS():   
         self.X, self.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
         self.gps_obj.get_RTK()
         self.show_Current_Location()
         print("Prec(mm), Yaw, Angle:", self.gps_obj.prec, self.robot_obj.Yaw, self.angle)
    
   def set_Current_Angle(self, offset_theta):
      self.robot_obj.yaw_Offset(theta=offset_theta, is_Simulation=False)
      
      print("offset yaw: ", offset_theta)
      self.Data['angle'], self.Data['offset_yaw'] = offset_theta, offset_theta
      with open('data.json', 'w') as f:
         json.dump(self.Data, f)

   def move_get_GPS_Angle(self):
      print("move_get_GPS_Angle")
      if self.gps_obj.get_GPS():
         self.X, self.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
         self.prev_X, self.prev_Y = self.X, self.Y
         print("Starting (X,Y):", self.prev_X, self.prev_Y)
         self.robot_obj.mag, self.robot_obj.theta = 30, 0 # self.entrySpeed.get(),0 # move forward to get GPS angle
        # time.sleep(10)
         # self.run_Mode = True
        # self.stop_Manual()
       #  x2, y2 = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
       #  print("Stopping (X,Y):", x2, y2)

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
      if self.is_Simulation.get(): 
         self.draw_Arrow_Location_Meters(self.angle, self.X, self.Y)
         self.gps_obj.lat, self.gps_obj.lon = self.gps_obj.get_Lat_Lon_from_X_Y(self.X, self.Y)    
      else:
         self.X, self.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
         self.draw_Arrow_Location_Meters(self.angle, self.X, self.Y)
         self.angle = self.normalize_angle( self.robot_obj.Yaw + self.robot_obj.Offset_Yaw)
   #   self.labelCoord.set('Lattitude: ' + str(self.gps_obj.lat) + '\nLongitute: ' + str(self.gps_obj.lon))

      dest_lat, dest_lon = self.waypoints[self.waypoint_count]
      X_dest, Y_dest = self.gps_obj.get_X_Y(lat=float(dest_lat), lon=float(dest_lon))  # relative to the reference in meters
      pixelX_dest, pixelY_dest = self.X_Y_to_Pixel(X_dest,Y_dest)    
      self.draw_Circle(pixelX_dest, pixelY_dest)
      
      #del_X, del_Y   = X_dest - self.X, Y_dest - self.Y 
      #self.dest_theta = self.rad_to_angle(math.atan2(del_X,del_Y))
    

      datastr  = '\nLattitude:  ' + '{0:.7f}'.format(self.gps_obj.lat) + '\nLongitute:  ' + '{0:.7f}'.format(self.gps_obj.lon)
      datastr += '\nRTK:  ' + str(self.gps_obj.rtk) + ', type:  ' + self.gps_obj.rtk_type
      datastr += ', Precision:  ' + str(self.gps_obj.prec) + ', Count:  ' + str(self.gps_count)
      datastr += '\nDistance:  ' + '{0:.2f}'.format(self.distance) + '\nDestination Angle:  ' + '{0:.0f}'.format(self.dest_theta)
      datastr += ', Orientation:  ' + str(int(self.angle)) + ", Angle dest-robot:  " + str(int(self.dest_theta - self.angle))
      #print('Datastr: ', datastr)
      self.labelCoord.set(datastr)
     # self.draw_Arrow_Location_Meters(self.angle, self.X, self.Y)
      self.canvas.itemconfig(self.new_canvas,image=self.image)
     # self.labelCoord.set('Lattitude: ' + str(self.lat) + ' , Longitute: ' + str(self.lon))
   
   def check_Location(self):
      new_lat, new_lon = self.entryLat.get(), self.entryLon.get()
      X, Y = self.gps_obj.get_X_Y(lat=float(new_lat), lon=float(new_lon))
      self.draw_Arrow_Location_Meters(0, X, Y)
      self.canvas.itemconfig(self.new_canvas,image=self.image)
      
   def X_Y_to_Pixel(self, X, Y):
      return int(self.ref_Xpixel+X*self.scale_xpix), int(self.ref_Ypixel-Y*self.scale_ypix)
   
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
   def draw_Circle(self, X, Y):
     #new_canvas =self.canvas.create_image(0,0, anchor="nw",image=self.image)
     self.create_Circle(X, Y, 20, fill="", outline="red", width=4)
    
# ==================== Testing ====================
#if __name__ == "__main__":


 #  gui_obj.root.mainloop()
