#pragma once


// CJoinDlg dialog

class CJoinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CJoinDlg)

public:
	CJoinDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJoinDlg();

// Dialog Data
	enum { IDD = IDD_JOINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_id;
	CString m_pw;
	CString m_code;
};
