// PenWidthsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Scribble.h"
#include "PenWidthsDlg.h"
#include "afxdialogex.h"


// CPenWidthsDlg dialog

IMPLEMENT_DYNAMIC(CPenWidthsDlg, CDialogEx)

CPenWidthsDlg::CPenWidthsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPenWidthsDlg::IDD, pParent)
	, m_nThinWidth(0)
	, m_nThickWidth(0)
{	
}

CPenWidthsDlg::~CPenWidthsDlg()
{
}

BOOL CPenWidthsDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   // TODO: Add extra initialization here
   m_ctrlThinSlider.SetRange(1, 20, FALSE);
   m_ctrlThickSlider.SetRange(1, 20, FALSE);
   m_ctrlThinSlider.SetPos(m_nThinWidth);
   m_ctrlThickSlider.SetPos(m_nThickWidth);

   return TRUE;  // return TRUE unless you set the focus to a control 
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CPenWidthsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_THIN_PEN_WIDTH, m_nThinWidth);
	DDV_MinMaxInt(pDX, m_nThinWidth, 1, 20);
	DDX_Text(pDX, IDC_THICK_PEN_WIDTH, m_nThickWidth);
	DDV_MinMaxInt(pDX, m_nThickWidth, 1, 20);
	DDX_Control(pDX, IDC_THIN_PEN_WIDTH_SLIDER, m_ctrlThinSlider);
	DDX_Control(pDX, IDC_THICK_PEN_WIDTH_SLIDER, m_ctrlThickSlider);
}


BEGIN_MESSAGE_MAP(CPenWidthsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_DEFAULT_PEN_WIDTH, &CPenWidthsDlg::OnClickedDefaultPenWidth)	
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPenWidthsDlg message handlers


void CPenWidthsDlg::OnClickedDefaultPenWidth()
{
	// TODO: Add your control notification handler code here
	m_nThinWidth = 9;
	m_nThickWidth = 10;
	m_ctrlThinSlider.SetPos(m_nThinWidth);
	m_ctrlThickSlider.SetPos(m_nThickWidth);
	UpdateData(FALSE);
}


void CPenWidthsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	//CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);  

	// Check which slider sent the notification  
	if (pSlider == &m_ctrlThinSlider)  
	{  
		if (m_ctrlThinSlider.GetPos() <= m_nThickWidth)
		{
			m_nThinWidth = m_ctrlThinSlider.GetPos();
		}	
		else
		{
			m_nThinWidth = m_nThickWidth;
			m_ctrlThinSlider.SetPos(m_nThickWidth);			
		}
	}
	else if (pSlider == &m_ctrlThickSlider)  
	{  
		if (m_ctrlThickSlider.GetPos() >= m_nThinWidth)
		{
			m_nThickWidth = m_ctrlThickSlider.GetPos();
		}	
		else
		{
			m_nThickWidth = m_nThinWidth;
			m_ctrlThickSlider.SetPos(m_nThinWidth);
		}
	}
	UpdateData(FALSE);

	// Check what happened  
	switch(nSBCode)
	{
	case TB_LINEUP:  
	case TB_LINEDOWN:  
	case TB_PAGEUP:  
	case TB_PAGEDOWN:  
	case TB_THUMBPOSITION:  
	case TB_TOP:  
	case TB_BOTTOM:  
	case TB_THUMBTRACK:  
	case TB_ENDTRACK:  
	default:  
		break;  
	}
}
