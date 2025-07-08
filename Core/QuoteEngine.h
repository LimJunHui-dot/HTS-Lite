#pragma once

#include "../Common/CommonTypes.h"
#include "../Network/SocketClient.h"
#include <string>
#include <map>
#include <functional>
#include <mutex>

// 실시간 시세 조회 UI
class QuoteEngine
{

public:
	QuoteEngine();
	~QuoteEngine();

	bool Initialize();
	void Shutdown();

	bool RequestQuote(const std::string& stockCode);
	bool GetQuoteData(const std::string& stockCode, QuoteData& quoteData);

	// 실시간 시세 구독/해제
	bool SubscribeRealTimeQuote(const std::string& stockCode);
	bool UnSubscribeRealTimeQuote(const std::string& stockCode);

	// 콜백 등록
	typedef std::function QuoteUpdateCallback;
	void RegisterQuoteUpdateCallback(QuoteUpdateCallback callback);

private:
	SocketClient* m_pSocketClient;
	std::map m_quoteDataMap;
	std::mutex m_dataMutex;

	// 실시간 데이터 수신 스레드
	std::thread m_receiveThread;
	std::atomic m_bRunning;

	// 콜백
	QuoteUpdateCallback m_quoteUpdateCallback;

	void ReceiveThreadFunc();
	void ProcessReceivedData(const std::string& data);
	bool ParseQuoteResponse(const std::string& response, QuoteData& quoteData);
	std::string CreateQuoteRequest(const std::string& stockCode);
	std::string CreateSubscribeRequest(const std::string& stockCode);
	std::string CreateUnsubscribeRequest(const std::string& stockCode);

	void NotifyQuoteUpdate(const std::string& stockCode, const QuoteData& quoteData);
};

