// SelectRotateAxis.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo3.h"
#include "SelectRotateAxis.h"
#include "afxdialogex.h"


// SelectRotateAxis 对话框

IMPLEMENT_DYNAMIC(SelectRotateAxis, CDialogEx)

SelectRotateAxis::SelectRotateAxis(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ROTATE, pParent)
	, whichSelected(0)
{

}

SelectRotateAxis::~SelectRotateAxis()
{
}

void SelectRotateAxis::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_X, whichSelected);
}


BEGIN_MESSAGE_MAP(SelectRotateAxis, CDialogEx)
END_MESSAGE_MAP()


// SelectRotateAxis 消息处理程序
