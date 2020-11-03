#include <pthread.h>
#include "protocols.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#define PORT 8080

//Variables globales de argumentos recibidos en consola
int ARGUMENT_MAX_THREADS;
char* ARGUMENT_PATH;
char* ARGUMENT_PORT;

//Variables globales para la creacion del PreThreadWebServer
int socket_Server;

//variables para el manejo de hilos con la biblioteca pthread
int new_socket_connection;
int total_threads;
int thread_number = 0;
pthread_mutex_t new_socket_mutex;
pthread_mutex_t thread_number_mutex;
pthread_cond_t threads_condition;
pthread_cond_t accept_condition;

void *connectionHandler(){

    int new_socket;

    while(1){

        //bloqueamos el thread actual
        pthread_mutex_lock(&new_socket_mutex);

        while(new_socket_connection == -1){

            pthread_cond_wait(&threads_condition, &new_socket_mutex);
        }

        new_socket = new_socket_connection; //establecemos la nueva conexion
        new_socket_connection = -1; //reinicia la bandera para el while anterior

        pthread_mutex_unlock(&new_socket_mutex); //desbloqueamos el thread

        if (new_socket > 0){
            attendIncomingRequest(new_socket, ARGUMENT_PATH, atoi(ARGUMENT_PORT), ARGUMENT_MAX_THREADS, total_threads);
            shutdown(new_socket, 2); //codigo 2 indica cierre de hilo
            close(new_socket);
            printf("Un hilo acaba de ser terminado\n");
            pthread_mutex_lock(&thread_number_mutex);
            total_threads--;
            pthread_mutex_unlock(&thread_number_mutex);
            pthread_cond_signal(&accept_condition);
        }
    }

    pthread_exit(NULL);
}

/*
    Se encarga de la configuracion para levantar el servidor
*/
void startPrethreadWebServer(){

    //Creacion de socket
    socket_Server = socket(AF_INET,SOCK_STREAM,0);
    if (socket_Server == -1){
        printf("Error al crear la conexion con el socket\n");
        exit(EXIT_FAILURE);
    }

    //unir el programa al socket creado
    struct sockaddr_in serverAddr;

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(ARGUMENT_PORT));

    int responde_code = bind(socket_Server, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

    if(responde_code < 0){
    	printf("Error al vincular el programa con el socket. Puerto %d\n",atoi(ARGUMENT_PORT));
    	exit(EXIT_FAILURE);
    }
    printf("Programa vinculado al socket en el puerto carepichas%d\n ", atoi(ARGUMENT_PORT));


    //Se crean los hilos para la conexion
    for(int i = 0; i < ARGUMENT_MAX_THREADS; i++){
        printf("response obtenido %d \n", i);
        pthread_t newThread;
        int responde_code = pthread_create(&newThread, NULL, connectionHandler, NULL);

        if (responde_code){
            printf("No se pudo crear el hilo nuevo para la conexion\n");
        }
    }


	//escucha conexiones nuevas
    printf("LISTEN");
    if(listen(socket_Server, 1024) == -1){
	
			printf("Error en listening......\n");
	}
    printf("Esperando conexiones entrantes\n");
    
    int new_connection;
    
    while(1){
	
		new_connection = accept(socket_Server, NULL, NULL);
		printf("Nueva conexion detectada...\n");
		if(new_connection == -1){
		
			printf("Error al proncesar nueva conexion...\n");
		}
		else{
			printf("TOTAL: %d\n", thread_number);
			
			
			pthread_mutex_lock(&new_socket_mutex);
         	int _newSocket = new_connection;
         	pthread_mutex_unlock(&new_socket_mutex);
         	pthread_cond_broadcast(&threads_condition);
         	pthread_mutex_lock(&thread_number_mutex);
         	thread_number++;
         	attendIncomingRequest(new_socket, ARGUMENT_PATH, atoi(ARGUMENT_PORT), ARGUMENT_MAX_THREADS, thread_number);
         	if (thread_number == ARGUMENT_MAX_THREADS)
         	{ 
            	pthread_cond_wait(&accept_condition, &thread_number_mutex);
            	printf("Maximas conecciones ");
         	}
         	pthread_mutex_unlock(&thread_number_mutex);
		}
	}
}


/*
    Manejo de argumentos para el servidor y su creacion
    comando de entrada
    $ prethread-webserver -n <cantidad-hilos> -w <path-www-root> -p <port>
*/
void createWebServer(int argc, char *argv[]){

    //Asignacion de valores desde los argumentos
    ARGUMENT_MAX_THREADS = atoi(argv[2]);
    ARGUMENT_PATH = argv[4];
    ARGUMENT_PORT = argv[6];

    //muestra los datos recibidos en la consola
    printf("HILOS RECIBIDOS %d\n", ARGUMENT_MAX_THREADS);
    printf("PATH RECIBIDO %s\n",ARGUMENT_PATH);
    printf("PUERTO RECIBIDO %s\n",ARGUMENT_PORT);

    printf("flag");
    startPrethreadWebServer();


}

int main(int argc, char *argv[]){

	/*int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 8080);

    //Creamos un hilo para la conexion
    //createThreads(int maxThreads)

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}*/


    createWebServer(argc,argv);

	/*while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			printf("Connection success\n");
		}

	}

	close(newSocket);*/

	return 0;
}
