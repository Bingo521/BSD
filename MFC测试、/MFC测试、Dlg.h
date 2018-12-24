
// MFC���ԡ�Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ShowPicture.h"

// CMFC���ԡ�Dlg �Ի���
class CMFC���ԡ�Dlg : public CDialogEx
{
// ����
public:
	CMFC���ԡ�Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

private:
	CString mPath;
	CImage mPic;
	ShowPicture *showDlg;
public:
	CEdit mTextPath;
	afx_msg void OnBnClickedButton2();
	CEdit mK;
	CEdit mRate;
	int miK;
	double mlfRate;
	afx_msg void OnEnChangeEdit4();
	LRESULT OnMessOK(WPARAM ,LPARAM);
	double mSub;
	CImage img;
	CWinThread * m_thread;
public:
	static UINT run(LPVOID pParam);
	double mStep;
	CButton m_btStart;
	CComboBox m_select;
	CString m_selectValue;
	CString m_ini_path;
};
struct param
{
	CImage *res;
	CImage *des;
	int k;
	double rate;
	double step;
	double sub;
	CWnd *parent;
	CString path;
	CString type;
	double t;
};