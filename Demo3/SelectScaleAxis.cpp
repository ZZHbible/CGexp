// SelectScaleAxis.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Demo3.h"
#include "SelectScaleAxis.h"
#include "afxdialogex.h"


// SelectScaleAxis �Ի���

IMPLEMENT_DYNAMIC(SelectScaleAxis, CDialogEx)

SelectScaleAxis::SelectScaleAxis(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SCALE, pParent)
	, whichSelected(0)
{

}

SelectScaleAxis::~SelectScaleAxis()
{
}

void SelectScaleAxis::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_X, whichSelected);
}


BEGIN_MESSAGE_MAP(SelectScaleAxis, CDialogEx)
END_MESSAGE_MAP()


// SelectScaleAxis ��Ϣ�������
