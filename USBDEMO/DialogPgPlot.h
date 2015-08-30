#pragma once

// CDialogPgPlot �Ի���
#include "DialogPage.h"
#include "Plot.h"
class CDialogPgPlot : public CDialogPage
{
	DECLARE_DYNAMIC(CDialogPgPlot)

public:
	CDialogPgPlot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPgPlot();
	//HWND m_hWnd;
	//HWND m_hMainWnd;
	char m_pData[1024];
	CPlot m_PlotShow;

	CWinThread *m_RecvThread ;
// �Ի�������
	enum { IDD = IDD_DIALOGPAGE_PLOT };
	BOOL Create(CWnd* pParent = NULL) { return CDialog::Create(IDD,pParent); }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	int RecvDataProcess(UCHAR *pData, LONG nLength);
	static UINT CDialogPgPlot::Recv_Thread( LPVOID pParam );
protected:
	afx_msg LRESULT OnRecvdat(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedRadio1();
};
