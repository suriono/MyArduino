import cv2, math, json, Robot, time, gps_class, vector_class, threading
import tkinter as tk
from tkinter import PhotoImage
from numpy import sign

class GUI (threading.Thread):
   image_path  = r'C:\Users\surio\Documents\github\MyArduino\Mower_4wheels\python_autonomous\house.png'
   
   # ---- PID ---------------
   PID_rad_D_coef, PID_rad_I_coef, PID_rad_I = 0.2, 0.01, 0.0
   delta_angle, gps_count, distance, dest_theta, run_Mode, is_new_waypoint = 0.0, 0, 100.0, 0, False, True
   low_power_spin_count, new_waypoint_angle_align_count, target_angle, angle_check_count = 0,0,0,0
   prev_angle = 0

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
      tk.Button(self.angleframe, text="GO", command= lambda: self.test_spin_Target(self.entryTarget_angle.get())).pack(side=tk.LEFT)
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

      # -------------------- Vector -----------------------------------------
      self.vector_obj = vector_class.VECTOR(waypoints=waypoints, lat_ref=lat_ref, lon_ref=lon_ref)
      self.vector_obj.set_Origin() #x_orig=self.X, y_orig=self.Y)  # The 1st origin

      self.show_Current_Location()

      # -------------------- Thread loops: GPS and Robot move ---------------
      self.loop_GPS()
      self.loop_Move_Robot()
      
      threading.Thread.__init__(self)
      
   # ================================= Multi Threadings ===================================
      
   def loop_GPS(self):
      dest_lat, dest_lon = self.waypoints[self.waypoint_count]
      X_dest, Y_dest = self.gps_obj.get_X_Y(lat=float(dest_lat), lon=float(dest_lon))  # relative to the reference in meters
      
      if not self.is_Simulation.get():
         if self.gps_obj.get_GPS():
            self.X, self.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
            #self.vector_obj.X, self.vector_obj.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
           # if self.gps_count % 10 == 0:
           #    self.gps_obj.get_RTK()
           #    print("     RTK: ", self.gps_obj.rtk_type)
               
            self.gps_count += 1

      self.vector_obj.update_Location(self.X, self.Y)
      #del_X, del_Y   = X_dest - self.X, Y_dest - self.Y
      #self.distance = math.sqrt(del_X*del_X+del_Y*del_Y)
      #self.distance = self.vector_obj.distance
         
      self.show_Current_Location()    # ================= may remove later to save graphic resources
      
      #print("GPS: ", self.gps_obj.lat, self.gps_obj.lon, self.gps_obj.prec, self.gps_count)
      self.root.after(250, self.loop_GPS)  # reschedule event in 2 seconds

   # ---------------------------- Move Robot Commands -------------------------------------------
   def loop_Move_Robot(self):
      dest_lat, dest_lon = self.waypoints[self.waypoint_count]
      X_dest, Y_dest = self.gps_obj.get_X_Y(lat=float(dest_lat), lon=float(dest_lon))  # relative to the reference in meters
      
      if not self.is_Simulation.get():
         self.robot_obj.move_Robot()               # automatically move given robot_obj.mag, theta, and delay
         self.angle = self.normalize_angle( self.robot_obj.Yaw + self.robot_obj.Offset_Yaw)
         
      del_X, del_Y   = X_dest - self.X, Y_dest - self.Y  # self.X,self.Y from loop_GPS() for not simulation
      self.dest_theta = self.rad_to_angle(math.atan2(del_X,del_Y))   # destination theta from 0-degree
            
      self.root.after(300, self.loop_Move_Robot)  # reschedule event in 2 seconds

   # -------------------------- Loop until angle target --------------------------

   def loop_Target_Angle(self):
      print("--- Loop Angle target: ", self.angle_check_count, self.target_angle, self.angle)
      del_angle = self.target_angle - self.angle
      min_mag, max_mag = 12,20         # min and max amplitude

      if self.angle_check_count < 5:  # check after settling to check the angle if it overshoots
         if abs(del_angle) > 10:     # tolerance angle to target
            self.angle_check_count = 0
            if ( (self.angle-self.target_angle)*(self.prev_angle-self.target_angle) < 0): # overshoot
               self.robot_obj.mag, self.PID_rad_I = 0, min_mag
            elif abs(self.angle - self.prev_angle) < 2: # not moving
               self.PID_rad_I += 1
            else:                                     # once move, decrease power to avoid overshoot
               self.PID_rad_I -= 3    # slow down once it moves
            self.PID_rad_I = max(min_mag, min(self.PID_rad_I, max_mag))  # mag integrate between min and max
            self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = self.PID_rad_I, 90 * sign(del_angle), 50  # one wa
            print("          spin mag: ", self.PID_rad_I) #, ", prev: ", self.prev_angle, self.angle)
            self.prev_angle = self.angle
            self.root.after(100, self.loop_Target_Angle)
         else:                               # stop and check the angle, if it overshoots
            self.angle_check_count += 1
            self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = 0, 90 * sign(del_angle), 100
            self.PID_rad_I = min_mag
            self.root.after(500, self.loop_Target_Angle)
      else:        # target angle reached, time to move on
         self.angle_check_count = 0
         self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = 0, 90 * sign(del_angle), 100
         print("  !!!!!!!!!!!!!! ====== Target angle reached ")

         if self.run_Mode:      # move the robot when it's ready.
            self.vector_obj.set_Origin()  # x_orig=self.X, y_orig=self.Y)  # The 1st origin
            #self.vector_obj.get_Destination(self.waypoints[self.waypoint_count])
            self.vector_obj.update_Location(self.X, self.Y)
            self.root.after(100, self.loop_Motor_Until_Tolerance)

         # -------------------------- Run Motors loop ----------------------------------
      
   def loop_Motor_Until_Tolerance(self):

      print("Dist: ", f"{self.vector_obj.distance:.2f}", ",(Wayp,Robot):(", f"{self.vector_obj.waypoint_angle_deg:.2f}",
            ",", f"{self.angle:.2f}", ")")
      if self.is_Simulation.get():
         if self.run_Mode and self.vector_obj.distance > 0.7:
            self.step_to_Location()
            self.root.after(100, self.loop_Motor_Until_Tolerance)  # reschedule event in 100mS
         else:
            print("==== Tolerance reached", self.vector_obj.distance, self.run_Mode)
            self.next_WayPoint()
            if self.waypoint_count > 0:
               self.vector_obj.distance = 10.0
               self.root.after(200, self.loop_Motor_Until_Tolerance)
            else:
               self.run_Mode = False
            
      else:  # NOT simulation, actual robot

         if self.run_Mode and self.vector_obj.distance > 1.0:
            self.step_to_Location()
            #print("   Yaw:  ", self.robot_obj.Yaw, ", Offset:  ", self.robot_obj.Offset_Yaw)
            self.root.after(300, self.loop_Motor_Until_Tolerance)  # reschedule event in 2 seconds
         else:
            print("==== Tolerance reached", self.vector_obj.distance, self.run_Mode)
            #self.run_Mode = False
            self.next_WayPoint()
            #if self.waypoint_count > 0:
            #   self.vector_obj.distance = 10.0
            #   self.root.after(300, self.loop_Motor_Until_Tolerance)
            #else:
            #   self.run_Mode = False
            #   self.stop_Motor()
            #   self.is_Simulation.set(True)
   
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
      self.vector_obj.set_Origin() #x_orig=self.X, y_orig=self.Y)  # The 1st origin
      self.vector_obj.get_Destination()
      self.vector_obj.update_Location(self.X, self.Y)
      self.run_Mode = True  # spin to the waypoint first
      self.vector_obj.distance, self.waypoint_count = 10.0, 0
      self.spin_Target(self.vector_obj.waypoint_angle_deg)

      #self.root.after(100, self.loop_Motor_Until_Tolerance)
   
   def next_WayPoint(self):
      self.waypoint_count = (self.waypoint_count + 1) % len(self.waypoints)
      #self.vector_obj.wayPoint_index = self.waypoint_count

      if self.waypoint_count > 0:  # not the end
         self.vector_obj.set_Origin(self.waypoint_count)  # x_orig=self.X, y_orig=self.Y)
         self.vector_obj.get_Destination(index=self.waypoint_count)
         self.vector_obj.update_Location(self.X, self.Y)
         self.spin_Target(self.vector_obj.waypoint_angle_deg)
         self.is_new_waypoint = True  # beginning of a new waypoint, spin to precision
         self.new_waypoint_angle_align_count = 0  # count until fully aligned to a new waypoint
         self.vector_obj.distance = 10.0
         #if self.run_Mode:
         #   self.root.after(300, self.loop_Motor_Until_Tolerance)
      else:
         self.run_Mode = False
         self.stop_Motor()
         self.is_Simulation.set(True)

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
      del_waypoint_current = self.vector_obj.waypoint_angle_deg - self.angle  # delta angle waypoint path and robot
      if self.is_Simulation.get():               # in simulation mode
         if math.cos(del_rad) < 0 :  # more than 90-degree then spin first
            self.spin_Robot(sign(math.sin(del_rad)) * 13 )
         #elif abs(del_angle) > 45:       # rotate 45-degree
         #   self.spin_Robot(sign(math.sin(del_rad)) * 60)
         else:
            self.PID_rad_D_coef, self.PID_rad_I_coef = 0.5, 0.1
            if abs(self.vector_obj.path_distance) > 0.5:
               self.delta_angle = self.normalize_angle(del_waypoint_current*self.PID_rad_D_coef - 10 * sign(self.vector_obj.path_distance))
               #self.delta_angle = - 15 * sign(self.vector_obj.path_distance)
            elif abs(self.vector_obj.path_distance) > 0.25:
               self.delta_angle = self.normalize_angle(del_waypoint_current*self.PID_rad_D_coef - 5 * sign(self.vector_obj.path_distance))
               #self.delta_angle = - 25 * sign(self.vector_obj.path_distance)
            else:  # very close to the waypoint path
               self.delta_angle = self.normalize_angle(del_waypoint_current - 2 * sign(self.vector_obj.path_distance))
            if abs(self.delta_angle) > 30:  # to limit the turn to stay on path, so it does not turn too much
               self.delta_angle = 30 * sign(self.delta_angle)
            self.angle = self.normalize_angle(self.angle + self.delta_angle)

            self.X += 0.02*speed*math.sin(math.radians(self.angle))
            self.Y += 0.02*speed*math.cos(math.radians(self.angle))
            cmd = self.robot_obj.move_theta(mag=speed, theta=self.delta_angle, delay=10)
            self.labelCommand.set(cmd)
         #self.show_Current_Location()

      else:                          # actually moving the robot

         if abs(del_angle) > 45:
            print("            > 45 degrees, currently:  ", del_angle)
            self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = 20, 90 * sign(del_angle), 100  # one way
         elif abs(del_angle) > 30:
            print("            > 30 degrees, currently:  ", del_angle)
            self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = 20, 90 * sign(del_angle), 10  # one way
         else:
            self.low_power_spin_count = 0
            self.is_new_waypoint = False
            self.PID_rad_D_coef, self.PID_rad_I_coef = 0.3, 0.1
            #del_waypoint_current = self.vector_obj.waypoint_angle_deg - self.angle
            if abs(self.vector_obj.path_distance) > 0.5:
               self.delta_angle = self.normalize_angle(del_waypoint_current*self.PID_rad_D_coef - 10 * self.vector_obj.path_distance)
                                                     #  - 10 * sign(self.vector_obj.path_distance))
               #if abs(self.delta_angle) < 5: self.delta_angle = 5 * sign(self.delta_angle)
            elif abs(self.vector_obj.path_distance) > 0.25:
               self.delta_angle = self.normalize_angle(del_waypoint_current*self.PID_rad_D_coef - 5 * sign(self.vector_obj.path_distance))
               #if abs(self.delta_angle) < 7: self.delta_angle = 7 * sign(self.delta_angle)
            else:  # very close to the waypoint path
               #self.delta_angle = self.normalize_angle(del_waypoint_current*self.PID_rad_D_coef - 1 * sign(self.vector_obj.path_distance))
               self.delta_angle = self.normalize_angle(del_waypoint_current*self.PID_rad_D_coef - 2 * sign(self.vector_obj.path_distance))

            if abs(self.delta_angle) > 30:   # to limit the turn to stay on path, so it does not turn too much
               self.delta_angle = 30 * sign(self.delta_angle)
            #print("   Turn : ", self.delta_angle, ", angle: ", self.angle, ", path angle: ",self.vector_obj.waypoint_angle_deg," , to path: ", self.vector_obj.path_distance)
            self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = speed, self.delta_angle, 1  # one way
      if abs(del_angle) < 30:  # when moving forward, print message
         print("     Turn :", self.delta_angle, ",(angle,path): ", self.angle, ",",
                  self.vector_obj.waypoint_angle_deg, "),dist path:", '{0:.2f}'.format(self.vector_obj.path_distance))

            #print("Distance: ", self.distance, ", Angle to destination: ", del_angle)
            #self.PID_rad_D_coef, self.PID_rad_I_coef = 0.3, 0.1
            #self.PID(del_X, del_Y) # to obtain self.delta_angle
            #self.robot_obj.mag, self.robot_obj.theta, self.robot_obj.delay = speed, self.delta_angle, 1  # one way

   # -------------------------Simulation purpose ------------------------------------
      
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
      if self.is_Simulation.get():
         print("Simulation Spin to target angle: ", target_theta)
         self.spin_Robot(target_theta-self.angle)
      else:
         #if abs(target_theta - self.angle) > 180:  # better to switch direction in this case
          #  target_theta = self.normalize_angle(360-target_theta)
         print("Spin to target angle: ", target_theta)
         self.target_angle, self.angle_check_count = target_theta, 0
         self.root.after(1000, self.loop_Target_Angle)

   def test_spin_Target(self, target_theta):
      self.run_Mode = False
      self.spin_Target(target_theta)

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
         #self.X, self.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
         self.gps_obj.get_RTK()
         self.show_Current_Location()
         print("Prec(mm), Yaw, Angle:", self.gps_obj.prec, self.robot_obj.Yaw, self.angle)
    
   def set_Current_Angle(self, offset_theta):
      self.robot_obj.yaw_Offset(theta=offset_theta, is_Simulation=False)
      
      print("offset yaw: ", offset_theta)
      self.Data['angle'], self.Data['offset_yaw'] = offset_theta, offset_theta
      with open('data.json', 'w') as f:
         json.dump(self.Data, f)

   def move_get_GPS_Angle(self):   # manually move forward to get the current angle based on GPS
      print("move_get_GPS_Angle")
      if self.gps_obj.get_GPS():
        # self.X, self.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
         print("Current GPS location: ", self.gps_obj.lat, self.gps_obj.lon)
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
      if self.is_Simulation.get():
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
         #self.X, self.Y = self.gps_obj.get_X_Y(lat=self.gps_obj.lat, lon=self.gps_obj.lon)
         self.draw_Arrow_Location_Meters(self.angle, self.X, self.Y)
         self.angle = self.normalize_angle( self.robot_obj.Yaw + self.robot_obj.Offset_Yaw)

      self.draw_Line_Meters(X1=self.vector_obj.X_orig, Y1=self.vector_obj.Y_orig, X2=self.vector_obj.X_dest,Y2=self.vector_obj.Y_dest, line_color="blue")     # line showing origin to destination
   #   self.labelCoord.set('Lattitude: ' + str(self.gps_obj.lat) + '\nLongitute: ' + str(self.gps_obj.lon))

      dest_lat, dest_lon = self.waypoints[self.waypoint_count]
      X_dest, Y_dest = self.gps_obj.get_X_Y(lat=float(dest_lat), lon=float(dest_lon))  # relative to the reference in meters
      pixelX_dest, pixelY_dest = self.X_Y_to_Pixel(X_dest,Y_dest)    
      self.draw_Circle(pixelX_dest, pixelY_dest)
      
      #del_X, del_Y   = X_dest - self.X, Y_dest - self.Y 
      #self.dest_theta = self.rad_to_angle(math.atan2(del_X,del_Y))

      datastr = 'Precision:  ' + str(self.gps_obj.prec/10) + 'mm, Count:  ' + str(self.gps_count)
      datastr += ', Lattitude:  ' + '{0:.7f}'.format(self.gps_obj.lat) + ', Longitute:  ' + '{0:.7f}'.format(self.gps_obj.lon)
      datastr += '\nRTK:  ' + str(self.gps_obj.rtk) + ', type:  ' + self.gps_obj.rtk_type
      #datastr += ', Precision:  ' + str(self.gps_obj.prec) + ', Count:  ' + str(self.gps_count)
      datastr += '\nDistance:  ' + '{0:.2f}'.format(self.vector_obj.distance) + '\nDestination Angle:  ' + '{0:.0f}'.format(self.dest_theta)
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

   def draw_Line_Meters(self, X1,Y1,X2,Y2, line_color="red"):
      (pixelX1, pixelY1), (pixelX2, pixelY2) = self.X_Y_to_Pixel(X1, Y1), self.X_Y_to_Pixel(X2, Y2)
      self.draw_Line(pixelX1, pixelY1, pixelX2, pixelY2, line_color=line_color)
    
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
     self.create_Circle(X, Y, 20, fill="", outline="red", width=4)
   def draw_Line(self, x1, y1, x2, y2, line_color="red"):
      self.canvas.create_line(x1,y1, x2, y2, fill=line_color, width=10)
    
# ==================== Testing ====================
#if __name__ == "__main__":


 #  gui_obj.root.mainloop()
