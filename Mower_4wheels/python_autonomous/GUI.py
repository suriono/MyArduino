import cv2, math, GPS_coordinate, Robot, time
import tkinter as tk
from tkinter import PhotoImage
from numpy import sign

class GUI:
   image_path  = r'C:\Users\UZMN\OneDrive - SkyWater Technology\Documents\GitHub\MyArduino\Mower_4wheels\House-google-map.png'
   
   # ---- PID ---------------
   PID_rad_D_coef, PID_rad_I_coef, PID_rad_I = 0.2, 0.01, 0.0
   delta_rad = 0.0
   robot_obj = Robot.Robot()
   
   # ---- Frames ------------
   root = tk.Tk()
   leftframe, rightframe, simframe,robotcmdframe = tk.Frame(root), tk.Frame(root), tk.Frame(root), tk.Frame(root)
   coordframe, checkframe, moveframe = tk.Frame(rightframe), tk.Frame(rightframe), tk.Frame(rightframe) 
   leftframe.pack(side=tk.LEFT)
   rightframe.pack(side=tk.TOP)
   coordframe.pack(side=tk.TOP)
   checkframe.pack(side=tk.TOP)
   moveframe.pack(side=tk.TOP)
   simframe.pack(side=tk.TOP)
   robotcmdframe.pack(side=tk.TOP)
   
   # ---- widget variables ---------
   entryLat, entryLon, entrySpeed, entryRadian, labelCoord = tk.StringVar(), tk.StringVar(), tk.StringVar(), tk.StringVar(), tk.StringVar()
   labelCommand, labelDistance = tk.StringVar(), tk.StringVar()
   
   # ----------- init ---------------
   
   def __init__(self, img_width=440, img_height=600, ref_Xpixel=100, ref_Ypixel=400, pixel_scale=19.8, lat_ref=0.0, lon_ref=0.0, waypoints=[]):
      self.img_width, self.img_height, self.ref_Xpixel, self.ref_Ypixel, self.pixel_scale = img_width, img_height, ref_Xpixel, ref_Ypixel, pixel_scale
      self.lat_ref, self.lon_ref, self.lat, self.lon, self.radian, self.X, self.Y = lat_ref, lon_ref, lat_ref, lon_ref, 0.0, 0.0, 0.0
      self.waypoints, self.waypoint_count = waypoints, 0
      
      self.GPS_obj = GPS_coordinate.GPS_Coordinate(lat_ref=lat_ref, lon_ref=lon_ref)
      
      # ---- canvas for the map -------
      self.canvas = tk.Canvas(self.leftframe, width=img_width, height=img_height)
      self.canvas.pack()
      self.image = PhotoImage(file=self.image_path)  # Replace with the path to your image file
      self.canvas.create_image(img_width, img_height, image=self.image, anchor='se')  # Position the image at the center of the canvas
      
      # ---- coordinates ---------------
      tk.Label(self.coordframe,textvariable=self.labelCoord).pack(side=tk.LEFT)
      
      tk.Label(self.checkframe,text="Test Lattitude").pack(side=tk.LEFT)
      tk.Entry(self.checkframe, textvariable=self.entryLat, width=18).pack(side=tk.LEFT)
      tk.Label(self.checkframe,text="Test Longitude").pack(side=tk.LEFT)
      tk.Entry(self.checkframe, textvariable=self.entryLon, width=18).pack(side=tk.LEFT)
      tk.Button(self.checkframe, text ="View test location", command=self.check_Location).pack(side=tk.LEFT)
      
      # ---- speed and initial direction ------------
      tk.Label(self.moveframe,text="Set Speed").pack(side=tk.LEFT)
      tk.Entry(self.moveframe, textvariable=self.entrySpeed, width=6).pack(side=tk.LEFT)
      tk.Label(self.moveframe,text="Rotate(rad)").pack(side=tk.LEFT)
      tk.Entry(self.moveframe, textvariable=self.entryRadian, width=6).pack(side=tk.LEFT)
      tk.Button(self.moveframe, text ="Test Move",  command = self.move_Test).pack(side=tk.LEFT)
      #tk.Button(self.moveframe, text ="Step to test location", command= lambda: self.step_to_Location(self.entryLat.get(),self.entryLon.get())).pack(side=tk.LEFT)
      tk.Button(self.moveframe, text ="Step to test location", command=self.step_to_Location).pack(side=tk.LEFT)
      tk.Button(self.moveframe, text="Next Waypoint", command=self.next_WayPoint).pack(side=tk.LEFT)
      
      # ----- simulation to waypoints -----------------
      #tk.Button(self.simframe, text="Sim go to waypoints", command= lambda: self.sim_WayPoints(self.entryLat.get(),self.entryLon.get())).pack(side=tk.LEFT)
      tk.Button(self.simframe, text="Sim go to waypoints", command= self.sim_WayPoints).pack(side=tk.LEFT)
      tk.Label(self.simframe, textvariable=self.labelDistance).pack(side=tk.LEFT)
   
      # ----- Robot command ---------------------------
      tk.Label(self.robotcmdframe, text="Command sent to robot: ").pack(side=tk.LEFT)
      tk.Label(self.robotcmdframe, textvariable=self.labelCommand).pack(side=tk.LEFT)
      
      self.show_Current_Location()
      
      # ---- default entry values --------------
      self.labelCoord.set('Current (Lattitude,Longitude): ' + str(self.lat_ref) + ' , ' + str(self.lon_ref))
      self.entryLat.set(44.74713533018441)
      self.entryLon.set(-93.19377913074761)
      self.entrySpeed.set(0.2)
      self.entryRadian.set(3.0)
      
   # ---------- Radian rounding ----------------------------------------------------------
   
   def radian_rounding(self, rad):
      if abs(rad) > math.pi: rad = rad - sign(rad) * math.pi * 2.0
      return rad
      
   # ---------- Simulation ---------------------------------------------------------------
   
   def sim_WayPoints(self):
      for index, wpoint in enumerate(self.waypoints):
         print(wpoint)
         self.sim_single_WayPoint()
         self.next_WayPoint()
      self.waypoint_count = 0
   
   def sim_single_WayPoint(self): # (lat, lon)):
      lat, lon = self.waypoints[self.waypoint_count]
      print("=====================waypoint", lat, lon)
      for i in range(1000):
         if self.step_to_Location() < 0.1: break
         self.root.update()
         time.sleep(0.01)
   
   def next_WayPoint(self):
      self.waypoint_count += 1
      
   # ---------- Robot --------------------------------------------------------------------
   
   def step_to_Location(self):
      lat, lon = self.waypoints[self.waypoint_count]
      X_dest, Y_dest = self.GPS_obj.get_X_Y(lat=float(lat), lon=float(lon))  # relative to the reference in meters
      pixelX_dest, pixelY_dest = self.X_Y_to_Pixel(X_dest,Y_dest)
      
      del_X, del_Y   = X_dest - self.X, Y_dest - self.Y # self.X_scale * math.sin(math.radians(del_lon))
      self.move_Robot(del_X, del_Y, float(self.entrySpeed.get())) # del_rad=self.delta_rad, speed=float(self.entrySpeed.get()))
      self.draw_Cicle(pixelX_dest, pixelY_dest)
      self.canvas.itemconfig(self.new_canvas,image=self.image)   
      
      del_X, del_Y   = X_dest - self.X, Y_dest - self.Y # self.X_scale * math.sin(math.radians(del_lon))
      distance = math.sqrt(del_X*del_X+del_Y*del_Y)
      dest_rad = self.radian_rounding(math.atan2(del_X,del_Y))
      self.labelDistance.set("Distance: " + '{0:.2f}'.format(distance) + ", Angle: " + '{0:.1f}'.format(math.degrees(dest_rad)))
      return distance
      
   def move_Robot(self, del_X, del_Y, speed): # del_rad=0, speed=0):
      del_rad = self.radian_rounding(math.atan2(del_X, del_Y) - self.radian) # math.fmod( math.atan2(del_X, del_Y) - self.radian, 2.0*math.pi)
      if math.cos(del_rad) < 0 :  # more than 90-degree then spin first
         self.delta_rad = sign(math.sin(del_rad)) * 1.57 # spin 90-degree first 
         self.spin_Robot()
      elif abs(del_rad) > 0.753:       # rotate 45-degree
         self.delta_rad = sign(math.sin(del_rad)) * 0.753 # spin 90-degree first 
         self.spin_Robot()
      else:
         self.PID(del_X, del_Y) 
         self.radian = self.radian_rounding(self.radian + self.delta_rad)
         self.X += speed*math.sin(self.radian)
         self.Y += speed*math.cos(self.radian)
        
         cmd = self.robot_obj.move_rad(mag=speed*100, rad=self.delta_rad, delay=10)
         self.labelCommand.set(cmd)
         self.show_Current_Location()
      #print(self.radian, self.delta_rad, del_rad)
      
   def spin_Robot(self):
      self.radian = self.radian_rounding(self.radian + self.delta_rad)
      self.PID_rad_I = 0
      cmd = self.robot_obj.spin_rad(rad=self.radian)
      self.labelCommand.set(cmd)
      time.sleep(0.1)
      self.show_Current_Location()
      time.sleep(0.1)
      return cmd
      
   def PID(self, dX, dY):
      drad = math.fmod( math.atan2(dX, dY) - self.radian + 2.0*math.pi, 2.0*math.pi)
      if abs(drad) > math.pi:
         drad = drad - sign(drad) * math.pi * 2.0
      if math.sin(self.delta_rad) * math.sin(drad) < 0: self.PID_rad_I = 0.0 # when change direction to prevent oscillation
      self.PID_rad_I += self.delta_rad*self.PID_rad_I_coef
      self.delta_rad = drad * self.PID_rad_D_coef + self.PID_rad_I
      
   # --------- Test -------------------
   
   def move_Test(self):
      self.radian = float(self.entryRadian.get())
      self.X += float(self.entrySpeed.get())*math.sin(self.radian)
      self.Y += float(self.entrySpeed.get())*math.cos(self.radian)
      self.show_Current_Location()
      self.canvas.itemconfig(self.new_canvas,image=self.image)
      print("Move test", self.X, self.Y)
      
      
   # ---------- Show Location -----
   
   def show_Current_Location(self):
      self.draw_Arrow_Location_Meters(self.radian, self.X, self.Y)
      self.lat, self.lon = self.GPS_obj.get_Lat_Lon_from_X_Y(self.X, self.Y)
      self.labelCoord.set('Current (Lattitude,Longitude): ' + str(self.lat) + ' , ' + str(self.lon))
#      print("Location: ", self.lat, self.lon)
   
   def check_Location(self):
      new_lat, new_lon = float(self.entryLat.get()), float(self.entryLon.get())
      X, Y = self.GPS_obj.get_X_Y(lat=float(new_lat), lon=float(new_lon))
      self.draw_Arrow_Location_Meters(0, X, Y)
      self.canvas.itemconfig(self.new_canvas,image=self.image)
      
   def X_Y_to_Pixel(self, X, Y):
      return int(self.ref_Xpixel+X*self.pixel_scale), int(self.ref_Ypixel-Y*self.pixel_scale)
   
   def draw_Arrow_Location_Meters(self, radian, X, Y):   # X and Y are meter from the reference
      pixelX, pixelY = self.X_Y_to_Pixel(X,Y)
      self.draw_Arrow(radian=radian, origX=pixelX, origY=pixelY)
    
   # ---------- Arrow ---------------
   
   def draw_Arrow(self, radian=0.7, origX=150, origY=120):
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
   lat_ref, lon_ref = 44.747035800, -93.1937129634583
   img_width, img_height, ref_Xpixel, ref_Ypixel, pixel_scale =560, 500, 260, 375, 19.0
   #WayPoints = [(44.74713533018441,-93.19377913074761), (44.747154, -93.19373), (44.74711,-93.19366), (44.747065, -93.19362), (44.747035800, -93.1937129634583)]
   WayPoints = [(44.74711,-93.19366), (44.74713533018441,-93.19377913074761), (44.747065, -93.19362), (44.747154, -93.19373), (44.747035800, -93.1937129634583)]

   gui_obj = GUI(img_width=img_width, img_height=img_height, ref_Xpixel=ref_Xpixel, ref_Ypixel=ref_Ypixel, pixel_scale=pixel_scale, lat_ref=lat_ref,lon_ref=lon_ref, waypoints=WayPoints)
   gui_obj.root.mainloop()
