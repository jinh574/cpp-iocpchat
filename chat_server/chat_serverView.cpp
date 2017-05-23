
// chat_serverView.cpp : Cchat_serverView 클래스의 구현
//
#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "chat_server.h"
#endif

#include "chat_serverDoc.h"
#include "chat_serverView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKET listen_sock;
CCriticalSection cs;
COdbc mydb;
std::vector<SOCKETINFO *> sock_list;
std::vector<SOCKETINFO *> room_sock_list;
std::vector<HANDLE> mWorkerThread_list;
std::vector<Room *> room_list;

// 스레드풀 스레드
UINT WorkerThread(LPVOID arg);

// Cchat_serverView

IMPLEMENT_DYNCREATE(Cchat_serverView, CView)

BEGIN_MESSAGE_MAP(Cchat_serverView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_COMMAND(ID_32771, &Cchat_serverView::On32771)
	ON_COMMAND(ID_32773, &Cchat_serverView::On32773)
	ON_COMMAND(ID_SERVER_STOP, &Cchat_serverView::OnServerStop)
	ON_UPDATE_COMMAND_UI(ID_32773, &Cchat_serverView::OnUpdate32773)
	ON_UPDATE_COMMAND_UI(ID_SERVER_STOP, &Cchat_serverView::OnUpdateServerStop)
END_MESSAGE_MAP()

// Cchat_serverView 생성/소멸

Cchat_serverView::Cchat_serverView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

Cchat_serverView::~Cchat_serverView()
{
}

BOOL Cchat_serverView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// Cchat_serverView 그리기

void Cchat_serverView::OnDraw(CDC* /*pDC*/)
{
	Cchat_serverDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void Cchat_serverView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cchat_serverView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cchat_serverView 진단

#ifdef _DEBUG
void Cchat_serverView::AssertValid() const
{
	CView::AssertValid();
}

void Cchat_serverView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cchat_serverDoc* Cchat_serverView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cchat_serverDoc)));
	return (Cchat_serverDoc*)m_pDocument;
}
#endif //_DEBUG


// Cchat_serverView 메시지 처리기


void Cchat_serverView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CRect rect;
	GetClientRect(&rect);
	LVCOLUMN LC;

	m_listCtrl.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rect, this, IDC_LIST_LOG);
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LC.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	LC.fmt = LVCFMT_LEFT;
	LC.iSubItem = 0;
	LC.pszText = "Status";
	LC.cx = 60;
	m_listCtrl.InsertColumn(0, &LC);

	LC.iSubItem = 1;
	LC.pszText = "Timestamp";
	LC.cx = 135;
	m_listCtrl.InsertColumn(1, &LC);
	
	LC.iSubItem = 2;
	LC.pszText = "Type";
	LC.cx = 80;
	m_listCtrl.InsertColumn(2, &LC);

	LC.iSubItem = 3;
	LC.pszText = "Source";
	LC.cx = 150;
	m_listCtrl.InsertColumn(3, &LC);

	LC.iSubItem = 4;
	LC.pszText = "Description";
	LC.cx = 400;
	m_listCtrl.InsertColumn(4, &LC);

	LVITEM LI;
	LI.mask = LVIF_TEXT;
	LI.iItem = 0;
	LI.iSubItem = 0;
	LI.pszText = "SYSTEM";
	m_listCtrl.InsertItem(&LI);
	CTime thetime;
	thetime = CTime::GetCurrentTime();
	CString timestamp;
	timestamp.Format(_T("%d-%d-%d %d:%d:%d"),thetime.GetYear(), thetime.GetMonth(), thetime.GetDay(), thetime.GetHour(), thetime.GetMinute(), thetime.GetSecond());
	m_listCtrl.SetItemText(LI.iItem, 1, timestamp);
	m_listCtrl.SetItemText(LI.iItem, 4, "Server Program Started.");

	if(mydb.ConnectMySQL()) {
		printLog(getTypeName(MSG_SYS), "Database Connect Success", "localhost", true);
	}
	else {
		printLog(getTypeName(MSG_SYS), "Database Connect Fail", "localhost", false);
	}
}


// 로그에 출력
void Cchat_serverView::printLog(CString msgDef, CString msg, CString source, bool status)
{
	LVITEM LI;
	LI.mask = LVIF_TEXT;
	LI.iSubItem = 0;
	CTime thetime;
	CString timestamp;

	//현재 시간 구하기 
	thetime = CTime::GetCurrentTime();
	timestamp.Format(_T("%d-%02d-%02d %02d:%02d:%02d"),thetime.GetYear(), thetime.GetMonth(), thetime.GetDay(), thetime.GetHour(), thetime.GetMinute(), thetime.GetSecond());

	//성공 실패
	LI.iItem = m_listCtrl.GetItemCount();
	if(status) {
		LI.pszText = "Success";
	}
	else {
		LI.pszText = "Fail";
	}

	//메시지 타입
	//type.Format("%d",msgDef);

	m_listCtrl.InsertItem(&LI);
	m_listCtrl.SetItemText(LI.iItem, 1, timestamp);
	m_listCtrl.SetItemText(LI.iItem, 2, msgDef);
	m_listCtrl.SetItemText(LI.iItem, 3, source);
	m_listCtrl.SetItemText(LI.iItem, 4, msg);

	m_listCtrl.EnsureVisible(m_listCtrl.GetItemCount()-1, FALSE);
}

// iocp 초기화
int Cchat_serverView::iocpInit(void)
{
	int retval;
	WSADATA wsa;
	CString szText;

	printLog(getTypeName(MSG_SYS),"Winsock Init...","localhost",true);
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		printLog(getTypeName(MSG_SYS),"Winsock Init Error","localhost",false);
		return 1;
	}
	else {
		printLog(getTypeName(MSG_SYS),"Winsock Init Success","localhost",true);
	}
	//입출력 완료 포트 생성
	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(hcp == NULL) {
		printLog(getTypeName(MSG_SYS),"Create IOCP Fail","localhost",false);
		return 1;
	}
	printLog(getTypeName(MSG_SYS),"Create IOCP Success","localhost",true);

	//CPU갯수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//CPU갯수*2 개의 작업자 스레드 생성
	CWinThread* hThread = NULL;
	m_hEvent = CreateEvent(0, FALSE, FALSE, 0);
	for(int i=0; i<(int)si.dwNumberOfProcessors*2+2; i++) {
		//hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		hThread = AfxBeginThread(::WorkerThread, this);
		if(hThread == NULL) {
			printLog(getTypeName(MSG_SYS),"Create WorkerThread Fail","localhost",false);
			return 1;
		}
		mWorkerThread_list.push_back(hThread);
		WaitForSingleObject(m_hEvent, INFINITE);
	}
	szText.Format("Create %d WorkerThread Success", (int)si.dwNumberOfProcessors*2+2);
	printLog(getTypeName(MSG_SYS),szText,"localhost",true);
}

UINT WorkerThread(LPVOID arg) {
	Cchat_serverView *ptr = (Cchat_serverView *) arg;
	ptr->WorkerThread();

	return 0;
}


// 스레드풀 스레드
void Cchat_serverView::WorkerThread(void)
{
	DWORD retval, tmp;
	CString szText;
	//char *data;
	DWORD cbTransferred;
	SOCKET client_sock;
	SOCKETINFO *ptr;
	SOCKADDR_IN clientaddr;
	int iter;
	
	int tmp_i = 2;

	//ACCEPT 정보
	SOCKADDR_IN *plocal = 0, *premote = 0;
	int locallen, remotelen;
	CString ip;
	
	while(1) {
		SetEvent(m_hEvent);
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred, (LPDWORD)&client_sock, (LPOVERLAPPED *)&ptr, INFINITE);

		//클라이언트 정보 얻기
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
		szText.Format(_T("TCP/%s:%d"),inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		ip.Format("%s",inet_ntoa(clientaddr.sin_addr));
		if(!retval || (retval && !cbTransferred)) {
			if(ptr->type == IO_ACCEPT) {
				cs.Lock();
				GetAcceptExSockaddrs(ptr->buf, 0, sizeof(SOCKADDR_IN)+16,sizeof(SOCKADDR_IN)+16, (SOCKADDR **)&plocal, &locallen, (SOCKADDR **)&premote, &remotelen);
				setsockopt(ptr->sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *) &listen_sock, sizeof(listen_sock)); 
				CreateIoCompletionPort( (HANDLE) ptr->sock, hcp, 0, 0);
				szText.Format(_T("TCP/%s:%d"),inet_ntoa(premote->sin_addr), ntohs(premote->sin_port));
				sock_list.push_back(ptr);

				printLog(getTypeName(MSG_SYS),"Client Connected.",szText,true);
				ptr->conn = true;
				ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
				ptr->wsabuf.buf = ptr->buf;
				ptr->wsabuf.len = BUFSIZE;
				ptr->type = IO_RECV;

				DWORD recvbytes;
				DWORD flags = 0;
				ptr->ioFlag++;
				retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
				if(retval == SOCKET_ERROR) {
					if(WSAGetLastError() != WSA_IO_PENDING) {
						ptr->ioFlag--;
						printLog(getTypeName(MSG_SYS), "WSARecv Error",szText,false);
					}
				}
				cs.Unlock();
				continue;
			}
			else {
				cs.Lock();
				//printf("\ntmp_i : %d\tthread_id : %d\n",tmp_i, AfxGetThread()->m_nThreadID);
				tmp_i++;
				
				BOOL bSuccess = false; 
				for(int i=0; i<sock_list.size(); i++) {
					if(sock_list[i]->sock == ptr->sock) {
						bSuccess = true;
						break;
					}
				}
				if(!bSuccess) {
					continue;
				}

				ptr->ioFlag--;
				ptr->conn = false;
				//printLog(getTypeName(ptr->ioFlag), "ioFlag", szText, false);
				if(!ptr->conn && ptr->ioFlag==0) {
					clientDisconn(ptr->sock);
					printLog(getTypeName(MSG_SYS),"Client Disconnected.",(LPCTSTR)szText,true);
					delete ptr;

					//유저목록 갱신
					Json::Value root;
					Json::StyledWriter writer;
					CString tmp;
					DWORD sendbytes;
				
					root["type"] = MSG_USER_LIST;
					//printf("Vector : %d\n", sock_list.size());
					for(int i=0; i<sock_list.size(); i++) {
						if(strcmp(sock_list[i]->id.GetBuffer(),"")) {
							root["user_list"].append(sock_list[i]->id.GetBuffer());
						}
					}
					tmp.Format("%s",writer.write(root).c_str());
					for(int i=0; i<sock_list.size(); i++) {
						if(!strcmp(sock_list[i]->id.GetBuffer(),"")) {
							continue;
						}
						ptr = sock_list[i];
						sendJson(ptr, tmp);
					}
				}
				cs.Unlock();
				continue;
			}
		}

		switch(ptr->type) {
		case IO_RECV:
				ptr->ioFlag--;
				ptr->recvbytes = cbTransferred;
				ptr->sendbytes = 0;
				ptr->buf[ptr->recvbytes] = '\0';
				cs.Lock();
				doMsg(ptr, szText, ip);
				cs.Unlock();
				break;
		case IO_SEND:
				ptr->ioFlag--;

				ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
				ZeroMemory(&ptr->buf, sizeof(ptr->buf));
				ptr->wsabuf.buf = ptr->buf;
				ptr->wsabuf.len = BUFSIZE;
				ptr->type = IO_RECV;

				DWORD recvbytes;
				DWORD flags = 0;
				ptr->ioFlag++;
				retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
				if(retval == SOCKET_ERROR) {
					if(WSAGetLastError() != WSA_IO_PENDING) {
						ptr->ioFlag--;
						printLog(getTypeName(MSG_SYS), "WSARecv Error",szText,false);
					}
				}
				break;
		}
	}
}


void Cchat_serverView::On32773()
{
	// TODO: Add your command handler code here
	iocpInit();
	serverRun();
	m_running = false;
}


BOOL Cchat_serverView::serverRun(void)
{
	int retval;
	CString szText;

	//socket()
	listen_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(listen_sock == INVALID_SOCKET) printLog(getTypeName(MSG_SYS),"Create Socket Fail","localhost",false);
	printLog(getTypeName(MSG_SYS),"Create Socket Success","localhost",true);

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR)  printLog(getTypeName(MSG_SYS),"Socket Bind Service Fail","localhost",false);
	printLog(getTypeName(MSG_SYS),"Sock Bind Service Success","localhost",true);
	
	BOOL on = TRUE;
	setsockopt(listen_sock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (const char*)&on, sizeof(int));
	
	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) printLog(getTypeName(MSG_SYS),"Socket Listen Service Fail","localhost",false);
	printLog(getTypeName(MSG_SYS),"Sock Listen Service Success","localhost",true);

	CreateIoCompletionPort((HANDLE)listen_sock, hcp, 0, 0);
	SOCKET client_sock;
	for(DWORD i=0; i<10; i++) {
		SOCKETINFO *ptr = new SOCKETINFO;
		if(ptr == NULL) {
			printLog(getTypeName(MSG_SYS),"SocketInfo Memory Set Fail",szText, false);
			break;
		}
		client_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);

		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ZeroMemory(&ptr->buf, sizeof(ptr->buf));
		ptr->sock = client_sock;
		ptr->type = IO_ACCEPT;
		ptr->ioFlag = 0;
		ptr->conn = true;

		if (!AcceptEx(listen_sock, ptr->sock, ptr->buf, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &ptr->overlapped))
		{
			if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return FALSE;
			}
		}
	}
	printLog(getTypeName(MSG_SYS),"Socket AcceptEx Success.","localhost", true);

	return 0;
}


// 서버 종료
BOOL Cchat_serverView::serverEnd(void) {
	for (DWORD i=0;i<mWorkerThread_list.size();i++)
		PostQueuedCompletionStatus(m_hEvent, 0, 0, NULL);

	for (DWORD i=0;i<mWorkerThread_list.size();i++) {
		WaitForSingleObject(mWorkerThread_list[i], INFINITE);

		CloseHandle(mWorkerThread_list[i]);
	}
	
	mWorkerThread_list.clear();
	
	if (m_hEvent)
		CloseHandle(m_hEvent);

	WSACleanup();

	return TRUE;
}


// 메세지 판별
void Cchat_serverView::doMsg(LPVOID arg, CString szText, CString ip)	{
	SOCKETINFO *ptr = (SOCKETINFO *) arg;
	DWORD type, retval, iter;
	CString tmp;

	if(!ptr->conn) {
		return;
	}
	//필요 변수
	CString id = NULL, pw = NULL, code = NULL, msg = NULL, data = NULL, roomName = NULL;
	BOOL success = false, isIn = false;;
	DWORD serialNum = NULL, roomNo, dbNo, tmp_i;
	DWORD sendbytes;
	Room *room;

	Json::Value root;
	Json::Reader reader;
	Json::StyledWriter writer;
	
	bool parsingSuccessful = reader.parse((LPCTSTR)ptr->buf,root);
	if ( !parsingSuccessful ) {
		std::cout  << "Failed to parse configuration\n"
			<< reader.getFormatedErrorMessages();
	}
	
	
	if(root.isNull()) {
		type = -1;
	}
	else {
		type = root.get("type","").asInt();
		//MessageBox(ptr->buf);
		//printf("%s\n%s\n\n",ptr->id, ptr->buf);
	}
	switch(type) {
	case MSG_CONN:
			cs.Lock();
			root.clear();
			root["type"] = MSG_USER_LIST;
			for(int i=0; i<sock_list.size(); i++) {
				if(strcmp(sock_list[i]->id.GetBuffer(),"")) {
					root["user_list"].append(sock_list[i]->id.GetBuffer());
				}
			}
			tmp.Format("%s",writer.write(root).c_str());
			for(int i=0; i<sock_list.size(); i++) {
				if(!strcmp(sock_list[i]->id.GetBuffer(),"")) {
					continue;
				}
				ptr = sock_list[i];
				sendJson(ptr, tmp);
			}
			if(!room_list.empty())
				updateRoomList();
			cs.Unlock();
			break;
	case MSG_JOIN:
		id = root.get("id","").asCString();
		pw = root.get("pw","").asCString();
		code = root.get("code","").asCString();
		success = false;
		
		printLog(getTypeName(type), "Client Join Request", szText, true);
		if(mydb.makeUser(id.GetBuffer(),pw.GetBuffer(),code.GetBuffer())) { //회원가입 성공
			printLog(getTypeName(type), "Client Join ACK Success", szText, true);
			success = true;
		}
		else { //회원가입 실패
			printLog(getTypeName(type), "Client Join ACK Fail", szText, false);
			success = false;
		}

		root.clear();
		root["type"] = MSG_JOIN_ACK;
		root["success"] = success;
		tmp.Format("%s",writer.write(root).c_str());
		sendJson(ptr, tmp);
		break;
	case MSG_LOGIN:
		id = root.get("id","").asCString();
		pw = root.get("pw","").asCString();

		printLog(getTypeName(type), "Client Login Request", szText, true);
		root.clear();
		serialNum = mydb.login(id.GetBuffer(),pw.GetBuffer());
		if(serialNum > 0) {
			for(int i=0; i<sock_list.size(); i++) {
				if(!strcmp(sock_list[i]->id.GetBuffer(),id.GetBuffer())) {
					isIn = true;
					break;
				}
			}
			if(!isIn) {
				//로그인 성공시
				printLog(getTypeName(type), "Client Login ACK Success", szText, true);
				ptr->serialNum = serialNum;
				ptr->id = id;
				root["type"] = MSG_LOGIN_AUTH;
				root["serialNum"] = (int)serialNum;
				if(mydb.authCheck(serialNum, ip.GetBuffer())) { //동일 아이피 접속
					root["success"] = true;
					printLog(getTypeName(type), "Login Auth Code Pass", szText, true);
					mydb.login_log(serialNum, ip.GetBuffer(), 1);
				}
				else { //새로운 아이피 접속
					root["success"] = false;
					printLog(getTypeName(type), "Login Auth Code Request", szText, true);
				}
			}
			else {
				root["type"] = MSG_LOGIN_ACK;
				root["success"] = false;
			}
		}
		else {
			//로그인 정보 불일치
			root.clear();
			root["type"] = MSG_LOGIN_ACK;
			root["success"] = false;
			printLog(getTypeName(type), "Client Login ACK Fail", szText, true);
		}
		tmp.Format("%s",writer.write(root).c_str());
		
		sendJson(ptr, tmp);
		break;
	case MSG_LOGIN_AUTH_CODE:
		printLog(getTypeName(type), "Client Login Auth Code Recv", szText, true);
		code = root.get("code","").asCString();
		serialNum = root.get("serialNum","").asUInt();

		root.clear();
		root["type"] = MSG_LOGIN_AUTH_ACK;
		if(mydb.authLogin(serialNum, code.GetBuffer(), ip.GetBuffer())) {
			root["success"] = true;
			printLog(getTypeName(type), "Client Login Auth Code ACK Successs", szText, true);
			mydb.login_log(serialNum, ip.GetBuffer(), 1);
		}
		else {
			root["success"] = false;
			printLog(getTypeName(type), "Client Login Auth Code ACK Fail", szText, true);
			mydb.login_log(serialNum, ip.GetBuffer(), 0);
		}
		tmp.Format("%s",writer.write(root).c_str());

		sendJson(ptr, tmp);
		break;
	case MSG_MESSAGE_LOBBY:
		msg = root.get("data", "").asCString();

		root.clear();
		root["type"] = MSG_BROADCAST_LOBBY;
		root["id"] = ptr->id.GetBuffer();
		root["data"] = msg.GetBuffer();

		tmp.Format("%s",writer.write(root).c_str());
		for(int i=0; i<sock_list.size(); i++) {
			sendJson(sock_list[i], tmp);
		}
		printLog(getTypeName(type), "Client Broadcast Msg in Lobby Success", szText, true);
		break;
	case MSG_CREATEROOM:
		data = root.get("name", "").asCString();
		room = new Room(room_list.size()+1, data);
				
		room->setDbNo(mydb.makeRoom(data.GetBuffer()));
		room_list.push_back(room);

		root.clear();
		root["type"] = MSG_CREATEROOM_ACK;
		root["success"] = true;
		root["roomNo"] = room_list.size();
		tmp.Format("%s",writer.write(root).c_str());
		sendJson(ptr, tmp);
		printLog(getTypeName(type), "Client Create Room Success", szText, true);

		break;
	case MSG_ENTERROOM:
		roomNo = root.get("roomNo","").asUInt();
		for(int i=0; i<room_list.size(); i++) {
			if(room_list[i]->getRoomNo(roomNo)) {
				success = room_list[i]->enterRoom(ptr);
				roomName = room_list[i]->getRoomName();
				break;
			}
		}

		root.clear();
		root["type"] = MSG_ENTERROOM_ACK;
		root["success"] = success;
		root["roomNo"] = (int)roomNo;
		root["roomName"] = roomName.GetBuffer();
		if(!success) {
			printLog(getTypeName(type), "Client Enter Room Fail", szText, false);
		}
		else {
			printLog(getTypeName(type), "Client Enter Room Success", szText, true);
		}
		tmp.Format("%s",writer.write(root).c_str());
		sendJson(ptr,tmp);

		break;
	case MSG_CHATROOM_COMP:
		success = root.get("success","").asBool();
		roomNo = root.get("roomNo","").asInt();
		if(success) {
			updateRoomList();
			printLog(getTypeName(type), "Chat Room List Update", szText, true);
			
			root.clear();
			tmp.Format("[%s]님이 입장하였습니다.",ptr->id);
			root["type"] = MSG_CHATROOM_BROADCAST;
			root["roomNo"] = (int)roomNo;
			root["data"] = tmp.GetBuffer();
			tmp.Format("%s",writer.write(root).c_str());
			sendRoomJson(roomNo, tmp);
		}
		break;
	case MSG_CHATROOM_DISCONN:
		roomNo = root.get("roomNo","").asInt();
		for(int i=0; i<room_list.size(); i++) {
			if(room_list[i]->getRoomNo(roomNo)) {
				tmp_i = room_list[i]->leaveRoom(ptr);
				iter = i;
				break;
			}
		}
		if(tmp_i==0) {
			//printf("IN\n");
			delete room_list[iter];
			room_list.erase(room_list.begin()+iter);
		}

		updateRoomList();
		printLog(getTypeName(type), "Chat Room List Update", szText, true);
		break;
	case MSG_CHATROOM_MESSAGE:
		roomNo = root.get("roomNo", "").asInt();
		serialNum = root.get("serialNum","").asInt();
		msg = root.get("data","").asCString();
		msg.Format("%s : %s",ptr->id, msg);
		root.clear();
		root["type"] = MSG_CHATROOM_BROADCAST;
		root["roomNo"] = (int)roomNo;
		root["data"] = msg.GetBuffer();
		tmp.Format("%s",writer.write(root).c_str());
		
		for(int i=0; i<room_list.size(); i++) {
			if(room_list[i]->getRoomNo(roomNo)) {
				dbNo = room_list[i]->getDbNo();
				break;
			}
		}
		mydb.log(serialNum, dbNo, msg.GetBuffer());

		sendRoomJson(roomNo, tmp);
		break;
	case MSG_WHISPER:
		id = root.get("to","").asCString();
		data = root.get("data","").asCString();

		root.clear();
		root["type"] = MSG_WHISPER_ACK;
		root["data"] = data.GetBuffer();
		root["from"] = ptr->id.GetBuffer();
		tmp.Format("%s",writer.write(root).c_str());
		for(int i=0; i<sock_list.size(); i++) {
			if(!strcmp(sock_list[i]->id.GetBuffer(),ptr->id.GetBuffer())) {
				iter = i;
				break;
			}
		}
		sendJson(sock_list[iter], tmp);
		printLog(getTypeName(type), "Client Send Whisper Msg", szText, true);
		break;
	case MSG_LOGIN_LOG:
		serialNum = root.get("serialNum","").asInt();
		tmp = mydb.getLogin_log(serialNum);
		sendJson(ptr, tmp);
		printLog(getTypeName(type), "Client Login Log Send Success", szText, true);
		break;
	case MSG_CHAT_LOG:
		serialNum = root.get("serialNum","").asInt();
		tmp = mydb.getChat_list(serialNum);
		sendJson(ptr, tmp);
		printLog(getTypeName(type), "Client Chat Log Send Success", szText, true);
		break;
	case MSG_CHAT_LOG_ROOM:
		roomNo = root.get("roomNo","").asInt();
		tmp = mydb.getChatRoom_list(roomNo);
		sendJson(ptr, tmp);
		printLog(getTypeName(type), "Client Login Log Message Send Success", szText, true);
		break;
	case MSG_CHAT_LOG_ROOM_MORE:
		roomNo = root.get("roomNo","").asInt();
		tmp_i = root.get("count","").asInt();
		tmp = mydb.getChatRoom_list_more(roomNo, tmp_i);
		sendJson(ptr, tmp);
		printLog(getTypeName(type), "Client Login Log More Send Success", szText, true);
		break;
	case MSG_CHAT_LOG_MORE:
		serialNum = root.get("serialNum","").asInt();
		tmp_i = root.get("count","").asInt();
		tmp = mydb.getChat_list_more(serialNum, tmp_i);
		sendJson(ptr, tmp);
		printLog(getTypeName(type), "Client Login Log Message More Send Success", szText, true);
		break;
	default:
		break;
	}
}


void Cchat_serverView::sendJson(LPVOID arg, CString json) {
	DWORD retval, sendbytes;
	SOCKETINFO *ptr = (SOCKETINFO *) arg;

	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->wsabuf.buf = json.GetBuffer();
	ptr->wsabuf.len = strlen(json);
	ptr->type = IO_SEND;
	ptr->ioFlag++;
	retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, NULL);
	if(retval == SOCKET_ERROR) {
		if(WSAGetLastError() != WSA_IO_PENDING) {
			//clientDisconn(ptr->sock);
			//delete ptr;
		}
	}
}


CString Cchat_serverView::getTypeName(DWORD n)	{
	CString result;
	if(n == MSG_CONN)					result = "Connect";
	else if(n == MSG_SYS)				result = "System";
	else if(n == MSG_USER_LIST)			result = "User list";
	else if(n == MSG_JOIN)				result = "Join Req.";
	else if(n == MSG_JOIN_ACK)			result = "Ack. Join";
	else if(n == MSG_LOGIN)				result = "Login Req.";
	else if(n == MSG_LOGIN_AUTH)		result = "Auth Req.";
	else if(n == MSG_LOGIN_AUTH_CODE)	result = "Code Recv.";
	else if(n == MSG_LOGIN_AUTH_ACK)	result = "Ack. Auth";
	else if(n == MSG_MESSAGE_LOBBY)		result = "Broadcast Req.";
	else if(n == MSG_BROADCAST_LOBBY)	result = "Brooadcast";
	else if(n == MSG_CREATEROOM)		result = "Create Room Req.";
	else if(n == MSG_ENTERROOM)			result = "Enter Room Req.";
	else if(n == MSG_ENTERROOM_ACK)		result = "Ack. Enter Room";
	else if(n == MSG_CHATROOM_COMP)		result = "Comp. Create Chatroom";
	else if(n == MSG_CHATROOM_LIST)		result = "Update room list";
	else if(n == MSG_CHATROOM_DISCONN)	result = "Disconn room";	
	else if(n == MSG_CHATROOM_MESSAGE)	result = "chatRoom Message";	
	else if(n == MSG_CHATROOM_BROADCAST)	result = "chatRoom Broadcast";	
	else if(n == MSG_WHISPER)			result = "Client whisper Req.";	
	else if(n == MSG_WHISPER_ACK)		result = "Ack. client Whisper";	
	else if(n == MSG_LOGIN_LOG)			result = "Login Log Req.";	
	else if(n == MSG_LOGIN_LOG_ACK)		result = "Ack. Login Log";	
	else if(n == MSG_CHAT_LOG)			result = "Chat Log Req.";	
	else if(n == MSG_CHAT_LOG_ACK)		result = "Ack. chat log";	
	else if(n == MSG_CHAT_LOG_ROOM)		result = "Chat msg log Req.";	
	else if(n == MSG_CHAT_LOG_ROOM_ACK)	result = "Ack. chat msg log";	
	else if(n == MSG_CHAT_LOG_ROOM_MORE)	result = "Chat msg log more Req.";	
	else if(n == MSG_CHAT_LOG_ROOM_MORE_ACK)	result = "Ack. Chat msg log more";	
	else if(n == MSG_CHAT_LOG_MORE)			result = "Chat Log more Req.";
	else if(n == MSG_CHAT_LOG_MORE_ACK)		result = "Ack. Chat Log more";
	else {
		result.Format("#%d",n);
	}
	return result;
}


BOOL Cchat_serverView::clientDisconn(SOCKET client) {
	int iter = -1;

	if(!client)
		return false;
	
	for(int i=0; i<sock_list.size(); i++) {
		//printf("%d, %d\n",sock_list[i]->sock,client);
		if(sock_list[i]->sock == client) {
			iter = i;
			break;
		}
	}

	if(iter==-1) {
		return false;
	}
	else {
		shutdown(client, SD_BOTH);
		closesocket(client);
		sock_list.erase(sock_list.begin()+iter);
	}
	
	return true;
}


BOOL Cchat_serverView::updateRoomList(void) {
	Json::Value root;
	Json::Reader reader;
	Json::StyledWriter writer;
	CString tmp;

	root["type"] = MSG_CHATROOM_LIST;
	for(int i=0; i<room_list.size(); i++) {
		root["roomNo"].append((int)room_list[i]->getRoomNo());
		root["userCount"].append((int)room_list[i]->getUserCount());
		root["roomName"].append(room_list[i]->getRoomName().GetBuffer());
	}
	tmp.Format("%s",writer.write(root).c_str());
	for(int i=0; i<sock_list.size(); i++) {
		sendJson(sock_list[i],tmp);
	}
	return true;
}


BOOL Cchat_serverView::sendRoomJson(DWORD n, CString data) {
	for(int i=0; i<room_list.size(); i++) {
		if(room_list[i]->getRoomNo(n)) {
			room_sock_list = *room_list[i]->getUserList();
			break;
		}
	}
	for(int i=0; i<room_sock_list.size(); i++) {
		sendJson(room_sock_list[i],data);
	}
	return true;
}


void Cchat_serverView::OnServerStop()
{
	// TODO: Add your command handler code here
	Json::Value root;
	Json::StyledWriter writer;
	CString tmp;

	root["type"] = MSG_ALL_DISCONN;
	tmp.Format("%s",writer.write(root).c_str());
	for(int i=0; i<sock_list.size(); i++) {
		sendJson(sock_list[i], tmp);
	}
	while(!sock_list.empty()) {}
	serverEnd();
	printLog(getTypeName(MSG_SYS), "All Client Successfully Disconnected.","localhost", true);

	m_running = true;
}


void Cchat_serverView::OnUpdate32773(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_running);
}


void Cchat_serverView::OnUpdateServerStop(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_running);
}
