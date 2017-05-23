#pragma once


// CGetIPDlg dialog

class CGetIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGetIPDlg)

public:
	CGetIPDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGetIPDlg();

// Dialog Data
	enum { IDD = IDD_GETIPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_ip;
	afx_msg void OnBnClickedOk();
	DWORD m_ipAddress;
};
