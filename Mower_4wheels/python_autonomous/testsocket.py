import socket, time

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 4003        # Port to listen on (non-privileged ports are > 1023)


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
   