
// ScribbleView.cpp : implementation of the CScribbleView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Scribble.h"
#endif

#include "ScribbleDoc.h"
#include "ScribbleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScribbleView

IMPLEMENT_DYNCREATE(CScribbleView, CScrollView)

BEGIN_MESSAGE_MAP(CScribbleView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CScribbleView construction/destruction

CScribbleView::CScribbleView()
{
	// TODO: add construction code here
}

CScribbleView::~CScribbleView()
{
}

BOOL CScribbleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CScribbleView drawing

void CScribbleView::OnDraw(CDC* pDC)
{
	CScribbleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	//Double Buffer
	CMemDC MemDC(*pDC, this);
	CDC& dc = MemDC.GetDC();
	CRect rect;
	GetClientRect(&rect);
	rect.right += this->GetScrollPos(SB_HORZ);
	rect.bottom += this->GetScrollPos(SB_VERT);
	//Fill Background
	dc.FillSolidRect(&rect, RGB(204, 214, 229));
	dc.SelectClipRgn(&m_rgnCanvasRegion);
	//Fill Canvas Background
	if (!GetDocument()->GetUseImageBck())
	{
		//Use a color
		CBrush backbrush(GetDocument()->GetBackgroundColor());
		dc.FillRgn(&m_rgnCanvasRegion, &backbrush);
	}
	else
	{
		//Use bmp image
		GetDocument()->m_imageBmp.BitBlt(dc.GetSafeHdc(), 0, 0, SRCCOPY);
	}
	//Draw lines
	CTypedPtrList<CObList, CStroke*>& strokeList = pDoc->m_strokeList;
	POSITION pos = strokeList.GetHeadPosition();
	while(pos != NULL)
	{
		CStroke* pStroke = strokeList.GetNext(pos);
		pStroke -> DrawStroke(&dc);
	}
}


// CScribbleView printing

BOOL CScribbleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CScribbleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CScribbleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CScribbleView diagnostics

#ifdef _DEBUG
void CScribbleView::AssertValid() const
{
	CView::AssertValid();
}

void CScribbleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScribbleDoc* CScribbleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScribbleDoc)));
	return (CScribbleDoc*)m_pDocument;
}
#endif //_DEBUG


// CScribbleView message handlers


void CScribbleView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default	

	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	m_pStrokeCur = GetDocument()->NewStroke();
	m_pStrokeCur->m_pointArray.Add(point);

	SetCapture();
	m_ptPrev = point;
	return;
}


void CScribbleView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default	

	if (GetCapture() != this)
		return;

	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	dc.SelectClipRgn(&m_rgnCanvasRegion);

	CPen* pOldPen = dc.SelectObject(GetDocument()->GetCurrentPen());
	m_pStrokeCur->m_pointArray.Add(point);
	dc.MoveTo(m_ptPrev);
	dc.LineTo(point);
	dc.SelectObject(pOldPen);
	m_ptPrev = point;
	return;
}


void CScribbleView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default	

	if (GetCapture() != this)
		return;

	CScribbleDoc* pDoc = GetDocument();

	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	dc.SelectClipRgn(&m_rgnCanvasRegion);

	CPen* pOldPen = dc.SelectObject(pDoc->GetCurrentPen());
	m_pStrokeCur->m_pointArray.Add(point);
	dc.MoveTo(m_ptPrev);
	dc.LineTo(point);
	dc.SelectObject(pOldPen);

	ReleaseCapture();
	return;
}


void CScribbleView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	SIZE size;
	size.cx = GetDocument()->GetCanvasWidth();
	size.cy = GetDocument()->GetCanvasHeight();
	SetScrollSizes(MM_TEXT, size);
	m_rgnCanvasRegion.CreateRectRgn(0, 0, GetDocument()->GetCanvasWidth(), GetDocument()->GetCanvasHeight());
}


BOOL CScribbleView::OnEraseBkgnd(CDC* pDC)
{	
	// TODO: Add your message handler code here and/or call default	
	return TRUE;
}
