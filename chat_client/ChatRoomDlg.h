#pragma once


// CChatRoomDlg dialog

class CChatRoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatRoomDlg)

public:
	CChatRoomDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChatRoomDlg();

// Dialog Data
	enum { IDD = IDD_CHATROOMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int roomNo;
	CListBox m_chatRoomCtrl;
	CString m_chatRoomMsg;
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
