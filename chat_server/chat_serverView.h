
// chat_serverView.h : Cchat_serverView 클래스의 인터페이스
//

#pragma once

class Cchat_serverView : public CView
{
protected: // serialization에서만 만들어집니다.
	Cchat_serverView();
	DECLARE_DYNCREATE(Cchat_serverView)

// 특성입니다.
public:
	Cchat_serverDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~Cchat_serverView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
protected:
public:
	// 로그에 출력
//	afx_msg void On32771();
	afx_msg void On32773();
	// iocp 초기화
	int iocpInit(void);
	void printLog(CString msgDef, CString msg, CString source, bool status);
	void WorkerThread(void);
	BOOL serverRun(void);
	BOOL serverEnd(void);

private:
	HANDLE hcp;
	HANDLE m_hEvent;
	CListCtrl m_listCtrl;
	//std::vector<SOCKETINFO *> mSock_list;
	//std::vector<HANDLE> mWorkerThread_list;
public:
	// 메세지 판별
	void doMsg(LPVOID arg, CString szText, CString ip);
	void sendJson(LPVOID arg, CString json);
	CString getTypeName(DWORD n);
	BOOL clientDisconn(SOCKET sock);
	BOOL updateRoomList(void);
	BOOL sendRoomJson(DWORD n, CString data);
	afx_msg void OnServerStop();
	afx_msg void OnUpdate32773(CCmdUI *pCmdUI);
private:
	bool m_running;
public:
	afx_msg void OnUpdateServerStop(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // chat_serverView.cpp의 디버그 버전
inline Cchat_serverDoc* Cchat_serverView::GetDocument() const
   { return reinterpret_cast<Cchat_serverDoc*>(m_pDocument); }
#endif

