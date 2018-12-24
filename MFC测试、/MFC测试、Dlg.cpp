
// MFC���ԡ�Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC���ԡ�.h"
#include "MFC���ԡ�Dlg.h"
#include "afxdialogex.h"
#include "ShowPicture.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC���ԡ�Dlg �Ի���



CMFC���ԡ�Dlg::CMFC���ԡ�Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC���ԡ�Dlg::IDD, pParent)
	, miK(0)
	, mlfRate(0)
	, mSub(0)
	, mStep(0)
	, m_selectValue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC���ԡ�Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mTextPath);
	DDX_Control(pDX, IDC_EDIT3, mK);
	DDX_Control(pDX, IDC_EDIT2, mRate);
	DDX_Text(pDX, IDC_EDIT3, miK);
	DDX_Text(pDX, IDC_EDIT2, mlfRate);
	DDX_Text(pDX, IDC_EDIT5, mSub);
	DDX_Text(pDX, IDC_EDIT4, mStep);
	DDX_Control(pDX, IDC_BUTTON2, m_btStart);
	DDX_Control(pDX, IDC_COMBO1, m_select);
	DDX_CBString(pDX, IDC_COMBO1, m_selectValue);
}

BEGIN_MESSAGE_MAP(CMFC���ԡ�Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC���ԡ�Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC���ԡ�Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT4, &CMFC���ԡ�Dlg::OnEnChangeEdit4)
	ON_MESSAGE(WM_MESSOK,&CMFC���ԡ�Dlg::OnMessOK)
END_MESSAGE_MAP()


// CMFC���ԡ�Dlg ��Ϣ�������

BOOL CMFC���ԡ�Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	FILE * file = NULL;
	m_ini_path = "LastTime.ini";
	/*char buffer[1024];
	if(_access(m_ini_path.GetBuffer(),0) == -1)
	{
		fopen_s(&file,m_ini_path.GetBuffer(),"w+");
		fprintf_s(file,"%s\n%lf\n%d","K-means",0.5,40);
		fclose(file);
	}*/
	//fopen_s(&file,m_ini_path.GetBuffer(),"r");
	mlfRate = 0.5;
	mStep = 2;
	miK = 40;
	mSub = 1;
	//fscanf_s(file,"%s%lf%d",buffer,&mSub,&miK);
	//fclose(file);
	UpdateData(false);
	m_thread = NULL; 
	CEdit *pEdit = (CEdit*)m_select.GetWindow(GW_CHILD);	
	pEdit->SetReadOnly(TRUE);
	pEdit->SetWindowText(TEXT("K-means"));
	m_select.AddString(TEXT("K-means"));
	m_select.AddString(TEXT("K-means1"));
	m_select.AddString(TEXT("GMM"));
	m_select.AddString(TEXT("GMM1"));
	m_select.AddString(TEXT("SLIC"));
	m_select.AddString(TEXT("Test"));
	m_select.AddString(TEXT("LeftK"));
	//showDlg = new ShowPicture(this);
	//showDlg->Create(IDD_DIALOG1);
	//showDlg->ShowWindow(SW_HIDE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFC���ԡ�Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC���ԡ�Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC���ԡ�Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC���ԡ�Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
 
    CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"), NULL);
 
	CString prePath = mPath;
    if (dlgFile.DoModal())
    {
        mPath = dlgFile.GetPathName();
    }
	if(mPath == prePath)
		return ;	

	mTextPath.SetWindowText(mPath);
}
 

void CMFC���ԡ�Dlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	/*FILE *file = NULL;
	fopen_s(&file,m_ini_path.GetBuffer(),"w");
	fprintf_s(file,"%s\n%lf\n%d",m_selectValue.GetBuffer(),&mSub,&miK);
	fclose(file);*/
	if(!mPic.IsNull())
	{
		mPic.Destroy();
	}
	if(!img.IsNull())
	{
		img.Destroy();
	}
	mPic.Load(mPath);
	if (mPic.IsNull())
    {
		MessageBox(TEXT("����λͼʧ�ܣ�"),TEXT("��ʾ"),MB_OK);
		return;
    }
	m_btStart.EnableWindow(false);
	m_btStart.SetWindowText(TEXT("����ת��..."));
	//showDlg->ShowWindow(true);
	//showDlg->setImg(&img);
	   
	param *pparam = new param;
	pparam->k = miK;
	pparam->des = &img;
	pparam->res = &mPic;
	pparam->path = mPath;
	pparam->parent = this;
	pparam->rate = mlfRate;
	pparam->step = mStep;
	pparam->sub = mSub;
	pparam->type = m_selectValue;
	run(pparam);
	//AfxBeginThread(run,pparam);	
	//CreateThread(NULL, 0, run,pparam, 0, NULL);
	/*unsigned *ret = new unsigned;
	*ret = 0;
	_beginthreadex(NULL,0,run,&pparam,0,ret);
	delete ret;*/
}

UINT  CMFC���ԡ�Dlg::run(LPVOID pParam)
{
	param *pa = (param*)pParam;
	char buffer[1024];
	time_t ts,te;
	ts = clock();
	if(pa->type == "K-means")
	{
		KMeans(*pa->res,*pa->des,pa->k,pa->rate,pa->step,pa->sub);
		sprintf_s(buffer,1024,"_%d_%.2lf_%.2lf_%.2lf",pa->k,pa->rate,pa->step,pa->sub);
	}
	else if(pa->type == "K-means1")
	{
		KMeans1(*pa->res,*pa->des,pa->k,pa->rate);
		sprintf_s(buffer,1024,"_%d_%.2lf",pa->k,pa->rate);
	}
	else if(pa->type == "GMM")
	{
		GMM(*pa->res,*pa->des,pa->k,pa->rate);
		sprintf_s(buffer,1024,"_%d_%.2lf",pa->k,pa->rate);
	}
	else if(pa->type == "GMM1")
	{
		GMM1(*pa->res,*pa->des,pa->k,pa->rate);
		sprintf_s(buffer,1024,"_%d_%.2lf",pa->k,pa->rate);
	}
	else if(pa->type == "SLIC")
	{
		int ret = SLIC(*pa->res,*pa->des,pa->k,pa->rate);
		sprintf_s(buffer,1024,"_%d_%.2lf",pa->k,pa->rate);
	}
	else if(pa->type == "Test")
	{
		Test(*pa->res,*pa->des,pa->k,pa->path);
		sprintf_s(buffer,1024,"_%d",pa->k);
	}
	else if(pa->type == "LeftK")
	{
		LeftK(*pa->res,*pa->des,pa->k,pa->path);
		sprintf_s(buffer,1024,"_%d",pa->k);
	}
	te = clock();

	CString path = pa->path;
	int i;
	for(i=path.GetLength();i>0;i--)
	{
		if(path[i]=='.')
		{
			break;
		}
	}
	//path = path.Left(i)+pa->type+CString(buffer)+path.Right(path.GetLength()-i);
	CString left =  path.Left(i)+pa->type+CString(buffer);
	CString right = path.Right(path.GetLength()-i);
	CString a;
	int t = 1;
	while(true)
	{
		a.Format(TEXT("(%d)"),t);
		path = left + a + right;
		if(_access(path.GetBuffer(),0) == -1)
		{
			break;
		}
		t++;
	}
	double span=difftime(te,ts);
	pa->t =span;
	pa->des->Save(path);
	pa->parent->PostMessage(WM_MESSOK,(UINT)pa,0);
	CString str;
	str.Format("Done! time:%.0lfms",span);
	AfxMessageBox(str);
	return 0;
} 

void CMFC���ԡ�Dlg::OnEnChangeEdit4()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
LRESULT CMFC���ԡ�Dlg::OnMessOK(WPARAM wparam,LPARAM rparam)
{
	param * pa = (param*)wparam;
	delete pa;
	m_btStart.SetWindowText(TEXT("��ʼת��"));
	m_btStart.EnableWindow(true);
	return 0;
}