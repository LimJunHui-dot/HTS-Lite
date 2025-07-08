#pragma once

#include "../Common/CommonTypes.h"
#include "../Network/SocketClient.h"
#include <string>



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
};

