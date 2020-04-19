#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <netinet/in.h>


#define PORT_SERVEUR 		50000		// port d'écoute du serveur
#define IP_SERVEUR			"127.0.0.1"	//@IP serveur web

int main(int argc, char* argv[])
{
 

 
 int num_socket, num_bind, num_listen, num_accept;
 unsigned int size;
 struct sockaddr_in client;
 struct sockaddr_in serveur;
 char requette[100];
 char reponse0[]="Nous avons recu votre requete numero 0";
 char reponse1[]="Nous avons recu votre requete numero 1";
 size=sizeof(struct sockaddr_in);


 /******* Creation de la socket SOCK_STREAM : socket() ***************/
num_socket = socket(AF_INET,SOCK_STREAM,0);
if(num_socket == -1){
    perror("socket");
    return -1;
}
	
/******* Association entre l'adresse IP et le port d'écoute : bind() ***/
serveur.sin_family=AF_INET;
serveur.sin_port=htons(PORT_SERVEUR);
serveur.sin_addr.s_addr = inet_addr(IP_SERVEUR);

num_bind = bind(num_socket,(struct sockaddr *)&serveur,sizeof(serveur));
if(num_bind == -1){
    perror("bind");
    return -2;
}

/******* Configuration des demandes de connexions : listen () ***/
num_listen = listen(num_socket,3);
if(num_listen == -1){
	perror("listen");
	return -3;
}
while(1){
	
	/******* Attente de nouvelle connexions : accept ()  **************/
	num_accept = accept(num_socket,(struct sockaddr *)&client,&size);
	fprintf(stderr,"%s\n",inet_ntoa(client.sin_addr));
	/*****************  Réception de la requette : recv() **************/
	int i = recv(num_accept,requette,100,0);
	
	fprintf(stderr,"%d , %s\n",i , requette);
	/*****************  Envoi de la réponse : send() *******************/
	if (requette[0]=='1')
	{
		send(num_accept,reponse1,sizeof(reponse1),0);
	}else if(requette[0]=='0'){
		send(num_accept,reponse0,sizeof(reponse0),0);
	}else
	{
		send(num_accept,"error",sizeof(reponse0),0);
	}
	
	/*********  Fermeture connexion avec le client : close() ***********/
	close(num_accept);
	puts("Connexion closed");
}

	/*****************  Fermeture socket : close() ******************/
close(num_socket);
return 0;
}
