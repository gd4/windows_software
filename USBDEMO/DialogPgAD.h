#pragma once

// CDialogPgAD 对话框

class CDialogPgAD : public CDialogPage
{
	DECLARE_DYNAMIC(CDialogPgAD)

public:
	CDialogPgAD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPgAD();

	CButton	m_Btn_DA0;
	CButton	m_Btn_DA1;
	CButton	m_Btn_DA2;
	CButton	m_Btn_DA3;
	CButton	m_Btn_DA4;
	CButton	m_Btn_DA5;
	CButton	m_Btn_DA6;
	CButton	m_Btn_DA7;

	unsigned long   m_Edit_DA0;
	unsigned long   m_Edit_DA1;
	unsigned long   m_Edit_DA2;
	unsigned long   m_Edit_DA3;
	unsigned long   m_Edit_DA4;
	unsigned long   m_Edit_DA5;
	unsigned long   m_Edit_DA6;
	unsigned long   m_Edit_DA7;
	unsigned long   m_Edit_AD;
	int m_AD_Num;
	unsigned long m_Edit_AD_Channel;
// 对话框数据
	enum { IDD = IDD_DIALOGPAGE_AD };
	BOOL Create(CWnd* pParent = NULL) { return CDialog::Create(IDD,pParent); }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDA0();
	afx_msg void OnBnClickedDA1();
	afx_msg void OnBnClickedDA2();
	afx_msg void OnBnClickedDA3();
	afx_msg void OnBnClickedDA4();
	afx_msg void OnBnClickedDA5();
	afx_msg void OnBnClickedDA6();
	afx_msg void OnBnClickedDA7();
	void USB_DA_xfer(unsigned long *channel, unsigned long *data);
	afx_msg void OnBnClickedBtnAD();
	void USB_AD_xfer(unsigned long channel);

};
