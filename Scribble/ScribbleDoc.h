
// ScribbleDoc.h : interface of the CScribbleDoc class
//


#pragma once

class CStroke : public CObject
{
public:
	CStroke(UINT nPenWidth, COLORREF m_colorPen);

protected:
	CStroke();
	DECLARE_SERIAL(CStroke)
		
// Attributes
protected:
	UINT m_nPenWidth;
	COLORREF m_colorPen;

public:
	CArray<CPoint, CPoint> m_pointArray;

// Operations
public:
	BOOL DrawStroke(CDC* pDC);

public:
	virtual void Serialize(CArchive& ar);
};

class CScribbleDoc : public CDocument
{
protected: // create from serialization only
	CScribbleDoc();
	DECLARE_DYNCREATE(CScribbleDoc)

// Attributes
public:
	CTypedPtrList<CObList, CStroke*> m_strokeList;
	CPen*	GetCurrentPen();
	CSize GetDocSize() { return m_sizeDoc; }
	int GetCanvasWidth() { return m_nCanvasWidth; }
	int GetCanvasHeight() { return m_nCanvasHeight; }
	COLORREF GetBackgroundColor() { return m_colorBackground; }
	UINT m_nCanvasWidth;
	UINT m_nCanvasHeight;

	//Background Image function
	bool GetUseImageBck() { return m_bUseImageBck; };
	CImage m_imageBmp;

protected:
	UINT m_nPenWidth;
	bool* m_pbThickPen;
	COLORREF* m_pcolorCur;
	UINT* m_pnThinWidth;
	UINT* m_pnThickWidth;
	CPen m_penCur;
	CSize m_sizeDoc;
	COLORREF m_colorBackground;
	CRegKey m_regRegistry;

	//Background Image variable
	bool m_bUseImageBck;
	CString m_csBckImagePath;

// Operations
public:
	CStroke* NewStroke();

protected:
	void InitDocument();
	void ReplacePen();
	BOOL InitCanvas();
	void LoadRegValue(CString csKeyName, UINT &KeyValue);
	void LoadRegValue(CString csKeyName, DWORD &KeyValue);

	//Background Image function
	BOOL LoadBmpFile(CString csPathName);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
	virtual void DeleteContents();

// Implementation
public:
	virtual ~CScribbleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:	
	afx_msg void OnUpdatePenThickOrThin(CCmdUI *pCmdUI);
	afx_msg void OnPenThickOrThin();
	afx_msg void OnPenWidths();
	afx_msg void OnPenColor();
};
