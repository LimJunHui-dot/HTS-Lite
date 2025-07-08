#pragma once
#include <string>


// 시세 데이터 구조체
struct QuoteData
{	
	std::string stockCode;		// 종목 코드
	std::string stockName;		// 종목명
	std::string currentPrice;	// 현재가
	std::string changePrice;	// 전일대비
	std::string changeRate;		// 등락률
	std::string openPrice;		// 시가
	std::string highPrice;		// 고가
	std::string lowPrice;		// 저가
	std::string volume;			// 거래량
	std::string tradingValue;	// 거래대금
	std::string marketCap;		// 시가총액
	std::string updateTime;		// 업데이트 시간

	QuoteData()
	{
		Clear();
	}

	void Clear()
	{
		stockCode.clear();
		stockName.clear();
		currentPrice = "0";
		changePrice = "0";
		changeRate = "0.00";
		openPrice = "0";
		highPrice = "0";
		lowPrice = "0";
		tradingValue = "0";
		marketCap = "0";
		updateTime.clear();
	}
};

// 주문 데이터 구조체
struct OrderData
{
	std::string orderNo;		// 주문 번호
	std::string stockCode;		// 종목코드
	std::string stockName;		// 종목명
	std::string orderType;		// 주문구분(매수/매도)
	std::string priceType;      // 호가구분 (지정가/시장가)
	int quantity;               // 주문수량
	int price;                  // 주문가격
	std::string orderTime;      // 주문시간
	std::string orderStatus;    // 주문상태
	int executedQty;            // 체결수량
	int executedPrice;          // 체결가격
	std::string executedTime;   // 체결시간

	OrderData()
	{
		Clear();
	}

	void Clear()
	{
		orderNo.clear();
		stockCode.clear();
		stockName.clear();
		orderType.clear();
		priceType.clear();
		quantity = 0;
		price = 0;
		orderTime.clear();
		orderStatus.clear();
		executedQty = 0;
		executedPrice = 0;
		executedTime.clear();
	}
};

// 계좌 정보 구조체
struct AccountInfo
{
    std::string accountNo;      // 계좌번호
    std::string accountName;    // 계좌명
    std::string brokerCode;     // 증권사코드
    long long balance;          // 예수금
    long long availableBalance; // 주문가능금액
    long long totalAsset;       // 총자산
    long long stockAsset;       // 주식자산
    std::string updateTime;     // 업데이트시간

    AccountInfo()
    {
        Clear();
    }

    void Clear()
    {
        accountNo.clear();
        accountName.clear();
        brokerCode.clear();
        balance = 0;
        availableBalance = 0;
        totalAsset = 0;
        stockAsset = 0;
        updateTime.clear();
    }
};

// 보유 종목 정보 구조체
struct HoldingInfo
{
    std::string stockCode;      // 종목코드
    std::string stockName;      // 종목명
    int quantity;               // 보유수량
    int avgPrice;               // 평균단가
    int currentPrice;           // 현재가
    long long purchaseAmount;   // 매입금액
    long long currentAmount;    // 평가금액
    long long profitLoss;       // 평가손익
    double profitRate;          // 수익률

    HoldingInfo()
    {
        Clear();
    }

    void Clear()
    {
        stockCode.clear();
        stockName.clear();
        quantity = 0;
        avgPrice = 0;
        currentPrice = 0;
        purchaseAmount = 0;
        currentAmount = 0;
        profitLoss = 0;
        profitRate = 0.0;
    }
};

// 에러 코드 정의
enum class ErrorCode
{
    SUCCESS = 0,
    NETWORK_ERROR = 1000,
    PROTOCOL_ERROR = 1001,
    AUTHENTICATION_ERROR = 1002,
    INVALID_PARAMETER = 1003,
    INSUFFICIENT_BALANCE = 1004,
    ORDER_LIMIT_EXCEEDED = 1005,
    SYSTEM_ERROR = 9999
};

// 메시지 타입 정의
enum class MessageType
{
    QUOTE_REQUEST,
    QUOTE_RESPONSE,
    ORDER_REQUEST,
    ORDER_RESPONSE,
    ACCOUNT_REQUEST,
    ACCOUNT_RESPONSE,
    REALTIME_QUOTE,
    REALTIME_ORDER,
    HEARTBEAT,
    ERROR_MESSAGE
};

// 로그 레벨 정의
enum class LogLevel
{
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4
};

// 유틸리티 함수들
namespace Utils
{
    std::string GetCurrentTimeString();
    std::string FormatNumber(long long number);
    std::string FormatPrice(int price);
    std::string FormatRate(double rate);
    bool IsValidStockCode(const std::string& stockCode);
    std::vector SplitString(const std::string& str, char delimiter);
    std::string TrimString(const std::string& str);
}