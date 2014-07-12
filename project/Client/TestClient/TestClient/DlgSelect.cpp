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
	, m_strNewName(_T(""))
{

}

CDlgSelect::~CDlgSelect()
{
}

void CDlgSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CharList);
	DDX_Text(pDX, IDC_EDT_NAME, m_strNewName);
}


BEGIN_MESSAGE_MAP(CDlgSelect, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSelect::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_NEW_CHAR, &CDlgSelect::OnBnClickedBtnNewChar)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDlgSelect::OnNMRClickList1)
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

	RefreshListCtrl();

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

void CDlgSelect::OnBnClickedBtnNewChar()
{
	UpdateData(TRUE);

	if(m_strNewName.IsEmpty())
	{
		MessageBox("name can not be empty!");
		return ;
	}

	CClientCmdHandler::GetInstancePtr()->SendNewCharReq(m_dwAccountID, (LPCTSTR)m_strNewName , 123);

	OnOK();
}

BOOL CDlgSelect::AddCharPickInfo( StCharPickInfo &PickInfo )
{
	if(m_nCount >= 4)
	{
		return FALSE;
	}

	m_CharInfoList.push_back(PickInfo);

	m_nCount++;

	return TRUE;
}

BOOL CDlgSelect::RefreshListCtrl()
{
	m_CharList.DeleteAllItems();

	CHAR szValue[64]={0};
	int i = 0;
	for(std::vector<StCharPickInfo>::iterator itor = m_CharInfoList.begin(); itor != m_CharInfoList.end(); itor++)
	{
		sprintf(szValue, "%d", itor->dwLevel);
		m_CharList.InsertItem(i, itor->szCharName);
		m_CharList.SetItemText(i, 1, szValue);
		m_CharList.SetItemText(i, 2, "男");
		m_CharList.SetItemText(i, 3,"法师");
		m_CharList.SetItemData(i, i);
		i++;
	}

	return TRUE;
}


void CDlgSelect::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if(pNMItemActivate->iItem == -1)
	{
		return ;
	}

	POINT pt;
	GetCursorPos(&pt);

	CMenu Menu;
	Menu.CreatePopupMenu();
	Menu.AppendMenu(MF_STRING, 101, "删除");
	int nValue = Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, this);
	if(nValue == 101 )
	{
		StCharPickInfo &Info = m_CharInfoList[pNMItemActivate->iItem];

		CClientCmdHandler::GetInstancePtr()->SendDelCharReq(m_dwAccountID, Info.u64CharID);
		
		OnOK();
	}

	

	

	*pResult = 0;
}

BOOL CDlgSelect::DelChar( UINT64 u64CharID )
{
	for(std::vector<StCharPickInfo>::iterator itor = m_CharInfoList.begin(); itor != m_CharInfoList.end(); ++itor)
	{
		if(itor->u64CharID = u64CharID)
		{
			m_CharInfoList.erase(itor);
			break;
		}
	}

	return TRUE;
}
