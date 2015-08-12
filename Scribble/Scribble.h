
// Scribble.h : main header file for the Scribble application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CScribbleApp:
// See Scribble.cpp for the implementation of this class
//

class CScribbleApp : public CWinApp
{
public:
	CScribbleApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnPenColor();
	afx_msg void OnPenThickOrThin();
	afx_msg void OnUpdatePenThickOrThin(CCmdUI *pCmdUI);
	afx_msg void OnPenWidths();
	DECLARE_MESSAGE_MAP()

// Attributes
public:
	COLORREF m_colorCur;
	bool m_bThickPen;
	UINT m_nThinWidth;
	UINT m_nThickWidth;

protected:
	CRegKey m_regRegistry;
};

extern CScribbleApp theApp;
