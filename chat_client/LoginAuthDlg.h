#pragma once


// CLoginAuthDlg dialog

class CLoginAuthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginAuthDlg)

public:
	CLoginAuthDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginAuthDlg();

// Dialog Data
	enum { IDD = IDD_LOGINAUTHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_code;
	afx_msg void OnBnClickedOk();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	virtual BOOL OnInitDialog();
};
