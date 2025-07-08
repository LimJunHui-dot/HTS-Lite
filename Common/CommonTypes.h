#pragma once
#include <string>


// �ü� ������ ����ü
struct QuoteData
{	
	std::string stockCode;		// ���� �ڵ�
	std::string stockName;		// �����
	std::string currentPrice;	// ���簡
	std::string changePrice;	// ���ϴ��
	std::string changeRate;		// �����
	std::string openPrice;		// �ð�
	std::string highPrice;		// ��
	std::string lowPrice;		// ����
	std::string volume;			// �ŷ���
	std::string tradingValue;	// �ŷ����
	std::string marketCap;		// �ð��Ѿ�
	std::string updateTime;		// ������Ʈ �ð�

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

// �ֹ� ������ ����ü
struct OrderData
{
	std::string orderNo;		// �ֹ� ��ȣ
	std::string stockCode;		// �����ڵ�
	std::string stockName;		// �����
	std::string orderType;		// �ֹ�����(�ż�/�ŵ�)
	std::string priceType;      // ȣ������ (������/���尡)
	int quantity;               // �ֹ�����
	int price;                  // �ֹ�����
	std::string orderTime;      // �ֹ��ð�
	std::string orderStatus;    // �ֹ�����
	int executedQty;            // ü�����
	int executedPrice;          // ü�ᰡ��
	std::string executedTime;   // ü��ð�

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

// ���� ���� ����ü
struct AccountInfo
{
    std::string accountNo;      // ���¹�ȣ
    std::string accountName;    // ���¸�
    std::string brokerCode;     // ���ǻ��ڵ�
    long long balance;          // ������
    long long availableBalance; // �ֹ����ɱݾ�
    long long totalAsset;       // ���ڻ�
    long long stockAsset;       // �ֽ��ڻ�
    std::string updateTime;     // ������Ʈ�ð�

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

// ���� ���� ���� ����ü
struct HoldingInfo
{
    std::string stockCode;      // �����ڵ�
    std::string stockName;      // �����
    int quantity;               // ��������
    int avgPrice;               // ��մܰ�
    int currentPrice;           // ���簡
    long long purchaseAmount;   // ���Աݾ�
    long long currentAmount;    // �򰡱ݾ�
    long long profitLoss;       // �򰡼���
    double profitRate;          // ���ͷ�

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

// ���� �ڵ� ����
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

// �޽��� Ÿ�� ����
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

// �α� ���� ����
enum class LogLevel
{
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4
};

// ��ƿ��Ƽ �Լ���
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