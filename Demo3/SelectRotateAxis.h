#pragma once


// SelectRotateAxis �Ի���

class SelectRotateAxis : public CDialogEx
{
	DECLARE_DYNAMIC(SelectRotateAxis)

public:
	SelectRotateAxis(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SelectRotateAxis();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROTATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	bool xSelected;
//	bool ySelected;
//	bool zSelected;
	int whichSelected;
};
