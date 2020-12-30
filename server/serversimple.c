#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT_NUMBER 9999
#define BUFFER_SIZE 1024

/*
AF_INET IPv4 Internet Protocol
SOCK_STREAM TCP byte Stream
INADDR_ANY Bind to all local interfaces
*/

int main (){
  int csocket,new_s,bufferlength,recvbuffer;
  socklen_t addr_len;
  struct sockaddr_in my_addr, client_addr;
  char str_buf[BUFFER_SIZE];
  char c_addr[BUFFER_SIZE];
  char c_msg[BUFFER_SIZE];

  //Generate a socket
  if((csocket=socket(AF_INET,SOCK_STREAM,0))<0){
    fprintf(stderr,"Cannot create socket\n");
    exit(1);
  }
  printf("Inirial Socket:%d\n",csocket);
  //Clear the structure
  memset((char *)&my_addr,0,sizeof(my_addr));
  //Bind setting
  my_addr.sin_family=AF_INET;
  my_addr.sin_addr.s_addr=htonl(INADDR_ANY);//0.0.0.0
  my_addr.sin_port=htons(PORT_NUMBER);
  //Bind
  if(bind(csocket,(struct sockaddr *)&my_addr,sizeof(my_addr))<0){
    fprintf(stderr,"Cannot bind socket\n");
    exit(1);
  }
  //Wait for a connection at the specified port
  if(listen(csocket,10)<0){
    fprintf(stderr,"Cannot listen socket\n");
    exit(1);
  }
  printf("Waiting for a connection on %d ...\n",PORT_NUMBER);
  //Clearing the structure just in case
  memset((char *)&client_addr,0,sizeof(client_addr));
  addr_len=sizeof(client_addr);
  //Acceoring connections
  if((new_s=accept(csocket,(struct sockaddr *)&client_addr,&addr_len))<0){
    fprintf(stderr,"Cannot accept socker\n");
    exit(1);
  }
  printf("newSocket:%d\n",new_s);
  printf("%x:Connected. \n",client_addr.sin_addr.s_addr);
  sprintf(c_addr,"%s",inet_ntoa(client_addr.sin_addr));
  printf("%s:Conected.\n",c_addr);
  //recieve a data
  if((recvbuffer=recv(new_s,str_buf,BUFFER_SIZE,0))<0){
    fprintf(stderr, "Cannot receive message\n");
    exit(1);
  }
  str_buf[recvbuffer]='\n';
  c_msg[recvbuffer+2]='\n';
 //Return the data if the data is received
 if(recvbuffer>0){
   bufferlength=strlen(str_buf);
   sprintf(c_msg,"OK%s",str_buf);
   if(send(new_s,c_msg,bufferlength+2,0)<0){
     fprintf(stderr, "Cannot send message\n");
     exit(1);
   }
   
   printf("Received Message:%s\n",str_buf);
 }
 //Receive data for close
 if((recvbuffer=recv(new_s,str_buf,BUFFER_SIZE,0))<0){
   fprintf(stderr, "Cannot receive message\n");
   close(new_s);
   close(csocket);
   exit(1);
 }else if(recvbuffer==0){
   close(new_s);
   close(csocket);
   printf("Received close message.\n");
 }else{
   printf("Too Many Messages\n");
   close(new_s);
   close(csocket);
   exit(1);
 }
printf("End of this Program.\n");
return 0;
}




    
