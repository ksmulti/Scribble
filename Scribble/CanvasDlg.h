#pragma once
#include "afxwin.h"


// CCanvasDlg dialog

class CCanvasDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCanvasDlg)

public:
	CCanvasDlg(int nCanvasWidth,int nCanvasHeight, COLORREF colorBackground, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCanvasDlg();
	COLORREF GetBackgroundColor() { return m_colorBackground; }

// Dialog Data
	enum { IDD = IDD_CANVAS_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetSetting(BOOL flag);
	COLORREF m_colorBackground;

	DECLARE_MESSAGE_MAP()
public:
	int m_nCanvasWidth;
	int m_nCanvasHeight;
	CString m_csBckImagePath;
	afx_msg void OnBnClickedBackgroundColorButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeBckimagePath();
	afx_msg void OnBnClickedBckimageClear();
	afx_msg void OnBnClickedBckimageBrowser();
protected:
	CEdit CBackgroundColorEdit;
	CEdit CCanvasWidthEdit;
	CEdit CCanvasHeightEdit;
	CButton CBckColorButton;
};
