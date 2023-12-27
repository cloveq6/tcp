#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5150
#define BACKLOG 5
#define BUF_SIZE 1088
#define FILENAME "../received_data.txt"

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

    char recvBuffer[BUF_SIZE];

    FILE *file = fopen(FILENAME, "wb"); // ���ļ���"wb" ��ʾ�Զ�����д�뷽ʽ���ļ�
    if (file == NULL) {
        perror("File open error");
        return 1;
    }

    while (1) {
        // Accept a connection from a client
        socklen_t clientAddrLen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Accept error");
        }

        while (1) {
            // Receive data
            ssize_t recvSize = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
            if (recvSize == -1) {
                perror("Receive error");
            } else if (recvSize == 0) {
                printf("Client disconnected\n");
                /*�رտͻ�������,����⵽�ͻ��˶Ͽ�����ʱ���ر� clientSocket��
                ����һ��ѭ���н����µ����ӡ���������ͬʱ�����������õ�����
                */
                close(clientSocket);
                break;
            } else {
                //Process received data (for demonstration, just print it)
                //printf("Received data from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                //for (int i = 0; i < recvSize; ++i) {
                //    printf("%d ", recvBuffer[i]);
                //}
                //printf("\n");

                // Write received data to the file
                fseek(file, 0, SEEK_END);
                fwrite(recvBuffer, 1, recvSize, file);
            }
        }
        // Close the file
        fclose(file);
    }



    // Close the server socket (this part won't be reached in an infinite loop)
    close(serverSocket);

    return 0;
}
