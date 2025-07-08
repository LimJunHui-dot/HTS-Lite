#pragma once

#include "../Common/CommonTypes.h"
#include "../Network/SocketClient.h"
#include <string>



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
};

