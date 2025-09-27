import socket, time

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 4000        # Port to listen on (non-privileged ports are > 1023)

def Socket_listening():
   s =socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.bind((HOST, PORT))
   s.listen() 
   conn, addr = s.accept()
   print('Connected by', addr)
   
   s.settimeout(0.4)
   data = conn.recv(1024)
   if not data:
      print("No data, close socket")
      s.close()
      return
   print(data)
   s.close()
   print("============================")
   #break
      #time.sleep(0.1)
   
# ===============================================
while True:
   Socket_listening()