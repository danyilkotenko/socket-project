// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080
  
struct msg_header{
	unsigned int msgId;
};


struct code{
  int codeId;
};

struct msg_auth{
  msg_header header;
  char username[20];
  char password[20];
};

int main(int argc, char const *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    int bufsize = 1024;
    char nameUser[20];
    bool logged = false;
    char buffer[bufsize];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    printf("\n=>Socket client has been created..\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("\nConnection to the server port\n");
    do{
    int choice;
    printf("Registration:1\nLogin:2\nChat:3\n");
    scanf("%d", &choice);
    if(choice!= 1 && choice!= 2 && choice!=3 ){
    printf("Please select the option\n");
    continue;
    }
    msg_auth user;
    printf("\nYour username:");
    scanf("%s",user.username);
    printf("\nYour password:");
    scanf("%s",user.password);
    user.header.msgId = choice;
    send(sock, &user ,sizeof(msg_auth),0);
    recv(sock , buffer, 1024,0);
    code* codeResponse = (code*) buffer;
    if(codeResponse->codeId==200){
    printf("\nSuccess \n");
    logged=true;
    strcpy(nameUser, user.username);
    }
    else if(codeResponse->codeId==201) printf("\nusername exist \n");
    else printf("\nError\n");
    }while(!logged);
    printf("Hello, %s\n", nameUser);
    ///////////////////////////////////////
    if(logged){
        printf("%s: ", nameUser);
        scanf("%s", &buffer[0]);
        send(sock, buffer, strlen(buffer), 0);

        if(strcmp(buffer, ":exit") == 0)
        {
            close(sock);
            printf("Disconnected. \n");
            exit(1);
        }
    if(recv(sock, buffer, 1024, 0) < 0)
    {
        printf("Error in receiving data. \n");
    }else{
        printf("Server: %s\n", buffer);
    }
}
    return 0;
}
