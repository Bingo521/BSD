
// MFC测试、Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC测试、.h"
#include "MFC测试、Dlg.h"
#include "afxdialogex.h"
#include "ShowPicture.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFC测试、Dlg 对话框



CMFC测试、Dlg::CMFC测试、Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC测试、Dlg::IDD, pParent)
	, miK(0)
	, mlfRate(0)
	, mSub(0)
	, mStep(0)
	, m_selectValue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC测试、Dlg::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(CMFC测试、Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC测试、Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC测试、Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT4, &CMFC测试、Dlg::OnEnChangeEdit4)
	ON_MESSAGE(WM_MESSOK,&CMFC测试、Dlg::OnMessOK)
END_MESSAGE_MAP()


// CMFC测试、Dlg 消息处理程序

BOOL CMFC测试、Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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
	mlfRate = 10;
	mStep = 2;
	miK = 400;
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
	m_select.AddString(TEXT("ICM"));
	m_select.AddString(TEXT("Test"));
	m_select.AddString(TEXT("LeftK"));
	//showDlg = new ShowPicture(this);
	//showDlg->Create(IDD_DIALOG1);
	//showDlg->ShowWindow(SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC测试、Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC测试、Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC测试、Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC测试、Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
 
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
 

void CMFC测试、Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox(TEXT("加载位图失败！"),TEXT("提示"),MB_OK);
		return;
    }
	m_btStart.EnableWindow(false);
	m_btStart.SetWindowText(TEXT("正在转换..."));
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

UINT  CMFC测试、Dlg::run(LPVOID pParam)
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
	else if(pa->type == "ICM")
	{
		ICM(*pa->res,*pa->des,pa->k,pa->rate);
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

void CMFC测试、Dlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
LRESULT CMFC测试、Dlg::OnMessOK(WPARAM wparam,LPARAM rparam)
{
	param * pa = (param*)wparam;
	delete pa;
	m_btStart.SetWindowText(TEXT("开始转换"));
	m_btStart.EnableWindow(true);
	return 0;
}