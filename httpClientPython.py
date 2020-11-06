import pycurl
import sys
from io import BytesIO

if ( sys.argv[1] == "-u" and sys.argv[2] != None):

	bytes_Object = BytesIO() #Captura los bytes de un archivo

	curl_connection = pycurl.Curl() #habilita las conexiones REST

	given_url = sys.argv[2]

	curl_connection.setopt(curl_connection.URL, given_url) #Configura la conexion curl al url ingresado

	curl_connection.setopt(curl_connection.WRITEDATA, bytes_Object) #Escribe los bytes codificados en utf-8 a la conexion curl

	curl_connection.perform() #Transfiere los datos del archivo al url destino

	curl_connection.close() #Cierra las conexiones REST

	get_content = bytes_Object.getvalue() #Obtiene el contenido guardado en los bytes de bytes_object
	
	print("Resultado del GET request \n%s",get_content.decode('utf8'))

else:
	print("USAGE: python httpClientPython -u <url-de-recurso-a-obtener>")
