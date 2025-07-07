#pragma once

#include "../Core/QuoteEngine.h"
#include "../Common/CommonTypes.h"

// �ǽð� �ü� ��ȸ UI
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
	// ��Ʈ�� ����
	CEdit m_editStockCode;
	CButton m_btnSearch;
	CListCTRL m_listQuote;
	CStatic m_staticStatus;

	// ���� ������
	QuoteEngine* m_pQuoteEngine;

	// ���� ��ȸ ���� ���� �ڵ�
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




