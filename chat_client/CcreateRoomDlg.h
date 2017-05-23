#pragma once


// CcreateRoomDlg dialog

class CcreateRoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CcreateRoomDlg)

public:
	CcreateRoomDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CcreateRoomDlg();

// Dialog Data
	enum { IDD = IDD_CCREATEROOMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_roomName;
	afx_msg void OnBnClickedOk();
};
