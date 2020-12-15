// SelectScaleAxis.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo3.h"
#include "SelectScaleAxis.h"
#include "afxdialogex.h"


// SelectScaleAxis 对话框

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


// SelectScaleAxis 消息处理程序
