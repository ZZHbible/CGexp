#pragma once


// SetPerspectiveCenter 对话框

class SetPerspectiveCenter : public CDialogEx
{
	DECLARE_DYNAMIC(SetPerspectiveCenter)

public:
	SetPerspectiveCenter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SetPerspectiveCenter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SET_CENTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float CX;
	float CY;
	float CZ;
};
