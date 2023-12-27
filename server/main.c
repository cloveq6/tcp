#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5150
#define BACKLOG 5
#define BUF_SIZE 1088
#define FILENAME "received_data.txt"

void handleConnection(int clientSocket);

void saveDataToFile(FILE *file, char *data, ssize_t dataSize);

int main() {
    printf("Server is running...\n");

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation error");
        return 1;
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind error");
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, BACKLOG) == -1) {
        perror("Listen error");
        close(serverSocket);
        return 1;
    }

    while (1) {
        // Accept a connection from a client
        socklen_t clientAddrLen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Accept error");
        }

        // Handle the connection in a separate function
        handleConnection(clientSocket);
    }

    // Close the server socket (this part won't be reached in an infinite loop)
    close(serverSocket);

    return 0;
}

void handleConnection(int clientSocket) {
    char recvBuffer[BUF_SIZE];

    FILE *file = fopen(FILENAME, "ab"); // Open the file in binary append mode
    if (file == NULL) {
        perror("File open error");
        return;
    }

    while (1) {
        // Receive data
        ssize_t recvSize = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
        if (recvSize == -1) {
            perror("Receive error");
        } else if (recvSize == 0) {
            printf("Client disconnected\n");
            close(clientSocket);
            break;
        } else {
            // Process received data and write to file
            saveDataToFile(file, recvBuffer, recvSize);
        }
    }

    // Close the file
    fclose(file);
}

void saveDataToFile(FILE *file, char *data, ssize_t dataSize) {
    // Write received data to the file
    fseek(file, 0, SEEK_END);
    fwrite(data, 1, dataSize, file);
}
