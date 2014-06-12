// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "DlgLogin.h"
#include "PacketDef/CommonPacket.h"
#include "NetworkMgr.h"
#include "CommandDef.h"
#include "DataBuffer/BufferHelper.h"
#include "CommandHandler.h"
#include "DataBuffer/BufferHelper.h"
#include "GameDef.h"
#include "PacketDef/ClientPacket.h"
#include "PacketDef/LoginPacket.h"

// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
	, m_strAccountName(_T(""))
	, m_strPassword(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ACCOUNT, m_strAccountName);
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlgLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CDlgLogin::OnBnClickedBtnConnect)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序

void CDlgLogin::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
}

void CDlgLogin::OnBnClickedOk()
{
	UpdateData(TRUE);

	CClientCmdHandler::GetInstancePtr()->SendLoginReq((LPCTSTR)m_strAccountName, (LPCTSTR)m_strPassword);

	OnOK();
} 


void CDlgLogin::OnBnClickedBtnConnect()
{
	CNetworkMgr::GetInstancePtr()->ConnectToServer("127.0.0.1", 7994);
}
