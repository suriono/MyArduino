from flask import Flask, render_template, request, json
import serial, time

# ===================== Serial class ===================

class MySerial:
   ser = serial.Serial()

   # ---------------------------
   #def __init__(self, portcom):
   def __init__(self):
      self.ser.baudrate = 115200
      self.ser.timeout = 1

   # ---------------------------
   def findPort(self):  # find Arduino serial port
      def testConnection():
         try:
            self.ser.open()
            self.serWrite('{"cmd":"TestConnection"}')
            readline = self.ser.readline()
            print(readline.decode())
            self.ser.close()

            return (readline.decode().rstrip() == "OK")
         except:
            return False

      self.ser.port = '/dev/ttyACM0'
      print(self.ser.port)
      if testConnection():
         return "Port: " + self.ser.port
      self.ser.port = '/dev/ttyACM1'
      print(self.ser.port)
      if testConnection():
         return "Port: " + self.ser.port
         return
      self.ser.port = '/dev/ttyACM2'
      print(self.ser.port)
      if testConnection():
         return "Port: " + self.ser.port
         return

   # ---------------------------
   def serOpen(self):
      try: 
         self.ser.open()
         print("Port: ", self.ser.port)
      except Exception as e:
         # print ("Error open serial port: " , str(e))
         self.findPort()
         # self.ser.close()
         try: 
            self.ser.open()
            print("Port: ", self.ser.port)
         except Exception as e:
            # print ("Error open serial port: " , str(e))
            exit()

   # ---------------------------
   def serClose(self):
      self.ser.close()

   # ---------------------------
   def serWrite(self, message):
      if self.ser.isOpen():
         #time.sleep(0.001)
         try:
            self.ser.flushInput() #flush input buffer, discarding all its contents
            self.ser.flushOutput()
            #time.sleep(0.1)
            self.ser.write(str.encode(message))  
         except Exception as e:
            print ("Error to write communication ...")
      else:
         print("Cannot open serial port")

   # ---------------------------
   def serReadline(self):
      if self.ser.isOpen():
         return self.ser.readline()
      else:
         print("Cannot open serial port")

# mySerial = MySerial('/dev/ttyACM0')
mySerial = MySerial()

# ===================== Main Webpage ===================

app = Flask(__name__)

@app.route('/')
def mainhome():
   print(mySerial.findPort())
   return render_template('simple-sender.html')

# ===================== Set Cursor, not used ==========

@app.route('/Set_Cursor', methods=['POST'])
def setcursor():
   data = {}
   data["cmd"] = "SetCursor"
   data["X"]   = request.form['cursorx']
   data["Y"]   = request.form['cursory']
   
   json_obj = json.dumps(data)
   mySerial.serOpen()
   mySerial.serWrite( json_obj )
   mySerial.serClose() 
   return json.dumps({'Command Received':'Set_Cursor'})

# ===================== Set Text =====================

@app.route('/Set_Text', methods=['POST'])
def settext():
   data = {}
   data["cmd"]   = "SetText"
   data["X"]     = request.form['cursorx']
   data["Y"]     = request.form['cursory']
   data["Size"]  = request.form['textsize']
   data["Red"]   = request.form['red']
   data["Green"] = request.form['green']
   data["Blue"]  = request.form['blue']
   data["Text"]  = request.form['sign_text']

   json_obj     = json.dumps(data)
   print(json_obj)
   mySerial.serOpen()
   mySerial.serWrite( json_obj )
   mySerial.serClose()
   return json.dumps({'Command Received':'Set_Text'})

# ====================== Delete All ============================

@app.route('/Delete_All', methods=['POST'])
def Delete_All():
   mySerial.serOpen()
   mySerial.serWrite('{"cmd":"DeleteAll"}');
   mySerial.serClose()
   return json.dumps({'Command Received':'Delete_All'})

# ====================== Save Pixels ============================

@app.route('/SavePixels', methods=['POST'])
def savepixel():
   pixelcolors = []
   mySerial.serOpen()
   mySerial.serWrite('{"cmd":"SavePixel"}');
   fp = open("lastPixels.json", "w")
   readsave = mySerial.serReadline()
   while len(readsave):
      pixelcolors.append(int(readsave.decode()))
      readsave = mySerial.serReadline()
   mySerial.serClose()
   pixeljson = {}
   pixeljson["Pixels"] = pixelcolors
   json.dump(pixeljson, fp)
   fp.close()
   return json.dumps({'Command Received':'Save'})

# ====================== Load Saved Pixels ============================

@app.route('/Load_SavePixels', methods=['POST'])
def loadpixel():
   try:
      fp = open("lastPixels.json", "r")
      print("Load")
      pixdata = json.load(fp) 
      fp.close()

      data = {}
      #data["Pixels"] = "\"" + str(pixdata["Pixels"]) + "\""
      data["Pixels"] = str(pixdata["Pixels"]) 

      data["cmd"]   = "SetPixels"
      json_obj     = json.dumps(data)
      print(json_obj)
      mySerial.serOpen()
      mySerial.serWrite( json_obj )
      mySerial.serClose()
   except:
      print("Cannot open lastPixels.json file") 
   return json.dumps({'Command Received':'Load Saved File'})

# ====================== Undo ============================

@app.route('/Undo', methods=['POST'])
def undo():
   data = {}
   data["cmd"]   = "SetText"
   data["X"]     = request.form['cursorx']
   data["Y"]     = request.form['cursory']
   data["Red"]   = 0
   data["Green"] = 0
   data["Blue"]  = 0
   data["Text"]  = request.form['sign_text']

   json_obj     = json.dumps(data)
   print(json_obj)
   mySerial.serOpen()
   mySerial.serWrite( json_obj )
   mySerial.serClose()

   return json.dumps({'Command Received':'Undo'})

# ====================== Set Brightness =======================

@app.route('/Set_Brightness', methods=['POST'])
def brightness():
   data = {}
   data["cmd"]    = "SetBrightness"
   data["Bright"] = request.form['brightness']

   json_obj       = json.dumps(data)
   print(json_obj)
   mySerial.serOpen()
   mySerial.serWrite( json_obj )
   mySerial.serClose()

   return json.dumps({'Command Received':'Set_Brightness'})

# ===================== Set Text =====================

@app.route('/Set_Pixels', methods=['POST'])
def setpixel():
   data = {}
   data["cmd"]    = "SetPixels"
   data["Pixels"] = request.form['pixels']

   json_obj       = json.dumps(data)
   print(json_obj)
   mySerial.serOpen()
   mySerial.serWrite( json_obj )
   mySerial.serClose()
   return json.dumps({'Command Received':'Set_Pixels'})

# ====================== Web IP and Port ============================

if __name__=="__main__":
   app.run(host= '0.0.0.0',port=5002,debug=True)

# ===================== General Initialization ======================


