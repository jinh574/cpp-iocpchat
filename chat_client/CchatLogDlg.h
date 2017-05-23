#pragma once


// CchatLogDlg dialog

class CchatLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CchatLogDlg)

public:
	CchatLogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CchatLogDlg();

// Dialog Data
	enum { IDD = IDD_CCHATLOGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_chatLogCtrl;
	CListBox m_chatLogdetailCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListChatLogList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBeginscrollListChatLogList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetfocusListChatLogRoom();
	afx_msg void OnDblclkListChatLogRoom();
private:
	int roomNo;
};
