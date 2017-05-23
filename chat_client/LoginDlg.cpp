// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chat_client.h"
#include "LoginDlg.h"
#include "JoinDlg.h"
#include "afxdialogex.h"
#include "json/json.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{

	m_ip = _T("");
	m_nick = _T("");
	m_id = _T("");
	m_pw = _T("");
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_PW, m_pw);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTT_JOIN, &CLoginDlg::OnClickedButtJoin)
END_MESSAGE_MAP()


// CLoginDlg message handlers


void CLoginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialog::OnOK();
}


void CLoginDlg::OnClickedButtJoin()
{
	// TODO: Add your control notification handler code here
	CJoinDlg dlg;
	int retval;
	if(dlg.DoModal() == IDOK) {
		Json::Value root;
		root["type"] = MSG_JOIN;
		root["id"] = dlg.m_id.GetBuffer();
		root["pw"] = dlg.m_pw.GetBuffer();
		root["code"] = dlg.m_code.GetBuffer();

		Json::StyledWriter writer;
		retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
		if(retval == SOCKET_ERROR) {
			MessageBox("send() Fail", "Error", 0);
		}
	}
}


void CLoginDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnCancel();
	AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
}
