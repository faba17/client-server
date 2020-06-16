#include <stdio.h>
#include <stdlib.h>
/* You will to add includes here */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

// Included to get the support library
#include <calcLib.h>
#define MAX_LINE_SIZE 256

using namespace std;
void read_line(int sockd, char *line);

int main(int argc, char *argv[]){
  
  /* Do more magic */
  char ip[50];
	int port = 0;
	char buffer[MAX_LINE_SIZE];

	if (argc != 2) {
		printf("Usage1: ./server ipto_listenon:port\n");
		return 0;
	}

	int len = strlen(argv[1]);
	for (int i = 0; i < len; i++) {
		if (argv[1][i] == ':') {
			argv[1][i] = ' ';
		}
	}
	printf("%s\n", argv[1]);
	if (sscanf(argv[1], "%s%d", ip, &port) != 2) {
		printf("Usage2: ./server ipto_listenon:port\n");
		return 0;
	}
  
  struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	memset(&(addr.sin_zero), 0, 8);
	
  int serverSockd = socket(AF_INET, SOCK_STREAM, 0);
  socklen_t addr_len = sizeof(struct sockaddr_in);
  if (bind(serverSockd, (struct sockaddr *) &addr, addr_len) == -1) {
      printf("bind error\n");
      return 0;
   }
	
   if (listen(serverSockd, 10) == -1) {
		printf("listen error\n");
		return 0;
   }
