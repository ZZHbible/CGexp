#pragma once


// SelectScaleAxis �Ի���

class SelectScaleAxis : public CDialogEx
{
	DECLARE_DYNAMIC(SelectScaleAxis)

public:
	SelectScaleAxis(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SelectScaleAxis();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCALE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int whichSelected;
};
