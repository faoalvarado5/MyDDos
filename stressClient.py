import os
import sys
import threading
import socket


ejecuciones = -1;
binario = "";
parametros = "";

def attack(command):
	#s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	os.system(command)
	print("Ataque enviado")
	
	


if (sys.argv [1] == "-n" and len(sys.argv) != 4):
	print("USAGE: python stressClient.py -n [# clientes] httpClientC [parametros del cliente]")
	ejecuciones = int(sys.argv[2])
	binario = sys.argv[3]
	print("binario recibido "+binario)
	parametros = sys.argv[4]
	print("parametros recibido "+parametros)
	
	cmd = "./"+ str(binario) +" "+str(parametros)
	print(cmd)
	
	for i in range(ejecuciones):
		#os.system(cmd)
		threading.Thread(target=attack(cmd)).start()
	

else:
	print("USAGE: stress -n <cantidad-hilos> httpclient <parametros del cliente>")
	sys.exit(-2);

