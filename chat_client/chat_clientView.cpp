
// chat_clientView.cpp : Cchat_clientView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "chat_client.h"
#endif

#pragma comment(lib, "ws2_32")
#pragma comment(lib,"mswsock")  // AcceptEx()
#pragma comment(lib, "json_vc71_libmtd")
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <WinSock2.h>
#include <MSWSock.h>

#include "LoginDlg.h"
#include "LoginAuthDlg.h"
#include "CcreateRoomDlg.h"
#include "GetIPDlg.h"
#include "CchatLogDlg.h"
#include "ChatRoomDlg.h"
#include "chat_clientDoc.h"
#include <vector>
#include "chat_clientView.h"
#include <json/json.h>

#define UM_LOGINAUTH			WM_USER + 1
#define UM_CHATROOM				WM_USER + 2
#define UM_closeChatRoom		WM_USER + 3
#define UM_sendChatRoom			WM_USER + 4
#define UM_closeProgram			WM_USER	+ 5
#define	UM_LoginLog				WM_USER + 6
#define	UM_CHATLOG				WM_USER + 7
#define UM_getChatLogDetail		WM_USER + 8
#define UM_getChatLogDetailMore	WM_USER + 9

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT recvThreadCallback(LPVOID arg);
int recvn(SOCKET s, char *buf, int len, int flags);
std::vector<CChatRoomDlg *> roomDlg_list;
CchatLogDlg *chatLogDlg;

// Cchat_clientView

IMPLEMENT_DYNCREATE(Cchat_clientView, CFormView)

BEGIN_MESSAGE_MAP(Cchat_clientView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTT_SEND, &Cchat_clientView::OnClickedButtSend)
	ON_WM_CREATE()
	ON_MESSAGE(UM_LOGINAUTH, &Cchat_clientView::OnUmLoginauth)
	ON_MESSAGE(UM_CHATROOM, &Cchat_clientView::OnUmChatroom)
	ON_BN_CLICKED(IDC_BUTT_CREATE_ROOM, &Cchat_clientView::OnClickedButtCreateRoom)
	ON_MESSAGE(UM_closeChatRoom, &Cchat_clientView::OnUmClosechatroom)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ROOM, &Cchat_clientView::OnDblclkListRoom)
	ON_MESSAGE(UM_sendChatRoom, &Cchat_clientView::OnUmSendchatroom)
	ON_LBN_DBLCLK(IDC_LIST_USER, &Cchat_clientView::OnDblclkListUser)
	ON_WM_DESTROY()
//	ON_WM_CLOSE()
	ON_WM_CLOSE()
	ON_MESSAGE(UM_closeProgram, &Cchat_clientView::OnCloseprogram)
	ON_COMMAND(ID_MENU_LOGIN_LOG, &Cchat_clientView::OnMenuLoginLog)
	ON_MESSAGE(UM_LoginLog, &Cchat_clientView::OnUmLoginlog)
	ON_COMMAND(ID_CHAT_LOG_LIST, &Cchat_clientView::OnChatLogList)
	ON_MESSAGE(UM_CHATLOG, &Cchat_clientView::OnUmChatlog)
	ON_MESSAGE(UM_getChatLogDetail, &Cchat_clientView::OnUmGetchatlogdetail)
	ON_MESSAGE(UM_getChatLogDetailMore, &Cchat_clientView::OnUmGetchatlogdetailmore)
END_MESSAGE_MAP()

// Cchat_clientView 생성/소멸

Cchat_clientView::Cchat_clientView()
	: CFormView(Cchat_clientView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.

	m_strData = _T("");
	//  m_ip = _T("");
	//  m_nick = _T("");
	serialNum = 0;
	m_id = _T("");
}

Cchat_clientView::~Cchat_clientView()
{
}

void Cchat_clientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_listUser);
	DDX_Control(pDX, IDC_LIST_CHAT, m_listChat);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strData);
	DDX_Control(pDX, IDC_LIST_ROOM, m_roomCtrl);
}

BOOL Cchat_clientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	return CFormView::PreCreateWindow(cs);
}

void Cchat_clientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_roomCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LVCOLUMN LC;

	LC.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	LC.fmt = LVCFMT_LEFT;
	LC.iSubItem = 0;
	LC.pszText = "No.";
	LC.cx = 60;
	m_roomCtrl.InsertColumn(0, &LC);

	LC.iSubItem = 1;
	LC.pszText = "방제목";
	LC.cx = 300;
	m_roomCtrl.InsertColumn(1, &LC);

	LC.iSubItem = 2;
	LC.pszText = "인원";
	LC.cx = 60;
	m_roomCtrl.InsertColumn(2, &LC);
}

void Cchat_clientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cchat_clientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cchat_clientView 진단

#ifdef _DEBUG
void Cchat_clientView::AssertValid() const
{
	CFormView::AssertValid();
}

void Cchat_clientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

Cchat_clientDoc* Cchat_clientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cchat_clientDoc)));
	return (Cchat_clientDoc*)m_pDocument;
}
#endif //_DEBUG


// Cchat_clientView 메시지 처리기


BOOL Cchat_clientView::clientInit(void) {
	int retval;
	CString ip;
	CGetIPDlg dlg;
	SOCKADDR_IN addr;
	if(dlg.DoModal() == IDOK) {
		addr.sin_addr.s_addr = (long)dlg.m_ipAddress;
		ip.Format(_T("%d.%d.%d.%d"), FIRST_IPADDRESS(dlg.m_ipAddress), SECOND_IPADDRESS(dlg.m_ipAddress), THIRD_IPADDRESS(dlg.m_ipAddress), FOURTH_IPADDRESS(dlg.m_ipAddress));	
	}
	//윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		//printChat("Winsock Init Fail");
		MessageBox("Winsock Init Fail","Error", 0);
	}
	//printChat("Winsock Init Success");

	//socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) MessageBox("socket() Fail","Error", 0);//printChat("socket() Error");
	//printChat("Sock Create Success");

	//cononet()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) {
		endClient("서버에 응답이없습니다.");
		return FALSE;
	}
	//printChat("Sock Connect Service Success");


	HANDLE hThread = AfxBeginThread(recvThreadCallback, this);
	if(hThread == NULL) {
		MessageBox("Create WorkerThread Fail", NULL, NULL);
	}
	//CloseHandle(hThread);

	return TRUE;
}


UINT recvThreadCallback(LPVOID arg) {
	Cchat_clientView *thread = (Cchat_clientView *) arg;
	thread->recvThread();

	return TRUE;
}

void Cchat_clientView::recvThread(void) {
	int retval, type, roomNo;
	char buf[BUFSIZE+1];
	CString tmp, roomName, id, data;
	Json::Value root, jsonData;
	Json::Reader reader;
	Json::StyledWriter writer;
	BOOL success;

	while(1) {
		//데이터 받기=
		ZeroMemory(buf, BUFSIZE);
		retval = recv(sock, buf, BUFSIZE, 0);
		//MessageBox(buf);
		//printf(buf);
		if(retval == SOCKET_ERROR) {
			endClient("서버와 연결이 끊어졌습니다.");
			break;
		}
		else if(retval == 0)
			break;
		//받은 데이터 출력
		buf[retval] = '\0';
		bool parsingSuccessful = reader.parse(buf,root);
		if ( !parsingSuccessful ) {
			std::cout  << "Failed to parse configuration\n"
				<< reader.getFormatedErrorMessages();
		}

		if(root.isNull()) {
			type = -1;
		}
		else {
			type = root.get("type","").asInt();
		}
		switch(type) {
		case MSG_JOIN_ACK:
				success = root.get("success","").asBool();
				if(success) {
					MessageBox("회원가입 성공", "SYSTEM", 0);
				}
				else {
					MessageBox("중복된 아이디이거나 회원가입을 실패하였습니다.", "Error", 0);
				}
				break;
		case MSG_LOGIN_ACK:
				success = root.get("success","").asBool();
				if(!success) {
					endClient("아이디 또는 비밀번호가 맞지 않거나 이미 로그인 중입니다.");
				}
				break;
		case MSG_LOGIN_AUTH:
				success = root.get("success","").asBool();
				serialNum = root.get("serialNum","").asUInt();
				if(!success) {
					SendMessage(UM_LOGINAUTH);
				}
				else {
					root.clear();
					root["type"] = MSG_CONN;
					retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
					if(retval == SOCKET_ERROR) {
						endClient("서버와의 연결이 끊겼습니다.");
					}
				}
				break;
		case MSG_LOGIN_AUTH_ACK:
				success = root.get("success","").asBool();
			
				if(!success) {
					endClient("인증번호가 유효하지 않습니다.");
				}
				else {
					root.clear();
					root["type"] = MSG_CONN;
					retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
					if(retval == SOCKET_ERROR) {
						endClient("서버와의 연결이 끊겼습니다.");
					}
				}
				break;
		case MSG_USER_LIST:
				if(!((CListBox *)GetDlgItem(IDC_LIST_USER)))
					break;

				((CListBox *)GetDlgItem(IDC_LIST_USER))->ResetContent();
				for(int i=0; i<root["user_list"].size(); i++) {
					((CListBox *)GetDlgItem(IDC_LIST_USER))->AddString(root["user_list"][i].asCString());
				}
				break;
		case MSG_BROADCAST_LOBBY:
				if(!((CListBox *)GetDlgItem(IDC_LIST_CHAT)))
					break;

				tmp.Format("%s : %s",root.get("id", "").asCString(), root.get("data", "").asCString());
				((CListBox *)GetDlgItem(IDC_LIST_CHAT))->AddString(tmp);
				((CListBox *)GetDlgItem(IDC_LIST_CHAT))->SetCurSel(((CListBox *)GetDlgItem(IDC_LIST_CHAT))->GetCount()-1);
				break;
		case MSG_CREATEROOM_ACK:
				success = root.get("success","").asBool();
				roomNo = root.get("roomNo","").asInt();
				if(success) {
					root.clear();
					root["type"] = MSG_ENTERROOM;
					root["roomNo"] = roomNo;
					retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
					if(retval == SOCKET_ERROR) {
						endClient("서버와의 연결이 끊겼습니다.");
					}
				}
				break;
		case MSG_ENTERROOM_ACK:
			success = root.get("success","").asBool();
			roomNo = root.get("roomNo","").asUInt();
			roomName = root.get("roomName","").asCString();
			if(success) {
				SendMessage(UM_CHATROOM, roomNo, (LPARAM)roomName.GetBuffer());
				root.clear();
				root["type"] = MSG_CHATROOM_COMP;
				root["roomNo"] = roomNo;
				root["success"] = true;
				retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
				if(retval == SOCKET_ERROR) {
					endClient("서버와의 연결이 끊겼습니다.");
				}
			}
			else {
				MessageBox("방에 들어가기를 실패 했습니다.","Error",0);
			}
			break;
		case MSG_CHATROOM_LIST:
			if(!m_roomCtrl)
				break;
			m_roomCtrl.DeleteAllItems();
			for(int i=0; i<root["roomNo"].size(); i++) {
				updateRoomList(root["roomNo"][i].asInt(),root["roomName"][i].asCString(),root["userCount"][i].asInt());
			}
			break;
		case MSG_CHATROOM_BROADCAST:
			roomNo = root.get("roomNo","").asInt();
			tmp = root.get("data","").asCString();

			for(int i=0; i<roomDlg_list.size(); i++) {
				if(roomDlg_list[i]->roomNo == roomNo) {
					roomDlg_list[i]->m_chatRoomCtrl.AddString(tmp);
					break;
				}
			}
			break;
		case MSG_WHISPER_ACK:
			data = root.get("data","").asCString();
			id = root.get("from","").asCString();
			id.Format("%s님에게 온 귓속말",id);
			MessageBox(data, id, 0);
			break;
		case MSG_LOGIN_LOG_ACK:
			SendMessage(UM_LoginLog, (WPARAM)buf);
			break;
		case MSG_CHAT_LOG_ACK:
			tmp.Format("%s",buf);
			updateChatLog(tmp);
			break;
		case MSG_CHAT_LOG_ROOM_ACK:
			tmp.Format("%s",buf);
			updateChatLogDetail(tmp);
			break;
		case MSG_CHAT_LOG_ROOM_MORE_ACK:
			tmp.Format("%s",buf);
			updateChatLogDetailMore(tmp);
			break;
		case MSG_CHAT_LOG_MORE_ACK:
			tmp.Format("%s",buf);
			updateChatLogMore(tmp);
			break;
		case MSG_ALL_DISCONN:
			endClient("서버가 종료 되었습니다. 클라이언트를 종료합니다.");
			break;
		default:
			break;
		}
	}
}

int recvn(SOCKET s, char *buf, int len, int flags) {
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0) {
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

void Cchat_clientView::printChat(CString str) {
	m_listChat.AddString(str);
	m_listChat.SetCurSel(m_listChat.GetCount()-1);
}


void Cchat_clientView::OnClickedButtSend() {
	// TODO: Add your control notification handler code here
	CString msg;
	DWORD retval;

	UpdateData(TRUE);
	msg = m_strData;
	m_strData = "";
	UpdateData(FALSE);

	Json::Value root;
	root["type"] = MSG_MESSAGE_LOBBY;
	root["data"] = msg.GetString();

	Json::StyledWriter writer;

	//데이터 보내기
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}
}


BOOL Cchat_clientView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_RETURN ) {
			UpdateData(TRUE);
			if(m_strData.GetLength()) {
				OnClickedButtSend();
			}
			UpdateData(FALSE);
			return FALSE;
		}
	} 
	return CFormView::PreTranslateMessage(pMsg);
}


int Cchat_clientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	clientInit();
	CLoginDlg dlg;
	DWORD retval;

	dlg.sock = this->sock;
	
	if(dlg.DoModal() == IDOK) {
		Json::Value root;
		root["type"] = MSG_LOGIN;
		root["id"] = dlg.m_id.GetBuffer();
		root["pw"] = dlg.m_pw.GetBuffer();
		m_id = dlg.m_id;
		Json::StyledWriter writer;
		retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
		if(retval == SOCKET_ERROR) {
			endClient("서버와의 연결이 끊겼습니다.");
		}
	}
	return 0;
}


afx_msg LRESULT Cchat_clientView::OnUmLoginauth(WPARAM wParam, LPARAM lParam)
{
	CLoginAuthDlg dlg;
	int retval;

	if(dlg.DoModal() == IDOK) {
		Json::Value root;
		root["type"] = MSG_LOGIN_AUTH_CODE;
		root["code"] = dlg.m_code.GetBuffer();
		root["serialNum"] = (int)serialNum;

		Json::StyledWriter writer;
		retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
		if(retval == SOCKET_ERROR) {
			endClient("서버와의 연결이 끊겼습니다.");
		}
	}
	else {
		AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
	}
	
	return 0;
}


void Cchat_clientView::OnClickedButtCreateRoom()
{
	// TODO: Add your control notification handler code here
	CcreateRoomDlg dlg;
	CString msg;
	DWORD retval;
	Json::Value root;
	Json::StyledWriter writer;

	if(dlg.DoModal() == IDOK) {
		root["type"] = MSG_CREATEROOM;
		root["name"] = dlg.m_roomName.GetBuffer();
	}
	else {
		return;
	}

	//데이터 보내기
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}
}


afx_msg LRESULT Cchat_clientView::OnUmChatroom(WPARAM wParam, LPARAM lParam)
{
	CString name = (LPCSTR) lParam;
	name.Format("[%d번방]%s",(int)wParam, name);
	CChatRoomDlg *chatDlg = new CChatRoomDlg();
	roomDlg_list.push_back(chatDlg);
	chatDlg->Create(IDD_CHATROOMDLG);
	chatDlg->SetWindowTextA(name);
	chatDlg->ShowWindow(SW_SHOW);
	chatDlg->roomNo = (int)wParam;

	return 0;
}


BOOL Cchat_clientView::updateRoomList(int roomNo, CString roomName, int roomCount)	{
	LVITEM LI;
	LI.mask = LVIF_TEXT;
	LI.iItem = 0;
	LI.iSubItem = 0;

	CString no, count;
	no.Format("%d",roomNo);
	count.Format("%d",roomCount);

	if(m_roomCtrl.GetItemCount()==0) {
		LI.pszText = no.GetBuffer();
		m_roomCtrl.InsertItem(&LI);
		m_roomCtrl.SetItemText(LI.iItem, 1, roomName.GetBuffer());
		m_roomCtrl.SetItemText(LI.iItem, 2, count.GetBuffer());
	}
	else {
		LI.iItem = m_roomCtrl.GetItemCount();
		LI.pszText = no.GetBuffer();
		m_roomCtrl.InsertItem(&LI);
		m_roomCtrl.SetItemText(LI.iItem, 1, roomName);
		m_roomCtrl.SetItemText(LI.iItem, 2, count.GetBuffer());
	}
	
	return true;
}


afx_msg LRESULT Cchat_clientView::OnUmClosechatroom(WPARAM wParam, LPARAM lParam)
{
	int retval, iter;
	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = MSG_CHATROOM_DISCONN;
	root["roomNo"] = (int)wParam;
	
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}

	if(roomDlg_list.empty())
		return 0;
	for(int i=0; i<roomDlg_list.size(); i++) {
		if(roomDlg_list[i]->roomNo == (int) wParam) {
			iter = i;
			break;
		}
	}
	roomDlg_list.erase(roomDlg_list.begin()+iter);

	return 0;
}


void Cchat_clientView::OnDblclkListRoom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int retval;
	Json::Value root;
	Json::StyledWriter writer;
	int row = m_roomCtrl.GetSelectionMark();
    if(row < 0)
        return;

    DWORD roomNo = atoi(m_roomCtrl.GetItemText(row, 0));
	root["type"] = MSG_ENTERROOM;
	root["roomNo"] = (int)roomNo;
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}
	
}


afx_msg LRESULT Cchat_clientView::OnUmSendchatroom(WPARAM wParam, LPARAM lParam)
{
	DWORD retval;
	Json::Value root;
	Json::StyledWriter writer;
	CString msg = (LPCSTR)lParam;
	DWORD no = (int)wParam;

	root["type"] = MSG_CHATROOM_MESSAGE;
	root["roomNo"] = (int)no;
	root["data"] = msg.GetBuffer();
	root["serialNum"] = (int)serialNum;
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}
	
	return 0;
}


void Cchat_clientView::OnDblclkListUser()
{
	// TODO: Add your control notification handler code here
	CString id, msg;
	Json::Value root;
	Json::StyledWriter writer;
	DWORD retval;

	UpdateData(TRUE);
	msg = m_strData;
	m_strData = "";
	UpdateData(FALSE);
	m_listUser.GetText(m_listUser.GetCurSel(),id);
	if(!msg.GetLength()) {
		MessageBox("내용을 입력해주세요.","알림", 0);
		return;
	}
	if(!strcmp(id.GetBuffer(), m_id.GetBuffer())) {
		MessageBox("자기 자신에게 보낼 수는 없습니다.", "알림", 0);
		return;
	}

	root.clear();
	root["type"] = MSG_WHISPER;
	root["to"] = id.GetBuffer();
	root["data"] = msg.GetBuffer();
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}
	
}

void Cchat_clientView::OnDestroy()
{
	CFormView::OnDestroy();
	// TODO: Add your message handler code here
}


void Cchat_clientView::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CFormView::OnClose();
}


BOOL Cchat_clientView::checkDlg(void) {
	if(roomDlg_list.empty()) {
		return false;
	}
	else {
		return true;
	}
}


afx_msg LRESULT Cchat_clientView::OnCloseprogram(WPARAM wParam, LPARAM lParam) {
	if(roomDlg_list.empty()) {
		return 1;
	}
	else {
		MessageBox("모든 채팅창을 끄고 종료해주세요.");
		return 0;
	}
}


void Cchat_clientView::OnMenuLoginLog()
{
	// TODO: Add your command handler code here
	Json::Value root;
	Json::StyledWriter writer;
	DWORD retval;

	root["type"] = MSG_LOGIN_LOG;
	root["serialNum"] = (int)serialNum;
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}
}


afx_msg LRESULT Cchat_clientView::OnUmLoginlog(WPARAM wParam, LPARAM lParam) {
	Json::Value root;
	Json::Value data;
	Json::Reader reader;
	CString json = (LPCSTR)wParam, result, tmp, timestamp, ip, status;
	bool parsingSuccessful = reader.parse(json.GetBuffer(),root);
	if ( !parsingSuccessful ) {
		std::cout  << "Failed to parse configuration\n"
			<< reader.getFormatedErrorMessages();
	}

	result.Format("시간\t\t\t아이피\t\t상태\n");
	for(int i=0; i<root["data"].size(); i++) {
		data = root["data"];
		timestamp = data[i]["timestamp"].asCString();
		ip = data[i]["ip"].asCString();
		status = data[i]["status"].asCString();
		if(!strcmp(status.GetBuffer(),"1")) {
			status = "성공";
		}
		else {
			status = "실패";
		}
		result.Format("%s%s\t%s\t\t%s\n", result, timestamp, ip, status);
	}
	MessageBox(result);

	return 0;
}


void Cchat_clientView::OnChatLogList()
{
	// TODO: Add your command handler code here
	SendMessage(UM_CHATLOG);
}


BOOL Cchat_clientView::updateChatLog(CString json) {
	LVITEM LI;
	LI.mask = LVIF_TEXT;
	LI.iItem = 0;
	LI.iSubItem = 0;
	CString tmp;
	Json::Value root, data;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(json.GetBuffer(),root);
	if ( !parsingSuccessful ) {
		std::cout  << "Failed to parse configuration\n"
			<< reader.getFormatedErrorMessages();
	}
	data = root["data"];
	for(int i=data.size()-1; i>=0; i--) {
		if(m_roomCtrl.GetItemCount()==0) {
			tmp = data[i]["room"].asCString();
			LI.pszText = tmp.GetBuffer();
			chatLogDlg->m_chatLogCtrl.InsertItem(&LI);
			
			tmp = data[i]["name"].asCString();
			chatLogDlg->m_chatLogCtrl.SetItemText(LI.iItem, 1, tmp.GetBuffer());
						
			tmp = data[i]["timestamp"].asCString();
			chatLogDlg->m_chatLogCtrl.SetItemText(LI.iItem, 2, tmp.GetBuffer());
		}
		else {
			LI.iItem = chatLogDlg->m_chatLogCtrl.GetItemCount();

			tmp = data[i]["room"].asCString();
			LI.pszText = tmp.GetBuffer();
			chatLogDlg->m_chatLogCtrl.InsertItem(&LI);

			tmp = data[i]["name"].asCString();
			chatLogDlg->m_chatLogCtrl.SetItemText(LI.iItem, 1, tmp.GetBuffer());
						
			tmp = data[i]["timestamp"].asCString();
			chatLogDlg->m_chatLogCtrl.SetItemText(LI.iItem, 2, tmp.GetBuffer());
		}
	}
	LI.iItem = chatLogDlg->m_chatLogCtrl.GetItemCount();
	tmp = "-1";
	LI.pszText = tmp.GetBuffer();
	chatLogDlg->m_chatLogCtrl.InsertItem(&LI);

	tmp = "더보기";
	chatLogDlg->m_chatLogCtrl.SetItemText(LI.iItem, 1, tmp.GetBuffer());

	return true;
}


afx_msg LRESULT Cchat_clientView::OnUmChatlog(WPARAM wParam, LPARAM lParam) {
	Json::Value root;
	Json::StyledWriter writer;
	int retval;
	if(chatLogDlg!=NULL) {
		chatLogDlg->m_chatLogCtrl.DeleteAllItems();
		chatLogDlg->ShowWindow(SW_SHOW);
	}
	else {
		chatLogDlg = new CchatLogDlg();
		chatLogDlg->Create(IDD_CCHATLOGDLG);
		chatLogDlg->ShowWindow(SW_SHOW);
	}
	root["type"] = MSG_CHAT_LOG;
	root["serialNum"] = (int)serialNum;
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}

	return 0;
}


afx_msg LRESULT Cchat_clientView::OnUmGetchatlogdetail(WPARAM wParam, LPARAM lParam) {
	DWORD roomNo = (int)wParam;
	Json::Value root;
	Json::StyledWriter writer;
	int retval;
	if((int)roomNo != -1) {
		root["type"] = MSG_CHAT_LOG_ROOM;
		root["roomNo"] = (int)roomNo;
	}
	else {
		root["type"] = MSG_CHAT_LOG_MORE;
		root["serialNum"] = (int)serialNum;
		root["count"] = (int)chatLogDlg->m_chatLogCtrl.GetItemCount()-1;
	}
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}
	return 0;
}


BOOL Cchat_clientView::updateChatLogDetail(CString json) {
	Json::Value root, data;
	Json::Reader reader;
	CString result;
	bool parsingSuccessful = reader.parse(json.GetBuffer(),root);
	if ( !parsingSuccessful ) {
		std::cout  << "Failed to parse configuration\n"
			<< reader.getFormatedErrorMessages();
	}
	data = root["data"];
	
	chatLogDlg->m_chatLogdetailCtrl.ResetContent();
	chatLogDlg->m_chatLogdetailCtrl.AddString("더보기");
	for(int i = data.size()-1; i >= 0; i--) {
		result.Format("[%s]%s",data[i]["timestamp"].asCString(), data[i]["message"].asCString());
		chatLogDlg->m_chatLogdetailCtrl.AddString(result);
	}

	return true;
}


afx_msg LRESULT Cchat_clientView::OnUmGetchatlogdetailmore(WPARAM wParam, LPARAM lParam)
{
	Json::Value root;
	Json::StyledWriter writer;
	CString result;
	int roomNo = (int)wParam;
	int count = (int)lParam;
	int retval;

	root["type"] = MSG_CHAT_LOG_ROOM_MORE;
	root["roomNo"] = (int)roomNo;
	root["count"] = (int)count;
	retval = send(sock, writer.write(root).c_str(), strlen(writer.write(root).c_str()), 0);
	if(retval == SOCKET_ERROR) {
		endClient("서버와의 연결이 끊겼습니다.");
	}
	
	return 0;
}


BOOL Cchat_clientView::updateChatLogDetailMore(CString json) {
	Json::Value root, data;
	Json::Reader reader;
	CString result;
	bool parsingSuccessful = reader.parse(json.GetBuffer(),root);
	if ( !parsingSuccessful ) {
		std::cout  << "Failed to parse configuration\n"
			<< reader.getFormatedErrorMessages();
	}
	data = root["data"];
	for(int i = 0; i < data.size(); i++) {
		result.Format("[%s]%s",data[i]["timestamp"].asCString(), data[i]["message"].asCString());
		chatLogDlg->m_chatLogdetailCtrl.InsertString(1, result);
	}
	if(data.size() == 0) {
		chatLogDlg->m_chatLogdetailCtrl.DeleteString(0);
	}

	return true;
}


BOOL Cchat_clientView::updateChatLogMore(CString json) {
LVITEM LI;
	LI.mask = LVIF_TEXT;
	LI.iItem = 0;
	LI.iSubItem = 0;
	CString tmp;
	Json::Value root, data;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(json.GetBuffer(),root);
	if ( !parsingSuccessful ) {
		std::cout  << "Failed to parse configuration\n"
			<< reader.getFormatedErrorMessages();
	}

	chatLogDlg->m_chatLogCtrl.DeleteItem(chatLogDlg->m_chatLogCtrl.GetItemCount()-1);
	data = root["data"];
	for(int i=0; i<data.size(); i++) {
		LI.iItem = chatLogDlg->m_chatLogCtrl.GetItemCount();

		tmp = data[i]["room"].asCString();
		LI.pszText = tmp.GetBuffer();
		chatLogDlg->m_chatLogCtrl.InsertItem(&LI);

		tmp = data[i]["name"].asCString();
		chatLogDlg->m_chatLogCtrl.SetItemText(LI.iItem, 1, tmp.GetBuffer());
						
		tmp = data[i]["timestamp"].asCString();
		chatLogDlg->m_chatLogCtrl.SetItemText(LI.iItem, 2, tmp.GetBuffer());
	}
	LI.iItem = chatLogDlg->m_chatLogCtrl.GetItemCount();
	tmp = "-1";
	LI.pszText = tmp.GetBuffer();
	chatLogDlg->m_chatLogCtrl.InsertItem(&LI);

	if(data.size() != 0) {
		tmp = "더보기";
		chatLogDlg->m_chatLogCtrl.SetItemText(LI.iItem, 1, tmp.GetBuffer());
	}

	return true;
}


BOOL Cchat_clientView::endClient(CString msg)
{
	roomDlg_list.clear();
	delete chatLogDlg;
	chatLogDlg = NULL;
	closesocket(sock);
	WSACleanup();
	MessageBox(msg);
	exit(0);
	
	return true;
}
