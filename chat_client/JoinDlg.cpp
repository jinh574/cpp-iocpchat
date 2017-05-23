// JoinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chat_client.h"
#include "JoinDlg.h"
#include "afxdialogex.h"


// CJoinDlg dialog

IMPLEMENT_DYNAMIC(CJoinDlg, CDialogEx)

CJoinDlg::CJoinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJoinDlg::IDD, pParent)
{

	m_id = _T("");
	m_pw = _T("");
	m_code = _T("");
}

CJoinDlg::~CJoinDlg()
{
}

void CJoinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID_JOIN, m_id);
	DDX_Text(pDX, IDC_EDIT_PW_JOIN, m_pw);
	DDX_Text(pDX, IDC_EDIT_CODE_JOIN, m_code);
}


BEGIN_MESSAGE_MAP(CJoinDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CJoinDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CJoinDlg message handlers


void CJoinDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
