// ChatRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chat_client.h"
#include "ChatRoomDlg.h"
#include "afxdialogex.h"

#define UM_LOGINAUTH		WM_USER + 1
#define UM_CHATROOM			WM_USER + 2
#define UM_closeChatRoom	WM_USER + 3
#define UM_sendChatRoom		WM_USER + 4

// CChatRoomDlg dialog

IMPLEMENT_DYNAMIC(CChatRoomDlg, CDialogEx)

CChatRoomDlg::CChatRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatRoomDlg::IDD, pParent)
{

	roomNo = 0;
	m_chatRoomMsg = _T("");
}

CChatRoomDlg::~CChatRoomDlg()
{
}

void CChatRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHAT_ROOM, m_chatRoomCtrl);
	DDX_Text(pDX, IDC_EDIT_MSG_ROOM, m_chatRoomMsg);
}


BEGIN_MESSAGE_MAP(CChatRoomDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CChatRoomDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CChatRoomDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChatRoomDlg message handlers


void CChatRoomDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnClose();
}


void CChatRoomDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	GetParentFrame()->GetActiveView()->SendMessage(UM_closeChatRoom,roomNo);
	CDialogEx::OnCancel();
}


void CChatRoomDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString msg;

	UpdateData(TRUE);
	msg = m_chatRoomMsg;
	m_chatRoomMsg = "";
	UpdateData(FALSE);
	GetParentFrame()->GetActiveView()->SendMessage(UM_sendChatRoom, roomNo, (LPARAM)msg.GetBuffer());


	//CDialogEx::OnOK();
}
