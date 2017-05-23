
// chat_clientView.h : Cchat_clientView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"


class Cchat_clientView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	Cchat_clientView();
	DECLARE_DYNCREATE(Cchat_clientView)

public:
	enum{ IDD = IDD_CHAT_CLIENT_FORM };

// Ư���Դϴ�.
public:
	Cchat_clientDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~Cchat_clientView();
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
	CListBox m_listUser;
	CListBox m_listChat;
	CString m_strData;
private:
//	CString m_ip;
public:
//	CString m_nick;
	BOOL clientInit(void);
private:
	SOCKET sock;
public:
	void recvThread(void);
	void printChat(CString str);
	afx_msg void OnClickedButtSend();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	afx_msg LRESULT OnUmLoginauth(WPARAM wParam, LPARAM lParam);
private:
	DWORD serialNum;
public:
	CListCtrl m_roomCtrl;
	afx_msg void OnClickedButtCreateRoom();
protected:
	afx_msg LRESULT OnUmChatroom(WPARAM wParam, LPARAM lParam);
public:
//	std::vector<CChatRoomDlg *> roomDlg_list;
	BOOL updateRoomList(int roomNo, CString roomName, int roomCount);
protected:
	afx_msg LRESULT OnUmClosechatroom(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDblclkListRoom(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnUmSendchatroom(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDblclkListUser();
	CString m_id;
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	BOOL checkDlg(void);
protected:
	afx_msg LRESULT OnCloseprogram(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMenuLoginLog();
protected:
	afx_msg LRESULT OnUmLoginlog(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnChatLogList();
	BOOL updateChatLog(CString json);
protected:
	afx_msg LRESULT OnUmChatlog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmGetchatlogdetail(WPARAM wParam, LPARAM lParam);
public:
	BOOL updateChatLogDetail(CString json);
protected:
	afx_msg LRESULT OnUmGetchatlogdetailmore(WPARAM wParam, LPARAM lParam);
public:
	BOOL updateChatLogDetailMore(CString json);
	BOOL updateChatLogMore(CString json);
	BOOL endClient(CString msg);
};

#ifndef _DEBUG  // chat_clientView.cpp�� ����� ����
inline Cchat_clientDoc* Cchat_clientView::GetDocument() const
   { return reinterpret_cast<Cchat_clientDoc*>(m_pDocument); }
#endif

