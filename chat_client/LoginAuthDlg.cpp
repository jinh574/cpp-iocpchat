// LoginAuthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chat_client.h"
#include "LoginAuthDlg.h"
#include "afxdialogex.h"


// CLoginAuthDlg dialog

IMPLEMENT_DYNAMIC(CLoginAuthDlg, CDialogEx)

CLoginAuthDlg::CLoginAuthDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginAuthDlg::IDD, pParent)
{

	m_code = _T("");
}

CLoginAuthDlg::~CLoginAuthDlg()
{
}

void CLoginAuthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE_AUTH, m_code);
}


BEGIN_MESSAGE_MAP(CLoginAuthDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginAuthDlg::OnBnClickedOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CLoginAuthDlg message handlers


void CLoginAuthDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


//BOOL CLoginAuthDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDialogEx::PreTranslateMessage(pMsg);
//}


int CLoginAuthDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	SetFocus();

	return 0;
}


//BOOL CLoginAuthDlg::OnInitDialog()
//{
//	CDialogEx::OnInitDialog();
//
//	// TODO:  Add extra initialization here
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// EXCEPTION: OCX Property Pages should return FALSE
//}
