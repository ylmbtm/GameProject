// DlgSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "TestClient.h"
#include "DlgSelect.h"
#include "CommandHandler.h"


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
	DDX_Control(pDX, IDC_LIST1, m_CharList);
}


BEGIN_MESSAGE_MAP(CDlgSelect, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSelect::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSelect 消息处理程序

BOOL CDlgSelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CharList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_CharList.InsertColumn(0, "角色名", LVCFMT_LEFT, 100);
	m_CharList.InsertColumn(1, "等级", LVCFMT_LEFT, 100);
	m_CharList.InsertColumn(2, "性别", LVCFMT_LEFT, 100);
	m_CharList.InsertColumn(3, "职业", LVCFMT_LEFT, 100);
	m_CharList.InsertColumn(4, "脸型", LVCFMT_LEFT, 100);

	CHAR szValue[64]={0};

	
	for(int i = 0; i < m_nCount; i++)
	{
		sprintf(szValue, "%d", m_CharInfoList[i].dwLevel);
		m_CharList.InsertItem(i, m_CharInfoList[i].szCharName);
		m_CharList.InsertItem(i, szValue);
		m_CharList.InsertItem(i, "男");
		m_CharList.InsertItem(i, "法师");
		m_CharList.SetItemData(i, i);
	}

	return TRUE; 
}

void CDlgSelect::OnBnClickedOk()
{
	int nIndex = m_CharList.GetSelectionMark();
	if(nIndex < 0)
	{
		return ;
	}

	int SelData = m_CharList.GetItemData(nIndex);

	CClientCmdHandler::GetInstancePtr()->SendPickCharReq(m_CharInfoList[SelData].u64CharID);

	OnOK();
}
