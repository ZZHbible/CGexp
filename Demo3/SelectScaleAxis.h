#pragma once


// SelectScaleAxis 对话框

class SelectScaleAxis : public CDialogEx
{
	DECLARE_DYNAMIC(SelectScaleAxis)

public:
	SelectScaleAxis(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SelectScaleAxis();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCALE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int whichSelected;
};
