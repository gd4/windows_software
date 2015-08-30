// DlgPgPipes.cpp : implementation file
//

#include "stdafx.h"
#include "USBDEMO.h"
#include "DlgPgPipes.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPgPipes dialog


CDlgPgPipes::CDlgPgPipes(CWnd* pParent /*=NULL*/)
	: CDialogPage(CDlgPgPipes::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPgPipes)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPgPipes::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPgPipes)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPgPipes, CDialogPage)
	//{{AFX_MSG_MAP(CDlgPgPipes)
	ON_BN_CLICKED(IDC_BUTTON_PIPTOWIN, OnButtonPiptowin)
	ON_BN_CLICKED(IDC_BUTTON_PIPTOFILE, OnButtonPiptofile)
	ON_BN_CLICKED(IDC_BUTTON_FILETOPIP, OnButtonFiletopip)
	ON_BN_CLICKED(IDC_BUTTON_GETPIPINF, OnButtonGetpipinf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPgPipes message handlers
void CDlgPgPipes::OnButtonGetpipinf() 
{
	// TODO: Add your control notification handler code here

	//USB_Configuration_infomation
	int ConfigCnt = theApp.m_pUSBDevice->ConfigCount();
	if(ConfigCnt == 0)
	{
		AfxMessageBox("操作失败，请复位GD4系统");
		return;
	}
	for (int c=0; c < ConfigCnt; c++) {
		CCyUSBConfig cfg = theApp.m_pUSBDevice->GetUSBConfig(c);
		for (int i=0; i<cfg.AltInterfaces; i++)
        { 
			CCyUSBInterface *ifc =cfg.Interfaces[i];
			for (int e=0; e<ifc->bNumEndpoints; e++)
           { 
			    CCyUSBEndPoint *ept =ifc->EndPoints[e+1];
	            // 显示获取的描述表
	            theApp.str = "*********************************************************************\n";
	            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	            
	            theApp.str = "the current usb's EndPoint information is:  \n";
	            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	            
	            theApp.str = "-----------------------------------------------------------  \n";
	            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	            
		        theApp.str.Format("EndPoint Descriptor:%d\n",(e+1));
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

                theApp.str = "--------------------------------\n";
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

                theApp.str.Format("bLength: 0x%x\n",ept->DscLen);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

                theApp.str.Format("bDescriptorType: %d\n",ept->DscType);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

                theApp.str.Format("bEndpointAddress: 0x%x\n",ept->Address);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

                theApp.str.Format("bmAttributes: 0x%x\n",ept->Attributes);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

                theApp.str.Format("wMaxPacketSize: %d\n",ept->MaxPktSize);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

                theApp.str.Format("bInterval: %d\n",ept->Interval);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	            
	            theApp.str = "-----------------------------------------------------------  \n";
	            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
			}
		}
	}
	            

	theApp.str = "\n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
}

/////////////////////////////////////////////////////////////////////////////////////
////// Change char to data number

int CDlgPgPipes::chartonumber(char str)
{
	int UsrData;
	switch(str)
	{
				case '0':
					UsrData = 0;
					break;
				case '1':
					UsrData = 1;
					break;
				case '2':
					UsrData = 2;
					break;
				case '3':
					UsrData = 3;
					break;
				case '4':
					UsrData = 4;
					break;
				case '5':
					UsrData = 5;
					break;
				case '6':
					UsrData = 6;
					break;
				case '7':
					UsrData = 7;
					break;
				case '8':
					UsrData = 8;
					break;
				case '9':
					UsrData = 9;
					break;
				case 'A':
				case 'a':
					UsrData = 0xa;
					break;
				case 'B':
				case 'b':
					UsrData = 0xb;
					break;
				case 'C':
				case 'c':
					UsrData = 0xc;
					break;
				case 'D':
				case 'd':
					UsrData = 0xd;
					break;
				case 'E':
				case 'e':
					UsrData = 0xe;
					break;
				case 'F':
				case 'f':
					UsrData = 0xf;
					break;
				case ' ':				// 如果有空格字符,则不做处理
				case 10:
					UsrData = 0xff;
					break;
				default:
					AfxMessageBox("有无效数据，请重新输入");
					UsrData = 0xff;
	}
	return UsrData;
}

void CDlgPgPipes::OnButtonPiptowin() 
{
	// TODO: Add your control notification handler code here
	// BULK数据的读操作
	long i = 0;
	UCHAR buffer[1024] = {0};
    long length = 512;
	if (theApp.m_pBulkInEpt != NULL)
	{
		theApp.m_pBulkInEpt->XferData(buffer,length);
	}

	/* 显示读回数据 */
	theApp.str = "*********************************************************************\n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	
	theApp.str.Format("The data read from the EP6 \n");
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("The data received is from  0x%x to 0x%x \n",(buffer[0]&0xff),(buffer[length-1]&0xff));
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str = "-----------------------------------------------------------  \n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	
	for(i=0; i < length; i++)
	{
		if(i%8 ==7)
		{
			theApp.str.Format("  0x%x  \n",(buffer[i]&0xff));
		}
		else
		{
			theApp.str.Format("  0x%x    ",(buffer[i]&0xff));
		}
		AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	}	

	theApp.str = "\n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
}

void CDlgPgPipes::OnButtonPiptofile() 
{
	// TODO: Add your control notification handler code here
	// BULK数据的读操作
	int i=0;
	UCHAR buffer[1024] = {0};
    long length = 512;
	if (theApp.m_pBulkInEpt != NULL)
	{
		theApp.m_pBulkInEpt->XferData(buffer,length);
	}
	
	/* 选择并打开输出信息文件 */
	fstream Myoutputfile;		
	CString strpathname;
	CString filter;
	unsigned short int Data[1024];

	filter = "文本文件(*.txt)||";
	CFileDialog	FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,filter);
	
	if(FileDlg.DoModal() ==IDOK)
	{
		/* 选择文件 */
		strpathname = FileDlg.GetPathName();			
		
		/* 打开所选定的文件 */
		Myoutputfile.open(strpathname, ios::out|ios::trunc);
		if(!Myoutputfile.rdbuf( )->is_open())
		{
			AfxMessageBox("错误！所选定的输出文件不能打开，请检查文件属性");
			return;
		}
	}

	/* 将字符转换成整形数*/
	for(i=0; i<length; i++)
	{
		Data[i] = buffer[i]&0x00FF;

	}
	UCHAR c0,c1;
	CString s0,s1;
	c0 = (UCHAR)Data[0];
	s0.Format("%x",c0);
	c1 = (UCHAR)Data[length-1];
	s1.Format("%x",c1);
	/* 显示读回数据 */
	Myoutputfile.setf(ios::hex|ios::showbase|ios::uppercase);
	Myoutputfile << "*********************************************************************\n";
	Myoutputfile <<  "The data read from the endpiont EP6"<< "\n";
	Myoutputfile <<  "The data received is from "<<s0<<" to "<<s1<<".\n";
	Myoutputfile <<  "-----------------------------------------------------------  \n";
	
	for(i=0; i<length; i++)
	{
		c0 = (UCHAR)Data[i];
		s0.Format("%x",c0);
		if(i%8 ==7)
		{
			
			Myoutputfile<<"    "<<s0<<endl;
		}
		else
		{
			Myoutputfile<<"    "<<s0;			
		}
	}	
	Myoutputfile.close();
}

void CDlgPgPipes::OnButtonFiletopip() 
{
	// TODO: Add your control notification handler code here
		//BULK数据的写操作
	LONG j = 0;
	LONG i=0;
	UCHAR Databuffer[1024] = {0};
	UCHAR buffer[1024]={0};
	LONG length = 0;
    
	
	fstream Mydatafile;		
	CString strpathname;
	CString filter;
	char Temp1,Temp2;
	LONG Datalen;							// 数据长度

	filter = "文本文件(*.txt)||";
	CFileDialog	FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,filter);
	
	if(FileDlg.DoModal() ==IDOK)
	{
		/* 选择文件 */
		strpathname = FileDlg.GetPathName();			
		
		/* 打开所选定的文件 */
		Mydatafile.open(strpathname, ios::in|ios::_Nocreate);
		if(!Mydatafile.rdbuf( )->is_open())
		{
			AfxMessageBox("错误！所选定的数据文件不能打开，命令无效");
			return;
		}
		
		/* 读取文件的数据内容 */
		i = 0;
        do
		{
			Mydatafile.read(&Temp1,1);	
			if((Temp1!=13)&&(Temp1!=9)&&(Temp1!=32)&&(Temp1!=10)&&(Temp1!=-52))	// 如果输入的字符不是换行,TAB,空格键
			{
				Databuffer[i] = chartonumber(Temp1);
				i++;
			}
		}
		while(!Mydatafile.eof());
	}

	else							// 如果取消选择文件,则退出
	{
		return;
	}

	/* 关闭文件 */
	Mydatafile.close();
	
	/* 如果是空文件,给出文件为空信息 */
	if(i==0)
	{
		AfxMessageBox("错误！ 所选定数据文件内容为空");
		return;
	}	
	
	/* 因为最后一个数据是重复的,应该除去 */
	Datalen = i-1;	
	
	/* 对接收到的数据进行整字节化处理 */
	if((Datalen%2) != 0)		// 如果是奇数个半字节
	{
		Databuffer[Datalen] = 0;
		Datalen++;
	}
	
	/* 将获取的数字组成数据字节 */			
	for(i=0,j=0; i<Datalen/2; i++,j++)
	{
		Temp1 = Databuffer[2*j];
		Temp2 = Databuffer[2*j+1];
		Databuffer[i] = Temp2+Temp1* (0x10);
	}

	/* 发送字节长度 */
	length = Datalen;

	if (theApp.m_pBulkOutEpt != NULL)
	{
		theApp.m_pBulkOutEpt->XferData(Databuffer,length);
	} else {
		AfxMessageBox("错误！ 请连接并打开GD4-USB2.0");
	}

	/* 显示发送的数据 */
	theApp.str = "*********************************************************************\n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	
	theApp.str.Format("The data transfered from file to the EP6\n");
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("The data transfered is from  %X to %X \n",(Databuffer[0]&0xff),(Databuffer[length-1]&0xff));
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str = "-----------------------------------------------------------  \n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	
	for(i=0; i<length; i++)
	{
		if(i%6 ==5)
		{
			theApp.str.Format("  %X  \n",(Databuffer[i]&0xff));
		}
		else
		{
			theApp.str.Format("  %X    ",(Databuffer[i]&0xff));
		}
		AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	}	

	theApp.str = "\n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
}
