
// ScribbleDoc.cpp : implementation of the CScribbleDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Scribble.h"
#endif

#include "ScribbleDoc.h"
#include "PenWidthsDlg.h"
#include "CanvasDlg.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CScribbleDoc

IMPLEMENT_DYNCREATE(CScribbleDoc, CDocument)

BEGIN_MESSAGE_MAP(CScribbleDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_PEN_THICK_OR_THIN, &CScribbleDoc::OnUpdatePenThickOrThin)
	ON_COMMAND(ID_PEN_THICK_OR_THIN, &CScribbleDoc::OnPenThickOrThin)
	ON_COMMAND(ID_PEN_WIDTHS, &CScribbleDoc::OnPenWidths)
	ON_COMMAND(ID_PEN_COLOR, &CScribbleDoc::OnPenColor)
END_MESSAGE_MAP()


// CScribbleDoc construction/destruction

CScribbleDoc::CScribbleDoc()
{
	// TODO: add one-time construction code here
	m_pbThickPen = &((CScribbleApp*)AfxGetApp())->m_bThickPen;
	m_pcolorCur = &((CScribbleApp*)AfxGetApp())->m_colorCur;
	m_pnThinWidth = &((CScribbleApp*)AfxGetApp())->m_nThinWidth;
	m_pnThickWidth = &((CScribbleApp*)AfxGetApp())->m_nThickWidth;
	m_regRegistry.Create(HKEY_CURRENT_USER, _T("Software\\MyScribble"));
}

CScribbleDoc::~CScribbleDoc()
{
}

void CScribbleDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	while (!m_strokeList.IsEmpty())
	{
		delete m_strokeList.RemoveHead();
	}
	CDocument::DeleteContents();
}

BOOL CScribbleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	InitDocument();
	if (!InitCanvas())
		return FALSE;

	return TRUE;
}

BOOL CScribbleDoc::OnOpenDocument(LPCTSTR lpszPathName)
{	
	// Init Document before OnOpenDocument(serialization), and then serialization will recover some variables.(m_nThinWidth, m_nThickWidth, ...)
	InitDocument();
	if (!CDocument::OnOpenDocument(lpszPathName))
	{
		return FALSE;
	}
	if (m_bUseImageBck)
	{
		if (!LoadBmpFile(m_csBckImagePath))
			return FALSE;
	}
	
	return TRUE;
}


// CScribbleDoc serialization

void CScribbleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << m_bUseImageBck;
		if (!m_bUseImageBck)
		{
			ar << m_nCanvasWidth;
			ar << m_nCanvasHeight;
			ar << m_colorBackground;
		}
		else
			ar << m_csBckImagePath;
	}
	else
	{
		// TODO: add loading code here
		ar >> m_bUseImageBck;
		if (!m_bUseImageBck)
		{
			ar >> m_nCanvasWidth;
			ar >> m_nCanvasHeight;
			ar >> m_colorBackground;
		}
		else
			ar >> m_csBckImagePath;
	}
	m_strokeList.Serialize(ar);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CScribbleDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CScribbleDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CScribbleDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CScribbleDoc diagnostics

#ifdef _DEBUG
void CScribbleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScribbleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CScribbleDoc commands

void CScribbleDoc::InitDocument()
{
	m_nPenWidth = 2;
	m_penCur.CreatePen(PS_SOLID, m_nPenWidth, *m_pcolorCur);
	m_sizeDoc = CSize(800, 900);
}

BOOL CScribbleDoc::InitCanvas()
{
	// Initialize canvas values
	m_nCanvasWidth = 800;
	m_nCanvasHeight = 600;
	m_colorBackground = RGB(255, 255, 255);

	// Load canvas value from register if exist
	LoadRegValue(_T("CanvasWidth"), m_nCanvasWidth);
	LoadRegValue(_T("CanvasHeight"), m_nCanvasHeight);
	LoadRegValue(_T("BackgroundColor"), m_colorBackground);
	
	// Pop up canvas properties dialog
	CCanvasDlg dlgCanvas(m_nCanvasWidth, m_nCanvasHeight, m_colorBackground);
	INT_PTR dlgResult = dlgCanvas.DoModal();
	if (dlgResult == IDOK)
	{
		m_bUseImageBck = !dlgCanvas.m_csBckImagePath.IsEmpty();
		if (!m_bUseImageBck)
		{
			// Set canvas values and save to registry
			m_nCanvasWidth = dlgCanvas.m_nCanvasWidth;
			m_nCanvasHeight = dlgCanvas.m_nCanvasHeight;
			m_colorBackground = dlgCanvas.GetBackgroundColor();

			m_regRegistry.SetDWORDValue(_T("CanvasWidth"), m_nCanvasWidth);
			m_regRegistry.SetDWORDValue(_T("CanvasHeight"), m_nCanvasHeight);
			m_regRegistry.SetDWORDValue(_T("BackgroundColor"), m_colorBackground);
		}
		else
		{
			// Open bmp image file as background
			if(!LoadBmpFile(dlgCanvas.m_csBckImagePath))
				return FALSE;
		}
		return TRUE;
	}
	else 
		return FALSE;
}

void CScribbleDoc::LoadRegValue(CString csKeyName, UINT &KeyValue)
{
	DWORD Value;
	long result;
	result = m_regRegistry.QueryDWORDValue(csKeyName, Value);
	if (result == ERROR_SUCCESS)
		KeyValue = Value;
}

void CScribbleDoc::LoadRegValue(CString csKeyName, DWORD &KeyValue)
{
	DWORD Value;
	long result;
	result = m_regRegistry.QueryDWORDValue(csKeyName, Value);
	if (result == ERROR_SUCCESS)
		KeyValue = Value;
}

BOOL CScribbleDoc::LoadBmpFile(CString csPathName)
{
	m_csBckImagePath = csPathName;
	HRESULT hBmp;
	hBmp = m_imageBmp.Load(m_csBckImagePath);
	if (hBmp != S_OK)
	{
		MessageBox(NULL, _T("Open Image File Error\n" + m_csBckImagePath + " is not exist"), _T("Error"), MB_ICONERROR | MB_OK);
		return FALSE;
	}
	m_nCanvasWidth = m_imageBmp.GetWidth();
	m_nCanvasHeight = m_imageBmp.GetHeight();
	return TRUE;
}

CStroke* CScribbleDoc::NewStroke()
{
	ReplacePen();
	CStroke* pStrokeItem = new CStroke(m_nPenWidth, *m_pcolorCur);
	m_strokeList.AddTail(pStrokeItem);
	SetModifiedFlag();

	return pStrokeItem;
}

void CScribbleDoc::OnUpdatePenThickOrThin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(*m_pbThickPen);
}


void CScribbleDoc::OnPenThickOrThin()
{
	// TODO: Add your command handler code here
	*m_pbThickPen = !*m_pbThickPen;
	ReplacePen();
}


void CScribbleDoc::ReplacePen()
{
	if (*m_pbThickPen)
		m_nPenWidth = *m_pnThickWidth;
	else
		m_nPenWidth = *m_pnThinWidth;

	m_penCur.DeleteObject();
	m_penCur.CreatePen(PS_SOLID, m_nPenWidth, *m_pcolorCur);
}


//////////////////////////////////////
//CStroke

IMPLEMENT_SERIAL(CStroke, CObject, 1)
CStroke::CStroke()
{

}

CStroke::CStroke(UINT nPenWidth, COLORREF colorPen)
{
	m_nPenWidth = nPenWidth;
	m_colorPen = colorPen;
}

void CStroke::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nPenWidth;
		ar << m_colorPen;
		m_pointArray.Serialize(ar);
	}
	else
	{
		ar >> m_nPenWidth;
		ar >> m_colorPen;
		m_pointArray.Serialize(ar);
	}
}

BOOL CStroke:: DrawStroke(CDC* pDC)
{
	CPen penStroke;
	if (!penStroke.CreatePen(PS_SOLID, m_nPenWidth, m_colorPen))
		return FALSE;
	CPen* pOldPen = pDC->SelectObject(&penStroke);
	pDC->MoveTo(m_pointArray[0]);
	for (int i=1; i < m_pointArray.GetSize(); i++)
	{
		pDC->LineTo(m_pointArray[i]);
	}

	pDC->SelectObject(pOldPen);
	return TRUE;
}

void CScribbleDoc::OnPenWidths()
{
	// TODO: Add your command handler code here
	CPenWidthsDlg dlg;
	dlg.m_nThinWidth = *m_pnThinWidth;
	dlg.m_nThickWidth = *m_pnThickWidth;

	if (dlg.DoModal() == IDOK)
	{
		*m_pnThinWidth = dlg.m_nThinWidth;
		*m_pnThickWidth = dlg.m_nThickWidth;
		ReplacePen();

		m_regRegistry.SetDWORDValue(_T("ThinPenWidth"), *m_pnThinWidth);
		m_regRegistry.SetDWORDValue(_T("ThickPenWidth"), *m_pnThickWidth);
	}
}

void CScribbleDoc::OnPenColor()
{
	// TODO: Add your command handler code here
	CMFCColorDialog colordlg(*m_pcolorCur);
	if (colordlg.DoModal() == IDOK)
	{
		*m_pcolorCur = colordlg.GetColor();
		ReplacePen();
	}
}

CPen* CScribbleDoc::GetCurrentPen()
{
	//Update pen to current properties
	ReplacePen();
	return &m_penCur;
}
