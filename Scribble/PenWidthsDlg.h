#pragma once
#include "afxcmn.h"


// CPenWidthsDlg dialog

class CPenWidthsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPenWidthsDlg)

public:
	CPenWidthsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPenWidthsDlg();

// Dialog Data
	enum { IDD = IDD_PEN_WIDTHS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog( );

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedDefaultPenWidth();
public:
	int m_nThinWidth;
	int m_nThickWidth;
	CSliderCtrl m_ctrlThinSlider;
	CSliderCtrl m_ctrlThickSlider;
	afx_msg void OnCustomdrawThinPenWidthSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawThickPenWidthSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
