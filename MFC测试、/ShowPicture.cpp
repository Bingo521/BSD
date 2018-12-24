// ShowPicture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC���ԡ�.h"
#include "ShowPicture.h"
#include "afxdialogex.h"


// ShowPicture �Ի���

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


// ShowPicture ��Ϣ�������


void ShowPicture::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Invalidate(true);
	CDialogEx::OnTimer(nIDEvent);
}
