import wifi_class, time

class Snow_Blower:
   wifi_obj =  wifi_class.wifi_class("192.168.11.202", 8000)
      
   def turn_On(self):
      self.wifi_obj.send_Message('{"cmd":"ON"}')
      
   def turn_Off(self):
      self.wifi_obj.send_Message('{"cmd":"OFF"}')
      
# ==================== Testing ====================

if __name__ == "__main__":
   snow_obj = Snow_Blower()   
   
   snow_obj.turn_On()
   time.sleep(1)
   snow_obj.turn_Off()
   