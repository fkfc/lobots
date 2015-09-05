#include "conexao.h"
#include <string.h>
#include <stdlib.h>



void TConexao::Decode() {
     #if defined(unix) || defined(__APPLE__)
     #else
    /*       fread(&tam, 1, 1, arquivo); 
           fread(&nome, 1, tam+1, arquivo); 
           fread(&ry, 1, sizeof(float), arquivo); 
           fread(&x, 1, sizeof(float), arquivo); 
           fread(&y, 1, sizeof(float), arquivo); 
           fread(&z, 1, sizeof(float), arquivo); 
           fread(&r, 1, 1, arquivo); 
           fread(&g, 1, 1, arquivo); 
           fread(&b, 1, 1, arquivo);*/
         if (in.bytes_received < 10) return;  
         char nome[50];
         float x, y, z, ry;
         char r, g, b, tam;       
         int i;
         
         if (        
            in.buffer[0] != 'L' ||
            in.buffer[1] != 'E' ||
            in.buffer[2] != 'G' ||
            in.buffer[3] != 'O' ) return;
    
         if (        
            in.buffer[4] == 'C' &&
            in.buffer[5] == 'O' &&
            in.buffer[6] == 'N' ) return;        
            
         
         i = 7; 
       
         memcpy((void *) &tam, (void *) &in.buffer[i], 1);
         i++;
         memcpy((void*) nome, (void *) &in.buffer[i], tam + 1);
         i += tam + 1;
         memcpy((void*) &ry, (void *) &in.buffer[i], sizeof(float));
         i += sizeof(float);
         memcpy((void*) &x, (void *) &in.buffer[i], sizeof(float));
         i += sizeof(float);
         memcpy((void*) &y, (void *) &in.buffer[i], sizeof(float));
         i += sizeof(float);
         memcpy((void*) &z, (void *) &in.buffer[i], sizeof(float));
         i += sizeof(float);
         memcpy((void*) &r, (void *) &in.buffer[i], 1);
         i++;
         memcpy((void*) &g, (void *) &in.buffer[i], 1);
         i++;
         memcpy((void*) &b, (void *) &in.buffer[i], 1);
         i++;
           
           for (int i = 0; i < strlen(nome); i++) if (nome[i] == '\\' || nome[i] == '/') nome[i] = DIR_SEP;
           TBlockSingle *novaPeca = new TBlockSingle(Balde->GetBlock(nome), x, y, z);
           novaPeca->ry = ry;
           novaPeca->cor.r = r;
           novaPeca->cor.g = g;
           novaPeca->cor.b = b;
           
           if (        
            in.buffer[4] == 'A' &&
            in.buffer[5] == 'D' &&
            in.buffer[6] == 'D' ) Mundo->AddBlock(novaPeca);
            
           if (        
            in.buffer[4] == 'R' &&
            in.buffer[5] == 'E' &&
            in.buffer[6] == 'M' ) {
                         TBlockSingle *Del = 0;
                         Del = Mundo->GetCollision(novaPeca); 
                         if (Del) Mundo->RemoveBlock(Del);
                         delete(novaPeca);
                         }
            
                  
           
       #endif
       
}       

void TConexao::Send(TBlockSingle *Peca, bool remove) {
    #if defined(unix) || defined(__APPLE__)
    #else 
         char buffer[2048];
         int i;
         char *nome;
           float x, y, z, ry;
           char r, g, b, tam;
         
           
         tam = strlen(Peca->modelo->nome);  
         nome = Peca->modelo->nome;
         x = Peca->x;
         y = Peca->y;
         z = Peca->z;
         ry = Peca->ry;
         r = Peca->cor.r;
         g = Peca->cor.g;
         b = Peca->cor.b;
         
         
         if (remove == false) sprintf(buffer, "LEGOADD");
         else sprintf(buffer, "LEGOREM");
         
         i = 7;
         memcpy((void *) &buffer[i], (void *) &tam, 1);
         i++;
         memcpy((void *) &buffer[i], (void*) nome, tam + 1);
         i += tam + 1;
         memcpy((void *) &buffer[i],(void*) &ry, sizeof(float));
         i += sizeof(float);
         memcpy((void *) &buffer[i],(void*) &x, sizeof(float));
         i += sizeof(float);
         memcpy((void *) &buffer[i],(void*) &y, sizeof(float));
         i += sizeof(float);
         memcpy((void *) &buffer[i],(void*) &z, sizeof(float));
         i += sizeof(float);
         memcpy((void *) &buffer[i],(void*) &r, 1);
         i++;
         memcpy((void *) &buffer[i],(void*) &g, 1);
         i++;
         memcpy((void *) &buffer[i],(void*) &b, 1);
         i++;
    
         
         sendto(out.sd, buffer, i, 0, (struct sockaddr *)&(out.server), out.server_length);
     #endif
     
}

void TConexao::SendConnect() {
     #if defined(unix) || defined(__APPLE__)
     #else
    	sscanf(srv, "%d.%d.%d.%d", &(out.a1), &(out.a2), &(out.a3), &(out.a4));
    
    	/* Open windows connection */
    	if (WSAStartup(0x0101, &(out.w)) != 0)
    	{
    		Display->Messagebox("Could not open Windows connection");
    		return;
    	}
    
    	/* Open a datagram socket */
    	out.sd = socket(AF_INET, SOCK_DGRAM, 0);
    	if (out.sd == INVALID_SOCKET)
    	{
    		Display->Messagebox("Could not create socket.");
    		WSACleanup();
    		return;
    	}
    
    	/* Clear out server struct */
    	memset((void *)&(out.server), '\0', sizeof(struct sockaddr_in));
    
    	/* Set family and port */
    	out.server.sin_family = AF_INET;
    	out.server.sin_port = htons(out.port_number);
    
    	/* Set server address */
    	out.server.sin_addr.S_un.S_un_b.s_b1 = (unsigned char)out.a1;
    	out.server.sin_addr.S_un.S_un_b.s_b2 = (unsigned char)out.a2;
    	out.server.sin_addr.S_un.S_un_b.s_b3 = (unsigned char)out.a3;
    	out.server.sin_addr.S_un.S_un_b.s_b4 = (unsigned char)out.a4;
    	
    
    	/* Clear out client struct */
    	memset((void *)&(out.client), '\0', sizeof(struct sockaddr_in));
    
    	/* Set family and port */
    	out.client.sin_family = AF_INET;
    	out.client.sin_port = htons(0);
    
    		/* Get host name of this computer */
    		gethostname(out.host_name, sizeof(out.host_name));
    		out.hp = gethostbyname(out.host_name);
    
    		/* Check for NULL pointer */
    		if (out.hp == NULL)
    		{
    			Display->Messagebox("Could not get host name.");
    			closesocket(out.sd);
    			WSACleanup();
    			return;
    		}
    		
    
    		/* Assign the address */
    		out.client.sin_addr.S_un.S_un_b.s_b1 = out.hp->h_addr_list[0][0];
    		out.client.sin_addr.S_un.S_un_b.s_b2 = out.hp->h_addr_list[0][1];
    		out.client.sin_addr.S_un.S_un_b.s_b3 = out.hp->h_addr_list[0][2];
    		out.client.sin_addr.S_un.S_un_b.s_b4 = out.hp->h_addr_list[0][3];
    
    
    
    	/* Bind local address to socket */
    	if (bind(out.sd, (struct sockaddr *)&(out.client), sizeof(struct sockaddr_in)) == -1)
    	{
    		Display->Messagebox("Cannot bind address to socket.");
    		closesocket(out.sd);
    		WSACleanup();
    		return;
    	}
    
    	/* Tranmsit data  */
    	strcpy(out.send_buffer, "LEGOCON");
    
        out.server_length = sizeof(struct sockaddr_in);
    	if (sendto(out.sd, out.send_buffer, (int)strlen(out.send_buffer) + 1, 0, (struct sockaddr *)&(out.server), out.server_length) == -1)
    	{
    		Display->Messagebox("Error transmitting data\n(ServerConnect message)");
    		closesocket(out.sd);
    		WSACleanup();
    		return;
    	}
    	connected = true;
	#endif

}

TConexao::TConexao() {
                     connected = false;
}

TConexao::TConexao(int port_local, int port_other, const char *srv, TDisplay *Display, TWorld *Mundo, TBlockContainer* Balde ) {
   connected = false;                    
   this->srv = (char*) malloc(strlen(srv));
   strcpy(this->srv, srv);
   this->Display = Display;
   this->Balde = Balde;
   this->Mundo = Mundo;
   
   #if defined(unix) || defined(__APPLE__)
      Display->Messagebox("Funcionalidade em rede não está implementada em sistemas unix neste momento.");
   #else
      this->in.port_number = port_local;
      this->out.port_number = port_other;
       _beginthread(Listener,
                     0,
                     (void *) this);
   #endif              
                      
}

void TConexao::MBox(char *msg) {
    // MessageBox(Display, "teste");
    Display->IsVisible();
}

void Listener(void *Convoid) {
     #if defined(unix) || defined(__APPLE__)
     #else 
         TConexao *Con = (TConexao *) Convoid;
         
    
    	/* Open windows connection */
    	if (WSAStartup(0x0101, &(Con->in.w)) != 0)
    	{
            Con->Display->Messagebox("Could not open Windows connection");
            return;
    	}
    	
    
    	/* Open a datagram socket */
    	Con->in.sd = socket(AF_INET, SOCK_DGRAM, 0);
    	if (Con->in.sd == INVALID_SOCKET)
    	{
            Con->Display->Messagebox("Could not create socket.");
    		WSACleanup();
    		return;
    	}
    
    
    	/* Clear out server struct */
    	memset((void *)&(Con->in.server), '\0', sizeof(struct sockaddr_in));
    
    
    	/* Set family and port */
    	Con->in.server.sin_family = AF_INET;
    	Con->in.server.sin_port = htons(Con->in.port_number);
    	
    
    
    
    		/* Get host name of this computer */
    		gethostname(Con->in.host_name, sizeof(Con->in.host_name));
    		Con->in.hp = gethostbyname(Con->in.host_name);
    
    
    		/* Check for NULL pointer */
    		if (Con->in.hp == NULL)
    		{
                Con->Display->Messagebox("Could not get host name.");
    			closesocket(Con->in.sd);
    			WSACleanup();
    			return;
    		}
    		
    		
    		/* Assign the address */
    		Con->in.server.sin_addr.S_un.S_un_b.s_b1 = Con->in.hp->h_addr_list[0][0];
    		Con->in.server.sin_addr.S_un.S_un_b.s_b2 = Con->in.hp->h_addr_list[0][1];
    		Con->in.server.sin_addr.S_un.S_un_b.s_b3 = Con->in.hp->h_addr_list[0][2];
    		Con->in.server.sin_addr.S_un.S_un_b.s_b4 = Con->in.hp->h_addr_list[0][3];
    
    	/* Bind address to socket */
    	if (bind(Con->in.sd, (struct sockaddr *)&(Con->in.server), sizeof(struct sockaddr_in)) == -1)
    	{
            Con->Display->Messagebox("Could not bind name to socket.");
    		closesocket(Con->in.sd);
    		WSACleanup();
    		return;
    	}
     
        Con->in.client_length = (int)sizeof(struct sockaddr_in);
        
        Con->SendConnect();
        
         
    	/* Loop and get data from server */
    	while (1)
    	{
    		Con->in.client_length = (int)sizeof(struct sockaddr_in);
    		Con->in.bytes_received = recvfrom(Con->in.sd, Con->in.buffer, BUFFER_SIZE, 0, (struct sockaddr *)&(Con->in.client), &(Con->in.client_length));
            Con->Decode();
            //Con->Display->Messagebox("rec udp");
    	}
    	closesocket(Con->in.sd);
    	WSACleanup();

    #endif


}

