#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>

#define BUF_SIZE 128 //sirve para definiciones de strings

//variable para cambiar la ruta de donde se encuentran los archivos html
char Files_Url[BUF_SIZE] = "/home/ubuntu/Desktop/Sistemas Operativos/Repositorio Programaciones/MyDDos/Files";

/*
    Entrada: Host y Puerto
    Salida: Tipo de variable para la informacion del host
*/
struct addrinfo *getHostInfo(char* host, char* port) {
  int r;
  struct addrinfo hints, *getaddrinfo_res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((r = getaddrinfo(host, port, &hints, &getaddrinfo_res))) {
    fprintf(stderr, "Host error %s\n", gai_strerror(r));
    return NULL;
  }

  return getaddrinfo_res;
}


/*
    Entrada: Recibe una clase addrinfo
    Salida: Codigo para la conexion
    Codigos
        -1 = conexion nula
        clienfd = conexion exitosa
*/
int establishConnection(struct addrinfo *info) {
  if (info == NULL) return -1;

  int clientfd;
  for (;info != NULL; info = info->ai_next) {
    if ((clientfd = socket(info->ai_family,
                           info->ai_socktype,
                           info->ai_protocol)) < 0) {
      perror("SOcket error");
      continue;
    }

    if (connect(clientfd, info->ai_addr, info->ai_addrlen) < 0) {
      close(clientfd);
      perror("Conecction error");
      continue;
    }

    freeaddrinfo(info);
    return clientfd;
  }

  freeaddrinfo(info);
  return -1;
}

/*
    Entrada: clientfd (resultado del metodo establishConnection)
    Salida: Ninguna, mensaje en pantalla sobre el get realizado
    Segun el url de los archivos, realiza un GET y envia al
    cliente la solicitud
*/
void GET(int clientfd) {
  char req[1000] = {0};
  sprintf(req, "GET %s HTTP/1.0\r\n\r\n",Files_Url);
  send(clientfd, req, strlen(req), 0);
}

/*
    Entrada: Argumentos en consola
    Salida: Codigo de resultado de ejecucion
        1: no puso suficientes argumentos
        -1: fallo de conexion
        0: Realizacion correcta del GET

*/
int main(int argc, char **argv) {
  int clientfd;
  char buf[BUF_SIZE];

  if (argc != 3) {
    fprintf(stderr, "--> ./httpclientC hostname port\n");
    return 1;
  }

  //Conexion con el host
  clientfd = establishConnection(getHostInfo(argv[1], argv[2]));
  if (clientfd == -1) {
    fprintf(stderr,
            "Failed to connect to: %s:%s%s \n",
            argv[1], argv[2], argv[3]);
    return -1;
  }

  //ejecucion del GET
  GET(clientfd);
  while (recv(clientfd, buf, BUF_SIZE, 0) > 0) {
    fputs(buf, stdout);
    memset(buf, 0, BUF_SIZE);
  }

  close(clientfd);
  return 0;
}
