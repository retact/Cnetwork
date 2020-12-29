/*httpclient simple program
  */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>

//Set Port Number and Buffer size
#define PORT_NUMBER 80
#define BUFFER_SIZE 8000

int main(int argc,char *argv[]){
  int s,len,n,str_len;
  struct sockaddr_in server_addr;
  struct hostent *servhost;
  char str_buf[BUFFER_SIZE];
  char *st,host_name[256],file_name[256];
  int loopnum=0;

  if(argc<2){
    printf("Make sure you use it correctry:httpclient.exe hostname/filename\n");
    exit(1);
  }

  st=strstr(argv[1],"/");
  if(st==NULL){
    printf("NOT found/.\n");
    printf("Make sure you use it correctry:httpclient.exe hostname/filename\n");
    exit(1);
  }else{
    str_len=strlen(argv[1])-strlen(st);
    strncpy(host_name,argv[1],str_len);
    host_name[str_len]='\0';
    strcpy(file_name,st);

    printf("str_len:%d\n",str_len);
    printf("--HOSTNAME:%s\n",host_name);
    printf("--FILENAME:%s\n",file_name);
    sprintf(str_buf,"GET%sHTTP/1.1\r\n",file_name);
    sprintf(str_buf,"%sHost:%s\r\nConnection:close\r\n",str_buf,host_name);
    sprintf(str_buf,"%s\r\n",str_buf);
  }

  //Create a socket
  if((s=socket(AF_INET,SOCK_STREAM,0))<0){
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
        exit(1);
      }
      server_addr.sin_addr.s_addr=*(unsigned int *)servhost->h_addr_list[0];
  }
  //Connect to the server
  if(connect(s,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
    fprintf(stderr,"Cannot connect\n");
    exit(1);
  }
  printf("--Connected.\n");
  len=strlen(str_buf);
  //Sending data
  if(send(s,str_buf,len,0)<0){
    fprintf(stderr,"Cannot send message\n");
    close(s);
    exit(1);
  }
  printf("--Send message\n%s\n",str_buf);

  while(1){
    loopnum++;
    printf("\n\n--Top of while()loop(%d)\n\n",loopnum);
    memset(str_buf,'\0',BUFFER_SIZE);
    if((n=recv(s,str_buf,BUFFER_SIZE,0))<0){
      fprintf(stderr,"Cannot receive message\n");
      close(s);
      exit(1);
    }else if(n==0){
      printf("\n\n--close received\n\n");
      printf("%s\n",str_buf);
      fflush(stdout);
      break;
    }else{
      printf("%s",str_buf);
      fflush(stdout);
    }
    close(s);
    return(0);
  }
}

