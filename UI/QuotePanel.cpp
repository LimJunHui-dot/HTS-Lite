#include "pch.h"
#include "QuotePanel.h"
#include "afxdialogex.h"
#include "../Resource.h"

IMPLEMENT_DYNAMIC(CQuotePanel, CDialogEx)

CQuotePanel::CQuotePanel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUOTE_PANEL, pParent)
	: m_pQuoteEngine(nullptr)
{

}

CQuotePanel::~CQuotePanel()
{

}

void CQuotePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_STOCK_CODE, m_editStockCode);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_LIST_QUOTE, m_listQuote);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
}

BEGIN_MESSAGE_MAP(CQuotePanel, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CQuotePanel::OnBnClickedBtnSearch)
	ON_EN_CHANGE(IDC_EDIT_STOCK_CODE, &CQuotePanel::OnEnChangedEditStockCode)
END_MESSAGE_MAP()

BOOL CQuotePanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitializeControls();
	InitializeQuoteList();

	return TRUE;
}

void CQuotePanel::InitializeControls()
{
	// �����ڵ� �Է� ����(6�ڸ�)
	m_editStockCode.SetLimitText(6);

	// �ʱ� ���� �޽���
	UpdateStatusMessage(_T("�����ڵ带 �Է��ϰ� ��ȸ ��ư�� Ŭ���ϼ���."));
}

void CQuotePanel::InitializeQuoteList()
{
	// ����Ʈ ��Ʈ�� ��Ÿ�� ����
	DWORD dwStyle = m_listQuote.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_listQuote.SetExtendedStyle(dwStyle);

	// �÷� �߰�
	m_listQuote.InsertColumn(0, _T("�׸�"), LVCFMT_LEFT, 100);
	m_listQuote.InsertColumn(1, _T("��"), LVCFMT_RIGHT, 120);
	m_listQuote.InsertColumn(2, _T("���ϴ��"), LVCFMT_RIGHT, 100);
	m_listQuote.InsertColumn(3, _T("�����"), LVCFMT_RIGHT, 80);

	// �ʱ� �� �߰�
	int nIndex = 0;
	m_listQuote.InsertItem(nIndex, _T("�����"));
	m_listQuote.InsertItem(++nIndex, _T("���簡"));
	m_listQuote.InsertItem(++nIndex, _T("�ð�"));
	m_listQuote.InsertItem(++nIndex, _T("��"));
	m_listQuote.InsertItem(++nIndex, _T("����"));
	m_listQuote.InsertItem(++nIndex, _T("�ŷ���"));
	m_listQuote.InsertItem(++nIndex, _T("�ŷ����"));
	m_listQuote.InsertItem(++nIndex, _T("�ð��Ѿ�"));

}

void CQuotePanel::OnBnClickedBtnSearch()
{
	CString stockCode;
	m_editStockCode.GetWindowText(stockCode);
	stockCode.Trim();

	if (stockCode.IsEmpty()) {
		AfxMessageBox(_T("�����ڵ带 �Է��ϼ���."));
		m_editStockCode.SetFocus();
		return;
	}

	if (stockCode.GetLength() != 6) {
		AfxMessageBox(_T("�����ڵ�� 6�ڸ����� �մϴ�."));
		m_editStockCode.SetFocus();
		return;
	}

	if (!m_pQuoteEngine) {
		AfxMessageBox(_T("�ü� ������ �ʱ�ȭ���� �ʾҽ��ϴ�."));
		return;
	}

	// ���� ��ȸ ���� �����ڵ� ����
	m_currentStockCode = stockCode;

	// �ü� ��û
	UpdateStatusMessage(_T("�ü� ��ȸ ��..."));
	m_btnSearch.EnableWindow(FALSE);

	std::string stdStockCode = CT2A(stockCode);
	bool result = m_pQuoteEngine->RequestQuote(stdStockCode);

	if (!result) {
		UpdateStatusMessage(_T("�ü� ��ȸ ��û ����"));
		m_btnSearch.EnableWindow(TRUE);
		AfxMessageBox(_T("�ü� ��ȸ ��û�� �����߽��ϴ�."));
	}
}

void CQuotePanel::OnEnChangedEditStockCode()
{
	CString stockCode;
	m_editStockCode.GetWindowText(stockCode);

	// ���ڸ� �Էµǵ��� ���͸�
	if (!stockCode.IsEmpty()) {
		CString filteredCode;
		for (int i = 0; i < stockCode.GetLength(); i++) {
			TCHAR ch = stockCode.GetAt();
			if (ch >= _T('0') && ch <= _T('9')) {
				filteredCode += ch;
			}
		}

		if (filteredCode != stockCode) {
			m_editStockCode.SetWindowText(filteredCode);
			m_editStockCode.SetSel(filteredCode.GetLength(), filteredCode.GetLength());
		}
	}
}

void CQuotePanel::UpdateDisplay()
{
	if (!m_pQuoteEngine || m_currentStockCode.IsEmpty()) {
		return;
	}

	std::string stdStockCode = CT2A(m_currentStockCode);
	QuoteData quoteData;

	if (m_pQuoteEngine->GetQuoteData(stdStockCode, quoteData)) {
		DisplayQuoteData(quoteData);
		UpdateStatusMessage(_T("�ü� ������Ʈ �Ϸ�"));
		m_btnSearch.EnableWindow(TRUE);
	}
}

void CQuotePanel::DisplayQuoteData(const QuoteData& quoteData)
{
	CString strValue, strChange, strRate;

	// �����
	m_listQuote.SetItemText(0, 1, CA2T(quoteData.stockName.c_str()));

	// ���簡
	strValue.Format(_T("%s"), CA2T(quoteData.currentPrice.c_str()));
	m_listQuote.SetItemText(1, 1, strValue);

	strChange.Format(_T("%s"), CA2T(quoteData.changePrice.c_str()));
	m_listQuote.SetItemText(1, 2, strChange);

	strRate.Format(_T("%s%%"), CA2T(quoteData.changeRate.c_str()));
	m_listQuote.SetItemText(1, 3, strRate);

	// �ð�
	strValue.Format(_T("%s"), CA2T(quoteData.openPrice.c_str()));
	m_listQuote.SetItemText(2, 1, strValue);

	// ��
	strValue.Format(_T("%s"), CA2T(quoteData.highPrice.c_str()));
	m_listQuote.SetItemText(3, 1, strValue);

	// ����
	strValue.Format(_T("%s"), CA2T(quoteData.lowPrice.c_str()));
	m_listQuote.SetItemText(4, 1, strValue);

	// �ŷ���
	strValue.Format(_T("%s"), CA2T(quoteData.volume.c_str()));
	m_listQuote.SetItemText(5, 1, strValue);

	// �ŷ����
	strValue.Format(_T("%s"), CA2T(quoteData.tradingValue.c_str()));
	m_listQuote.SetItemText(6, 1, strValue);

	// �ð��Ѿ�
	strValue.Format(_T("%s"), CA2T(quoteData.marketCap.c_str()));
	m_listQuote.SetItemText(7, 1, strValue);
}

void CQuotePanel::UpdateStatusMessage(const CString& message)
{
	m_staticStatus.SetWindowText(message);
}