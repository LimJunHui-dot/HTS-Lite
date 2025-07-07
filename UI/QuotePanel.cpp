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
	// 종목코드 입력 제한(6자리)
	m_editStockCode.SetLimitText(6);

	// 초기 상태 메시지
	UpdateStatusMessage(_T("종목코드를 입력하고 조회 버튼을 클릭하세요."));
}

void CQuotePanel::InitializeQuoteList()
{
	// 리스트 컨트롤 스타일 설정
	DWORD dwStyle = m_listQuote.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_listQuote.SetExtendedStyle(dwStyle);

	// 컬럼 추가
	m_listQuote.InsertColumn(0, _T("항목"), LVCFMT_LEFT, 100);
	m_listQuote.InsertColumn(1, _T("값"), LVCFMT_RIGHT, 120);
	m_listQuote.InsertColumn(2, _T("전일대비"), LVCFMT_RIGHT, 100);
	m_listQuote.InsertColumn(3, _T("등락률"), LVCFMT_RIGHT, 80);

	// 초기 행 추가
	int nIndex = 0;
	m_listQuote.InsertItem(nIndex, _T("종목명"));
	m_listQuote.InsertItem(++nIndex, _T("현재가"));
	m_listQuote.InsertItem(++nIndex, _T("시가"));
	m_listQuote.InsertItem(++nIndex, _T("고가"));
	m_listQuote.InsertItem(++nIndex, _T("저가"));
	m_listQuote.InsertItem(++nIndex, _T("거래량"));
	m_listQuote.InsertItem(++nIndex, _T("거래대금"));
	m_listQuote.InsertItem(++nIndex, _T("시가총액"));

}

void CQuotePanel::OnBnClickedBtnSearch()
{
	CString stockCode;
	m_editStockCode.GetWindowText(stockCode);
	stockCode.Trim();

	if (stockCode.IsEmpty()) {
		AfxMessageBox(_T("종목코드를 입력하세요."));
		m_editStockCode.SetFocus();
		return;
	}

	if (stockCode.GetLength() != 6) {
		AfxMessageBox(_T("종목코드는 6자리여야 합니다."));
		m_editStockCode.SetFocus();
		return;
	}

	if (!m_pQuoteEngine) {
		AfxMessageBox(_T("시세 엔진이 초기화되지 않았습니다."));
		return;
	}

	// 현재 조회 중인 종목코드 저장
	m_currentStockCode = stockCode;

	// 시세 요청
	UpdateStatusMessage(_T("시세 조회 중..."));
	m_btnSearch.EnableWindow(FALSE);

	std::string stdStockCode = CT2A(stockCode);
	bool result = m_pQuoteEngine->RequestQuote(stdStockCode);

	if (!result) {
		UpdateStatusMessage(_T("시세 조회 요청 실패"));
		m_btnSearch.EnableWindow(TRUE);
		AfxMessageBox(_T("시세 조회 요청에 실패했습니다."));
	}
}

void CQuotePanel::OnEnChangedEditStockCode()
{
	CString stockCode;
	m_editStockCode.GetWindowText(stockCode);

	// 숫자만 입력되도록 필터링
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
		UpdateStatusMessage(_T("시세 업데이트 완료"));
		m_btnSearch.EnableWindow(TRUE);
	}
}

void CQuotePanel::DisplayQuoteData(const QuoteData& quoteData)
{
	CString strValue, strChange, strRate;

	// 종목명
	m_listQuote.SetItemText(0, 1, CA2T(quoteData.stockName.c_str()));

	// 현재가
	strValue.Format(_T("%s"), CA2T(quoteData.currentPrice.c_str()));
	m_listQuote.SetItemText(1, 1, strValue);

	strChange.Format(_T("%s"), CA2T(quoteData.changePrice.c_str()));
	m_listQuote.SetItemText(1, 2, strChange);

	strRate.Format(_T("%s%%"), CA2T(quoteData.changeRate.c_str()));
	m_listQuote.SetItemText(1, 3, strRate);

	// 시가
	strValue.Format(_T("%s"), CA2T(quoteData.openPrice.c_str()));
	m_listQuote.SetItemText(2, 1, strValue);

	// 고가
	strValue.Format(_T("%s"), CA2T(quoteData.highPrice.c_str()));
	m_listQuote.SetItemText(3, 1, strValue);

	// 저가
	strValue.Format(_T("%s"), CA2T(quoteData.lowPrice.c_str()));
	m_listQuote.SetItemText(4, 1, strValue);

	// 거래량
	strValue.Format(_T("%s"), CA2T(quoteData.volume.c_str()));
	m_listQuote.SetItemText(5, 1, strValue);

	// 거래대금
	strValue.Format(_T("%s"), CA2T(quoteData.tradingValue.c_str()));
	m_listQuote.SetItemText(6, 1, strValue);

	// 시가총액
	strValue.Format(_T("%s"), CA2T(quoteData.marketCap.c_str()));
	m_listQuote.SetItemText(7, 1, strValue);
}

void CQuotePanel::UpdateStatusMessage(const CString& message)
{
	m_staticStatus.SetWindowText(message);
}