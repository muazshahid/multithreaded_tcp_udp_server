#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//Initialize Handler Function for TCP and UDP Client
void *tcp_client_handler(void *);
void *udp_client_handler(void *);

//Initialize required variables for TCP and UDP Client
struct sockaddr_in udpServer, udpClient;

int udpSock, myUdpSock, udpLen;
int tcpSock, myTcpSock, tcpLen;
struct sockaddr_in tcpServer, tcpClient;

int main(int argc, char *argv[])
{
  //Buffer for TCP Data
  char tcp_buff[1024];
  //Initialize Pthread for TCP and UDP
  pthread_t tcp_thread_id;
  pthread_t udp_thread_id;
  // Create TCP Socket
  tcpSock = socket(AF_INET, SOCK_STREAM, 0);
  //Create UDP Socket
  udpSock = socket(AF_INET, SOCK_DGRAM, 0);
  bzero(&udpServer, sizeof(udpServer));

  //Check for both sockets if they have been created successfully
  if (tcpSock < 0)
  {
    perror("Failed to create Socket");
    exit(1);
  }
  if (udpSock < 0)
  {
    perror("Failed to create Socket");
    exit(1);
  }
  // assign IP, PORT for TCP Server
  tcpServer.sin_family = AF_INET;
  tcpServer.sin_addr.s_addr = INADDR_ANY;
  tcpServer.sin_port = htons(1234);
  //assign IP, PORT for UDP Server
  udpServer.sin_family = AF_INET;
  udpServer.sin_addr.s_addr = INADDR_ANY;
  udpServer.sin_port = htons(1235);

  // Binding newly created socket to given IP and its verification for TCP Server
  if (bind(tcpSock, (struct sockaddr *)&tcpServer, sizeof(tcpServer)) < 0)
  {
    perror("Bind Failed");
  }
  // Binding newly created socket to given IP and its verification for UDP Server
  if (bind(udpSock, (struct sockaddr *)&udpServer, sizeof(udpServer)) < 0)
  {
    perror("Bind Failed");
  }

  //Start listening on both sockets
  listen(udpSock, 0);
  listen(tcpSock, 0);

  puts("Listening to multiple clients\n");

  //Inifinite while loop running to handle multiple clients connecting to both servers
  while (1)
  {
    socklen_t tcpLen = sizeof(tcpClient);

    //Accept the data packet from client
    myTcpSock = accept(tcpSock, (struct sockaddr *)&tcpClient, &tcpLen);

    //Checking if the TCP Connection was established
    if (myTcpSock == -1)
    {
      perror("TCP Connection failed");
    }
    else
    { //Start a new thread for TCP but do not wait for it
      pthread_create(&tcp_thread_id, NULL, tcp_client_handler, (void *)&myTcpSock);
      pthread_detach(tcp_thread_id);
    }
    //Checking the UDP Socket
    if (myUdpSock == -1)
    {
      perror("UDP Connection failed");
    }
    else
    { //Start a new thread for UDP but do not wait for it
      pthread_create(&udp_thread_id, NULL, udp_client_handler, (void *)&myUdpSock);
      pthread_detach(udp_thread_id);
    }

  }
  return 0;
}

//The function below handles the TCP clients connecting to the server using multithreading,
//clients connects and sends a message to the server which is handled by this function. Server recieves the
//message and sends back the same message to the client
void *tcp_client_handler(void *myTcpSock)
{
  //Get the socket descriptor
  int read_size;
  int sock_desk = *(int*)myTcpSock;
  char *message , client_message[2000];

  //Receive a message from tcpClient
  while ( (read_size = recv(sock_desk , client_message , 2000 , 0)) > 0 )
  {
    client_message[read_size] = '\0';
    //Data on server end is displayed using labels of UDP Data and TCP Data to differentiate between the data recived on each type of server
    puts("\nTCP Data:\n");
    puts(client_message);
    //Send the message back to tcpClient
    write(sock_desk , client_message , strlen(client_message));
    //clear the message buffer
    memset(client_message, 0, 2000);
  }

  if (read_size == 0)
  {
    puts("Client disconnected");
    fflush(stdout);
  }
  else if (read_size == -1)
  {
    perror("Recieving failed");
  }
  return 0;
}

//The function below handles UDP clients connecting to the server, it does the same work as the TCP function above.
void *udp_client_handler(void *myUdpSock)
{
  int n;
  char udp_buff[1024];
  for (;;)
  {
    socklen_t udpLen = sizeof(udpClient);
    n = recvfrom(udpSock, udp_buff, 1024, 0, (struct sockaddr *)&udpClient, &udpLen);
    //Send the message back to UDP Client
    sendto(udpSock, udp_buff, n, 0, (struct sockaddr *)&udpClient, sizeof(udpClient));
    udp_buff[udpLen] = 0;
    //Data on server end is displayed using labels of UDP Data and TCP Data to differentiate between the data recived on each type of server
    printf("\nUDP Data:\n");
    printf("%s", udp_buff);
  }
  return 0;
}