// CanvasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Scribble.h"
#include "CanvasDlg.h"
#include "afxdialogex.h"


// CCanvasDlg dialog

IMPLEMENT_DYNAMIC(CCanvasDlg, CDialogEx)

CCanvasDlg::CCanvasDlg(int nCanvasWidth, int nCanvasHeight, COLORREF colorBackground, CWnd* pParent /*=NULL*/)
	: CDialogEx(CCanvasDlg::IDD, pParent)
	, m_csBckImagePath(_T(""))
{
	m_nCanvasWidth = nCanvasWidth;
	m_nCanvasHeight = nCanvasHeight;
	m_colorBackground = colorBackground;
}

CCanvasDlg::~CCanvasDlg()
{
}

void CCanvasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CANVAS_WIDTH, m_nCanvasWidth);
	DDV_MinMaxInt(pDX, m_nCanvasWidth, 1, 9999);
	DDX_Text(pDX, IDC_CANVAS_HEIGHT, m_nCanvasHeight);
	DDV_MinMaxInt(pDX, m_nCanvasHeight, 1, 9999);
	DDX_Control(pDX, IDC_BACKGROUND_COLOR_EDIT, CBackgroundColorEdit);
	DDX_Control(pDX, IDC_CANVAS_WIDTH, CCanvasWidthEdit);
	DDX_Control(pDX, IDC_CANVAS_HEIGHT, CCanvasHeightEdit);
	DDX_Control(pDX, IDC_BACKGROUND_COLOR_BUTTON, CBckColorButton);
	DDX_Text(pDX, IDC_BCKIMAGE_PATH, m_csBckImagePath);
}


BEGIN_MESSAGE_MAP(CCanvasDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BACKGROUND_COLOR_BUTTON, &CCanvasDlg::OnBnClickedBackgroundColorButton)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_BCKIMAGE_PATH, &CCanvasDlg::OnEnChangeBckimagePath)
	ON_BN_CLICKED(IDC_BCKIMAGE_CLEAR, &CCanvasDlg::OnBnClickedBckimageClear)
	ON_BN_CLICKED(IDC_BCKIMAGE_BROWSER, &CCanvasDlg::OnBnClickedBckimageBrowser)
END_MESSAGE_MAP()


// CCanvasDlg message handlers


void CCanvasDlg::OnBnClickedBackgroundColorButton()
{
	// TODO: Add your control notification handler code here
	CMFCColorDialog colordlg(m_colorBackground);
	if (colordlg.DoModal() == IDOK)
	{
		m_colorBackground = colordlg.GetColor();
		this->Invalidate();
	}
}


HBRUSH CCanvasDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_BACKGROUND_COLOR_EDIT: 
		COLORREF bkColor = m_colorBackground;
		CRect rect;
		pWnd->GetClientRect(&rect);
		CBrush br;
		br.CreateSolidBrush(bkColor);
		pDC->FillRect(rect, &br);
		pDC->SetBkColor(bkColor);
		hbr= CreateSolidBrush(bkColor);
		break;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CCanvasDlg::OnEnChangeBckimagePath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	if (m_csBckImagePath.IsEmpty())
		SetSetting(TRUE);
	else
		SetSetting(FALSE);
}


void CCanvasDlg::OnBnClickedBckimageClear()
{
	// TODO: Add your control notification handler code here
	m_csBckImagePath.Empty();
	SetSetting(TRUE);
	UpdateData(FALSE);
}


void CCanvasDlg::OnBnClickedBckimageBrowser()
{
	// TODO: Add your control notification handler code here
	CString szFilter = _T("bitmap|*.bmp||");
	CFileDialog fd(TRUE, _T("bmp"), _T("*.bmp"), OFN_HIDEREADONLY, szFilter, this);
	if (fd.DoModal() == IDOK)
	{
		m_csBckImagePath = fd.GetPathName();
		SetSetting(FALSE);
		UpdateData(FALSE);
	}
}


void CCanvasDlg::SetSetting(BOOL flag)
{
	CBackgroundColorEdit.EnableWindow(flag);
	CCanvasWidthEdit.EnableWindow(flag);
	CCanvasHeightEdit.EnableWindow(flag);
	CBckColorButton.EnableWindow(flag);
}
