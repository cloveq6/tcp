#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.120.129"
#define PORT 5150
#define BUF_SIZE 1088

int main() {
    printf("Client is running...\n");

    int clientSocket;
    struct sockaddr_in serverAddr;
    char sendBuffer[BUF_SIZE];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation error");
        return 1;
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connect error");
        close(clientSocket);
        return 1;
    }

    // Send data to the server
    strcpy(sendBuffer, "Hello, Server!");
    for(int i=0; i<100; ++i){
        send(clientSocket, sendBuffer, strlen(sendBuffer), 0);
        sleep(1);
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
