/*
    Name - Mohammad Saif
    ID - 2018B3A70750P
    Note - The code runs perfectly on windows. Have tried to add code so that it runs on Linux also as some libraries and all are different
    Compile using 'gcc filename.c -lws2_32' for windows
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

// Code to handle different OS
#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
#else // For linux system
    #include <sys/socket.h>
    #include <arpa/inet.h>
#endif

// Some macros
#define MAX_ENTRIES 250
#define MAX_STRING 55
#define MAX_PENDING 10
#define NET_BUFF 250

// Keywords for commands
char byeS[]="Bye";
char putS[]="put";
char delS[]="del";
char getS[]="get";



void swap(char* a,char* b) {
  char tmp=*a;
  *a=*b;
  *b=tmp;
}


void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap((str+start), (str+end));
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}









// Structure to store key-value pairs in memory
struct pair {
    int key;
    char* value;
};

// Structure to store parsed client request
struct request {
    int command;
    int key;
    char* str;
};

FILE* fp;
struct pair** data;  //Pointer to point to data stored in memory
int data_end=0;      //Pointer to store the end pointer of data array in memory

void rect() {
    fp=fopen("database.txt","a");
    fputc('\n',fp);
    fputs("- END",fp);
    fclose(fp);
}

//Function to read the latest database into memory from 'database.txt'
void read_data() {
    data_end=0;
    fp=fopen("database.txt","r");
    data=(struct pair**)malloc(MAX_ENTRIES * sizeof(struct pair*));
    char* buffer=(char*)malloc(MAX_STRING);
    int buff_size=MAX_STRING;
    char tmp;
    int buff_p=0;
    while((tmp=fgetc(fp))!=EOF) {
        buffer[buff_p++]=tmp;
        if(tmp=='\n') {
            buff_p=0;
            int c=0;
            int k=0;
            while(buffer[c]!=' ') {
                k=k*10+(buffer[c]-'0');
                c++;
            }
            c++;
            char* str=(char*)malloc(MAX_STRING);
            int c2=0;
            while(buffer[c]!='\n') {
                str[c2]=buffer[c];
                c++;c2++;
            }
            str[c2]='\0';
            data[data_end]=(struct pair*)malloc(sizeof(struct pair));
            data[data_end]->key=k;
            data[data_end]->value=str;
            data_end++;
        }
    }
    fclose(fp);
}

//Function to update the database by any user
void save_data() {
    int p=0;
    fp=fopen("database.txt","w");
    while(p!=data_end) {
        if(data[p]==NULL) {
            p++;
            continue;
        }
        char *tp=(char*)malloc(5);
        itoa(data[p]->key,tp,10);
        int c=0;
        while(tp[c]!='\0') fputc(tp[c++],fp);
        c=0;
        fputc(' ',fp);
        while((data[p]->value)[c]!='\0') fputc((data[p]->value)[c++],fp);
        fputc('\n',fp);
        p++;
    }
    fputs("- END",fp);
    fclose(fp);
}

//Printing the database
void print_data() {
    int p=0;
    printf("----------------------DATABASE----------------------\n");
    while(p!=data_end) {
        if(data[p]==NULL) {
            p++;
            continue;
        }
        printf("Key: %d\t\tValue: %s\n",data[p]->key,data[p]->value);
        p++;
    }
    printf("----------------------DATABASE----------------------\n");
}

//Function to find if the key exists in the database
int look_up(int l) {
    int p=0;
    while(p!=data_end) {
        if(data[p++]->key==l) {
            return p-1;
            break;
        }
    }
    return -1;
}

//Function to insert key-value in memory as well as database
int insert(int l,char* st) {
    int ind;

    if((ind=look_up(l))!=-1) {
        return -1;
    }
    data[data_end]=(struct pair*)malloc(sizeof(struct pair));
    data[data_end]->key=l;
    data[data_end]->value=st;
    data_end++;
    char c;
    ind=0;
    return 1;
}

//Function to delete a key
int delete(int l) {
    int ind;
    // If key does not exist
    if((ind=look_up(l))==-1) {
        return -1;
    }
    struct pair* tmp=data[ind];
    data[ind]=NULL;
    free(tmp);
    printf("Delete operation successful!\n");
    return 1;
}

//Function to parse the inputted request by the client
struct request detect(char* st) {
    char* ex=(char*)malloc(250);
    int c=0,c1=0;
    while(st[c]!=' ' && st[c]!='\0') {
        ex[c1]=st[c];
        c++;c1++;
    }
    ex[c1]='\0';
    c++;
    struct request ans;
    if(strcmp(ex,putS)==0) {
        ans.command=1;
        c1=0;
        while(st[c]!=' ') {
            ex[c1]=st[c];
            c++;c1++;
        }
        ex[c1]='\0';
        ans.key=atoi(ex);
        c1=0;
        c++;
        while(st[c]!='\0') {
            ex[c1]=st[c];
            c++;c1++;
        }
        ex[c1]='\0';
        c1=0;
        ans.str=(char*)malloc(250);
        while(ex[c1]!='\0') {
            ans.str[c1]=ex[c1];c1++;
        }
        ans.str[c1]='\0';
        return ans;
    }
    else if(strcmp(ex,getS)==0) {
        ans.command=2;
        c1=0;
        while(st[c]!='\0') {
            ex[c1]=st[c];
            c++;c1++;
        }
        ex[c1]='\0';
        ans.key=atoi(ex);
        return ans;
    }
    else if(strcmp(ex,delS)==0) {
        ans.command=3;
        c1=0;
        while(st[c]!='\0') {
            ex[c1]=st[c];
            c++;c1++;
        }
        ex[c1]='\0';
        ans.key=atoi(ex);
        return ans;
    }
    ans.command=4;
    return ans;
}

int main() {
    read_data(); // Reading data already present in the database

    /*
    print_data();
    char tmp[]="Hello";
    insert(3,tmp);
    insert(10,tmp);
    insert(15,tmp);
    delete(27);
    delete(10);
    save_data();
    print_data();*/

    //Handling different OS
    #if defined(_WIN32) || defined(_WIN64)
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
            printf("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
            exit(1);
        }
    #endif

    pid_t childpid;

    /*CREATING A TCP SOCKET*/
    int serverSocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0) { printf ("Error while server socket creation"); exit (0); }
    printf ("Server Socket Created\n");

    /*CONSTRUCTING LOCAL ADDRESS STRUCTURE*/
    struct sockaddr_in serverAddress, clientAddress;
    memset (&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(32345);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    printf ("Server address assigned\n");
    int temp = bind(serverSocket, (struct sockaddr*) &serverAddress,  sizeof(serverAddress));
    if (temp < 0)
    { printf ("Error while binding\n");
        exit (0);
    }
    printf ("Binding successful\n");
    int temp1 = listen(serverSocket, MAX_PENDING);
    if (temp1 < 0)
    {  printf ("Error in listen");
        exit (0);
    }
    printf ("Now Listening\n");
    char msg[NET_BUFF]="def";
    //Loop for multiple clients
    for ( ; ; ) {
        int clientLength = sizeof(clientAddress);
        int clientSocket = accept (serverSocket, (struct sockaddr*) &clientAddress, &clientLength);
        if (clientLength < 0) {printf ("Error in client socket"); exit(0);}
        printf("%s\n","Received request...\n");
        if ( (childpid = fork ()) == 0 ) {
          printf ("%s\n","Child created for dealing with client requests\n");
          //close listening socket
          close (serverSocket);
          printf ("Handling Client %s\n", inet_ntoa(clientAddress.sin_addr));
          int flag=0;
          //Loop for multiple sequential commands
          while(strcmp(msg,byeS)!=0) {
              printf("Awaiting Request!\n");
              int temp2 = recv(clientSocket, msg, NET_BUFF, 0);
              msg[temp2] = '\0';
              printf ("%s\n", msg);
              struct request req=detect(msg); // Code to parse input request into different components like command key and string respectively
              int bytesSent,res;
              switch(req.command) {
                  case 1: //client entered put command
                      read_data(); //In any case read latest data from text file
                      res=insert(req.key,req.str);
                      if(res==-1) bytesSent = send (clientSocket,"Key already exists",strlen("Key already exists"),0);
                      else bytesSent = send (clientSocket,"OK",strlen("OK"),0);
                      save_data(); //Save changes to datafile
                      // rect();
                      break;
                  case 2: //client entered get command
                      read_data(); //In any case read latest data from text file
                      res=look_up(req.key);
                      if(res==-1) bytesSent = send (clientSocket,"Key not found",strlen("Key not found"),0);
                      else bytesSent = send (clientSocket,data[res]->value,strlen(data[res]->value),0);
                      break;
                  case 3: //client entered del command
                      read_data(); //In any case read latest data from text file
                      res=delete(req.key);
                      if(res==-1) bytesSent = send (clientSocket,"Key not found",strlen("Key not found"),0);
                      else bytesSent = send (clientSocket,"OK",strlen("OK"),0);
                      save_data(); //Save changes to datafile
                      break;
                  case 4: //client entered bye command
                      bytesSent = send (clientSocket,"Goodbye",strlen("Goodbye"),0);
                      flag=1;
                      break;
                  default:
                      printf("Invalid request! Try again!\n");
                      break;
              }
              //Client associated exits...
              if(flag==1) {
                  printf("Client quitted!Exiting this fork......\n");
                  break;
              }
          }
          break; // To end process
          // Close the socket according to the operating system
          #if defined(_WIN32) || defined(_WIN64)
              closesocket(clientSocket);
              WSACleanup();
          #else
              close(clientSocket);
          #endif
        }

    }
    return 0;
}
