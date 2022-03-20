/*
    Name - Mohammad Saif
    ID - 2018B3A70750P
    Note - The code runs perfectly on windows. Have tried to add code so that it runs on Linux also as some libraries and all are different
    Compile using 'gcc filename.c -lws2_32' for windows
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   
#include <unistd.h>

//Code to handle different OS
#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
#endif


#define BUFSIZE 250
int main()
{
    #if defined(_WIN32) || defined(_WIN64)    
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
            printf("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
            exit(1);
        }
    #endif

    /* CREATE A TCP SOCKET*/
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) { printf ("Error in opening a socket"); exit (0);}
    printf ("Client Socket Created\n");

    /*CONSTRUCT SERVER ADDRESS STRUCTURE*/
    struct sockaddr_in serverAddr;
    memset (&serverAddr,0,sizeof(serverAddr)); 

    /*memset() is used to fill a block of memory with a particular value*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(32345); //You can change port number here
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Specify server's IP address here
    printf ("Address assigned\n");
    
    /*ESTABLISH CONNECTION*/
    int c = connect (sock, (struct sockaddr*) &serverAddr , sizeof (serverAddr));
    printf ("%d\n",c);
    if (c < 0) 
    { printf ("Error while establishing connection"); 
        exit (0);
    }
    printf ("Connection Established\n");
    
    /*SEND DATA*/
    char msg[BUFSIZE];
    char byeS[]="Goodbye";
    char recvBuffer[BUFSIZE];
    //Loop to support multiple sequential commands
    while(strcmp(recvBuffer,byeS)!=0) {
        printf("ENTER REQUEST:\n");
        fgets(msg,BUFSIZE,stdin);
        int bytesSent = send (sock, msg, strlen(msg), 0);
        if (bytesSent != strlen(msg)) 
        { printf("Error while sending the message"); 
            exit(0);
        }
        // printf ("Data Sent\n");
        /*RECEIVE BYTES*/
        int bytesRecvd = recv (sock, recvBuffer, BUFSIZE-1, 0);
        if (bytesRecvd < 0) 
        { printf ("Error while receiving data from server"); 
            exit (0);
        }
        recvBuffer[bytesRecvd] = '\0';
        printf ("%s\n",recvBuffer);
    }
    // Close the socket
    #if defined(_WIN32) || defined(_WIN64)  
        closesocket(sock);
        WSACleanup();
    #else
        close(sock);
    #endif

    return 0;
}