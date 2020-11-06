#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* ARGUMENT_PATH;

int requestHandler(char *host, char* port, char* method, char *file);

int parsePath(int argc, char *argv[]){

	//Asignacion path
    ARGUMENT_PATH = argv[2];
    char *host = strtok(argv[2], ":");
    char *port = strtok(NULL, "/");
    char *method = strtok(NULL, "/");
    char *file = strtok(NULL, "\n");

    requestHandler(host, port, method, file);
    return 0;
}

int main(int argc, char *argv[]){

	parsePath(argc,argv);
	return 0;
}

int requestHandler(char *host, char* port, char* method, char *file){

	printf("Host: %s\n", host);
    printf("Puerto: %s\n", port);
    //printf("Puerto: %s\n", &port[2]); /este es el puerto para protocolos
    printf("Metodo: %s\n", method);
    printf("Nombre: %s\n", file);
	
	int clientSocket;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error en conexion\n");
		exit(1);
	}
	printf("[+]Client socket creado\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(port));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		printf("[-]Error en conexion\n");
		exit(1);
	}
	printf("[+]Conectado al server\n");

	
	bzero(buffer, sizeof(buffer));
	strcat(buffer, method);
	strcat(buffer, "/");
	strcat(buffer, file);
	send(clientSocket, buffer, strlen(buffer), 0);
	while(1){
		bzero(buffer, sizeof(buffer));
		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}
		else if(strcmp(buffer, "STATUS: OK") == 0){
			printf("STATUS: OK\n");
			exit(0);
		}else{
			printf("STATUS: BAD REQUEST\n");
			exit(0);
		}
	}
	close(clientSocket);

	return 0;
}
