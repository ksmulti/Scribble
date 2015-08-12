
// Scribble.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Scribble.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ScribbleDoc.h"
#include "ScribbleView.h"
#include "PenWidthsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScribbleApp

BEGIN_MESSAGE_MAP(CScribbleApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CScribbleApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_PEN_COLOR, &CScribbleApp::OnPenColor)
	ON_COMMAND(ID_PEN_THICK_OR_THIN, &CScribbleApp::OnPenThickOrThin)
	ON_UPDATE_COMMAND_UI(ID_PEN_THICK_OR_THIN, &CScribbleApp::OnUpdatePenThickOrThin)
	ON_COMMAND(ID_PEN_WIDTHS, &CScribbleApp::OnPenWidths)
END_MESSAGE_MAP()


// CScribbleApp construction

CScribbleApp::CScribbleApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Scribble.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_colorCur = RGB(0, 0, 0);
	m_bThickPen = false;
	m_nThinWidth = 2;
	m_nThickWidth = 5;

	m_regRegistry.Create(HKEY_CURRENT_USER, _T("Software\\MyScribble"));
	long result;
	DWORD Value;

	result = m_regRegistry.QueryDWORDValue(_T("ThinPenWidth"), Value);
	if (result == ERROR_SUCCESS)
		m_nThinWidth = Value;

	result = m_regRegistry.QueryDWORDValue(_T("ThickPenWidth"), Value);
	if (result == ERROR_SUCCESS)
		m_nThickWidth = Value;
}

// The one and only CScribbleApp object

CScribbleApp theApp;


// CScribbleApp initialization

BOOL CScribbleApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ScribTYPE,
		RUNTIME_CLASS(CScribbleDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CScribbleView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CScribbleApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	return CWinApp::ExitInstance();
}

// CScribbleApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CScribbleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CScribbleApp message handlers

void CScribbleApp::OnPenColor()
{
	// TODO: Add your command handler code here
	CMFCColorDialog colordlg(m_colorCur);
	if (colordlg.DoModal() == IDOK)
	{
		m_colorCur = colordlg.GetColor();
	}
}


void CScribbleApp::OnPenThickOrThin()
{
	// TODO: Add your command handler code here
	m_bThickPen = !m_bThickPen;
}


void CScribbleApp::OnUpdatePenThickOrThin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bThickPen);
}


void CScribbleApp::OnPenWidths()
{
	// TODO: Add your command handler code here
	CPenWidthsDlg dlg;
	dlg.m_nThinWidth = m_nThinWidth;
	dlg.m_nThickWidth = m_nThickWidth;

	if (dlg.DoModal() == IDOK)
	{
		m_nThinWidth = dlg.m_nThinWidth;
		m_nThickWidth = dlg.m_nThickWidth;

		m_regRegistry.SetDWORDValue(_T("ThinPenWidth"), m_nThinWidth);
		m_regRegistry.SetDWORDValue(_T("ThickPenWidth"), m_nThickWidth);
	}
}