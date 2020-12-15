#pragma once


// SetProjCenter 对话框

class SetProjCenter : public CDialogEx
{
	DECLARE_DYNAMIC(SetProjCenter)

public:
	SetProjCenter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetProjCenter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SET_PROJ_CENTER_PERSPECTIVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float cx;
	float cy;
	float cz;
};
