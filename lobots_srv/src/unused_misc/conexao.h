#ifndef conexao_h
#define conexao_h

     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widget.h"
#include "balde.h"
#include "world.h"

#define BUFFER_SIZE 4096
#if defined(unix) || defined(__APPLE__)
  #define DIR_SEP '/'  
#else 
  #define DIR_SEP '\\'
  #include <winsock.h>
  #include <process.h>
#endif


class TConexao {
      public:
             TConexao( int port_local, int port_other, const char *srv, TDisplay *Display, TWorld *Mundo, TBlockContainer* Balde );
             TConexao();
     #if defined(unix) || defined(__APPLE__)
     #else
             struct {
            	WSADATA w;							/* Used to open windows connection */
            	unsigned short port_number;			/* Port number to use */
            	int a1, a2, a3, a4;					/* Components of address in xxx.xxx.xxx.xxx form */
            	int client_length;					/* Length of client struct */
            	int bytes_received;					/* Bytes received from client */
            	SOCKET sd;							/* Socket descriptor of server */
            	struct sockaddr_in server;			/* Information about the server */
            	struct sockaddr_in client;			/* Information about the client */
            	char buffer[BUFFER_SIZE];			/* Where to store received data */
            	struct hostent *hp;					/* Information about this computer */
            	char host_name[256];				/* Name of the server */
            	time_t current_time;				/* Current time */
            } in;
            struct {
                WSADATA w;								/* Used to open Windows connection */
                unsigned short port_number;				/* The port number to use */
                SOCKET sd;								/* The socket descriptor */
                int server_length;						/* Length of server struct */
                char send_buffer[BUFFER_SIZE];                       /* Data to send */
                time_t current_time;					/* Time received */
                struct hostent *hp;						/* Information about the server */
                struct sockaddr_in server;				/* Information about the server */
                struct sockaddr_in client;				/* Information about the client */
                int a1, a2, a3, a4;						/* Server address components in xxx.xxx.xxx.xxx form */
                int b1, b2, b3, b4;						/* Client address components in xxx.xxx.xxx.xxx form */
                char host_name[256];					/* Host name of this computer */                   
            } out;
     #endif
            
            bool connected;
            char *srv;  //server address
            TDisplay *Display;
            TBlockContainer *Balde;
            TWorld *Mundo;
            
            void MBox(char *msg);
            
            void Send(TBlockSingle *Peca, bool remove = false);
            void SendConnect();
            void Decode();
            
            
             
             


	                   
};
void Listener(void *Convoid);

#endif
