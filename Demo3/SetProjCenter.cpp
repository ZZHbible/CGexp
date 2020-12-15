// SetProjCenter.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo3.h"
#include "SetProjCenter.h"
#include "afxdialogex.h"


// SetProjCenter 对话框

IMPLEMENT_DYNAMIC(SetProjCenter, CDialogEx)

SetProjCenter::SetProjCenter(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SET_PROJ_CENTER_PERSPECTIVE, pParent)
	, cx(0)
	, cy(0)
	, cz(-100)
{

}

SetProjCenter::~SetProjCenter()
{
}

void SetProjCenter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, cx);
	DDX_Text(pDX, IDC_EDIT_Y, cy);
	DDX_Text(pDX, IDC_EDIT_Z, cz);
}


BEGIN_MESSAGE_MAP(SetProjCenter, CDialogEx)
END_MESSAGE_MAP()


// SetProjCenter 消息处理程序
