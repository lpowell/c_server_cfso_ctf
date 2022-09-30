#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX 80
#define PORT 4096
#define SA struct sockaddr

// void func(int connfd){
// 	char buff[MAX];
// 	int n;

// 	for (;;){
// 		bzero(buff, MAX);
// 		read(connfd, buff, sizeof(buff));
// 		printf("From client: %s\t To client: ", buff);
// 		bzero(buff, MAX);
// 		n = 0;
// 		while ((buff[n++] = getchar()) != '\n');
// 		write(connfd, buff, sizeof(buff));
// 		if(strncmp("exit", buff, 4) == 0){
// 			printf("Server exit...\n");
// 			break;
// 		}
// 	}
// }

void *challenges(void *p){
	printf("Thread working");
	int sock = *(int*)p;
	for(;;){
		printf("First for");
		// Challenges
		char C_One[] = "ChallengeOne\n\0";
		char C_Two[] = "ChallengeTwo\n\0";
		char C_Three[] = "ChallengeThree\n\0";
		char C_Wrong[] = "Wrong Answer!\n\0";
		char flag[] = "flag{flag}\n\0";
		//String to send
		char sendstr[MAX];
		//String received
		char recstr[MAX];
		char guide[] = "Type exit to close connection\n\0";
		int n;
		// write(connfd,guide,sizeof(guide));
		for(;;){
			printf("Second for");
			n = write(sock, C_One, sizeof(C_One));
			if(n < 0){
				printf("Could not write to client");
				exit(0);
			}

			while(read(sock, recstr, sizeof(recstr)) != 0){
				printf("1");
				if(strncmp("AnswerOne", recstr, 9) == 0){
					printf("First answer");
					for(;;){
						printf("0");
						write(sock, C_Two, sizeof(C_Two));
						read(sock, recstr, sizeof(recstr));
						if(strncmp("AnswerTwo", recstr, 9) == 0){
							for(;;){
								write(sock, C_Three, sizeof(C_Three));
								read(sock, recstr, sizeof(recstr));
								if(strncmp("AnswerThree", recstr, 11) == 0){
									write(sock, flag, sizeof(flag));
									shutdown(sock, SHUT_RDWR);
									close(sock);
									pthread_exit(NULL);
								}else{
									write(sock, C_Wrong, sizeof(C_Wrong));
								}
							}
						}else{
							write(sock, C_Wrong, sizeof(C_Wrong));
						}
					}
				}else{
					write(sock, C_Wrong, sizeof(C_Wrong));
				}
			}
			shutdown(sock, SHUT_RDWR);
			close(sock);
			pthread_exit(NULL);

		}
	}
	shutdown(sock, SHUT_RDWR);
	close(sock);
	pthread_exit(NULL);
}



int main(){
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd ==-1){
		printf("Failed to create socket\n");
		exit(0);
	}else{
		printf("Socket created succesfully\n");
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) !=0){
		printf("Socket failed to bind\n");
		exit(0);
	}else{
		printf("Socket binded\n");
	}
	if ((listen(sockfd, 20)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
   	for(;;){
   	    len = sizeof(cli);	
   		// Accept the data packet from client and verification
   		connfd = accept(sockfd, (SA*)&cli, &len);

   		if (connfd < 0) {
   		    printf("server accept failed...\n");
   		    exit(0);
   		}
   		else
   		    printf("server accept the client...\n");

   		// Pass connection to thread
   		pthread_t thread;
   		// int *conn = malloc(sizeof(int));
   		// *conn = connfd;
   		pthread_create(&thread, NULL, challenges, (void*) &connfd);
   		pthread_detach(thread);
   	}

   
    // After chatting close the connection
    
}
