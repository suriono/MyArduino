import cv2

# ========= Modify below ================
Filepath = "picture-resize.png"
Outfile  = "bitmap.txt"

# =============== Do not modify ===================
img = cv2.imread(Filepath)
assert img is not None, "file could not be read, check with os.path.exists()"
fout = open(Outfile, "w")

count = 0
for ncol in range(img.shape[1]):
   for nrow in range(img.shape[0]):  
      # pixrow = nrow
      # if ncol % 2:   # when odd, the row goes the opposite direction
      #     pixrow = img.shape[0] - nrow - 1
       #if ncol < 5:
        #   print (nrow, pixrow, ncol)
       red, green, blue = img[nrow,ncol]
       if red > 10 or green > 10 or blue > 10: 
         #  print(red, green, blue, row, ncol, count)
           print(str(ncol) + "," + str(nrow) + "," + str(red) + "," + str(green) + "," + str(blue))
           fout.write(str(ncol) + "," + str(nrow) + "," + str(red) + "," + str(green) + "," + str(blue) + ",")
    
       count += 1
           
fout.close()
print ("Image size (row,column): ", img.shape)
