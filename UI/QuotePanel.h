#pragma once

#include "../Core/QuoteEngine.h"
#include "../Common/CommonTypes.h"

// 실시간 시세 조회 UI
class CQuotePanel : public CDialogEx
{
	DECLARE_DYNAMIC(CQuotePanel)

public:
	CQuotePanel(CWnd* pParent = nullptr);
	virtual ~CQuotePanel();

#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_QUOTE_PANEL};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	// 컨트롤 변수
	CEdit m_editStockCode;
	CButton m_btnSearch;
	CListCTRL m_listQuote;
	CStatic m_staticStatus;

	// 엔진 포인터
	QuoteEngine* m_pQuoteEngine;

	// 현재 조회 중인 종목 코드
	CString m_currentStockCode;

	void InitializeControls();
	void InitializeQuoteList();
	void DisplayQuoteData(const QuoteData& quoteData);
	void UpdateStatusMessage(const CString& message);

public:
	void SetQuoteEngine(QuoteEngine* pEngine) {
		m_pQuoteEngine = pEngine;
	}
	void UpdateDisplay();

	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnEnChangedEditStockCode();
};




