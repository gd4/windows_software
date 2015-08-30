// DialogPgPlot.cpp : 实现文件
//

#include "stdafx.h"
#include "USBDEMO.h"
#include "DialogPgPlot.h"
#include "afxdialogex.h"


// CDialogPgPlot 对话框

IMPLEMENT_DYNAMIC(CDialogPgPlot, CDialogPage)
static int Switch;

CDialogPgPlot::CDialogPgPlot(CWnd* pParent /*=NULL*/)
	: CDialogPage(CDialogPgPlot::IDD, pParent)
{
	m_RecvThread = NULL;
}

CDialogPgPlot::~CDialogPgPlot()
{
}

void CDialogPgPlot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PLOT, m_PlotShow);
}

BEGIN_MESSAGE_MAP(CDialogPgPlot, CDialogPage)
	ON_BN_CLICKED(IDSTART, &CDialogPgPlot::OnBnClickedStart)
	ON_BN_CLICKED(IDSTOP, &CDialogPgPlot::OnBnClickedStop)
	ON_MESSAGE(WM_RECVDAT, &CDialogPgPlot::OnRecvdat)
	//ON_BN_CLICKED(IDC_RADIO1, &CDialogPgPlot::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CDialogPgPlot 消息处理程序


void CDialogPgPlot::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序
    ::PostMessage(GetSafeHwnd(), WM_RECVDAT, 1, 0);
}

void CDialogPgPlot::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	::PostMessage(GetSafeHwnd(), WM_RECVDAT, 2, 0);
}

int CDialogPgPlot::RecvDataProcess(UCHAR *pData, LONG nLength)
{
	//UCHAR buffer[512];
	//ZeroMemory(buffer,512);
	//LONG length = 512;
	int i = 0;
	UCHAR head = 0x8A;
	long USB_Data;
	float Value;
	if (theApp.m_pBulkInEpt != NULL)
	{
		theApp.m_pBulkInEpt->XferData(pData,nLength);
	}
	else{
	//  AfxMessageBox("操作失败，请打开USB设备");
		return 0;
	}
	//for(int i=0;i<nLength;i++) {
	//    pData[i] = i%100;
	//	Value = pData[i];
	//	m_PlotShow.SetData(Value);
    //}
	//while(1) {
	//	if(i > 512) return 1;
	//    if(pData[i] == head) break;
	//    i++;
	//}
	while(1){
		//if(pData[i] == head) {
		//	i++;
		//} else {
		    //memcpy(&channel,buffer+i,4);
		    memcpy(&USB_Data,pData+i,4);
			i = i+4;
			USB_Data = ((USB_Data&0x0FFF)*5000)>>12;
			Value = (float)USB_Data*0.02;
	        m_PlotShow.SetData(Value);
		    if(i > nLength)
			    break;
		//}
	}
	return 0;
}

afx_msg LRESULT CDialogPgPlot::OnRecvdat(WPARAM wParam, LPARAM lParam)
{
	//bool Switch;
	int i = 0;
	UCHAR buffer[512];
	ZeroMemory(buffer,512);
	LONG length = 512;
	LONG channel = 0;

	unsigned long pkthead = 0x7A7A7A7A;
	unsigned long id = 0x7A7A7A7A;
	if(wParam == 1) {
		Switch = TRUE;
	    theApp.str = "Start Recv Data! \n";
	    AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL); 
	    memcpy(buffer,   &pkthead,4);
	    memcpy(buffer+4, &id,4);
	    memcpy(buffer+8, &channel,4);
	    //memcpy(buffer+12, data,4);

	    if (theApp.m_pBulkOutEpt != NULL)
	    {
	        theApp.m_pBulkOutEpt->XferData(buffer,length);
	    } else {
	        AfxMessageBox("错误！ 请连接并打开GD4-USB2.0");
	    }
		if(m_RecvThread == NULL) {
		     m_RecvThread = AfxBeginThread(Recv_Thread,this);
		}
	 }
	 if(wParam == 2){
		 pkthead = 0x7B7B7B7B;
	     id = 0x7B7B7B7B;
		 Switch = FALSE;
	     theApp.str = "stop Recv Data! \n";
		 memcpy(buffer,   &pkthead,4);
	     memcpy(buffer+4, &id,4);
	     memcpy(buffer+8, &channel,4);
	     //memcpy(buffer+12, data,4);

	     if (theApp.m_pBulkOutEpt != NULL)
	     {
	         theApp.m_pBulkOutEpt->XferData(buffer,length);
	     } else {
	         AfxMessageBox("错误！ 请连接并打开GD4-USB2.0");
	     }
	     AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	 	
	 }
	 return 0;
}

UINT CDialogPgPlot::Recv_Thread( LPVOID pParam ) {
	CDialogPgPlot *DlgPlot = (CDialogPgPlot *)pParam;
	UCHAR pData[512];
	LONG nLength = 512;
	//ZeroMemory(pData,512);
	while(1) {
		nLength = 512;
	    ZeroMemory(pData,512);
		if(Switch == TRUE) {
		    //for(int i=0;i<nLength;i++) {
			//    pData[i] = i%100;
		    //}
		    DlgPlot->RecvDataProcess(pData,nLength);
		}
	}
	return 0;
}

void CDialogPgPlot::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
}
