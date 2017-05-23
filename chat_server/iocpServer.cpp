// iocpServer.cpp : implementation file
//

#include "stdafx.h"
#include "chat_server.h"
#include "iocpServer.h"


// iocpServer

IMPLEMENT_DYNAMIC(iocpServer, CListCtrl)

iocpServer::iocpServer()
{

}

iocpServer::iocpServer(CListCtrl *ctrl) {
	this->ctrl = ctrl;
}

iocpServer::~iocpServer()
{
}


BEGIN_MESSAGE_MAP(iocpServer, CWnd)
END_MESSAGE_MAP()



// iocpServer message handlers

void iocpServer::iocpInit() {
	/*
	int retval;
	WSADATA wsa;
	char szText[128];

	ctrl->AddString("Winsock Init...");
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		m_log->AddString("Winsock Init Error");
		return 1;
	}
	else {
		m_log->AddString("Winsock Init Success");
	}
	//입출력 완료 포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(hcp == NULL) {
		m_log->AddString("Create IOCP Fail");
		return 1;
	}
	m_log->AddString("Create IOCP Success");

	//CPU갯수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//CPU갯수*2 개의 작업자 스레드 생성
	HANDLE hThread;
	THREADPARAM *thparam = new THREADPARAM;
	thparam->hcp = hcp;
	thparam->m_log = m_log;
	for(int i=0; i<(int)si.dwNumberOfProcessors*2+2; i++) {
		hThread = CreateThread(NULL, 0, WorkerThread, thparam, 0, NULL);
		if(hThread == NULL) {
			m_log->AddString("Create WorkerThread Fail");
			return 1;
		}
		CloseHandle(hThread);
	}
	sprintf(szText, "Create %d WorkerThread Success", (int)si.dwNumberOfProcessors*2+2);
	m_log->AddString(szText);
	*/
}

void printLog(int msgDef, char* msg, bool status) {
	LVITEM LI;
	LI.mask = LVIF_TEXT | LVIF_STATE;
	LI.iItem = 0;
	LI.iSubItem = 0;
	LI.stateMask = LVIS_STATEIMAGEMASK;
	LI.pszText = L"마루치";
}
