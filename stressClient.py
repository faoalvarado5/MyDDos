import socket
import time

c = int(raw_input("Numero de conexiones: \n"))
url = raw_input("URL del recurso: \n")

while(c):
	time.sleep(1)
	s = 0
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((socket.gethostname(), 8080))
	s.send(url)
	#msg = s.recv(1024)
	#print(msg.decode("utf-8"))
	c -= 1

print("Todas las solicitudes hechas, finalizado con exito.")
