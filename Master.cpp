#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MASTERRID 0
#define BACKLOG 10	 // how many pending connections queue will hold
#define MAXDATASIZE 100


/*
 * Master.cpp
 * created by Jesse Roach on Oct. 16th 2018
 * receives join request from slave of form
 *   1B     4B (Joy!)
 * | GID | 0x4A6F7921 |
 * sends response to slave of form
 *   1B        4B         1B         4B
 * | GID | 0x4A6F7921 | yourRID | nextSlaveIP |
 * GID is Group ID
 * 0x4A6F7921 is the magic number, an ascii phrase
 * your RID is the slave's assigned Ring ID
 * nextSlaveIP is the ipaddr of the next slave behind the one currently requesting
 * Master will not accept requests that are not both 5 bytes and contain the magic number
 *
 */
void displayBuffer(char *Buffer, int length);
void initialize();
void addSlave(int slaveIP, int slaveSocketFD);

int nextSlaveIP;
unsigned char nextRID;

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
//
//typedef struct RingNode
//{
//
//}RingNode;

int main(int argc, char *argv[])
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;

    char s[INET6_ADDRSTRLEN];
    int rv;

    int numbytes;
    char buf[MAXDATASIZE];

    int nextRID = 1;//this gets passed to slaves and incremented
    int nextSlaveIP;//this just points to the next ip address in line

    //check for command line args with port number
    if (argc != 2)
    {
        fprintf(stderr,"usage: Master MasterPort# \n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) {
            close(sockfd);

            if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
                perror("recv");
                exit(1);
            }

            buf[numbytes] = '\0';

            printf("Server: received '%s'\n",buf);

            displayBuffer(buf,numbytes);

            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }
}


void displayBuffer(char *Buffer, int length){
    int currentByte, column;

    currentByte = 0;
    printf("\n>>>>>>>>>>>> Content in hexadecimal <<<<<<<<<<<\n");
    while (currentByte < length){
        printf("%3d: ", currentByte);
        column =0;
        while ((currentByte < length) && (column < 10)){
            printf("%2x ",Buffer[currentByte]);
            column++;
            currentByte++;
        }
        printf("\n");
    }
    printf("\n\n");
}

void initialize() {
    nextSlaveIP = 0; //change this to localhost
    nextRID = MASTERRID;
}

void addSlave(int slaveIP, int slaveSocketFD) {
    //if (nextSlaveIP != localhost) {
    //this needs to be changed once we figure out how to get localhost ip
        char toSend[10];
        toSend[0] = 13;
        toSend[1] = 0x4A;
        toSend[2] = 0x6F;
        toSend[3] = 0x79;
        toSend[4] = 0x21;
        toSend[5] = (char) nextRID;

        //i don't know for sure that this will work, it might need to be changed if nextSlaveIP changes
        for (unsigned char i = 0; i < 4; i++) {
            toSend[6 + i] = (char) ((nextSlaveIP >> (8 * (3 - i))) & 0xff);
        }
    //}

    write(slaveSocketFD, toSend, sizeof(toSend));

    theirRID++;
    nextSlaveIP = slaveIP;
}
