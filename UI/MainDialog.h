#pragma once

#include "QuotePanel.h"
#include "OrderPanel.h"
#include "../Core/QuoteEngine.h"
#include "../Core/OrderManager.h"
#include "../Core/AccountManager.h"

// ���� ���̾�α� Ŭ����

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

	// �гε�
	CQuotePanel* m_pQuotePanel;
	COrderPanel* m_pOrderPanel;
	
	// �ٽ� ������
	QuoteEngine* m_pQuoteEngine;
	OrderManager* m_pOrderManager;
	AccountManager* m_pAccountManager;

	// Ÿ�̸�(�ü� ���ſ�)
	static const UINT_PTR TIMER_QUOTE_UPDATE = 1001;
	static const UINT TIMER_INTERVAL = 1000; // 1��

	void InitializeTabs();
	void InitializeEngines();
	void CleanupEngines();
	void UpdateQuoteDisplay();

public:
	// �� ���� �̺�Ʈ ó��
	afx_msg void OnTcnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
};

