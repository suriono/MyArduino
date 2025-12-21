import socket, time
# import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('192.168.254.151', 8000)
# print >>sys.stderr, 'connecting to %s port %s' % server_address
sock.connect(server_address)


isOK = True

for nn in range(0,50):
    
    #if isOK:
    try:
    
    # Send data
    #message = 'a'
    # print >>sys.stderr, 'sending "%s"' % message
        sock.settimeout(0.01)
        sock.sendall(b'hello uz  ')
    except:
        print("failed sendall")
            #time.sleep(0.01)

    # Look for the response
    #amount_received = 0
    #amount_expected = len(message)
    
    #while amount_received < 2 : #amount_expected:
    sock.settimeout(0.2)
    data = sock.recv(2)
    #time.sleep(0.01)
    #amount_received += len(data)
    #count = 0
    if data == b'OK':
        print(data)
    time.sleep(0.1)
    #    isOK = True
    #else:
    #    time.sleep(0.1)
    #    print("not OK")
    #    isOK = False
      #  print >>sys.stderr, 'received "%s"' % data
    
#    finally:
    # print >>sys.stderr, 'closing socket'
sock.close()