import cv2, math, GPS_coordinate
import tkinter as tk
from tkinter import PhotoImage

class GUI:
   image_path  = r'C:\Users\UZMN\OneDrive - SkyWater Technology\Documents\GitHub\MyArduino\Mower_4wheels\House-google-map.png'
   
   # ---- Frames ------------
   root = tk.Tk()
   leftframe, rightframe = tk.Frame(root), tk.Frame(root)
   coordframe, checkframe, moveframe = tk.Frame(rightframe), tk.Frame(rightframe), tk.Frame(rightframe) 
   leftframe.pack(side=tk.LEFT)
   rightframe.pack(side=tk.TOP)
   coordframe.pack(side=tk.TOP)
   checkframe.pack(side=tk.TOP)
   moveframe.pack(side=tk.TOP)
   
   # ---- widget variables ---------
   entryLat, entryLon, entrySpeed, entryRadian, labelCoord = tk.StringVar(), tk.StringVar(), tk.StringVar(), tk.StringVar(), tk.StringVar()
   
   # ----------- init ---------------
   
   def __init__(self, img_width=440, img_height=600, ref_Xpixel=100, ref_Ypixel=400, pixel_scale=19.8, lat_ref=0.0, lon_ref=0.0):
      self.img_width, self.img_height, self.ref_Xpixel, self.ref_Ypixel, self.pixel_scale = img_width, img_height, ref_Xpixel, ref_Ypixel, pixel_scale
      self.lat_ref, self.lon_ref, self.lat, self.lon, self.radian, self.X, self.Y = lat_ref, lon_ref, lat_ref, lon_ref, 0.0, 0.0, 0.0
      
      self.GPS_obj = GPS_coordinate.GPS_Coordinate(lat_ref=lat_ref, lon_ref=lon_ref)
      
      # ---- canvas for the map -------
      self.canvas = tk.Canvas(self.leftframe, width=img_width, height=img_height)
      self.canvas.pack()
      self.image = PhotoImage(file=self.image_path)  # Replace with the path to your image file
      self.canvas.create_image(img_width, img_height, image=self.image, anchor='se')  # Position the image at the center of the canvas
      
      # ---- coordinates ---------------
      tk.Label(self.coordframe,textvariable=self.labelCoord).pack(side=tk.LEFT)
      
      tk.Label(self.checkframe,text="Set Lattitude").pack(side=tk.LEFT)
      self.wid_check_lat = tk.Entry(self.checkframe, textvariable=self.entryLat, width=18).pack(side=tk.LEFT)
      tk.Label(self.checkframe,text="Set Longitude").pack(side=tk.LEFT)
      self.wid_check_lon = tk.Entry(self.checkframe, textvariable=self.entryLon, width=18).pack(side=tk.LEFT)
      tk.Button(self.checkframe, text ="Check the location", command=self.check_Location).pack(side=tk.LEFT)
      
      # ---- speed and initial direction ------------
      tk.Label(self.moveframe,text="Set Speed").pack(side=tk.LEFT)
      tk.Entry(self.moveframe, textvariable=self.entrySpeed, width=6).pack(side=tk.LEFT)
      tk.Label(self.moveframe,text="Initial orientation(deg)").pack(side=tk.LEFT)
      tk.Entry(self.moveframe, textvariable=self.entryRadian, width=6).pack(side=tk.LEFT)
      tk.Button(self.moveframe, text ="Test Move",  command = self.move_Test).pack(side=tk.LEFT)
    
      #self.draw_Arrow(0, ref_Xpixel, ref_Ypixel)
      self.show_Current_Location()
      
      # ---- default entry values --------------
      self.labelCoord.set('Current (Lattitude,Longitude): ' + str(self.lat_ref) + ' , ' + str(self.lon_ref))
      self.entryLat.set(44.74713533018441)
      self.entryLon.set(-93.19377913074761)
      self.entrySpeed.set(0.2)
      self.entryRadian.set(0.0)
      
   # ---------- Robot --------------------------------------------------------------------
   
   def move_Test(self):
      self.radian = float(self.entryRadian.get())
      self.X = self.X + float(self.entrySpeed.get())*math.sin(self.radian)
      self.Y = self.Y + float(self.entrySpeed.get())*math.cos(self.radian)
      self.show_Current_Location()
      print("Move test", self.X, self.Y)
      
      
   # ---------- Show Location -----
   
   def show_Current_Location(self):
      #X, Y = self.GPS_obj.get_X_Y(self.lat, self.lon)
      self.draw_Arrow_Location_Meters(self.radian, self.X, self.Y)
   
   def check_Location(self):
      new_lat, new_lon = float(self.entryLat.get()), float(self.entryLon.get())
      X, Y = self.GPS_obj.get_X_Y(lat=float(new_lat), lon=float(new_lon))
      self.draw_Arrow_Location_Meters(0, X, Y)
      
   def draw_Arrow_Location_Meters(self, radian, X, Y):   # X and Y are meter from the reference
      self.draw_Arrow(radian=radian, origX=int(self.ref_Xpixel+X*self.pixel_scale), origY=int(self.ref_Ypixel-Y*self.pixel_scale))
    
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
     
     new_canvas =self.canvas.create_image(0,0, anchor="nw",image=self.image)
     self.canvas.create_line(origX, origY, x2,y2,fill="red",width=10)
     self.canvas.create_line(tipx1,tipy1, x2,y2,fill="yellow",width=10)
     self.canvas.create_line(tipx2,tipy2, x2,y2,fill="yellow",width=10)
     self.canvas.itemconfig(new_canvas,image=self.image)
      
# ==================== Testing ====================
if __name__ == "__main__":
   lat_ref, lon_ref = 44.747035800, -93.1937129634583
   img_width, img_height, ref_Xpixel, ref_Ypixel, pixel_scale =560, 500, 260, 375, 19.0

   gui_obj = GUI(img_width=img_width, img_height=img_height, ref_Xpixel=ref_Xpixel, ref_Ypixel=ref_Ypixel, pixel_scale=pixel_scale, lat_ref=lat_ref,lon_ref=lon_ref)
  # gui_obj = GUI(img_width=440, img_height=600, ref_Xpixel=200, ref_Ypixel=260, lat_ref=44.747035800, lon_ref=-93.1937129634583)
   #gui_obj.draw_Arrow()
   gui_obj.root.mainloop()
