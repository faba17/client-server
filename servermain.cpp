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
void createEquation(char* buffer, char* result);

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
	
initCalcLib();

	while (1) {
		
		printf("waiting...\n");
		struct sockaddr_in client_addr;
		int sockd = accept(serverSockd, (struct sockaddr *) &client_addr,
				&addr_len);

		if (sockd < 0) {
			printf("Error: accept failed.\n");
			continue;
		}

		sprintf(buffer, "TEXT TCP 1.0\n\n");
		send(sockd, buffer, strlen(buffer), 0);

		read_line(sockd, buffer);
		if (strcmp("OK", buffer) == 0) {
			char result[100];
			char userResult[100];
			createEquation(buffer, result);

			printf("%sanswer:%s\n", buffer, result);
			
                send(sockd, buffer, strlen(buffer), 0);

			read_line(sockd, userResult);
			printf("user: %s\n", userResult);

			if (strcmp(userResult, result) == 0) {
				sprintf(buffer, "OK\n");
				send(sockd, buffer, strlen(buffer), 0);
			} else {
				sprintf(buffer, "ERROR\n");
				send(sockd, buffer, strlen(buffer), 0);
			}

			close(sockd);
		} else {
			close(sockd);
		}
	}
}

void read_line(int sockd, char *line) {
	char c;
	int count = 0;
	int maxLen = MAX_LINE_SIZE;

	memset(line, 0, maxLen);
	while (read(sockd, &c, 1) == 1) {
		if (c == '\n') {
			break;
		}

		if (count < maxLen) {
			line[count++] = c;
		}
	}
}

void createEquation(char* buffer, char* result) {
	char *ptr = randomType(); // Get a random arithemtic operator.

	double f1, f2, fresult;
	int i1, i2, iresult;

	
	if (ptr[0] == 'f') {
		//printf("Float\t");
		f1 = randomFloat();
		f2 = randomFloat();

		
		sprintf(buffer, "%8.8g %8.8g", f1, f2);
		sscanf(buffer, "%lf%lf", &f1, &f2);

		if (strcmp(ptr, "fadd") == 0) {
			fresult = f1 + f2;
		} else if (strcmp(ptr, "fsub") == 0) {
			fresult = f1 - f2;
		} else if (strcmp(ptr, "fmul") == 0) {
			fresult = f1 * f2;
		} else if (strcmp(ptr, "fdiv") == 0) {
			fresult = f1 / f2;
		}
		sprintf(buffer, "%s %8.8g %8.8g\n", ptr, f1, f2);
		sprintf(result, "%8.8g", fresult);
	}

}
