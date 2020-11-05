#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int asd(int newSocket);

//Variables globales de argumentos recibidos en consola
int ARGUMENT_MAX_THREADS;
char* ARGUMENT_PATH;
char* ARGUMENT_PORT;
char *FILE_NAME;

char webpage[]=
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<html><head><title>Pagina Web</title></head>\r\n"
"<body><center><h1>PreFork Web Server</h1><br>\r\n"
"<img src=\"img.jpeg\"><br>"
"<label>Operativos2020</label></center></body></html>\r\n";

int spaceAvailable(int *arrayClients){
	
	for(int i=0; i<ARGUMENT_MAX_THREADS; i++){
		if(arrayClients[i] == -1){
			return 1;
		}
	}
	return 0;
}

int asd(int newSocket){

	/*
	char buf[2048];
	int img;
	struct stat st;

	memset(buf, 0, 2048);
	read(newSocket, buf, 2047);
	
	printf("buf::::%s\n", buf);

	if(!strncmp(buf, "GET /img.jpeg", 13)){
		img = open("img.jpeg", O_RDONLY);
		stat("img.jpeg", &st);  
		int size = st.st_size;
		sendfile(newSocket, img, NULL, size);
		close(img);
	}else{
	write(newSocket, webpage, sizeof(webpage)-1);}
	close(newSocket);
	*/
	return 1;
}

int startPreforkWebServer(){

	int sockfd;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	
	int arrayClients[ARGUMENT_MAX_THREADS];
	for(int i=0; i<ARGUMENT_MAX_THREADS; i++){
		arrayClients[i] = -1;
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error en conexion\n");
		exit(1);
	}
	printf("[+]Server socket creado\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(ARGUMENT_PORT));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		printf("[-]Error en binding\n");
		exit(1);
	}
	printf("[+]Bind al puerto %d\n", atoi(ARGUMENT_PORT));

	if(listen(sockfd, 1024) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error en binding\n");
	}

	while(spaceAvailable(arrayClients)){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		
		for(int i=0; i<ARGUMENT_MAX_THREADS; i++){
			if(arrayClients[i] == -1){
				arrayClients[i] = newSocket;
				break;
			}
		}
		
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if(!fork()){
			
			close(sockfd);
			
			while(1){
				bzero(buffer, sizeof(buffer));
				if(recv(newSocket, buffer, 1024, 0) < 0){
					printf("[-]Error in receiving data.\n");
				}
				else {
					//printf("buff->%s", buffer);
					char *method = strtok(buffer, "/");
					FILE_NAME = strtok(NULL, "\n");
					printf("Method: %s\n", method);
					printf("Trying to retrieve: %s\n", FILE_NAME);
					if(asd(newSocket) == 1){ //llamo a la funcion de gestion de la imagen
						send(newSocket, "STATUS: OK", strlen("STATUS: OK"), 0);
						bzero(buffer, sizeof(buffer));
						printf("Desconectando de %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
						printf("-------------------------------------------------------------------------------\n");
						break;
					}else{
					
						send(newSocket, "STATUS: BAD REQUEST", strlen("STATUS: BAD REQUEST"), 0);
						bzero(buffer, sizeof(buffer));
						printf("Desconectando de %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
						printf("-------------------------------------------------------------------------------\n");
						break;
					}
				}
			}
		}
		for(int i=0; i<ARGUMENT_MAX_THREADS; i++){
			if(arrayClients[i] == newSocket){
				arrayClients[i] = -1;
				break;
			}
		}
		close(newSocket);
	}
	printf("No hay espacios disponibles en el server.\n");

	return 0;
}

int createServer(int argc, char *argv[]){

	//Asignacion de valores desde los argumentos
    ARGUMENT_MAX_THREADS = atoi(argv[2]);
    ARGUMENT_PATH = argv[4];
    ARGUMENT_PORT = argv[6];

    //muestra los datos recibidos en la consola
    printf("HILOS RECIBIDOS [%d]\n", ARGUMENT_MAX_THREADS);
    printf("PATH RECIBIDO [%s]\n",ARGUMENT_PATH);
    printf("PUERTO RECIBIDO [%s]\n",ARGUMENT_PORT);

    startPreforkWebServer();
    return 0;
}

int main(int argc, char *argv[]){

	createServer(argc,argv);
	return 0;
}
