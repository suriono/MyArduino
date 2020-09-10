from flask import Flask, render_template, request, json
import serial, time

# ===================== Serial class ===================

class MySerial:
   ser = serial.Serial()

   #def __init__(self, portcom):
   def __init__(self):
      self.ser.baudrate = 57600

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

   def serClose(self):
      self.ser.close()

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
def signUp():
   print(mySerial.findPort())
   return render_template('simple-sender.html')

# ===================== Set Cursor, not used ==========

@app.route('/Set_Cursor', methods=['POST'])
def setcursor():
   data = {}
   data["X"]   = request.form['cursorx']
   data["Y"]   = request.form['cursory']
   data["cmd"] = "SetCursor"
   
   json_obj = json.dumps(data)
   mySerial.serOpen()
   mySerial.serWrite( json_obj )
   mySerial.serClose() 
   return json.dumps({'Command Received':'Set_Cursor'})

# ===================== Set Text =====================

@app.route('/Set_Text', methods=['POST'])
def settext():
   data = {}
   data["X"]    = request.form['cursorx']
   data["Y"]    = request.form['cursory']
   data["Text"] = request.form['sign_text']
   data["cmd"]  = "SetText"

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

# ====================== Test Connection =======================

@app.route('/Test_Connection', methods=['POST'])
def Test_Connection():
   print("Test Connection")
   print(mySerial.findPort())
   # mySerial.serClose()

   return json.dumps({'Test Connection':'Failed'})

# ====================== Web IP and Port ============================

if __name__=="__main__":
   app.run(host= '0.0.0.0',port=5002,debug=True)

# ===================== General Initialization ======================


