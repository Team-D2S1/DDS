#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <queue>
#include <thread>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

#define MAIN_SERVER_PORT 7777
#define DEDICATED_SERVER_START_PORT 7778
#define MAX_NUM_SERVERS 10
#define MAX_BUFFER_SIZE 1024

using namespace std;


queue<int> availablePorts;
mutex portMutex;

void InitializePorts()
{
	for (int i = 0; i < MAX_NUM_SERVERS; i++)
	{
		availablePorts.push(DEDICATED_SERVER_START_PORT + i);
	}
}

int AllocatePort()
{
	// 포트 관리 도중 같은 포트를 해제하거나 할당하면 안된다
	// 
	// lock_guard : mutex 관리에 대한 실수 줄이도록 해줌
	// 뮤텍스를 자동으로 잠그고 해당 범위를 벗어나면 lock_guard의 소멸자 호출되고 mutext 해제함
	lock_guard<mutex> lock(portMutex);
	if(availablePorts.empty()) return -1; 
	int port = availablePorts.front();
	availablePorts.pop();
	return port;
}

void ReleasePort(int port)
{
	// 포트 관리 도중 같은 포트를 해제하거나 할당하면 안된다
	lock_guard<mutex> lock(portMutex);
	availablePorts.push(port);
}

// 입력된 port에서 Dedicated Server 실행한다
bool StartDedicatedServer(int port)
{
	// ****** Server.exe 경로 작성 ******
	string command = "D:\\Project\\DDSBuild\\WindowsServer\\DDSServer.exe -port=" + to_string(port) + " -log";

	STARTUPINFOA si = { sizeof(STARTUPINFOA) };
	PROCESS_INFORMATION pi;

	if (!CreateProcessA(NULL, (LPSTR)command.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cerr << "[ERROR] Failed to start Dedicated Server on port " << port << endl;
		return false;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	cout << "[MainServer] Dedicated Server started on port " << port << endl;
	return true;
}

void HandleClient(SOCKET clientSocket)
{
	char buffer[MAX_BUFFER_SIZE] = { 0, };
	int bytesReceived = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);

	if (bytesReceived > 0)
	{
		string request(buffer, bytesReceived);
		cout << "[MainServer] Received request : " << request << endl;

		// Request별 작업 수행

		// 방 생성 요청
		if (request.find("Client_CreateRoom") == 0)
		{
			cout << "[MainServer] Client CreateRoom Accept" << endl;
			int port = AllocatePort();
			if (port == -1)
			{
				string response = "NoAvailablePort";
				cout << "[MainServer] " << response << endl;
				send(clientSocket, response.c_str(), response.size(), 0);
			}
			else
			{
				if (StartDedicatedServer(port))
				{
					string response = "Port : " + to_string(port);
					cout << "[MainServer] " << response << endl;
					send(clientSocket, response.c_str(), response.size(), 0);
				}
				else
				{
					string response = "ServerStartFailed";
					cout << "[MainServer] " << response << endl;
					send(clientSocket, response.c_str(), response.size(), 0);
					ReleasePort(port);
				}
			}
		}
		else
		{
			cout << "[MainServer] Unknown Request" << endl;
		}
	}
}

void RunMainServer()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);

	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cerr << "[ERROR] Winsock Initialization Failed!" << endl;
		return;
	}

	// 서버 소켓 생성
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		cerr << "[ERROR] Socket creation failed!" << endl;
		WSACleanup();
		return;
	}

	// 서버 주소 설정
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(MAIN_SERVER_PORT);

	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
	{
		std::cerr << "[ERROR] Binding failed!" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return;
	}

	if (listen(serverSocket, 5) == SOCKET_ERROR)
	{
		cerr << "[ERROR] Listening failed!" << endl;
		closesocket(serverSocket);
		WSACleanup();
		return;
	}

	cout << "[MainServer] Listening on port " << MAIN_SERVER_PORT << "..." << endl;

	// 클라이언트 연결 수락
	while (true)
	{
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSocket == INVALID_SOCKET) 
		{
			cerr << "[ERROR] Accepting client failed!" << endl;
			continue;
		}

		std::thread(HandleClient, clientSocket).detach();
	}

	closesocket(serverSocket);
	WSACleanup();
}

int main()
{
	InitializePorts();
	RunMainServer();
	return 0;
}