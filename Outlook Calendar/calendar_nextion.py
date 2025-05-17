import win32com.client, datetime, time, serial
from tkinter import *
import firebase_admin, time
from firebase_admin import credentials, db

# =================================================

class Firebase_Python():  
   def set_Credential(self):
      try:
         cred = credentials.Certificate("C:/Users/UZMN/OneDrive - SkyWater Technology/Documents/GitHub/home0-90616-firebase-adminsdk-kafvj-eb40a1993f.json")
         firebase_admin.initialize_app(cred, {
            'databaseURL': "https://home0-90616.firebaseio.com"
         })
      except:
         pass
      
   def get_Json_value(self):
      ref = db.reference('officewhereabout')
      print(ref.get())
      
   def put_Message(self, key, msg):
      firebase_ref = db.reference('officewhereabout')
      firebase_ref.update({key:msg})
      
# =================================================

class MySerial:
   ser = serial.Serial()
   
   def __init__(self, portcom):
      self.ser.port = portcom
   
   def serWrite(self, message):
      if self.ser.isOpen():
         time.sleep(1)
         try:
            self.ser.flushInput() #flush input buffer, discarding all its contents
            self.ser.flushOutput()
            time.sleep(0.5)
            self.ser.write(str.encode(message))  
         except Exception as e:
            print ("Error to write communication ...")
      else:
         print("Cannot open serial port")
   
   def serOpen(self):
      try: 
         self.ser.open()
      except Exception as e:
         self.ser.close()
         print ("error open serial port: " , str(e))
         exit()
   
   def serClose(self):
      self.ser.close()
      time.sleep(1)
	  
# ========================================================

class MyOutlook:
   myApp = win32com.client.Dispatch("Outlook.Application")
   nameSpace = myApp.GetNamespace("MAPI")
   Appts = nameSpace.GetDefaultFolder(9).Items
   Appts.IncludeRecurrences = True
   Appts.Sort("[Start]")
   
   def convert_OutlookTime_to_ISO8601(self, outlooktime):
      timestr = outlooktime.strftime("%Y-%m-%d %H:%M:%S.%f")
      return datetime.datetime.strptime(timestr, '%Y-%m-%d %H:%M:%S.%f')
   
   
   def get_Appointment(self, checkstarttime):
      now    = datetime.datetime.now()
      nowtimestamp   = datetime.datetime.timestamp(now)
      begin  = checkstarttime.strftime("%m/%d/%Y %H:%M")
      oneweek= checkstarttime + datetime.timedelta(days=14)
      end    = oneweek.date().strftime("%m/%d/%Y")
      appts  = self.Appts.Restrict("[Start] >= '" +begin+ "' AND [END] <= '" +end+ "'")
     
      event1 = None
      event1start = 0
      event2 = None
      firsteventNotfound = True
      
      for a in appts:
         starttime = self.convert_OutlookTime_to_ISO8601(a.Start) # datetime.datetime.strptime(startstr, '%Y-%m-%d %H:%M:%S.%f')
         starttimestamp = datetime.datetime.timestamp(starttime)
         
         endtime = self.convert_OutlookTime_to_ISO8601(a.End) # datetime.datetime.strptime(startstr, '%Y-%m-%d %H:%M:%S.%f')
         endtimestamp = datetime.datetime.timestamp(endtime)
         
         #print("Appointments: " , a.Start, a.End, a.Subject, a.BusyStatus,endtimestamp) #, a.Duration a.Organizer) # , a.End)
         # show appointment up to 10 minutes before it ends
         if nowtimestamp < (endtimestamp-600) and a.BusyStatus > 1: # 0=free, 1=tentative
            if firsteventNotfound:
               #print("Busy=", a.BusyStatus, nowtimestamp, datetime.datetime.fromtimestamp(endtimestamp))
               print("Busy Status", a.BusyStatus)
               event1 = a
               event1start = starttimestamp
               event1end   = endtimestamp
               firsteventNotfound = False
            else:
               event2 = a
               return event1, event1start, event1end, a, starttimestamp
      return None

# =====================================
      
class GUI():
   
   def Start_Reminder():
      global Reminder_Start_Flag
      Reminder_Start_Flag = True
   def Stop_Reminder():
      global Reminder_Start_Flag
      Reminder_Start_Flag = False
   def Update(self):
      self.master.update_idletasks()
      self.master.update()
   def setStatus(self, msg):
      self.Status.set("Status: " + msg)
   def Close(self):
      self.master.destroy()
   def setMeeting(self, meet1, location1, meet2, location2):
      self.MeetingName1.set(meet1)
      self.LocationName1.set(location1)
      self.MeetingName2.set(meet2)
      self.LocationName2.set(location2)
   
   
   master = Tk()
   master.title("Meeting Reminder User Interface")
   topFrame = Frame(master)
   topFrame.pack(side=TOP)
   
   PortName = StringVar()
   Label(topFrame, text="Serial Port").grid(row=0, column=0)
   Port_id = Entry(topFrame, textvariable=PortName, width=15)
   PortName.set("COM7")
   Port_id.grid(row=0, column=1)
   
   Button(topFrame, text="START reminder", command=Start_Reminder).grid(row=1, column=0)
   Button(topFrame, text="STOP reminder" , command=Stop_Reminder ).grid(row=1, column=1)

   MeetingName1  = StringVar()
   LocationName1 = StringVar()
   Label(topFrame, text="Meeting:").grid(row=2, column=0)
   Label(topFrame, textvariable=MeetingName1).grid(row=2, column=1)
   Label(topFrame, text="Location:").grid(row=3, column=0)
   Label(topFrame, textvariable=LocationName1).grid(row=3, column=1)

   MeetingName2  = StringVar()
   LocationName2 = StringVar()
   Label(topFrame, text="Next Meeting:").grid(row=4, column=0)
   Label(topFrame, textvariable=MeetingName2).grid(row=4, column=1)
   Label(topFrame, text="Next Location:").grid(row=5, column=0)
   Label(topFrame, textvariable=LocationName2).grid(row=5, column=1)
   
   bottomFrame = Frame(master)
   bottomFrame.pack(side=LEFT)
   Status = StringVar()
   Status_id = Label(bottomFrame, textvariable=Status)
   Status.set("Status: ...")
   Status_id.pack(side=LEFT)
   
# =====================================
   
global Reminder_Start_Flag
Reminder_Start_Flag = False

myGUI = GUI()

# === Comment the following lines to start automatically ==
#while not Reminder_Start_Flag:
#   myGUI.Update()
#   myGUI.setStatus("Waiting to start the Reminder .... ")
#   time.sleep(0.1)
# === Uncomment the following lines to start automatically ==
Reminder_Start_Flag = True

# Starting the reminder ======================
myOutlook = MyOutlook()
myserial = MySerial(myGUI.PortName.get())
myserial.serOpen()

# -------- Firebase -----------
myFirebase = Firebase_Python()
myFirebase.set_Credential()
myFirebase.get_Json_value()
now_hour = datetime.datetime.fromtimestamp(time.time()).hour
second_to_6PM = 18 - now_hour 
myFirebase.put_Message("expiration",(int(time.time())+3600*second_to_6PM)*1000)
myFirebase.put_Message("location",'"In the Building"')
# -----------------------------

counter = 0
interval_update = 60       # interval to check the calendar
start_time = time.time() - interval_update - 1 # make sure it starts 1st time
while 1:                   # to run indefinitely
# while counter < 500:

   elapsed = time.time()-start_time
   myGUI.setStatus("Count down to check calendar: " + str(interval_update-int(elapsed)))
   myGUI.Update()
   if not Reminder_Start_Flag: # stop the program
      break
    
   if elapsed > interval_update:
      start_time = time.time()
      counter = counter + 1
      myGUI.setStatus("Retrieving the Reminder ....")
      myGUI.Update()
      
      #myGUI.Close()
      
      nowtime = datetime.datetime.now()
      todaymidnight = datetime.datetime(nowtime.year, nowtime.month, nowtime.day)

      event1, event1start, event1end, event2, event2start = myOutlook.get_Appointment(todaymidnight)
   
      print("   =========== First next or current appointment ===========")
      nexttime1 = int(event1start - datetime.datetime.timestamp(nowtime))
      time1end  = int(event1end   - datetime.datetime.timestamp(nowtime))
      print("Current appointment:", event1.Subject , event1.Location, " in", nexttime1, "seconds")
   
      print("   =========== After that the next appointment ===========")
      nexttime2 = int(event2start - datetime.datetime.timestamp(nowtime))
      print("After that appointment:", event2.Subject, event2.Location, " in", nexttime2, "seconds")

      myGUI.setMeeting(event1.Subject, event1.Location, event2.Subject, event2.Location)
      
      # 1st appointment
      myserial.serWrite("1>" + event1.Subject[:25] + "<")
      # Delete the "SKY " in my particular case for room number
      location = event1.Location.replace("SKY B1.2 ","").replace(" Room","")
      myserial.serWrite("2>" + location + "<")
      myserial.serWrite("3>" + str(nexttime1) + "<")
      myserial.serWrite("4>" + str(time1end)  + "<")
   
    
      # 2nd appointment after the 1st one
      myserial.serWrite("5>" + event2.Subject[:25] + "<")
      # Delete the "SKY " in my particular case
      location = event2.Location.replace("SKY B1.2 ","").replace(" Room","")
      myserial.serWrite("6>" + location + "<")
      myserial.serWrite("7>" + str(nexttime2) + "<")
       
   time.sleep(0.5)

myserial.serClose()
myGUI.Close()


#print("=======The End ===============")

##################### GUI Loop ==========================
mainloop()
