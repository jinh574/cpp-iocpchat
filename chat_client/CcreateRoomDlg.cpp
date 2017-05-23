// CcreateRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chat_client.h"
#include "CcreateRoomDlg.h"
#include "afxdialogex.h"


// CcreateRoomDlg dialog

IMPLEMENT_DYNAMIC(CcreateRoomDlg, CDialogEx)

CcreateRoomDlg::CcreateRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcreateRoomDlg::IDD, pParent)
{

	m_roomName = _T("");
}

CcreateRoomDlg::~CcreateRoomDlg()
{
}

void CcreateRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROOMNAME, m_roomName);
}


BEGIN_MESSAGE_MAP(CcreateRoomDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CcreateRoomDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CcreateRoomDlg message handlers


void CcreateRoomDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
