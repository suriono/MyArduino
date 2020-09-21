from flask import Flask, render_template, request, json
import serial, time, requests
from datetime import datetime
from datetime import timedelta
import threading 

DataJson = {}

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
   dimTimer.stop() 

   mySerial.serOpen()
   mySerial.serWrite('{"cmd":"DeleteAll"}');
   mySerial.serClose()
   return json.dumps({'Command Received':'Delete_All'})

# ====================== Save Pixels ============================

@app.route('/SavePixels', methods=['POST'])
def savepixel():
   dimTimer.stop() 

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
   DataJson["Pixels"]  = pixelcolors
   # print(DataJson["Pixels"])
   json.dump(pixeljson, fp)
   fp.close()

   dimTimer.restart() 
   return json.dumps({'Command Received':'Save'})

# ===================== Open Saved Pixels file ========================

def openSavedFile():
   try:
      global DataJson
      fp = open("lastPixels.json", "r")
      print("Load lastPixels.json file")
      DataJson = json.load(fp) 
      fp.close()
      return True
   except:
      print("Cannot open lastPixels.json file") 
      return False

# ====================== Load Saved Pixels ============================

@app.route('/Load_SavePixels', methods=['POST'])
def loadpixel():
   if openSavedFile():
      global DataJson
      dimTimer.stop() 
      data = {}
      data["Pixels"] = str(DataJson["Pixels"]) 

      data["cmd"]   = "SetPixels"
      json_obj     = json.dumps(data)
      mySerial.serOpen()
      mySerial.serWrite( json_obj )
      mySerial.serClose()
      dimTimer.restart() 

   return json.dumps({'Command Received':'Load Saved File'})

# ===================== Set Brightness =====================

def set_Brightness(bright):
   if openSavedFile():
      global DataJson
      newData = {}
      newp = []
      #print(DataJson["Pixels"])
      #print(len(DataJson["Pixels"]))
      count = 0
      for eachp in DataJson["Pixels"]:
         if count % 2:
            b = int((eachp & 255) * bright / 255)
            g = int(((eachp >> 8) & 255) * bright / 255) << 8
            r = int((eachp >> 16) * bright / 255) << 16 
            newcolor = b + g + r
            newp.append(newcolor)
            #newp.append(eachp)
         else:
            newp.append(eachp)

         count = count + 1
      
      data = {}
      data["Pixels"] = str(newp)
      data["cmd"]   = "SetPixels"
      json_obj     = json.dumps(data)
      mySerial.serOpen()
      mySerial.serWrite( json_obj )
      mySerial.serClose()

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

# ================== Test Night Time Brightness ========

@app.route('/Test_NightBrightness', methods=['POST'])
def nightbrightness():
   mySerial.serOpen()
   mySerial.serWrite('{"cmd":"Test_NightBrightness"}');
   mySerial.serClose()

   return json.dumps({'Command Received':'Test_NightBrightness'})

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

# ====================== Get Dusk and Dawn =========================

def get_Dusk_Dawn(lat, long):
   link = "http://api.sunrise-sunset.org/json?lat=%f&lng=%f&formatted=0" % (lat, long)
   f = requests.get(link)
   data = f.text
   dawn_h, dawn_m, dawn_s = data[34:42].split(':')
   dusk_h, dusk_m, dusk_s = data[71:79].split(':')
   # CST is 5+UTC
   dusk_h = (int(dusk_h) + 24 - 5) % 24
   sunrise = (int(dawn_h)-5) *60 + int(dawn_m)
   sunset  = dusk_h * 60 + int(dusk_m)

   now = datetime.now()
   midnight = now.replace(hour=0, minute=0, second=0, microsecond=0)
   now_minutes = int((now - midnight).seconds / 60)

   print("Now = %s, Sunrise = %s, Sunset = %s minutes" % (now_minutes, sunrise, sunset), end ='')

   return now_minutes > sunset or now_minutes < sunrise

# ================ Background whether to dim at night ============

class TimerDim():
   def __init__(self, interval):
      self.isDim          = False
      self.isCheckSunTime = True
      self.lasttime       = time.time()
      self.interval       = interval
      self.thread = threading.Thread(target=self.run, args=())
      self.thread.daemon  = True         # Daemonize thread
      self.thread.start()                # Start the execution

   def run(self):
      while True:
         elapsed = time.time() - self.lasttime

         if elapsed > self.interval and self.isCheckSunTime:
            isDark = get_Dusk_Dawn(44.74683,-93.193575)
            if isDark:
               print(" Nighttime, ", end = "")
               if not self.isDim:  # has not been dimmed yet
                  set_Brightness(15)
                  self.isDim = True
                  print("     =======   Time to dim =====")
            else:
               print(", Daytime, ", end = "")
               if self.isDim:  # has not been dimmed yet
                  set_Brightness(80)
                  self.isDim = False
                  print("     =======   Time to NOT to dim ====")

            # self.lasttime = time.time()
            print("Elapsed: %.1f" % (elapsed))
            time.sleep(self.interval)
         else:
            print("== Not checking Sun Time ====")
            time.sleep(4)
      print("========Exiting Sun Time Thread ====")

   def stop(self):
      self.isCheckSunTime = False  # global to stop dimTimer thread
      self.lasttime       = time.time()
      print("========Stop Sun Time Dimmer Thread ====")

   def restart(self):
      self.isCheckSunTime = True  # global to stop dimTimer thread
      print("========Restart Sun Time Dimmer Thread ====")

# ====================== Web IP and Port ============================

if __name__=="__main__":

   imTimer = TimerDim(10)

   app.run(host= '0.0.0.0',port=5000,debug=True)

# ===================== General Initialization ======================


