// DlgPgDevreq.cpp : implementation file
//

#include "stdafx.h"
#include "USBDEMO.h"
#include "DlgPgDevreq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPgDevreq dialog

CDlgPgDevreq::CDlgPgDevreq(CWnd* pParent /*=NULL*/)
	: CDialogPage(CDlgPgDevreq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPgDevreq)
	m_strdirection = _T("");
	m_strrecipient = _T("");
	m_strtype = _T("");
	m_brequest = _T(0);
	m_windex = _T(0);
	m_wvalue = _T(0);
	m_strbuffer = _T("12345678");
	m_buffersize = _T(8);
	//}}AFX_DATA_INIT
	//m_strdirection = _T("1:IN");
	//m_strrecipient = _T("2:Endpoint");
	//m_strtype = _T("1:Class");
}


void CDlgPgDevreq::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPgDevreq)
	DDX_Control(pDX, IDC_EDIT_WVALUE, m_ctrlwvalue);
	DDX_Control(pDX, IDC_EDIT_WINDEX, m_ctrlwindex);
	DDX_Control(pDX, IDC_EDIT_BUFFER_SIZE, m_ctrlbuffersize);
	DDX_Control(pDX, IDC_EDIT_BUFFER, m_ctrlbuffer);
	DDX_Control(pDX, IDC_EDIT_BREQUEST, m_ctrlbrequest);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ctrltype);
	DDX_Control(pDX, IDC_COMBO_RECIPIENT, m_ctrlrecipient);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_ctrldirection);
	DDX_CBString(pDX, IDC_COMBO_DIRECTION, m_strdirection);
	DDX_CBString(pDX, IDC_COMBO_RECIPIENT, m_strrecipient);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_strtype);
	DDX_Text(pDX, IDC_EDIT_BREQUEST, m_brequest);
	DDX_Text(pDX, IDC_EDIT_WINDEX, m_windex);
	DDX_Text(pDX, IDC_EDIT_WVALUE, m_wvalue);
	DDX_Text(pDX, IDC_EDIT_BUFFER, m_strbuffer);
	DDV_MaxChars(pDX, m_strbuffer, 130);
	DDX_Text(pDX, IDC_EDIT_BUFFER_SIZE, m_buffersize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPgDevreq, CDialogPage)
	//{{AFX_MSG_MAP(CDlgPgDevreq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPgDevreq message handlers

BOOL CDlgPgDevreq::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctrldirection.AddString(_T("0:DIR_TO_DEVICE"));
	m_ctrldirection.AddString(_T("1:DIR_FROM_DEVICE"));

	m_ctrlrecipient.AddString(_T("0:TGT_DEVICE"));
	m_ctrlrecipient.AddString(_T("1:TGT_INTFC"));
	m_ctrlrecipient.AddString(_T("2:TGT_ENDPT"));
	m_ctrlrecipient.AddString(_T("3:TGT_OTHER"));
    
	m_ctrltype.AddString(_T("0:REQ_STD"));
	m_ctrltype.AddString(_T("1:REQ_CLASS"));
	m_ctrltype.AddString(_T("2:REQ_VENDOR"));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
///////                Change input sting to data 
int CDlgPgDevreq::CStringToHex(CString strData,char *pUchBuff,int nMaxLen)
{
	int nLen=0;
	char chData;
	byte bHData,bLData;
	int nCol=0;
	int nTotalLen=strData.GetLength();
	int nFlag=0;
	for (int i=0;i<nTotalLen;i++)
	{
		if (i>=nMaxLen)
		{
			break;
		}
		chData=strData.GetAt(i);
		if (chData!=' ')
		{
			nFlag++;
		}
		if (chData>='0'&&chData<='9')
		{
			if (nCol<1)
			{
				bHData=chData-'0';
			}
			else
			{
				bLData=chData-'0';
			}
			nCol++;
		}
		else if (chData>='a'&&chData<='f')
		{
			if (nCol<1)
			{
				bHData=chData-'a'+10;
			}
			else
			{
				bLData=chData-'a'+10;
			}
			nCol++;
		}
		else if (chData>='A'&&chData<='F')
		{
			if (nCol<1)
			{
				bHData=chData-'A'+10;
			}
			else
			{
				bLData=chData-'A'+10;
			}
			nCol++;
		}
		if (nCol>1)
		{
			nCol=0;
			bHData = bHData<<4;//*16;//
			pUchBuff[nLen]=bHData|bLData;
			bHData=0;
			bLData=0;
			nLen++;
		}
	}
	if (nFlag%2!=0)
	{
		if (nLen<=nMaxLen)
		{
			bHData = bHData<<4;//*16;//
			pUchBuff[nLen]=bHData|bLData;
			nLen++;
		}
	}
	return nLen;
}

void CDlgPgDevreq::OnOK() 
{
	// TODO: Add extra validation here	
	int i = 0;
	int recnBytes;
	unsigned char buffer[64];
	LONG buflen = 0;
	CString str;
	UpdateData(TRUE);
	UpdateData(FALSE);

	ZeroMemory(buffer, 64);
	if(theApp.m_pUSBDevice == NULL)
	{
		AfxMessageBox("操作失败，请打开USB设备");
		return;
	}
	CCyControlEndPoint  *ept = theApp.m_pUSBDevice->ControlEndPt;
	m_intdirection = m_ctrldirection.GetCurSel();
	m_intrecipient = m_ctrlrecipient.GetCurSel();
	m_inttype = m_ctrltype.GetCurSel();
	
	ept->Target    = (CTL_XFER_TGT_TYPE)m_intrecipient;
	ept->ReqType   = (CTL_XFER_REQ_TYPE)m_inttype;
	ept->Direction = (CTL_XFER_DIR_TYPE)m_intdirection;
	ept->ReqCode   = m_brequest;
	ept->Value     = m_wvalue;
	ept->Index     = m_windex;

	if(ept->Direction == DIR_TO_DEVICE) {
		//strtodata();
		CStringToHex( m_strbuffer,m_Endpoint0data,m_buffersize);
		buflen = m_buffersize/2;
		for(i = 0 ;i < buflen; i++)
	    {
		    buffer[i] = m_Endpoint0data[i];
	    }
	    ept->Write(buffer,buflen);
		/* 在输出窗口中显示接收到的数据 */
		theApp.str = "*********************************************************************\n";
		AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

		theApp.str = "The data that transfer to the Endpoint0 as following:  \n";
		AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

		theApp.str = "-------------------------------------------------------  \n";
		AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
		if(buflen !=0)
		{
			for(i = 0;i<buflen;i++)
			{
				theApp.str.Format("0x%x    ",buffer[i]);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
			}

			theApp.str = "\n";
			AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
		} else {
			theApp.str.Format("Write failed");
			AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
		}
	}
	if(ept->Direction == DIR_FROM_DEVICE) {
		
	    ept->Read(buffer,buflen);
		recnBytes = buflen;
		/* 在输出窗口中显示接收到的数据 */
		if(recnBytes !=0)
		{
			theApp.str = "*********************************************************************\n";
			AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

			theApp.str = "The data that recieved from the Endpoint0 as following:  \n";
			AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

			theApp.str = "-------------------------------------------------------  \n";
			AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
			for(i = 0;i<recnBytes;i++)
			{
				theApp.str.Format("0x%x    ",buffer[i]);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
			}

			theApp.str = "\n";
			AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
		}
	}
	
}

