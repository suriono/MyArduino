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
     # print(red,green,blue)
#      if blue > 80 and green > 80 and red > 80:
      if blue > 120 and green > 120 and red > 120:
         for i in range(3): 
           # img.itemset((nrow,ncol,i),0)
            img[nrow,ncol] = 00

cv2.imwrite("picture-black-blackground.png", img)
print ("Image size: ", img.shape)
