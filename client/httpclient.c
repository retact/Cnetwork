/*httpclient simple program
  */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>//_LP64,_ILP32
#include<sys/socket.h>//socket interface
#include<netinet/in.h>//internet,protocol,family
#include<netdb.h>//network database
#include<arpa/inet.h>//network control define
#include<unistd.h>//implementation specific functions

//Set Port Number and Buffer size
#define PORT_NUMBER 80//HTTP
#define BUFFER_SIZE 8000


int main(int argc,char *argv[]){
  int csocket,bufferlength,recvbuffer,str_len;
  struct sockaddr_in server_addr;
  struct hostent *servhost;
  char str_buf[BUFFER_SIZE];
  char *uribox,host_name[256],file_name[256];
  int loopnum=0;

  if(argc<2){
    printf("Make sure you use it correctry:httpclient.exe hostname/filename\r\nAssign command line arguments");
    exit(1);
  }

  uribox=strstr(argv[1],"/");
  if(uribox==NULL){
    printf("NOT found/.\n");
    printf("Make sure you use it correctry:httpclient.exe hostname/filename\n");
    exit(1);
  }else{
    str_len=strlen(argv[1])-strlen(uribox);
    strncpy(host_name,argv[1],str_len);
    host_name[str_len]='\0';
    strcpy(file_name,uribox);
    printf("str_len:%d\n",str_len);
    printf("--HOSTNAME:%s\n",host_name);
    printf("--FILENAME:%s\n",file_name);
    sprintf(str_buf,"GET%sHTTP/1.1\r\n",file_name);
    sprintf(str_buf,"%sHost:%s\r\nConnection:close\r\n",str_buf,host_name);
    sprintf(str_buf,"%s\r\n",str_buf);
  }

  //Create a socket
  if((csocket=socket(AF_INET,SOCK_STREAM,0))<0){
    fprintf(stderr,"Cannot create socket\n");
    exit(1);
  }
  //Clear the structure
  memset((char*)&server_addr,0,sizeof(server_addr));

  server_addr.sin_family=AF_INET;
  server_addr.sin_port=htons(PORT_NUMBER);
  server_addr.sin_addr.s_addr=inet_addr(host_name);
  
  if(server_addr.sin_addr.s_addr==-1){
      servhost=gethostbyname(host_name);
      if(servhost==NULL){
        fprintf(stderr,"Not Found Server:%s\n",host_name);
      }
      server_addr.sin_addr.s_addr=*(unsigned int *)servhost->h_addr_list[0];
  }
  //Connect to the server
  if(connect(csocket,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
    fprintf(stderr,"Cannot connect\n");
    exit(1);
  }
  printf("--Connected.\n");
  bufferlength=strlen(str_buf);
  //Sending data
  if(send(csocket,str_buf,bufferlength,0)<0){
    fprintf(stderr,"Cannot send message\n");
    close(csocket);
    exit(1);
  }
  printf("--Send message\n%s\n",str_buf);

  while(1){
    loopnum++;
    printf("\n\n--Top of while()loop(%d)\n\n",loopnum);
    memset(str_buf,'\0',BUFFER_SIZE);
    if((recvbuffer=recv(csocket,str_buf,BUFFER_SIZE,0))<0){
      fprintf(stderr,"Cannot receive message\n");
      close(csocket);
      exit(1);
    }else if(recvbuffer==0){
      printf("\n\n--close received\n\n");
      printf("%s\n",str_buf);
      fflush(stdout);
      break;
    }else{
      printf("%s",str_buf);
      fflush(stdout);
    }
    close(csocket);
    return(0);
  }
}

