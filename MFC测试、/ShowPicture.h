#pragma once


// ShowPicture �Ի���

class ShowPicture : public CDialogEx
{
	DECLARE_DYNAMIC(ShowPicture)
	
public:
	ShowPicture(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ShowPicture();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CImage *img;
public:
	void setImg(CImage *img);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
