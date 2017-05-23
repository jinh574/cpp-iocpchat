#pragma once
#pragma comment(lib, "ws2_32")
#include "msgDef.h"
#include <WinSock2.h>

// iocpServer

class iocpServer : public CWnd
{
	DECLARE_DYNAMIC(iocpServer)
private:
	CListCtrl *ctrl;

public:
	iocpServer();
	iocpServer(CListCtrl *ctrl);
	virtual ~iocpServer();

	void iocpInit();
	void printLog(int msgDef, char* msg, bool status);
protected:
	DECLARE_MESSAGE_MAP()
};


struct SOCKETINFO {
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE+1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
	bool type;
};