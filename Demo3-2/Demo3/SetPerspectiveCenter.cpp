// SetPerspectiveCenter.cpp: 实现文件
//

#include "stdafx.h"
#include "Demo3.h"
#include "SetPerspectiveCenter.h"
#include "afxdialogex.h"


// SetPerspectiveCenter 对话框

IMPLEMENT_DYNAMIC(SetPerspectiveCenter, CDialogEx)

SetPerspectiveCenter::SetPerspectiveCenter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SET_CENTER, pParent)
	, CX(0)
	, CY(0)
	, CZ(0)
{

}

SetPerspectiveCenter::~SetPerspectiveCenter()
{
}

void SetPerspectiveCenter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, CX);
	DDX_Text(pDX, IDC_EDIT_Y, CY);
	DDX_Text(pDX, IDC_EDIT_Z, CZ);
}


BEGIN_MESSAGE_MAP(SetPerspectiveCenter, CDialogEx)
END_MESSAGE_MAP()


// SetPerspectiveCenter 消息处理程序
