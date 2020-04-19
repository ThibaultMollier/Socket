#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char* argv[])
{	

const char* hostname = "home";


int16_t opt = 0;
char requette[] = "get /sensor?room=chth&temp=19.0&hum=45.0&time=2020-04-07 HTTP/1.1\r\nHost: home\r\n\r\n";
while ((opt = getopt(argc,argv,":01")) != -1){
    switch (opt) {
        case '1':
            requette[0] = '1';
            break;
        case '0':
            requette[0] = '0';
            break;
        }
}

 int num_socket, num_connect, num_send, num_recv;
 struct sockaddr_in serveur;
 char IP[16]="192.168.1.27";				// @ Ip du serveur
 char reponse[1000]; 

 struct addrinfo hints,*res;
 memset (&hints, 0, sizeof (hints));
 hints.ai_family = AF_INET;
 hints.ai_socktype = SOCK_STREAM;
int err = getaddrinfo(hostname, "80", &hints, &res);
if (err !=0){
    perror("getaddrinfo");
    fprintf(stderr,"%d\n",err);
    return -1;
}

 
/******* Creation de la socket SOCK_STREAM : socket() ***************/
num_socket = socket(AF_INET,SOCK_STREAM,0);
if(num_socket == -1){
    perror("socket");
    return -1;
}

/******* Connexion avec un serveur distant : connect() ***************/
serveur.sin_family=   	AF_INET 	;
serveur.sin_port=		htons(80)	;
serveur.sin_addr.s_addr = inet_addr(IP)	;


//num_connect = connect(num_socket,(struct sockaddr*)&serveur,sizeof(serveur));
num_connect = connect(num_socket,res->ai_addr,res->ai_addrlen);
if(num_connect == -1){
    perror("connect");
    return -2;
}
/*****************  Requette : send() ****************************/

num_send = send(num_socket, requette,sizeof(requette),0);

/*****************  Réponse : rcv () ****************************/

num_recv = recv(num_socket,reponse,sizeof(reponse),0);

printf("%s \n",reponse);

/*****************  Fermeture socket : close() ******************/
close(num_socket);

return 0;	
}
