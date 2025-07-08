#include "pch.h"
#include "QuoteEngine.h"
#include "../Common/CommonTypes.h"
#include "Logger.h"
#include <vector>
#include <chrono>
#include <iomanip>

QuoteEngine::QuoteEngine()
	: m_pSocketClient(nullptr), m_bRunning(false)
{

}

QuoteEngine::~QuoteEngine()
{
	Shutdown();
}

bool QuoteEngine::Initialize()
{
	try {
		// ���� Ŭ���̾�Ʈ ����
		m_pSocketClient = new SocketClient();

		// ���� ����
		Config& config = Config::GetInstance();
		std::string serverIP = config.GetString("Server", "IP", "127.0.0.1");
		int serverPort = config.GetInt("Server", "Port", 9999);

		if (!m_pSocketClient->Connect(serverIP, serverPort)) {
			Logger::GetInstance().LogError("�ü� ���� ���� ����: " + serverIP + ":" + std::to_string(serverPort));
			return false;
		}

		Logger::GetInstance().LogInfo("�ü� ���� ���� ����: " + serverIP + ":" + std::to_string(serverPort));

		// ���� ������ ����
		m_bRunning = false;
		m_receiveThread = std::thread(&QuoteEngine::ReceiveThreadFunc, this);
		
		return true;
		
	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogError("QuoteEngine �ʱ�ȭ ����: " + std::string(e.what()));
		return false;
	}
}

void QuoteEngine::Shutdown()
{
	m_bRunning = false;

	if (m_receiveThread.joinable()) {
		m_receiveThread.join();
	}

	if (m_pSocketClient) {
		delete m_pSocketClient;
		m_pSocketClient = nullptr;
	}

	Logger::GetInstance().LogInfo("QuoteEngine ���� �Ϸ�");
}

bool QuoteEngine::RequestQuote(const std::string& stockCode)
{
	if (!m_pSocketClient || !m_pSocketClient->IsConnected()) {
		Logger::GetInstance().LogError("�ü� ������ ������� ����");
		return false;
	}

	try {
		std::string request = CreateQuoteRequest(stockCode);
		int result = m_pSocketClient->Send(request);

		if (result <= 0) {
			Logger::GetInstance().LogError("�ü� ��û ���� ����: " + stockCode);
			return false;
		}

		Logger::GetInstance().LogInfo("�ü� ��û ���� ����: " + stockCode);
		return true;
	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogError("�ü� ��û �� ����: " + std::string(e.what()));
		return false;
	}
}

bool QuoteEngine::GetQuoteData(const std::string& stockCode, QuoteData& quoteData)
{
	std::lock_guard lock(m_dataMutex);

	auto it = m_quoteDataMap.find(stockCode);
	if (it != m_quoteDataMap.end()) {
		quoteData = it->second;
		return true;
	}

	return false;
}

bool QuoteEngine::SubscribeRealTimeQuote(const std::string& stockCode)
{
	if (!m_pSocketClient || !m_pSocketClient->IsConnected()) {
		return false;
	}

	try {
		std::string request = CreateSubscribeRequest(stockCode);
		int result = m_pSocketClient->Send(request);

		Logger::GetInstance().LogInfo("�ǽð� �ü� ����: " + stockCode);
		return result > 0;
	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogError("�ǽð� �ü� ���� ����: " + std::string(e.what()));
		return false;
	}
}

bool QuoteEngine::UnSubscribeRealTimeQuote(const std::string& stockCode)
{
	if (!m_pSocketClient || !m_pSocketClient->IsConnected()) {
		return false;
	}

	try {
		std::string request = CreateUnsubscribeRequest(stockCode);
		int result = m_pSocketClient->Send(request);

		Logger::GetInstance().LogInfo("�ǽð� �ü� ���� ����: " + stockCode);
		return result > 0;

	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogError("�ǽð� �ü� ���� ���� ����: " + std::string(e.what()));
		return false;
	}
}

void QuoteEngine::RegisterQuoteUpdateCallback(QuoteUpdateCallback callback)
{
	m_quoteUpdateCallback = callback;
}

void QuoteEngine::ReceiveThreadFunc()
{
	const int BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];

	Logger::GetInstance().LogInfo("�ü� ���� ������ ����");

	while (m_bRunning) {
		try {
			if (!m_pSocketClient || !m_pSocketClient->IsConnected()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				continue;
			}

			int received = m_pSocketClient->Receive(buffer, BUFFER_SIZE - 1);
			if (received > 0) {
				buffer[received] = '\0';
				std::string data(buffer);
				ProcessReceivedData(data);
			}
			else if (received == 0) {
				Logger::GetInstance().LogWarning("���� ������ �����");
			}
		}
		catch (const std::exception& e) {
			Logger::GetInstance().LogError("�ü� ���� �� ����: " + std::string(e.what()));
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}

	Logger::GetInstance().LogInfo("�ü� ���� ������ ����");
}

void QuoteEngine::ProcessReceivedData(const std::string& data)
{
	try {
		// JSON �Ǵ� �������� �������� �Ľ�
		// ���⼭�� ������ ������(|) ������ �������� ���

		std::vector lines;
		std::stringstream ss(data);
		std::string line;

		while (std::getline(ss, line)) {
			if (!line.empty()) {
				lines.push_back(line);
			}
		}

		for (const auto& lineData : lines) {
			QuoteData quoteData;
			if (ParseQuoteResponse(lineData, quoteData)) {
				// ������ ����
				{
					std::lock_guard lock(m_dataMutex);
					m_quoteDataMap(quoteData.stockCode) = quoteData;
				}

				// �ݹ� ȣ��
				NotifyQuoteUpdate(quoteData.stockCode, quoteData);
			}
		}

	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogError("���� ������ ó�� ����: " + std::string(e.what()));
	}
}

bool QuoteEngine::ParseQuoteResponse(const std::string& response, QuoteData& quoteData)
{
	try {
		// ��������: QUOTE|�����ڵ�|�����|���簡|���ϴ��|�����|�ð�|��|�ŷ���|�ŷ����|�ð��Ѿ�
		std::vector tokens;
		std::stringstream ss(response);
		std::string token;

		while (std::getline(ss, tokens, 'I')) {
			tokens.push_back(token);
		}

		if (tokens.size() < 12 || tokens[0] != "QUOTE") {
			return false;
		}

		quoteData.stockCode = tokens[1];
		quoteData.stockName = tokens[2];
		quoteData.currentPrice = tokens[3];
		quoteData.changePrice = tokens[4];
		quoteData.changeRate = tokens[5];
		quoteData.openPrice = tokens[6];
		quoteData.highPrice = tokens[7];
		quoteData.lowPrice = tokens[8];
		quoteData.volume = tokens[9];
		quoteData.tradingValue = tokens[10];
		quoteData.marketCap = tokens[11];

		// ���� �ð� ����
		auto now = std::chrono::system_clock::now();
		auto time_t = std::chrono::system_clock::to_time_t(now);
		std::stringstream timeStream;
		timeStream << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
		quoteData.updateTime = timeStream.str();

		return true;
	}
	catch (const std::exception& e) {
		Logger::GetInstance().LogError("�ü� ���� �Ľ� ����: " + std::string(e.what()));
		return false;
	}

}

std::string QuoteEngine::CreateQuoteRequest(const std::string& stockCode)
{
	// ��������: REQ_QUOTE|�����ڵ�
	return "REQ_QUOTE|" + stockCode + "\n";
}

std::string QuoteEngine::CreateSubscribeRequest(const std::string& stockCode)
{
	// ��������: SUBSCRIBE|�����ڵ�
	return "SUBSCRIBE|" + stockCode + "\n";
}

std::string QuoteEngine::CreateUnsubscribeRequest(const std::string& stockCode)
{
	// ��������: UNSUBSCRIBE|�����ڵ�
	return "UNSUBSCRIBE|" + stockCode + "\n";
}

void QuoteEngine::NotifyQuoteUpdate(const std::string& stockCode, const QuoteData& quoteData)
{
	if (m_quoteUpdateCallback) {
		try {
			m_quoteUpdateCallback(stockCode, quoteData);
		}
		catch (const std::exception& e) {
			Logger::GetInstance().LogError("�ü� ������Ʈ �ݹ� ����: " + std::string(e.what()));
		}
	}
}
