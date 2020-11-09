#include <stdio.h>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib") 

using namespace std;

int  main() {
	//----------------------
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf(" Error at WSAStartup()\n ");
	//----------------------
	// Create a SOCKET for connecting to server
	SOCKET ConnectSocket;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		printf(" Error at socket() : % ld\n " , WSAGetLastError());
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(clientService.sin_addr));
	clientService.sin_port = htons(27015);
	//----------------------
	// Connect to server.
	if (connect(ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf(" Failed to connect.\n " );
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	//----------------------
	// Declare and initialize variables.
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendbuf[32] = "Client: Sending data.";
	char recvbuf[32] = "";
	//----------------------
	// Send and receive data.
	cout << "enter your word: ";
	cin >> sendbuf;
	bytesSent = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
	bytesRecv = recv(ConnectSocket, recvbuf, 32, 0);
	string k = (string)recvbuf;
	int n = stoi(k);
	if (n == 0)
	{
		cout << "no entry ";
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			bytesRecv = recv(ConnectSocket, recvbuf, 32, 0);
			if (bytesRecv == 0 || bytesRecv == WSAECONNRESET) {
				printf("Connection Closed.\n");
				break;
			}
			cout << recvbuf << endl;
		}
	}
	WSACleanup();
}