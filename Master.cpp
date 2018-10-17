#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MASTERRID 0;

typedef struct RingNode
{
    int myRID;
    int nextRID;
    int nextSlaveIP;
}RingNode;

int main(int argc, char *argv[])
{
    //check for command line args with port number
    if (argc != 2)
    {
        fprintf(stderr,"usage: Master MasterPort# \n");
        exit(1);
    }
}