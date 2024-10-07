import socket, time

HOST = '192.168.0.27'  # Standard loopback interface address (localhost)
PORT = 8000        # Port to listen on (non-privileged ports are > 1023)


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        s.settimeout(1.0)
        while True:
            data = conn.recv(1024)
            if not data:
                print("No data, close socket")
                s.close()
                break
            print(data)
            s.close()
            print("============================")
            break
            time.sleep(0.1)
    #print ("close connection....")
    #s.close()
    #exit
   