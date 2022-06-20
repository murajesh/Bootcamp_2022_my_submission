/* run using ./server <port> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <queue>
#include "http_server.hh"

#define MAX_THREADS 5
#define MAX_QUEUE_SIZE 20

pthread_mutex_t mutex_l = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill_c = PTHREAD_COND_INITIALIZER;
std::queue<int> c_sockfds_qu;
int count = 0;

void process_client(int); //function prototype

void error(char *msg) {
  perror(msg);
  exit(1);
}

/* This is the worker thread function. This function will 
 * process the client connections.
 *  */

void *thread_fn(void *arg) {
  int cli_sockfd;
  pthread_t tid;
  tid = pthread_self();
  
//  pthread_mutex_lock(&mutex);
//  while (count == 0)
//	 pthread_cond_wait(&fill, &mutex);
//  cli_sockfd = c_sockfds.pop(); //Get the client connection socket fd
//  pthread_cond_signal(&empty);
//  pthread_mutex_unlock(&mutex);
//  
//  /* process the client request */
//  handle_request(cli_sockfd);
  while (1) {
	pthread_mutex_lock(&mutex_l);
	while (count == 0)
	      pthread_cond_wait(&fill_c, &mutex_l);
	cli_sockfd = c_sockfds_qu.front();
  c_sockfds_qu.pop();
	count--;
	pthread_cond_signal(&empty_c);
	pthread_mutex_unlock(&mutex_l);
  printf("The thread id is %u, client socket fd is %d \n", tid, cli_sockfd);
	process_client(cli_sockfd);
  }

}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
  
  pthread_t pth_t[MAX_THREADS];


  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }

  /* create a thread pool of 5 threads */

  for (int i = 0; i < MAX_THREADS; i++) {
	  pthread_create(&pth_t[i], NULL, thread_fn, NULL);
  }

  /* create socket */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* fill in port number to listen on. IP address can be anything (INADDR_ANY)
   */

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* bind socket to this port number on this machine */

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  /* listen for incoming connection requests */

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  /* Accept connections forever */

  while (1) {
    /* accept a new request, create a newsockfd */

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
      error("ERROR on accept");
    
    /* Write the connection socket file descriptor to queue */
    
    pthread_mutex_lock(&mutex_l);
    while (count == MAX_QUEUE_SIZE)
            pthread_cond_wait(&empty_c, &mutex_l);
    c_sockfds_qu.push(newsockfd);
    count++;
    pthread_cond_signal(&fill_c);
    pthread_mutex_unlock(&mutex_l);

  }

  return 0; //It should never reach here
}

  /* process the client socket connection */

  void process_client(int client_sock_fd) {

    char buffer[256];
    int n;
    std::string response;

    /* read message from client */

    bzero(buffer, 256);
    n = read(client_sock_fd, buffer, 255);
    if (n < 0)
      error("ERROR reading from socket");
    printf("Here is the message: %s", buffer);
    
    response = handle_request(buffer)->get_string();
    /* send reply to client */
    
    n = write(client_sock_fd, (void *) response.c_str(), response.size());
    //n = write(newsockfd, "I got your message", 18);
    if (n < 0)
      error("ERROR writing to socket");

    //sleep(5);
    /* close the socket */
    close(client_sock_fd);
    //printf("Closed the client sock fd: %d", client_sock_fd);

  }

