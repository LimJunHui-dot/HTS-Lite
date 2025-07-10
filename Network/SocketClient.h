#pragma once

#include <string>


#pragma comment(lib, "ws2_32.lib")

// TCP/UDP 소켓 클라이언트
class SocketClient
{
public:
	SocketClient();
	virtual ~SocketClient();

	bool Initialize();
	void Cleanup();

	bool Connect(const std::string& serverIP, int port);
	void Disconnect();

	int Send(const std::string& data);
	int Receive(char* buffer, int bufferSize);

	bool SetNonBlocking(bool nonBlocking);
	bool SetTimeout(int timeoutSeconds);
	bool IsConnected() const { return m_bConnected; }


private:
	SOCKET m_socket;
	bool m_bConnected;
	bool m_bInitialized;

	std::string m_serverIP;
	int m_serverPort;

	bool InitializeWinsock();
	void CleanupWinsock();
};

