from GUI import GUI #, GPS_coordinate
import json

with open('config.json', 'r') as file:
    config = json.load(file)

# Now 'data' is a Python dictionary (or list, depending on the JSON structure)
print(config)


lat_ref, lon_ref, Xpix_ref, Ypix_ref  = config["ref_lat"], config["ref_lon"], config["ref_xpix"], config["ref_ypix"]
img_width, img_height, scaleX, scaleY = config['img_width'], config['img_height'], config['scale_xpix'], config['scale_ypix']
# gazebo landing:
WayPoints = [(44.74684019, -93.193508378)]

#WayPoints = [(44.746852, -93.193487)]
#WayPoints = [(44.746922, -93.193533)] # by pine tree
#WayPoints = [(44.746922, -93.193533), (44.746852, -93.193487)]


gui_obj = GUI(img_width=img_width, img_height=img_height, ref_Xpixel=Xpix_ref, ref_Ypixel=Ypix_ref,
              scale_xpix=scaleX,scale_ypix=scaleY, lat_ref=lat_ref, lon_ref=lon_ref, waypoints=WayPoints)
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
