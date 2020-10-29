import pycurl
from io import BytesIO

bytes_Object = BytesIO() #Captura los bytes de un archivo

curl_connection = pycurl.Curl() #habilita las conexiones REST

given_url = raw_input("Direccion web del recurso a descargar\n")

curl_connection.setopt(curl_connection.URL, given_url) #Configura la conexion curl al url ingresado

curl_connection.setopt(curl_connection.WRITEDATA, bytes_Object) #Escribe los bytes codificados en utf-8 a la conexion curl

curl_connection.perform() #Transfiere los datos del archivo al url destino

curl_connection.close() #Cierra las conexiones REST

get_content = bytes_Object.value() #Obtiene el contenido guardado en los bytes de bytes_object

