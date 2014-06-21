#pragma once


// CDlgGame 对话框

class CDlgGame : public CDialog
{
	DECLARE_DYNAMIC(CDlgGame)

public:
	CDlgGame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGame();

// 对话框数据
	enum { IDD = IDD_Dlg_Game };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

public:
	VOID  DrawPlayer(HDC hDC, int nX, int nY, char *szName);

	VOID  DrawNpc(HDC hDC,int nX, int nY, char *szName);

	VOID  DrawItem(HDC hDC,int nX, int nY);

	VOID  Draw(HDC hDC,int nX, int nY, COLORREF clr, int nRadius);

	afx_msg void OnSize(UINT nType, int cx, int cy);
};
