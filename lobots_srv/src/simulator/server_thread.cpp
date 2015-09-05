/***************************************************************************
 *   Copyright (C) 2009 by Felipe Kermentz Ferraz Costa   *
 *   felipekfcosta@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

// Server Thread File


#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "users_table.h"
#include "server_thread.h"
#include "protocol.h"
//#include <linux/tcp.h>

 
#define fatal() \
        { printf("error!"); }


#define BUFFER_SIZE 512

TSimulador *sim;
t_tab_users* table;

void *srv_thread_function(void *arg) {
	printf("server thread started!\n"); // hooray
	
	
// initializing variables
	ptsrv_thread_args targs = (ptsrv_thread_args) arg;
	sim = targs->sim;
	int PORT = targs->port;
	
	
	
	
	int socketfd;                       /* Socket file descriptor  */
	struct sockaddr_in server_name;     /* server socket's address */
	struct sockaddr_in client_name;     /* client socket's address */

	fd_set active_fd_set, read_fd_set;  /* Array of file descriptors */

	int nbytes;
	char buffer[BUFFER_SIZE];
	
	int socketdest;
	char* pacote;
	char str_aux[80];
	FILE* motd;
	char name[150]; //robot name buffer

	table = start_table();
 /* Create the socket. 

    socket (NAMESPACE, STYLE, PROTOCOL)

      - namespace: local, other, internet: PF_INET || PF_INET6 

      - style: SOCK_STREAM, SOCK_DGRAM

      - protocol: 0 means 'auto' */

	socketfd = socket (PF_INET, SOCK_STREAM, 0); 

	if (socketfd < 0)
   	fatal ();

	/* Give the socket a name. */

	server_name.sin_family = AF_INET;                   /* Ineternet family */
	server_name.sin_port = htons (PORT);                /* set port  */
	server_name.sin_addr.s_addr = htonl (INADDR_ANY);   /* Any internetface */

	/* Bind the socket to the port. */

	if (bind (socketfd, (struct sockaddr *) &server_name, sizeof (server_name)) < 0)
		fatal ();
	
     // 23 apr 2010 - Disabling Nagle algorithm	in the protocol stack, so it sends packages
     // immeadiately, without waiting for more bytes to be available to send, which is more
     // appropriate for this real-time program,
     char flag = 1;
     setsockopt(socketfd, IPPROTO_TCP, 1, (char *) &flag, sizeof(int)); 

	/* Tell the OS to start to listen to the socket. */

	if (listen (socketfd, 1) < 0)
		fatal ();

	/* Initialize the set of active sockets. */

	FD_ZERO (&active_fd_set);
	FD_SET (socketfd, &active_fd_set);

	for (;;) {
		int i,j;

		/* Block until there is activity in the socket. */
		read_fd_set = active_fd_set;

		if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        fatal ();
	
	
	
	

     
     
     /* Service all the sockets with input pending. */

		for (i = 0; i < FD_SETSIZE; ++i) {
			if (FD_ISSET (i, &read_fd_set)) {
				if (i == socketfd) { /* Connection request on original socket. */
					int new_socket;
					socklen_t size;

					/* Accept the the connection and associate it
					with a new socket. */

					size = sizeof (client_name);
					new_socket = accept (socketfd, (struct sockaddr *) &client_name, &size);
					if (new_socket < 0)
						fatal ();
					fprintf (stderr, "Server: connect from host %s, port %hd.\n", inet_ntoa (client_name.sin_addr), ntohs (client_name.sin_port));
					FD_SET (new_socket, &active_fd_set);      /* Add the new socket to the set of active fd */
					
					// adding new user to the table
					//add_user (table, inet_ntoa (client_name.sin_addr), new_socket);
					add_user (table, new_socket);
				}
				else {                                 /* Data arriving on an already-connected socket. */
					nbytes = read (i, buffer, BUFFER_SIZE);
					buffer[nbytes-1] = 0;
					if (nbytes ==0) {                  /* Cient ended the transmission */ 
						printf ("Client disconnected.\n");
						remove_user (table, i);
						print_table (table);
						close (i);
						FD_CLR (i, &active_fd_set);
					}
					else if (nbytes < 0) {            /* Read error */ 
						fprintf (stderr, "Read error, client disconnected.\n");
						remove_user (table, i);
						print_table (table);
						close (i);
						FD_CLR (i, &active_fd_set);
					}
					else {                            // client message
						//debug
						//printf("%s\n", buffer);
						if (IsPackageValid(buffer, nbytes)) {
							if (NewRobot(buffer, name)) { //new robot connected, get name
								printf("New robot added: %s\n", name);
								set_id(table, i, name); //add the new name to the table
								strcpy(sim->robotnames[sim->num_available_clients], name);
								sim->num_available_clients++;
								print_table (table);
								sim->clientlist_update = true;
								//CreateUpdate(buffer, &nbytes, sim->GetRobot(get_id(table, i)));
								//write(i, buffer, nbytes);
							} else {
								//update settings for this robot
								if (GetUpdate(buffer, nbytes, sim->GetRobot(get_id(table, i))) ) {
									//printf("got update from %s\n", sim->GetRobot(get_id(table, i))->robot_name);
									CreateUpdate(buffer, &nbytes, sim->GetRobot(get_id(table, i)));
									write(i, buffer, nbytes);
								} else {
								    //printf("pacote de ok\n");
								    CreateUpdate(buffer, &nbytes, sim->GetRobot(get_id(table, i)));
								    write(i, buffer, nbytes);
								}
							}
						}
						//nbytes = write (i, buffer, strlen(buffer) + 1);
						
					
						/*switch (get_tipo(buffer)) {
						case 2:                       // mensagem privada;
							nome = get_dest(buffer);
							socketdest = busca_usuario(tabela, nome);
							if (socketdest) {
								nbytes = write (socketdest, buffer, strlen(buffer) + 1);
								nbytes = write (i, buffer, strlen(buffer) + 1);
							}
							free(nome);
						break;
						case 3:                      //mensagem de sauda��o de novo usuario
							nome = get_rem(buffer);
							adiciona_usuario(tabela, nome, i);
							imprime_tabela(tabela);
							free(nome);
							if ((motd = fopen("motd", "r")) != NULL) {   // abre o Message Of The Day
								while (fgets(str_aux,79, motd)) {
									pacote = monta_pacote_sis(str_aux, nome);
									nbytes = write (i, pacote, pacote[0]);
									usleep(2000);
								}
								fclose(motd);
							}
						default: // mensagem comum (tambem repassa para todos os usuarios a mensagem de saudacao - case 3 nao tem break)
							for (j = 3; j < FD_SETSIZE; ++j) {
								if (j != socketfd) nbytes = write (j, buffer, strlen(buffer) + 1);
    						}	
						break;
						case 4:                   // mensagens do sistema
							switch (get_sis(buffer)) {
							case 'l':             // cliente requisitando a listagem de usuarios conectados
								reset_iterator(tabela);
								nome = get_rem(buffer);
								strcpy(str_aux, "Usuarios conectados:\n");
								pacote = monta_pacote_sis(str_aux, nome);
								nbytes = write (i, pacote, pacote[0]);
								free(pacote);
								do {
									usleep(2000);
									strcpy(str_aux, get_next_nome(tabela));
									strcat(str_aux, "\n");
									pacote = monta_pacote_sis(str_aux, nome);
									nbytes = write (i, pacote, pacote[0]);
									free(pacote);
								} while (! iterator_finished(tabela));
								usleep(2000);
								strcpy(str_aux, "Fim da listagem.\n");
								pacote = monta_pacote_sis(str_aux, nome);
								nbytes = write (i, pacote, pacote[0]);
								free(pacote);
							break;
							}
						break;
						
						}*/
					}
				}
			}
		}
	}	
	
}

void StartRobot(TRobo *Robot) {
    printf("Starting robots!\n");
	char package[BUFFER_SIZE];
	int nbytes;
	int socket;
	socket = get_socket(table, Robot->robot_name);
	
	//CreateUpdate(package, &nbytes, Robot);
	//write(socket, package, nbytes);
	
	package[0] = 'l';
	package[1] = 'p';
	package[2] = '1';
	package[3] = 'S';
	package[4] = 'I';
	write(socket, package, 5);
	
	//CreateUpdate(package, &nbytes, Robot);
	//write(socket, package, nbytes);
	
}



void StopRobot(TRobo *Robot) {
    printf("Stopping robots\n");
	char package[BUFFER_SIZE];
	int nbytes;
	int socket;
	socket = get_socket(table, Robot->robot_name);
	
	package[0] = 'l';
	package[1] = 'p';
	package[2] = '1';
	package[3] = 'S';
	package[4] = 'O';
	write(socket, package, 5);
	
}


void SendWaypoint(TRobo *Robot, int x, int y, int z) {
	char package[BUFFER_SIZE];
	int nbytes;
	int socket;
	CreateWaypoint(package, &nbytes, x, y, z);
	socket = get_socket(table, Robot->robot_name);
	write(socket, package, nbytes);	
}