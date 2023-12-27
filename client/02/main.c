#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.120.129"
#define PORT 5150
#define BUF_SIZE 1088

void readAndSendFile(const char* filename, int socket);

int main() {
    printf("Client is running...\n");

    int clientSocket;
    struct sockaddr_in serverAddr;

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

    // Specify the file path to read and send
    const char* filepath = "../kjv.txt";

    // Read file and send to the server
    readAndSendFile(filepath, clientSocket);

    // Close the socket
    close(clientSocket);

    return 0;
}

void readAndSendFile(const char* filename, int socket) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File open error");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char sendBuffer[BUF_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(sendBuffer, 1, sizeof(sendBuffer), file)) > 0) {
        if (send(socket, sendBuffer, bytesRead, 0) == -1) {
            perror("Send error");
            break;
        }
        usleep(100);
    }

    fclose(file);
}
