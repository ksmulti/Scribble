
// ScribbleView.h : interface of the CScribbleView class
//

#pragma once


class CScribbleView : public CScrollView
{
protected: // create from serialization only
	CScribbleView();
	DECLARE_DYNCREATE(CScribbleView)

// Attributes
public:
	CScribbleDoc* GetDocument() const;

protected:
	CStroke* m_pStrokeCur;
	CPoint m_ptPrev;
	CRgn m_rgnCanvasRegion;

// Operations
public:

protected:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CScribbleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in ScribbleView.cpp
inline CScribbleDoc* CScribbleView::GetDocument() const
   { return reinterpret_cast<CScribbleDoc*>(m_pDocument); }
#endif

