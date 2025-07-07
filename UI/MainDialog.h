#pragma once

#include "QuotePanel.h"
#include "OrderPanel.h"
#include "../Core/QuoteEngine.h"
#include "../Core/OrderManager.h"
#include "../Core/AccountManager.h"

// 메인 다이얼로그 클래스

class CMainDialog : public CDialogEx
{
public:
	CMainDialog(CWnd* pParent = nullptr);
	virtual ~CMainDialog();

#ifdef AFX_DESIGN_TIME
	enum { IDO = IDD_HTSLITE_DIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM IParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP();

private:
	HICON m_hIcon;
	CTabCtrl m_tabCtrl;

	// 패널들
	CQuotePanel* m_pQuotePanel;
	COrderPanel* m_pOrderPanel;
	
	// 핵심 엔진들
	QuoteEngine* m_pQuoteEngine;
	OrderManager* m_pOrderManager;
	AccountManager* m_pAccountManager;

	// 타이머(시세 갱신용)
	static const UINT_PTR TIMER_QUOTE_UPDATE = 1001;
	static const UINT TIMER_INTERVAL = 1000; // 1초

	void InitializeTabs();
	void InitializeEngines();
	void CleanupEngines();
	void UpdateQuoteDisplay();

public:
	// 탭 변경 이벤트 처리
	afx_msg void OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
};

