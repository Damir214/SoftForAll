#include "stdafx.h"
#include <winsock.h>
#include <iostream>
#include <thread>
#include <Windows.h>
#include <vector>

#pragma comment(lib,"ws2_32.lib")

#define SIZE 2048

// ������� ������ ��� �������� ���������.
void sendMessage(int socket_desc)
{
	char message[SIZE];
	memset(message, 0, SIZE);

	while (true) {
		std::cout << "You: ";
		std::cin.getline(message, SIZE);
		// �������� �� �������� ������ ������.
		if (strlen(message) < 1)
			continue;
		// �������� ������ �� ������.
		if (send(socket_desc, message, strlen(message), 0) < 0) {
			std::cout << "\nSend failed!\n";
			break;
		}
	}
}

int main() {
	WSADATA wsa;
	int socket_desc, read_size;
	sockaddr_in server;
	char server_reply[SIZE];

	memset(server_reply, 0, SIZE);
	// ������������� WinsockAPI.
	std::cout << "Initialising Winsock...\n";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "Failed. Error Code : " << WSAGetLastError() << "\n";
		system("pause");
		return 1;
	}
	std::cout << "Initialised.\n";
	// ������������� ������ ������.
	if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cout << "Could not create socket.\n";
		system("pause");
		return 2;
	}
	std::cout << "Socket created.\n";
	// ������� ���������� ��� ������.
	server.sin_addr.s_addr = inet_addr("192.168.0.100"); //192.168.43.204; 172.20.157.87
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	// ����������� � �������.
	if (connect(socket_desc, (sockaddr*)&server, sizeof(server)) < 0) {
		std::cout << "Connection to server was failed.\n";
		system("pause");
		return 3;
	}
	std::cout << "Connected to server.\n";
	// �������� � ������������ ������ ��� �������� ���������.
	std::thread sending(sendMessage, socket_desc);
	sending.detach();
	// � ����� ����������� ����� ��������� � ������������ ��������� �� �������.
	while (true) {
		memset(server_reply, 0, SIZE);
		// �������� �� ��������� ������.
		if ((read_size = recv(socket_desc, server_reply, SIZE, 0)) <= 0) {
			std::cout << "\nrecv failed\n";
			break;
		}
		// �������� �� ��������� ������ ������.
		if (strlen(server_reply) < 1)
			continue;
		// ���������� � �������.
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buff;
		GetConsoleScreenBufferInfo(h, &buff);
		SetConsoleCursorPosition(h, { 0, buff.dwCursorPosition.Y });

		std::cout << server_reply << "\nYou: ";
	}

	std::cout << "\nServer disconected.\n";
	system("pause");
	// �������� ������ � WinsockAPI.
	shutdown(socket_desc, 1);
	//closesocket(socket_desc);
	WSACleanup();
	return 0;
}