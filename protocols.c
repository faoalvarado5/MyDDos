
#include "protocols.h"
#include <stdio.h>


char *_serverFilesPath = "/home/ubuntu/Desktop/Operativos/Progras/MyDDos/Files";
int delay = 1;

//Puerto 8080 defecto, el resto segun las referencias
void attendIncomingRequest(int socket_number, char *ARGUMENT_PATH, int ARGUMENT_PORT, int ARGUMENT_MAX_THREADS, int total_threads)
{
    switch(ARGUMENT_PORT){
        case 8080:
            attendIncomingHttpRequest(socket_number, ARGUMENT_PATH);
            break;

        case 8021:
            attendIncomingFtpRequest(socket_number, ARGUMENT_MAX_THREADS, total_threads);
            break;

        case 8022:
            attendIncomingSSHRequest(socket_number);
            break;

        case 8023:
            attendIncomingTelnetRequest(socket_number);
            break;

        case 8025:
            attendIncomingSMTPRequest(socket_number);
            break;

        case 8053:
            attendIncomingDNSRequest(socket_number);
            break;

        case 8161:
            attendIncomingSNMPRequest(socket_number);
            break;

        default:
            attendIncomingHttpRequest(socket_number, ARGUMENT_PATH);
            break;
    }

}

void attendIncomingDNSRequest(int pConnection)
{
	printf("Conexion con el puerto DNS reconocida");
    sleep(1);
}

void attendIncomingSSHRequest(int pConnection)
{
	printf("Conexion con el puerto SSH reconocida");
    sleep(1);
}

/*Funcion que se encarga de atender una nueva peticion*/
void attendIncomingHttpRequest(int pConnection, char *pServerRootPath)
{
	printf("Conexion con el puerto HTTP reconocida\n");
	sleep(1);
}



void attendIncomingFtpRequest(int pConnection, int pActiveProcess, int pProcessMax)
{
	int activeProcessInt = pActiveProcess;
	char activeProcessStr[21];
	char maxProcessStr[21];
	sprintf(activeProcessStr, "%d", activeProcessInt);
	sprintf(maxProcessStr, "%d", pProcessMax);
	char ftpResponse[85] = "220 FTP server, ";
	strcat(ftpResponse, activeProcessStr);
	strcat(ftpResponse, " Clientes activos de ");
	strcat(ftpResponse, maxProcessStr);
	strcat(ftpResponse, " Simulación de clientes permitidos.\r\n");
	printf("%s",ftpResponse);

	sleep(1);
}

void attendIncomingSNMPRequest(int pConnection)
{
	char *versionSNMP = "Version: 1\r\n";
	char *community = "Community: public \r\n";
	char *pdu = "PDU type: GET\r\n";
	char *request = "Request Id: 0\r\n";
	char *error = "Error Status: NO ERROR\r\n";
	char *errorIndex = "Error Index: 0\r\n";
	printf("%s, %s, %s, %s, %s, %s, ",versionSNMP, community, pdu, request, error, errorIndex);
	sleep(1);
}

void attendIncomingTelnetRequest(int pConnection)
{
	char *welcomeTelnet = "Bienvenido al servidor...\r\n";
	char *loginMsg = "Login: \r\n";
	char *passwordMsg = "Password: \r\n";
	printf("%s, %s, %s",welcomeTelnet, loginMsg, passwordMsg);
	sleep(1);
}

void attendIncomingSMTPRequest(int pConnection)
{
	char *welcomeSMTP = "220 Server SMTP\r\n";
	printf("%s", welcomeSMTP);
	sleep(1);
}
/*

void attendOutputGetRequest(int pConnection, char* resource, char *pServerRootPath)
{
	/*if(!strcmp(pRequestInfo->_resource, "/"))
	{
		pRequestInfo->_resource = "/index.html";
		pServerRootPath = _serverFilesPath;
	}
	int resource = checkResource(pRequestInfo, pServerRootPath);
	if ( resource < 0)
	{
	    if (errno == EACCES)
	    {
	    	pRequestInfo->_status = 401;
	    }
	    else
	    {
	    	pRequestInfo->_resource = "/404.html";
	    	resource = checkResource(pRequestInfo, _serverFilesPath);
	    	pRequestInfo->_status = 404;
	    }
	}

    //obtiene la extension
	char *Extension = strtok(resource,".");
	char *Content;

	if(!strcmp(Extension, "html")){
    		Content = "text/html";
    	}
    	else if(!strcmp(Extension, "cgi")){
    		Content = "text/html";
    	}
    	else if(!strcmp(Extension, "jpeg")){
    		Content = "image/jpeg";
    	}
    	else if(!strcmp(Extension, "png")){
    		Content = "image/png";
    	}
    	else if(!strcmp(Extension, "txt")){
    		Content = "text";
    	}
    	else if(!strcmp(Extension, "jpg")){
    		Content = "image/jpg";
    	}
    	else if(!strcmp(Extension, "css")){
    		Content = "text/css";
    	}
    	else if(!strcmp(Extension, "js")){
    		Content = "application/javascript";
    	}
    	else if(!strcmp(Extension, "xml")){
    		Content = "application/xml";
    	}
    	else if(!strcmp(Extension, "mp3")){
    		Content = "audio/mpeg";
    	}
    	else if(!strcmp(Extension, "mpeg")){
    		Content = "video/mpeg";
    	}
    	else if(!strcmp(Extension, "mpg")){
    		Content = "video/mpeg";
    	}
    	else if(!strcmp(Extension, "mp4")){
    		Content = "video/mp4";
    	}
    	else if(!strcmp(Extension, "mpv")){
    		Content = "video/quicktime";
    	}
    	else{
    		printf("No se reconoce el archivo ingresado \n");
    		exit(EXIT_FAILURE);
    	}




	int auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
	if(auxValue == -1)
	{
		print("Error: No se pudo abrir el recurso!! \n");
	}
	else
	{
		if (pRequestInfo->_status == 200 || pRequestInfo->_status == 404)
		{
			if (sendResource(pConnection, resource) < 0)
			{
				print("No se pudo enviar el archivo al destinatario.");
			}
			close(resource);
		}
	}
}*/

/*void attendOutputRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType)
{
	if(pRequestInfo->_method == UNSUPPORTED)
	{
		pRequestInfo->_status = 501;
		int auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
	else if(pRequestInfo->_method == GET)
	{
		attendOutputGetRequest(pConnection, pRequestInfo, pServerRootPath, pContentType);
	}
}*/
