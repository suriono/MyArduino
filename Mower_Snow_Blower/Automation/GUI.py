import cv2, json, MQTT, VECTOR, math
import tkinter as tk
from tkinter import PhotoImage

class GUI_class:

    # ----------- Frames ------------------------
    root = tk.Tk()
    leftframe, rightframe = tk.Frame(root), tk.Frame(root)
    leftframe.pack(side=tk.LEFT)
    rightframe.pack(side=tk.LEFT)
    mqttframe = tk.Frame(rightframe)
    mqttframe.pack(side=tk.TOP, anchor=tk.W)
    motionframe = tk.Frame(rightframe)
    motionframe.pack(side=tk.TOP, anchor=tk.W)
    wpframe = tk.Frame(rightframe)
    wpframe.pack(side=tk.TOP, anchor=tk.W)

    # ----------- Widget variables --------------------------
    is_Simulation, entrySpeed = tk.BooleanVar(), tk.IntVar()
    DEBUG_STR = tk.StringVar()

    # ----------- External objects --------------------------
    MQTT_obj = MQTT.MQTT_class()
    MQTT_obj.mqtt_client.loop_start()

    # --------------------- INIT ----------------------------
    yaw, yaw_count, yaw_count_last, runMode = 0,0,0,0
    is_Simulation.set(True)


    def __init__(self):
        with open('data.json', 'r') as file:
            data = json.load(file)
            #self.waypoints, self.yaw_offset = data['waypoints'], data['yaw_offset']
            self.yaw_offset = data['yaw_offset']
        with open('config.json', 'r') as file:
            # ---- config file ------------------------
            config = json.load(file)
            self.img_width,self.img_height,self.ref_Xpix,self.ref_Ypix = config['img_width'], config['img_height'],config['ref_xpix'], config['ref_ypix']
            self.scale_xpix,self.scale_ypix = config['scale_xpix'], config['scale_ypix']
            # ---- parameters -------------------------
            self.VECTOR_obj = VECTOR.VECTOR_class(waypoints=data['waypoints'],lat_ref=config['ref_lat'],lon_ref=config['ref_lon'])
            self.VECTOR_obj.X, self.VECTOR_obj.Y = self.VECTOR_obj.get_X_Y(data['last_location'][0],data['last_location'][1])
            self.VECTOR_obj.set_Origin()       # initial waypoint origin
            # ---- canvas for the map -------
            self.canvas = tk.Canvas(self.leftframe, width=self.img_width, height=self.img_height)
            self.canvas.pack()
            self.image = PhotoImage(file='house.png')  # Replace with the path to your image file
            self.canvas.create_image(self.img_width, self.img_height, image=self.image,anchor='se')  # Position the image at the center of the canvas

        # -----------Frames --------------

        tk.Label(self.mqttframe,textvariable=self.DEBUG_STR).pack(side=tk.LEFT)

        tk.Label(self.motionframe, text="Set Speed").pack(side=tk.LEFT, anchor=tk.W)
        tk.Entry(self.motionframe, textvariable=self.entrySpeed, width=6).pack(side=tk.LEFT, anchor=tk.E)

        tk.Checkbutton(self.wpframe, text='Simulation', variable=self.is_Simulation, onvalue=1, offvalue=0).pack(side=tk.LEFT)
        tk.Button(self.wpframe, text="Go to waypoints", command=self.Start_WayPoints).pack(side=tk.LEFT)

        # ----------- Initial values ---------
        self.entrySpeed.set(data['speed'])
        self.loop_Updates()

    # ------------------------- Loop Updates --------------------------------
    def loop_Updates(self):
        self.yaw_count_last = self.yaw_count

        if self.is_Simulation.get():   # if simulation
            if self.runMode > 0:
                if self.VECTOR_obj.Distance > 1.0:   # until distance to waypoint tolerance
                    # print("   runMode:", self.runMode)
                    self.VECTOR_obj.step_to_Location(is_Simulation=True)
                elif self.VECTOR_obj.WP_index < len(self.VECTOR_obj.waypoints)-1: # next waypoint
                    self.VECTOR_obj.WP_index += 1
                    self.VECTOR_obj.WayPoint_Go(speed=self.entrySpeed.get())
                else:                                            # end of waypoints
                    self.runMode = 0
                    print("---------- End of waypoints -----------")

        else:                         # ----------------   robot moves

            yaw, self.yaw_count = self.MQTT_obj.yaw, self.MQTT_obj.imu_count
            self.VECTOR_obj.Angle = yaw + self.yaw_offset

            if self.runMode > 0:
                if self.VECTOR_obj.Distance > 1.0:   # until distance to waypoint tolerance
                    print("     Distance", self.VECTOR_obj.Distance)
                    self.VECTOR_obj.step_to_Location(is_Simulation=False)
            self.DEBUG_STR.set("IMU: Yaw=" + str(self.VECTOR_obj.Angle ) + ", Count=" + str(self.yaw_count))

        # Current location
        x,y,angle = self.VECTOR_obj.X, self.VECTOR_obj.Y,self.VECTOR_obj.Angle # self.VECTOR_obj.get_X_Y(lat=self.VECTOR_obj.gps_obj.lat_ref, lon=self.VECTOR_obj.gps_obj.lon_ref)
        self.draw_Arrow_Location_Meters(angle=angle, X=x, Y=y)

        # Next Waypoint
        wplat,wplon = self.VECTOR_obj.waypoints[self.VECTOR_obj.WP_index]
        xwp,ywp = self.VECTOR_obj.get_X_Y(lat=wplat, lon=wplon)
        xwppix,ywppix = self.X_Y_to_Pixel(xwp,ywp)
        self.draw_Circle(X=xwppix, Y=ywppix)
        self.canvas.itemconfig(self.new_canvas, image=self.image)
        #print("GPS ref: ", self.VECTOR_obj.gps_obj.lat_ref, self.VECTOR_obj.gps_obj.lon_ref, x, y,xwp,ywp)
        self.draw_Line_Meters(X1=self.VECTOR_obj.WP_Xorig, Y1=self.VECTOR_obj.WP_Yorig, X2=self.VECTOR_obj.X_dest,
                              Y2=self.VECTOR_obj.Y_dest, line_color="blue")  # line showing origin to destination

        self.root.after(500, self.loop_Updates)        # loop_Updates

    # ----------------------- WayPoints -----------------------------------
    def Start_WayPoints(self):
        self.VECTOR_obj.WP_index = 0
        self.VECTOR_obj.WayPoint_Go(speed=self.entrySpeed.get())
    #    print("Waypoints")
    #    self.VECTOR_obj.set_Origin()
    #    self.VECTOR_obj.get_Destination()
        self.runMode = 1

    # ----------------------------------- Drawings ------------------------------

    def X_Y_to_Pixel(self, X, Y):
        return int(self.ref_Xpix + X * self.scale_xpix), int(self.ref_Ypix - Y * self.scale_ypix)

    def draw_Arrow_Location_Meters(self, angle, X, Y):  # X and Y are meter from the reference
        pixX, pixY = self.X_Y_to_Pixel(X, Y)
        radian = math.radians(angle)
        arrow_length = self.img_width / 5
        # center = (X,Y) # (int(self.img_width-arrow_length*0.6),int(self.img_height-arrow_length*0.6))
        x2 = int(arrow_length * math.sin(radian) + pixX) # origX)
        y2 = int(-arrow_length * math.cos(radian) + pixY)
        tipx1 = int(arrow_length / 2 * math.sin(-radian - 0.4) + x2)
        tipy1 = int(arrow_length / 2 * math.cos(-radian - 0.4) + y2)
        tipx2 = int(arrow_length / 2 * math.sin(-radian + 0.4) + x2)
        tipy2 = int(arrow_length / 2 * math.cos(-radian + 0.4) + y2)

        linecolor = "red" if self.yaw_count == self.yaw_count_last else "green"

        self.new_canvas = self.canvas.create_image(0, 0, anchor="nw", image=self.image)
        self.canvas.create_line(pixX, pixY, x2, y2, fill=linecolor, width=10)
        self.canvas.create_line(tipx1, tipy1, x2, y2, fill="yellow", width=10)
        self.canvas.create_line(tipx2, tipy2, x2, y2, fill="yellow", width=10)

    def create_Circle(self, x, y, r, **kwargs):
        return self.canvas.create_oval(x - r, y - r, x + r, y + r, **kwargs)

    def draw_Circle(self, X, Y):
        self.create_Circle(X, Y, 20, fill="", outline="red", width=4)

    def draw_Line_Meters(self, X1, Y1, X2, Y2, line_color="red"):
        (pixX1, pixY1), (pixX2, pixY2) = self.X_Y_to_Pixel(X1, Y1), self.X_Y_to_Pixel(X2, Y2)
    #    self.draw_Line(pixelX1, pixelY1, pixelX2, pixelY2, line_color=line_color)
    #def draw_Line(self, x1, y1, x2, y2, line_color="red"):
        self.canvas.create_line(pixX1, pixY1, pixX2, pixY2, fill=line_color, width=10)

# ==================== Testing ====================
if __name__ == "__main__":
   # WayPoints = [(44.746900, -93.193607)]  # left by garage

    GUI_obj = GUI_class()
    GUI_obj.root.mainloop()