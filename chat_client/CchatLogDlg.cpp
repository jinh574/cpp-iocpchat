// CchatLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chat_client.h"
#include "CchatLogDlg.h"
#include "afxdialogex.h"


// CchatLogDlg dialog

IMPLEMENT_DYNAMIC(CchatLogDlg, CDialogEx)

CchatLogDlg::CchatLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CchatLogDlg::IDD, pParent)
{

	roomNo = 0;
}

CchatLogDlg::~CchatLogDlg()
{
}

void CchatLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHAT_LOG_LIST, m_chatLogCtrl);
	DDX_Control(pDX, IDC_LIST_CHAT_LOG_ROOM, m_chatLogdetailCtrl);
}


BEGIN_MESSAGE_MAP(CchatLogDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CHAT_LOG_LIST, &CchatLogDlg::OnClickListChatLogList)
//	ON_NOTIFY(LVN_BEGINSCROLL, IDC_LIST_CHAT_LOG_LIST, &CchatLogDlg::OnBeginscrollListChatLogList)
	ON_LBN_DBLCLK(IDC_LIST_CHAT_LOG_ROOM, &CchatLogDlg::OnDblclkListChatLogRoom)
END_MESSAGE_MAP()


// CchatLogDlg message handlers


BOOL CchatLogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_chatLogCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LVCOLUMN LC;

	LC.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	LC.fmt = LVCFMT_LEFT;
	LC.iSubItem = 0;
	LC.pszText = "No.";
	LC.cx = 0;
	m_chatLogCtrl.InsertColumn(0, &LC);

	LC.iSubItem = 1;
	LC.pszText = "방제목";
	LC.cx = 110;
	m_chatLogCtrl.InsertColumn(1, &LC);

	LC.iSubItem = 2;
	LC.pszText = "시간";
	LC.cx = 100;
	m_chatLogCtrl.InsertColumn(2, &LC);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CchatLogDlg::OnClickListChatLogList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int row = m_chatLogCtrl.GetSelectionMark();
    if(row < 0)
        return;

	roomNo = atoi(m_chatLogCtrl.GetItemText(row, 0));
	GetParentFrame()->GetActiveView()->SendMessage(WM_USER + 8, roomNo);
}


void CchatLogDlg::OnDblclkListChatLogRoom()
{
	// TODO: Add your control notification handler code here
	CString tmp;
	m_chatLogdetailCtrl.GetText(m_chatLogdetailCtrl.GetCurSel(),tmp);
	if(!tmp.Compare("더보기")) {
		GetParentFrame()->GetActiveView()->SendMessage(WM_USER + 9, roomNo, m_chatLogdetailCtrl.GetCount()-1);
	}
}
