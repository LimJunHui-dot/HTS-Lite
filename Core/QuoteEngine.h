#pragma once

#include "../Common/CommonTypes.h"
#include "../Network/SocketClient.h"
#include <string>
#include <map>
#include <functional>
#include <mutex>

// �ǽð� �ü� ��ȸ UI
class QuoteEngine
{

public:
	QuoteEngine();
	~QuoteEngine();

	bool Initialize();
	void Shutdown();

	bool RequestQuote(const std::string& stockCode);
	bool GetQuoteData(const std::string& stockCode, QuoteData& quoteData);

	// �ǽð� �ü� ����/����
	bool SubscribeRealTimeQuote(const std::string& stockCode);
	bool UnSubscribeRealTimeQuote(const std::string& stockCode);

	// �ݹ� ���
	typedef std::function QuoteUpdateCallback;
	void RegisterQuoteUpdateCallback(QuoteUpdateCallback callback);

private:
	SocketClient* m_pSocketClient;
	std::map m_quoteDataMap;
	std::mutex m_dataMutex;

	// �ǽð� ������ ���� ������
	std::thread m_receiveThread;
	std::atomic m_bRunning;

	// �ݹ�
	QuoteUpdateCallback m_quoteUpdateCallback;

	void ReceiveThreadFunc();
	void ProcessReceivedData(const std::string& data);
	bool ParseQuoteResponse(const std::string& response, QuoteData& quoteData);
	std::string CreateQuoteRequest(const std::string& stockCode);
	std::string CreateSubscribeRequest(const std::string& stockCode);
	std::string CreateUnsubscribeRequest(const std::string& stockCode);

	void NotifyQuoteUpdate(const std::string& stockCode, const QuoteData& quoteData);
};

