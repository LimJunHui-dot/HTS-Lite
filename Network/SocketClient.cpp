#include "pch.h"
#include "SocketClient.h"

SocketClient::SocketClient()
	: m_socket(INVALID_SOCKET)
	: m_bConnected(false)
	: m_bInitialized(false)
	: m_serverPort(0)
{

}

SocketClient::~SocketClient()
{
	Cleanup();
}

bool SocketClient::Initialize()
{
	if (m_bInitialized) {
		return true;
	}

	if (!InitializeWinsock()) {
		return false;
	}

	m_bInitialized = true;
	Logger::GetInstance().LogInfo("SocketClient: 초기화 완료");
	return true;


}

void SocketClient::Cleanup()
{
	Disconnect();

	if (m_bInitialized) {
		CleanupWinsock();
		m_bInitialized = false;
	}
}

bool SocketClient::Connect(const std::string& serverIP, int port)
{
	if (!m_bInitialized && !Initialize()) {
		return false;
	}

	if (m_bConnected) {
		Disconnect();
	}

	try {
		// 소켓
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET) {
			Logger::GetInstance().LogError("소켓 생성 실패: " + std::to_string(WSAGetLastError()));
			return false;
		}

		// 서버 주소 설정
		sockaddr_in serverAddr;
		ZeroMemory(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);

		int result = inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);
		if (result <= 0) {
			Logger::GetInstance().LogError("잘못된 IP 주소: " + serverIP);
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			return false;
		}

		// 연결
		result = connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr));
		if (result == SOCKET_ERROR) {
			Logger::GetInstance().LogError("서버 연결 실패: " + std::to_string(WSAGetLastError()));
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			return false;
		}

		m_serverIP = serverIP;
		m_serverPort = port;
		m_bConnected = true;

		Logger::GetInstance().LogInfo("서버 연결 성공: " + serverIP + ":" + std::to_string(port));
		return true;
	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogInfo("연결 중 예외 발생: " + std::string(e.what()));
		if (m_socket != INVALID_SOCKET) {
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

		return false;
	}
}

void SocketClient::Disconnect()
{
	if (m_socket != INVALID_SOCKET) {
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	
	m_bConnected = false;

	if (!m_serverIP.empty()) {
		Logger::GetInstance().LogInfo("서버 연결 해제: " + m_serverIP + ":" + std::to_string(m_serverPort));
	}
}


int SocketClient::Send(const std::string& data)
{
	if (!m_bConnected || m_socket == INVALID_SOCKET) {
		Logger::GetInstance().LogError("연결되지 않은 상태에서 전송 시도");
		return -1;
	}

	try {
		int result = send(m_socket, data.c_str(), static_cast(data.length()), 0);

		if (result == SOCKET_ERROR) {
			int error = WSAGetLastError();
			Logger::GetInstance().LogError("데이터 전송 실패: " + std::to_string(error));

			if (error == WSAECONNRESET || error == WSAECONNABORTED) {
				m_bConnected = false;
			}
			return -1;
		}

		Logger::GetInstance().LogDebug("데이터 전송 성공: " + std::to_string(result) + " bytes");
		return result;
	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogError("전송 중 예외 발생: " + std::string(e.what()));
		return -1;
	}
	return 0;
}

int SocketClient::Receive(char* buffer, int bufferSize)
{
	if (!m_bConnected || m_socket == INVALID_SOCKET || !buffer || bufferSize <= 0) {
		return -1;
	}

	try {
		int result = recv(m_socket, buffer, bufferSize, 0);

		if (result == SOCKET_ERROR) {
			int error = WSAGetLastError();

			if (error == WSAEWOULDBLOCK) {
				// 비블로킹 모드에서 데이터가 없는 경우
				return 0;
			}

			Logger::GetInstance().LogError("데이터 수신 실패: " + std::to_string(error));

			if (error == WSAECONNRESET || error == WSAECONNABORTED) {
				m_bConnected = false;
			}
			return -1;
		}

		if (result == 0) {
			// 연결이 정상적으로 종료됨
			Logger::GetInstance().LogInfo("서버에서 연결을 종료함");
			m_bConnected = false;
			return 0;
		}

		Logger::GetInstance().LogDebug("데이터 수신 성공: " + std::to_string(result) + " bytes");
		return result;

	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogError("수신 중 예외 발생: " + std::string(e.what()));
		return -1;
	}
}

bool SocketClient::SetNonBlocking(bool nonBlocking)
{
	if (m_socket == INVALID_SOCKET) {
		return false;
	}

	u_long mode = nonBlocking ? 1 : 0;
	int result = ioctlsocket(m_socket, FIONBIO, &mode);

	if (result == SOCKET_ERROR) {
		Logger::GetInstance().LogError("비블로킹 모드 설정 실패: " + std::to_string(WSAGetLastError()));
		return false;
	}

	return true;
}

bool SocketClient::SetTimeout(int timeoutSeconds)
{
	if (m_socket == INVALID_SOCKET) {
		return false;
	}

	int timeout = timeoutSeconds * 1000; // 밀리초로 변환

	// 수신 타임아웃 설정
	int result = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO,
		(char*)&timeout, sizeof(timeout));
	if (result == SOCKET_ERROR) {
		Logger::GetInstance().LogError("수신 타임아웃 설정 실패: " + std::to_string(WSAGetLastError()));
		return false;
	}

	// 송신 타임아웃 설정
	result = setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO,
		(char*)&timeout, sizeof(timeout));
	if (result == SOCKET_ERROR) {
		Logger::GetInstance().LogError("송신 타임아웃 설정 실패: " + std::to_string(WSAGetLastError()));
		return false;
	}

	return true;
}

bool SocketClient::InitializeWinsock()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0) {
		Logger::GetInstance().LogError("Winsock 초기화 실패: " + std::to_string(result));
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		Logger::GetInstance().LogError("Winsock 2.2를 지원하지 않음");
		WSACleanup();
		return false;
	}

	return true;
}

void SocketClient::CleanupWinsock()
{
	WSACleanup();
}



