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

//Read one line data coming in from socket
void read_line(int sockd, char *line);

int main(int argc, char *argv[]) {
	//parse arguments: referenceIP:refPort
	char ip[50];
	int port = 0;
	char buffer[MAX_LINE_SIZE];

	if (argc != 2) {
		printf("Usage: ./client referenceIP:refPort\n");
		return 0;
	}

	int len = strlen(argv[1]);
	for(int i=0;i<len;i++){
		if(argv[1][i] == ':'){
			argv[1][i] = ' ';
		}
	}
	if (sscanf(argv[1], "%s%d", ip, &port) != 2) {
		printf("Usage: ./client referenceIP:refPort\n");
		return 0;
	}

	//create socket address
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	memset(&(addr.sin_zero), 0, 8);

	//create client socket
	int sockd = socket(AF_INET, SOCK_STREAM, 0);

	socklen_t addr_len = sizeof(struct sockaddr_in);
	if (connect(sockd, (struct sockaddr *) &addr, addr_len) < 0) {
		printf("failed to connect server\n");
		return 0;
	}

int acceptServer = 0;
	while (1) {
		read_line(sockd, buffer);
		printf("%s\n", buffer);

		if (strcmp("TEXT TCP 1.0", buffer) == 0) {
			acceptServer = 1;
		}

		if (strlen(buffer) == 0) {
			//got a empty line, protocol data is end
			break;
		}
	}

	if (acceptServer) {
		sprintf(buffer, "OK\n");
		send(sockd, buffer, strlen(buffer), 0);
	} else {
		sprintf(buffer, "NO\n");
		send(sockd, buffer, strlen(buffer), 0);
		close(sockd);
		return 0;
	}
