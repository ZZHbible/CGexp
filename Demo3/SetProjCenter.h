#pragma once


// SetProjCenter �Ի���

class SetProjCenter : public CDialogEx
{
	DECLARE_DYNAMIC(SetProjCenter)

public:
	SetProjCenter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetProjCenter();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SET_PROJ_CENTER_PERSPECTIVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float cx;
	float cy;
	float cz;
};
