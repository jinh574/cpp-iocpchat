// GetIPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chat_client.h"
#include "GetIPDlg.h"
#include "afxdialogex.h"


// CGetIPDlg dialog

IMPLEMENT_DYNAMIC(CGetIPDlg, CDialogEx)

CGetIPDlg::CGetIPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGetIPDlg::IDD, pParent)
{

	m_ip = _T("");
	m_ipAddress = 0;
}

CGetIPDlg::~CGetIPDlg()
{
}

void CGetIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ipAddress);
}


BEGIN_MESSAGE_MAP(CGetIPDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGetIPDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGetIPDlg message handlers


void CGetIPDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
