// DlgGame.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "DlgGame.h"
#include "CommandHandler.h"


// CDlgGame 对话框

IMPLEMENT_DYNAMIC(CDlgGame, CDialog)

CDlgGame::CDlgGame(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGame::IDD, pParent)
{

}

CDlgGame::~CDlgGame()
{
}

void CDlgGame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGame, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgGame 消息处理程序

void CDlgGame::OnPaint()
{
	CPaintDC dc(this);

	CClientCmdHandler *pClientCmdHandler = CClientCmdHandler::GetInstancePtr();
	if(pClientCmdHandler == NULL)
	{
		return ;
	}

	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(&rc, RGB(255,255,255));
	
	DrawPlayer(dc.GetSafeHdc(), pClientCmdHandler->m_HostPlayer.m_ObjectPos.x, pClientCmdHandler->m_HostPlayer.m_ObjectPos.z, pClientCmdHandler->m_HostPlayer.m_szObjectName);

	for(CPlayerObjectMgr::iterator itor = pClientCmdHandler->m_PlayerObjMgr.begin(); itor != pClientCmdHandler->m_PlayerObjMgr.end(); ++itor)
	{
		CPlayerObject *pObject = itor->second;

		DrawPlayer(dc.GetSafeHdc(),  pObject->m_ObjectPos.x, pObject->m_ObjectPos.z, pObject->m_szObjectName);

		//DrawItem(dc.GetSafeHdc(), rand()%300,rand()%300);

		//DrawNpc(dc.GetSafeHdc(), rand()%300,rand()%300);
	}

	return ;
}

void CDlgGame::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

VOID CDlgGame::DrawPlayer( HDC hDC, int nX, int nY , char *szName)
{
	RECT Rc;
	Rc.left = nX - 20;
	Rc.right = nX + 20;
	Rc.top   = nY - 20;
	Rc.bottom = nY+10;
	DrawText(hDC, szName,strlen(szName), &Rc, DT_CENTER);
	Draw(hDC, nX, nY, RGB(255, 0 , 0), 5);
	return ;
}

VOID CDlgGame::DrawNpc( HDC hDC,int nX, int nY , char *szName)
{
	RECT Rc;
	Rc.left = nX - 20;
	Rc.right = nX + 20;
	Rc.top   = nY - 20;
	Rc.bottom = nY+10;
	DrawText(hDC, szName, strlen(szName), &Rc, DT_CENTER);
	Draw(hDC, nX, nY, RGB(0 ,255,  0), 5);
	return ;
}

VOID CDlgGame::DrawItem( HDC hDC,int nX, int nY )
{
	Draw(hDC, nX, nY, RGB(0 ,  0, 255), 7);
	return ;
}

VOID CDlgGame::Draw( HDC hDC,int nX, int nY, COLORREF clr, int nRadius )
{
	CPoint point;

	MoveToEx(hDC, nX-(nX%50)-50, nY-(nY%50)-50, &point);
	LineTo(hDC, nX-(nX%50)+100, nY-(nY%50)-50);
	LineTo(hDC, nX-(nX%50)+100, nY-(nY%50)+100);
	LineTo(hDC, nX-(nX%50)-50, nY-(nY%50)+100);
	LineTo(hDC, nX-(nX%50)-50, nY-(nY%50)-50);

	MoveToEx(hDC, nX-75, nY-75, &point);
	LineTo(hDC, nX+75, nY-75);
	LineTo(hDC, nX+75, nY+75);
	LineTo(hDC, nX-75, nY+75);
	LineTo(hDC, nX-75, nY-75);

	HBRUSH hNewBrush = CreateSolidBrush(clr);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hNewBrush);
	Ellipse(hDC, nX - nRadius, nY - nRadius, nX + nRadius, nY + nRadius);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hNewBrush);
}
