import wifi_class

wifi_obj = wifi_class.wifi_class(host="192.168.11.200")
Outstr  = '{"mag":0, "theta": 0, "delay": 10}<'

for i in range(10):
   wifi_obj.send_Message(Outstr)
   #wifi_obj.connect_Wifi()
   if wifi_obj.listen_Wifi(64): 
      in_json = wifi_obj.read_json
      print(in_json, in_json['yaw'])
wifi_obj.close_Connection()