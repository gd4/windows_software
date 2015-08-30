// DlgPgDesc.cpp : implementation file
//

#include "stdafx.h"
#include "USBDEMO.h"
#include "DlgPgDesc.h"
#include "USBDEMODlg.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPgDesc dialog


CDlgPgDesc::CDlgPgDesc(CWnd* pParent /*=NULL*/)
	: CDialogPage(CDlgPgDesc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPgDesc)
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CDlgPgDesc, CDialogPage)
	//{{AFX_MSG_MAP(CDlgPgDesc)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_DESC, OnButtonDeviceDesc)
	ON_BN_CLICKED(IDC_IntfDesc, &CDlgPgDesc::OnBnClickedIntfdesc)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPgDesc message handlers

void CDlgPgDesc::OnButtonDeviceDesc() 
{
	// TODO: Add your control notification handler code here	
	//USB_DEVICE_DESCRIPTOR DevDesc;
	USB_DEVICE_DESCRIPTOR *pDevDesc = new USB_DEVICE_DESCRIPTOR;
	theApp.m_pUSBDevice->GetDeviceDescriptor(pDevDesc);
	
	if(pDevDesc== NULL)
	{
		AfxMessageBox("操作失败，请复位GD4-USB2.0系统");
		return;
	}
	
	/*显示获取的描述表*/
    
	theApp.str = "*********************************************************************\n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str = "the current usb's device descriptor is:  \n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str = "-----------------------------------------------------------  \n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Vendor ID is: 0x%x  \n",pDevDesc->idVendor);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Product ID is: 0x%x  \n",pDevDesc->idProduct);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	
	theApp.str.Format("  Device ID is: 0x%x  \n",pDevDesc->bcdDevice);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Maximum Packet Size is: 0x%x  \n",pDevDesc->bMaxPacketSize0);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Device class is: 0x%x  \n",pDevDesc->bDeviceClass);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Device sub-class is: 0x%x  \n",pDevDesc->bDeviceSubClass);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Manufacturer string index is: 0x%x  \n",pDevDesc->iManufacturer);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Product string index is: 0x%x  \n",pDevDesc->iProduct);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Serial number string index is: 0x%x  \n",pDevDesc->iSerialNumber);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	theApp.str.Format("  Number of configurations is: 0x%x  \n",pDevDesc->bNumConfigurations);
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	
	theApp.str = "\n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	delete pDevDesc;
}

void CDlgPgDesc::OnBnClickedIntfdesc()
{
	// TODO: 在此添加控件通知处理程序代码
		//USB_Configuration_infomation
	int ConfigCnt = theApp.m_pUSBDevice->ConfigCount();
	if(ConfigCnt == 0)
	{
		AfxMessageBox("操作失败，请复位GD4系统");
		return;
	}
	for (int c=0; c < ConfigCnt; c++) {
		CCyUSBConfig cfg = theApp.m_pUSBDevice->GetUSBConfig(c);

		theApp.str = "*********************************************************************\n";
	    AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	     
	    theApp.str = "the current usb's configuration information is:  \n";
	    AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

		theApp.str.Format("bLength: 0x%x\n",cfg.bLength); 
		AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

        theApp.str.Format("bDescriptorType: %d\n",cfg.bDescriptorType); 
		AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

        theApp.str.Format("wTotalLength: %d (0x%x)\n",cfg.wTotalLength,cfg.wTotalLength); 
        AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

        theApp.str.Format("bNumInterfaces: %d\n",cfg.bNumInterfaces);
        AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

        theApp.str.Format("bConfigurationValue: %d\n",cfg.bConfigurationValue); 
        AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

        theApp.str.Format("iConfiguration: %d\n",cfg.iConfiguration);
        AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

        theApp.str.Format("bmAttributes: 0x%x\n",cfg.bmAttributes); 
        AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

        theApp.str.Format("MaxPower: %d\n",cfg.MaxPower); 
		AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

		for (int i=0; i<cfg.AltInterfaces; i++)
        { 
			CCyUSBInterface *ifc =cfg.Interfaces[i];
			theApp.str = "*********************************************************************\n";
	        AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
	     
	        theApp.str = "the current usb's interface information is:  \n";
	        AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

			theApp.str.Format("Interface  Descriptor:%d\n",(i+1));
			AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

            theApp.str = "--------------------------------\n";
            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

            theApp.str.Format("bLength: 0x%x\n",ifc->bLength); 
            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

            theApp.str.Format("bDescriptorType: %d\n",ifc->bDescriptorType);
            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

            theApp.str.Format("bInterfaceNumber: %d\n",ifc->bInterfaceNumber);
            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

            theApp.str.Format("bAlternateSetting: %d\n",ifc->bAlternateSetting);
            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

            theApp.str.Format("bNumEndpoints: %d\n",ifc->bNumEndpoints);
            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

            theApp.str.Format("bInterfaceClass: %d\n",ifc->bInterfaceClass);
            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

            theApp.str = "**********************************\n";
			AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

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
	            
				theApp.str = "**********************************\n";
			    AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);

	            theApp.str = "-----------------------------------------------------------  \n";
	            AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
			}
		}
	}  
	theApp.str = "\n";
	AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_OUTPUTSTR,NULL);
}
