// DialogPgAD.cpp : 实现文件
//

#include "stdafx.h"
#include "USBDEMO.h"
#include "DialogPgAD.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
// CDialogPgAD 对话框
using namespace std;

IMPLEMENT_DYNAMIC(CDialogPgAD, CDialogEx)

CDialogPgAD::CDialogPgAD(CWnd* pParent /*=NULL*/)
	:CDialogPage(CDialogPgAD::IDD, pParent)
	, m_Edit_AD_Channel(0)
{
	m_Edit_DA0 = _T(1000);
	m_Edit_DA1 = _T(1000);
	m_Edit_DA2 = _T(1000);
	m_Edit_DA3 = _T(1000);
	m_Edit_DA4 = _T(1000);
	m_Edit_DA5 = _T(1000);
	m_Edit_DA6 = _T(1000);
	m_Edit_DA7 = _T(1000);
	m_AD_Num = 0;
	m_Edit_AD_Channel = _T(0);
	m_Edit_AD = _T(0);
}

CDialogPgAD::~CDialogPgAD()
{
}

void CDialogPgAD::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DA0, m_Edit_DA0);
	DDX_Text(pDX, IDC_EDIT_DA1, m_Edit_DA1);
	DDX_Text(pDX, IDC_EDIT_DA2, m_Edit_DA2);
	DDX_Text(pDX, IDC_EDIT_DA3, m_Edit_DA3);
	DDX_Text(pDX, IDC_EDIT_DA4, m_Edit_DA4);
	DDX_Text(pDX, IDC_EDIT_DA5, m_Edit_DA5);
	DDX_Text(pDX, IDC_EDIT_DA6, m_Edit_DA6);
	DDX_Text(pDX, IDC_EDIT_DA7, m_Edit_DA7);
	DDX_Text(pDX, IDC_EDIT_AD, m_Edit_AD);
	DDX_Radio(pDX, IDC_RadioAD0, m_AD_Num);
	DDV_MinMaxInt(pDX, m_AD_Num, 0, 7);
	DDX_Text(pDX, IDC_EDIT_AD_CHANNEL, m_Edit_AD_Channel);
}

BEGIN_MESSAGE_MAP(CDialogPgAD, CDialogPage)

	ON_BN_CLICKED(IDC_DA0, &CDialogPgAD::OnBnClickedDA0)
	ON_BN_CLICKED(IDC_DA1, &CDialogPgAD::OnBnClickedDA1)
	ON_BN_CLICKED(IDC_DA2, &CDialogPgAD::OnBnClickedDA2)
	ON_BN_CLICKED(IDC_DA3, &CDialogPgAD::OnBnClickedDA3)
	ON_BN_CLICKED(IDC_DA4, &CDialogPgAD::OnBnClickedDA4)
	ON_BN_CLICKED(IDC_DA5, &CDialogPgAD::OnBnClickedDA5)
	ON_BN_CLICKED(IDC_DA6, &CDialogPgAD::OnBnClickedDA6)
	ON_BN_CLICKED(IDC_DA7, &CDialogPgAD::OnBnClickedDA7)
	ON_BN_CLICKED(IDC_BTNAD, &CDialogPgAD::OnBnClickedBtnAD)
END_MESSAGE_MAP()


// CDialogPgAD 消息处理程序


void CDialogPgAD::OnBnClickedDA0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned long channel = 0;
	USB_DA_xfer(&channel, &m_Edit_DA0);
}
  
void CDialogPgAD::OnBnClickedDA1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned long channel = 1;
	USB_DA_xfer(&channel, &m_Edit_DA1);
}


void CDialogPgAD::OnBnClickedDA2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned long channel = 2;
	USB_DA_xfer(&channel, &m_Edit_DA2);
}


void CDialogPgAD::OnBnClickedDA3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned long channel = 3;
	USB_DA_xfer(&channel, &m_Edit_DA3);
}


void CDialogPgAD::OnBnClickedDA4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned long channel = 4;
	USB_DA_xfer(&channel, &m_Edit_DA4);
}


void CDialogPgAD::OnBnClickedDA5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned long channel = 5;
	USB_DA_xfer(&channel, &m_Edit_DA5);
}


void CDialogPgAD::OnBnClickedDA6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned long channel = 6;
	USB_DA_xfer(&channel, &m_Edit_DA6);
}


void CDialogPgAD::OnBnClickedDA7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	unsigned long channel = 7;
	USB_DA_xfer(&channel, &m_Edit_DA7);
}

void CDialogPgAD::USB_DA_xfer(unsigned long *channel, unsigned long *data)
{
	int i = 0;
	UCHAR buffer[512];
	ZeroMemory(buffer,512);
	LONG length = 512;
	unsigned long pkthead = 0x5A5A5A5A;
	unsigned long id = 0x5A5A5A5A;
	memcpy(buffer,   &pkthead,4);
	memcpy(buffer+4, &id,4);
	memcpy(buffer+8, channel,4);
	memcpy(buffer+12, data,4);
	//for(i = 0; i<512; i++) {
	//	buffer[i] = i;
	//}
	//for(i = 0;i<12;i++)
	//{
	//	theApp.str.Format("0x%x    ",buffer[i]);
	//	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	//}
	//
	//theApp.str = "\n";
	//AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	if (theApp.m_pBulkOutEpt != NULL)
	{
	    theApp.m_pBulkOutEpt->XferData(buffer,length);
	} else {
	    AfxMessageBox("错误！ 请连接并打开GD4-USB2.0");
	}
	return;
}


void CDialogPgAD::OnBnClickedBtnAD()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	USB_AD_xfer(m_AD_Num);
	//m_Edit_AD = m_AD_Num;
	UpdateData(false);
}


void CDialogPgAD::USB_AD_xfer(unsigned long channel)
{
	UCHAR buffer[512];
	ZeroMemory(buffer,512);
	LONG length = 512;
	int i = 0;
	UCHAR head = 0x6A;
	unsigned long pkthead = 0x6A6A6A6A;
	unsigned long id = 0x6A6A6A6A;
	unsigned long value = 0;
	memcpy(buffer,   &pkthead,4);
	memcpy(buffer+4, &id,4);
	memcpy(buffer+8, &channel,4);
	//for(i = 0; i<512; i++) {
	//	buffer[i] = i;
	//}
	//for(i = 0;i<12;i++)
	//{
	//	theApp.str.Format("0x%x    ",buffer[i]);
	//	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	//}
	//
	//theApp.str = "\n";
	//AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	if (theApp.m_pBulkOutEpt != NULL)
	{
	    theApp.m_pBulkOutEpt->XferData(buffer,length);
	} else {
	    AfxMessageBox("错误！ 请连接并打开GD4-USB2.0");
	}
	Sleep(100);   //wait 100ms
	if (theApp.m_pBulkInEpt != NULL)
	{
		theApp.m_pBulkInEpt->XferData(buffer,length);
	}
	while(1) {
		if(i > 512) return;
	    if(buffer[i] == head) break;
	    i++;
	}
	while(1){
		if(buffer[i] == head) {
			i++;
		} else {
		    memcpy(&channel,buffer+i,4);
		    memcpy(&value,buffer+i+4,4);
		    break;
		}
	}
	m_Edit_AD_Channel = channel;
	m_Edit_AD = value;
	return;
}
