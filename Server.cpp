#include <stdio.h>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void ToCharArray(SOCKET soc, string st)
{
	char res[32] = "";
	for (size_t  i = 0; i < 32 && i < st.length(); i++)
	{
		res[i] = st[i];
	}
	send(soc, res, 32, 0);
}
void Find_string(SOCKET socket, string s)
{
	printf("The proccess has started \n");
	cout << "received word " << s << endl;
	ifstream f;
	f.open("Example.txt");
	string temp;
	int num = 0;
	string str[10];
	bool flag = false;
	while (getline(f, temp))
	{
		size_t pos = temp.find(s);
		if (pos != string::npos)
		{
			str[num] = temp;
			num++;

		}
	}
	string k = to_string(num);
	cout << k << endl;
	ToCharArray(socket, k);
	for (int i = 0; i < num; i++)
	{
		ToCharArray(socket, str[i]);
		cout << str[i] << endl;
	}
	
	
}
int main() {
	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n ");
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	SOCKET ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket() : % ld\n ", WSAGetLastError());
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(27015);
	if (bind(ListenSocket,
		(SOCKADDR*) &service,
		sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed.\n ");
		closesocket(ListenSocket);
		exit(EXIT_FAILURE);
	}
	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 1) == SOCKET_ERROR)
		printf("Error listening on socket.\n ");

		//----------------------
		// Create a SOCKET for accepting incoming requests.
		SOCKET AcceptSocket;
	printf("Waiting for client to connect...\n ");
	//----------------------
	// Accept the connection.
	while (1) {
		AcceptSocket = SOCKET_ERROR;
		while (AcceptSocket == SOCKET_ERROR) {
			AcceptSocket = accept(ListenSocket, NULL, NULL);
		}
		printf("Client connected.\n ");
		ListenSocket = AcceptSocket;
		break;
	}
	char word[32] = "";
	recv(ListenSocket, word, 32, 0);
	string st = (string)word;
	Find_string(ListenSocket, st);
	WSACleanup();
	return 0;
}
