#include "protocols.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

//#define PORT 8080

//Variables globales de argumentos recibidos en consola
int ARGUMENT_MAX_THREADS;
char* ARGUMENT_PATH;
char* ARGUMENT_PORT;


//Variables globales para la creacion del PreThreadWebServer
int socket_Server;


//HTML
char HTML[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html> \r \n"
    "<html><head><title> Esto es un servidor web en C </title></head></html>\r\n"
    "<body> ema se la come </body>";

//variables para el manejo de hilos con la biblioteca pthread
int new_socket_connection;
int total_threads;
int thread_number = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *connection_handler(void *p_client_socket);//prototipo de la funcion


/*
    Manejo de argumentos para el servidor y su creacion
    comando de entrada
    $ prethread-webserver -n <cantidad-hilos> -w <path-www-root> -p <port>
*/
void processParameters(int argc, char *argv[]){

    //Asignacion de valores desde los argumentos
    ARGUMENT_MAX_THREADS = atoi(argv[2]);
    ARGUMENT_PATH = argv[4];
    ARGUMENT_PORT = argv[6];

    //muestra los datos recibidos en la consola
    printf("HILOS RECIBIDOS %d\n", ARGUMENT_MAX_THREADS);
    printf("PATH RECIBIDO %s\n",ARGUMENT_PATH);
    printf("PUERTO RECIBIDO %s\n",ARGUMENT_PORT);

    printf("Parametros aceptados\n");

}

int main(int argc, char *argv[]){

	processParameters(argc,argv);

    //Creacion de socket
    socket_Server = socket(AF_INET,SOCK_STREAM,0); //configura el socket para conexiones TCP

    int client_socket, addr_size;

    if (socket_Server == -1){
        printf("Error al crear la conexion con el socket\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Socket creado con el FD %d\n", socket_Server);
    }


    struct sockaddr_in serverAddr, clientAddr;

    //unir el programa al socket creado
    //memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(ARGUMENT_PORT));

    int responde_code = bind(socket_Server, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

    if(responde_code < 0){
        printf("Error al vincular el programa con el socket. Puerto %d\n",atoi(ARGUMENT_PORT));
        exit(-2);
    }
    printf("Programa vinculado al socket en el puerto %d\n ", atoi(ARGUMENT_PORT));



    printf("LISTEN\n");
    //escucha conexiones, desde 0 hasta ARGUMENT_MAX_THREADS
    if(listen(socket_Server, ARGUMENT_MAX_THREADS) == -1){

            printf("Error en listening......\n");
    }



    while(1){

        printf("Esperando conexiones entrantes\n");

        addr_size = sizeof(clientAddr);
        client_socket = accept(socket_Server, (struct sockaddr*)&clientAddr, &addr_size);

        thread_number++;
        printf("Nueva conexion detectada... Conexiones totales> %d\n", thread_number);


        if(client_socket == -1){

            printf("Error al procesar nueva conexion...\n");
        }
        else{
            printf("TOTAL: %d\n", thread_number);


            //para saber cual thread se esta procesando
            pthread_t track_thread;

            //se crea una referencia en memoria nueva, para no molestar la conexion original
            int *pclient = malloc(sizeof(int));
            *pclient = client_socket;

            thread_number++;
            pthread_create(&track_thread,NULL,connection_handler, pclient);
            pthread_join(track_thread, NULL);

        }
    }

	return 0;
}



/*
    acciones realizadas para cada thread de conexion
*/
void *connection_handler(void *p_client_socket){

    int new_socket = *(int*)(p_client_socket); //guarda en variable local la referencia recibida en parametro

    free(p_client_socket); //se libera la referencia, no se necesita por que tenemos una copia en new socket


        if (new_socket > 0){

            //hace algo con la conexion nueva
            //write(new_socket, HTML, sizeof(HTML));
            //conteo basico de threads procesadas
            pthread_mutex_lock(&lock);
            printf("una nueva conexion detectada: %d\n",thread_number);
            send(new_socket, HTML, sizeof(HTML),0);
            attendIncomingRequest(new_socket, ARGUMENT_PATH, atoi(ARGUMENT_PORT), ARGUMENT_MAX_THREADS, total_threads);

            shutdown(new_socket, 2); //codigo 2 indica cierre de hilo
            close(new_socket);
            printf("Un hilo acaba de ser terminado\n");
            thread_number--;
            pthread_mutex_unlock(&lock);


        }


    return NULL; //debe regresar null por que la funcion es void*
}
