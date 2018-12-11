#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <strings.h>

#define SERVER_PORT 21
#define SERVER_ADDR "192.168.28.96"

int main(int argc, char** argv) {

  /*
  * TCP
  */

	int	sockfd;
	struct	sockaddr_in server_addr;
	char	buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";
	int	bytes;

	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(SERVER_PORT);		/*server TCP port must be network byte ordered */

	/*open a TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    		perror("socket()");
        	exit(0);
    	}
	/*connect to the server*/
  if(connect(sockfd,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)) < 0) {
    perror("connect()");
    exit(0);
	}

  /*send a string to the server*/
	bytes = write(sockfd, buf, strlen(buf));
	printf("Bytes written: %d\n", bytes);

	close(sockfd);

  /*
  * IP
  */

  struct hostent *h;

  if ((h=gethostbyname(argv[1])) == NULL) {
    herror("gethostbyname");
    exit(1);
  }

  printf("Host name  : %s\n", h->h_name);
  printf("IP Address : %s\n",inet_ntoa(*((struct in_addr *)h->h_addr)));

  return 0;
}

void parseInfo(char* info, char* user, char* password, char* host, char* path){
	unsigned int i = 6;
	unsigned int j = 0;
	unsigned int length = strlen(info);
	unsigned int state = 0;
	unsigned char buf;

	while(i<length){
		buf = *(buf+i);

		switch(state){
			case 0:
				if(buf == ':'){
					*(user + j) = 0; //end char
					state++;
					j = 0;
				}else{
					*(user + j) = buf;
					j++;
				}
				break;
			case 1:
				if(buf == '@'){
					*(password + j) = 0; //end char
					state++;
					j = 0;
				}else{
					*(password + j) = buf;
					j++;
				}
				break;
			case 2:
				if(buf == '/'){
					*(host + j) = 0; //end char
					state++;
					j = 0;
				}else{
					*(host + j) = buf;
					j++;
				}
				break;
			case 3:
				*(path + j) = buf;
				j++;
				break;
		}

		*(path + j) = 0; //end char

		i++;
	}
}
