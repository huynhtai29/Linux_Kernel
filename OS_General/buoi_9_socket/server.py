import socket
import time


listensock = socket.socket()
Port = 8000
maxConnections = 999
IP = socket.gethostname()
listensock.bind(('192.168.1.6',Port))
listensock.listen(maxConnections)
print("server stared at " , IP + "on port" + str(Port))
if(listensock.accept())
print("new connection made !")  