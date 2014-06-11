// DlgSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "DlgSelect.h"


// CDlgSelect 对话框

IMPLEMENT_DYNAMIC(CDlgSelect, CDialog)

CDlgSelect::CDlgSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelect::IDD, pParent)
{

}

CDlgSelect::~CDlgSelect()
{
}

void CDlgSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSelect, CDialog)
END_MESSAGE_MAP()


// CDlgSelect 消息处理程序
