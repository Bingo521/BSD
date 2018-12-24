#pragma once


// ShowPicture 对话框

class ShowPicture : public CDialogEx
{
	DECLARE_DYNAMIC(ShowPicture)
	
public:
	ShowPicture(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ShowPicture();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CImage *img;
public:
	void setImg(CImage *img);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
