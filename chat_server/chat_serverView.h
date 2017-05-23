
// chat_serverView.h : Cchat_serverView Ŭ������ �������̽�
//

#pragma once

class Cchat_serverView : public CView
{
protected: // serialization������ ��������ϴ�.
	Cchat_serverView();
	DECLARE_DYNCREATE(Cchat_serverView)

// Ư���Դϴ�.
public:
	Cchat_serverDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~Cchat_serverView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
protected:
public:
	// �α׿� ���
//	afx_msg void On32771();
	afx_msg void On32773();
	// iocp �ʱ�ȭ
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
	// �޼��� �Ǻ�
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

#ifndef _DEBUG  // chat_serverView.cpp�� ����� ����
inline Cchat_serverDoc* Cchat_serverView::GetDocument() const
   { return reinterpret_cast<Cchat_serverDoc*>(m_pDocument); }
#endif

