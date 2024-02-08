import cv2, math, openpyxl
#from openpyxl.styles import PatternFill

# ========= Modify below ================
Filepath = "picture-original.png"

# =============== Do not modify ===================
img = cv2.imread(Filepath)
assert img is not None, "file could not be read, check with os.path.exists()"

for nrow in range(img.shape[0]):
   for ncol in range(img.shape[1]):   
      red, green, blue = img[nrow,ncol]
      print(red, green, blue)
      if blue > 80 and green > 80 and red > 80:
         for i in range(3): 
            img.itemset((nrow,ncol,i),0)
      elif blue < 50 and green < 50 and red < 50:
	     
         img.itemset((nrow,ncol,0),0)
         img.itemset((nrow,ncol,1),0)
         img.itemset((nrow,ncol,2),200)
	     #for i in range(3): 
          #  img.itemset((nrow,ncol,i),0)

cv2.imwrite("picture-black-blackground.png", img)
