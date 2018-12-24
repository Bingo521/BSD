// ShowPicture.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC测试、.h"
#include "ShowPicture.h"
#include "afxdialogex.h"


// ShowPicture 对话框

IMPLEMENT_DYNAMIC(ShowPicture, CDialogEx)

ShowPicture::ShowPicture(CWnd* pParent /*=NULL*/)
	: CDialogEx(ShowPicture::IDD, pParent)
{
	img = NULL;
}

ShowPicture::~ShowPicture()
{

}

void ShowPicture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ShowPicture, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ShowPicture 消息处理程序


void ShowPicture::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CDC *pdc = GetDC();
	
	if(img!=NULL)
		img->Draw(pdc->m_hDC,0,0);
	
	this->ReleaseDC(pdc);
}
void ShowPicture::setImg(CImage *img)
{
	this->img = img;
	MoveWindow(0,0,img->GetWidth(),img->GetHeight());
}

void ShowPicture::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Invalidate(true);
	CDialogEx::OnTimer(nIDEvent);
}
