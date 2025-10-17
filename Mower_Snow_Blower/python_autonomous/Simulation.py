from GUI import GUI #, GPS_coordinate
import json

with open('config.json', 'r') as file:
    config = json.load(file)

# Now 'data' is a Python dictionary (or list, depending on the JSON structure)
print(config)


lat_ref, lon_ref, Xpix_ref, Ypix_ref  = config["ref_lat"], config["ref_lon"], config["ref_xpix"], config["ref_ypix"]
img_width, img_height, scaleX, scaleY = config['img_width'], config['img_height'], config['scale_xpix'], config['scale_ypix']

# left by garage
WayPoints = [(44.74703774, -93.193708591)]    # left by garage
#WayPoints = [(44.747072054, -93.19365584)]     # right by garage
#WayPoints = [(44.747105882, -93.193756515)]    # end driveway left
#WayPoints = [(44.747124651, -93.193705424)]     # end driveway right

WayPoints = [(44.747105882, -93.193756515), (44.747124651, -93.193705424), (44.747072054, -93.19365584), (44.74703774, -93.193708591)]


#WayPoints = [(44.746900, -93.193607)] # by deck
#WayPoints = [(44.746804, -93.193456)] # maple south
#WayPoints = [(44.746759, -93.193588)]  # SW gazebo
#WayPoints = [(44.746823, -93.193653)]  # trampoline
#WayPoints = [(44.746922, -93.193533), (44.746859, -93.193494)] # mapple north, gazebo east
#WayPoints = [(44.746922, -93.193533), (44.746900, -93.193607) ] # pine north, deck
#WayPoints = [(44.746922, -93.193533), (44.746804, -93.193456)]  # pine north, pine south
#WayPoints = [(44.746759, -93.193588),(44.746804, -93.193456), (44.746922, -93.193533), (44.746900, -93.193607)] # trampoline to deck
#WayPoints = [(44.746922, -93.193533), (44.746804, -93.193456), (44.746759, -93.193588), (44.746823, -93.193653)] # deck to trampoline


gui_obj = GUI(img_width=img_width, img_height=img_height, ref_Xpixel=Xpix_ref, ref_Ypixel=Ypix_ref,
              scale_xpix=scaleX,scale_ypix=scaleY, lat_ref=lat_ref, lon_ref=lon_ref, waypoints=WayPoints)
#gui_obj.X, gui_obj.Y = -23, 30
gui_obj.root.mainloop()

#lat_ref, lon_ref = 44.747035800, -93.1937129634583
#img_width, img_height, ref_Xpixel, ref_Ypixel, pixel_scale =560, 500, 260, 375, 19.0

#gui_obj = GUI.GUI(img_width=img_width, img_height=img_height, ref_Xpixel=ref_Xpixel, ref_Ypixel=ref_Ypixel, pixel_scale=pixel_scale, lat_ref=lat_ref,lon_ref=lon_ref)
#gui_obj = GUI(img_width=440, img_height=600, ref_Xpixel=200, ref_Ypixel=260, lat_ref=44.747035800, lon_ref=-93.1937129634583)


#coord_obj = GPS_coordinate.GPS_Coordinate(lat_ref=lat_ref, lon_ref=lon_ref)
#X, Y = coord_obj.get_X_Y(lat=44.74713533018441, lon=-93.19377913074761)
#print(X,Y)
#gui_obj.draw_Arrow_Location_Meters(X, Y)

#gui_obj.draw_Arrow()

# gazebo landing:
#WayPoints = [(44.746841171, -93.193508501)]
#WayPoints = [(44.746940, -93.193495)]  # North East Fence
#WayPoints = [(44.746879, -93.193539)]  # North East Gazebo

#WayPoints = [(44.7468561, -93.1934728)] # gazebo east
#WayPoints = [(44.746922, -93.193533)] # by maple north
#WayPoints = [(44.746900, -93.193607)] # by deck
#WayPoints = [(44.746804, -93.193456)] # maple south
#WayPoints = [(44.746759, -93.193588)]  # SW gazebo
#WayPoints = [(44.746823, -93.193653)]  # trampoline
#WayPoints = [(44.746922, -93.193533), (44.746859, -93.193494)] # mapple north, gazebo east
#WayPoints = [(44.746922, -93.193533), (44.746900, -93.193607) ] # pine north, deck
#WayPoints = [(44.746922, -93.193533), (44.746804, -93.193456)]  # pine north, pine south
#WayPoints = [(44.746759, -93.193588),(44.746804, -93.193456), (44.746922, -93.193533), (44.746900, -93.193607)] # trampoline to deck
#WayPoints = [(44.746922, -93.193533), (44.746804, -93.193456), (44.746759, -93.193588), (44.746823, -93.193653)] # deck to trampoline

