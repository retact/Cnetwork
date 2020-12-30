#include <stdio.h>    
#include <stdlib.h>
#include <sys/types.h>//sub
#include <sys/socket.h>
#include <netinet/in.h>//struct sockaddr
#include <string.h>
#include <arpa/inet.h>//in_addr
#include <unistd.h>
#define PORT_NUMBER 9999
#define BUFFER_SIZE 1024

/*
AF_INET  IPv4 Internet Protocol
SOCK_STREAM  TCP byte Stream 
inet_addr()  Conversion to NetworkByteOrder//IPNumber
hton()  Conversion to NetworkOrder//PortNumber
*/

int main(int argc,char *argv[]){
  if(argc!=3){
    printf("Enter your Ip Address and message like this:\n client.exe 127.0.0.1 00X0000HOGEHOEGHOGE");
    exit(0);
  }

  int csocket,bufferlength,recvbuffer;
  struct sockaddr_in server_addr;
  char str_buf[BUFFER_SIZE];
  char *address = argv[1];
  //Generate a socket
  if((csocket=socket(AF_INET,SOCK_STREAM,0))<0){
    fprintf(stderr,"Cannot create socket\n");
    exit(1);
  }
  //Clear the structure
  memset((char *)&server_addr,0,sizeof(server_addr));
  //Connct Settings
  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=inet_addr(address);
  server_addr.sin_port=htons(PORT_NUMBER);
  
  if(connect(csocket,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
    fprintf(stderr,"Cannot connect\n");
    exit(1);
  }
  printf("Connected\n");
  
  sprintf(str_buf,"%s",argv[2]);
  bufferlength=strlen(str_buf);
  printf("SentMessage1:%s\n",str_buf);

  //Send a data
  if(send(csocket,str_buf,bufferlength,0)<0){
    fprintf(stderr,"Cannot send message\n");
    exit(1);
  }
  printf("SentMessage2:%s\n",str_buf);
  //Recive a data
  if((recvbuffer=recv(csocket,str_buf,bufferlength+2,0))<0){
    fprintf(stderr,"Cannot receive message\n");
    exit(1);
  }else{
    if(recvbuffer>0){
      str_buf[recvbuffer]='\n';
      printf("ReseivedMessage:%s\n",str_buf);
    }else{
      printf("Server connection is closed\n");
      close(csocket);
      exit(1);
    }
  }
  close(csocket);
  return(0);
}


