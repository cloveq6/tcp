#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.120.129"
#define PORT 5150
#define BUF_SIZE 1088

void connectToServer(int socket);

FILE* openFile(const char* filepath);

void closeResources(int socket, FILE* file);

void sendFileData(int socket, FILE* file);

int main() {
    printf("Client is running...\n");

    int clientSocket;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation error");
        return 1;
    }

    // Connect to the server and send file
    connectToServer(clientSocket);

    // Close the socket
    close(clientSocket);

    return 0;
}

void connectToServer(int socket) {
    struct sockaddr_in serverAddr;

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connect error");
        close(socket);
        exit(EXIT_FAILURE);
    }

    // Specify the file path to read and send
    const char* filepath = "kjv.txt";

    FILE* file = openFile(filepath);

    // Send file data
    sendFileData(socket, file);

    // Close resources
    closeResources(socket, file);
}

FILE* openFile(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }

    return file;
}

void closeResources(int socket, FILE* file) {
    fclose(file);
}

void sendFileData(int socket, FILE* file) {
    char sendBuffer[BUF_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(sendBuffer, 1, sizeof(sendBuffer), file)) > 0) {
        if (send(socket, sendBuffer, bytesRead, 0) == -1) {
            perror("Send error");
            break;
        }
        usleep(100);
    }
}
