
// TestClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "TestClientDlg.h"
#include "NetworkMgr.h"
#include "PacketDef/ClientPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "CommandDef.h"
#include "CommandHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestClientDlg 对话框




CTestClientDlg::CTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LST_LOG, m_LogList);
}

BEGIN_MESSAGE_MAP(CTestClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_COMMAND(ID_Login, &CTestClientDlg::OnLogin)
	ON_WM_TIMER()
	ON_COMMAND(ID_DISCONNECT, &CTestClientDlg::OnDisconnect)
	ON_COMMAND(ID_CONNECT, &CTestClientDlg::OnConnect)
	ON_COMMAND(ID_LEAVE_GAME, &CTestClientDlg::OnLeaveGame)
END_MESSAGE_MAP()

// CTestClientDlg 消息处理程序
BOOL CTestClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_DlgGame.Create(IDD_Dlg_Game, this);

	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.left += 200;
	rcClient.right -= 2;
	rcClient.bottom -=2;
	m_DlgGame.MoveWindow(rcClient);

	CNetworkMgr::GetInstancePtr()->InitNetSystem();

	SetTimer(1, 25, NULL);

	AllocConsole();

	freopen("CONOUT$","w+t",stdout);   

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//  如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(IsWindow(m_DlgGame.GetSafeHwnd()))
	{
		CRect rcClient;
		GetClientRect(rcClient);
		rcClient.left += 200;
		rcClient.right -= 2;
		rcClient.bottom -=2;
		m_DlgGame.MoveWindow(rcClient);
		m_DlgGame.Invalidate();
	}
}

void CTestClientDlg::OnLogin()
{
	//CDlgLogin DlgLogin;

	//DlgLogin.DoModal();

	CNetworkMgr::GetInstancePtr()->ConnectToServer("127.0.0.1", 7994);
}

void CTestClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CNetworkMgr::GetInstancePtr()->OnTime();

	CDialog::OnTimer(nIDEvent);

	//HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); //本例以输出为例

	//WriteConsole(hCon, "afdsfsd", 10, NULL,NULL);

	//printf("afdsfdsfsdfsa");
}

void CTestClientDlg::OnDisconnect()
{ 
	// TODO: 在此添加命令处理程序代码
	CNetworkMgr::GetInstancePtr()->DisConnect();
}

void CTestClientDlg::OnConnect()
{
	//CNetworkMgr::GetInstancePtr()->Start();
}

void CTestClientDlg::OnLeaveGame()
{
	StCharLeaveGameReq CharLeaveGameReq;

	CharLeaveGameReq.dwLeaveReason = 1;

	CBufferHelper WriteHelper(TRUE, CNetworkMgr::GetInstancePtr()->m_pWriteBuffer);

	WriteHelper.BeginWrite(CMD_CHAR_LEAVE_GAME_REQ, CMDH_SENCE, 12, CClientCmdHandler::GetInstancePtr()->m_HostPlayer.GetObjectID());

	WriteHelper.Write(CharLeaveGameReq);

	WriteHelper.EndWrite();

	CNetworkMgr::GetInstancePtr()->SendData(CNetworkMgr::GetInstancePtr()->m_pWriteBuffer->GetData(), CNetworkMgr::GetInstancePtr()->m_pWriteBuffer->GetDataLenth());
}
