#pragma once


// SelectRotateAxis 对话框

class SelectRotateAxis : public CDialogEx
{
	DECLARE_DYNAMIC(SelectRotateAxis)

public:
	SelectRotateAxis(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SelectRotateAxis();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROTATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	bool xSelected;
//	bool ySelected;
//	bool zSelected;
	int whichSelected;
};
