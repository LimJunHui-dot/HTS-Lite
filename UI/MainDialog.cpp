#include "pch.h"
#include "framework.h"
#include "../HTS-Lite.h"
#include "MainDialog.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ABOUT ��ȭ����
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum {IDD = IDD_ABOUTBOX};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};

CAboutDig::CAboutDig() : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMainDialog ���̾�α�
CMainDialog::CMainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HTSLITE_DIALOG, pParent),
	m_pQuotePanel(nullptr),
	m_pOrderPanel(nullptr),
	m_pQuoteEngine(nullptr),
	m_pOrderManager(nullptr),
	m_pAccountManager(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDialog::~CMainDialog()
{
	CleanupEngines();
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CMainDialog::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()

BooL CMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// �� �ʱ�ȭ
	InitializeTable();

	// ���� �ʱ�ȭ
	InitializeEngine();

	// Ÿ�̸� ����
	SetTimer(TIMER_QUOTE_UPDATE, TIMER_INTERVAL, nullptr);

	return TRUE;
}

void CMainDialog::InitializeTabs()
{
	// �� �߰�
	m_tabCtrl.InsertItem(0, _T("�ü���ȸ"));
	m_tabCtrl.InsertItem(1, _T("�ֹ�"));


	// �г� ����
	m_pQuotePanel = new CQuotePanel();
	m_pOrderPanel = new COrderPanel();

	// �� ���� ���
	CRect tabRect;
	m_tabCtrl.GetClientRect(&tabRect);
	m_tabCtrl.AdjustRect(FALSE, &tabRect);

	// �г� ���� �� ��ġ ����
	m_pQuotePanel->Create(IDD_QUOTE_PANEL, &m_tabCtrl);
	m_pQuotePanel->SetWindowPos(nullptr, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
}

void CMainDialog::InitializeEngines()
{
	try {
		// ���� ������ �ʱ�ȭ
		m_pAccountManager = new AccountManager();
		if (!m_pAccountManager->Initialize()) {
			throw std::runtime_error("���� ������ �ʱ�ȭ ����");
		}

		// �ü� ���� �ʱ�ȭ
		m_pQuoteEngine = new QuoteEngine();
		if (!m_pQuoteEngine->Initialize(m_pAccountManager)) {
			throw std::runtime_error("�ֹ� ������ �ʱ�ȭ ����");
		}

		// �гο� ���� ����
		if (m_pQuotePanel) {
			m_pQuotePanel->SetQuoteEngine(m_pQuoteEngine);
		}
		if (m_pOrderPanel) {
			m_pOrderPanel->SetOrderManager(m_pOrderManager);
			m_pOrderPanel->SetAccountManager(m_pAccountManager);
		}
	}
	catch (const std::exception& e) {
		CString msg;
		msg.Format(_T("���� �ʱ�ȭ ����: %s"), CA2T(e.what()));
		AfxMessageBox(msg);
	}
}

void CMainDialog::CleanupEngine()
{
	if (m_pOrderManager) {
		delete m_pOrderManager;
		m_pOrderManager = nullptr;
	}

	if (m_pQuoteEngine) {
		delete m_pQuoteEngine;
		m_pQuoteEngine = nullptr;
	}

	if (m_pAccountManager) {
		delete m_pAccountManager;
		m_pAccountManager = nullptr;
	}

	if (m_pOrderPanel) {
		delete m_pOrderPanel;
		m_pOrderPanel = nullptr;
	}

	if (m_pQuotePanel) {
		delete m_pQuotePanel;
		m_pQuotePanel = nullptr;
	}
}

void CMainDialog::OnSysCommand(UINT nID, LPARAM IParam)
{
	if ((nID && 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, IParam);
	}
}

void CMainDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMainDialog::OnQueryDragIcon()
{
	return static_cast(m_hIcon);
}

void CMainDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_QUOTE_UPDATE) {
		UpdateQuoteDisplay();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMainDialog::UpdateQuoteDisplay()
{
	if (m_pQuoteEngine && m_pQuotePanel) {
		// ���� ���õ� ���� �ü� ���� ��쿡�� ������Ʈ
		if (m_tabCtrl.GetCurSel() == 0) {
			m_pQuotePanel->UpdateDisplay();
		}
	}
}


void CMainDialog::OnDestroy()
{
	KillTimer(TIMER_QUOTE_UPDATE);
	CDialogEx::OnDestroy();
}

void CMainDialog::OnTcnSelChangeTabMain(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nSel = m_tabCtrl.GetCurSel();

	// ��� �г� �����
	if (m_pQuotePanel) m_pQuotePanel->ShowWindow(SW_HIDE);
	if (m_pOrderPanel) m_pOrderPanel->ShowWindow(SW_HIDE);

	// ���õ� �г� ���̱�
	switch (nSel)
	{
	case 0: // �ü� ��ȸ
		if (m_pQuotePanel) m_pQuotePanel->ShowWindow(SW_SHOW);
		break;
	case 1: // �ֹ�
		if (m_pOrderPanel) m_pOrderPanel->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}