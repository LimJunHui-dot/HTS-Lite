
// HTS-Lite.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "HTS-Lite.h"
#include "UI/MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHTSLiteApp
// 메시지 맵 정의
// ID_HELP 명령에 대해 기본 도움말 핸들러를 연결
BEGIN_MESSAGE_MAP(CHTSLiteApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHTSLiteApp 생성
CHTSLiteApp::CHTSLiteApp() : m_pLogger(nullptr), m_pNetworkManager(nullptr)
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// m_pLogger, m_pNetworkManager가 할당되면 메모리를 해제
CHTSLiteApp::~CHTSLiteApp()
{
	if (m_pNetworkManager) {
		delete m_pNetworkManager;
		m_pNetworkManager = nullptr;
	}

	if (m_pLogger) {
		delete m_pLogger;
		m_pLogger = nullptr;
	}
}




// CHTSLiteApp 초기화
BOOL CHTSLiteApp::InitInstance()
{
	// Windows XP에서는 InitCommonControlsEx()를 필요로 합니다.
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("HTS-Lite Application"));

	// 로거 초기화
	m_pLogger = new Logger();
	if (!m_pLogger->Initialize(_T("logs"))) {
		AfxMessageBox(_T("로거 초기화 실패"));
		return FALSE;
	}

	// 네트워크 매니저 초기화
	m_pNetworkManager = new NetworkManager();
	if (!m_pNetworkManager->Initialize()) {
		AfxMessageBox(_T("네트워크 초기화 실패"));
		return FALSE;
	}

	// 메인 다이얼로그 실행
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

int CHTSLiteApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

CHTSLiteApp theApp;


