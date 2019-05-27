
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
	, muti_thread(_T(""))
	, m_imgPath(_T(""))
	, m_thread_num(0)
	, m_seq_num(0)
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
	DDX_CBString(pDX, IDC_COMBO2, muti_thread);
	DDX_Control(pDX, IDC_COMBO2, muti_thread_c);
	DDX_Text(pDX, IDC_EDIT1, m_imgPath);
	DDX_Text(pDX, IDC_EDIT6, m_thread_num);
	DDX_Text(pDX, IDC_EDIT7, m_seq_num);
}

BEGIN_MESSAGE_MAP(CMFC���ԡ�Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC���ԡ�Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC���ԡ�Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT4, &CMFC���ԡ�Dlg::OnEnChangeEdit4)
	ON_MESSAGE(WM_MESSOK,&CMFC���ԡ�Dlg::OnMessOK)
	ON_MESSAGE(WM_MESSOK1,&CMFC���ԡ�Dlg::OnMessOK1)
	ON_MESSAGE(WM_MESSOK2,&CMFC���ԡ�Dlg::OnMessOK2)
	ON_WM_CLOSE()
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
	mlfRate = 10;
	mStep = 2;
	miK = 400;
	mSub = 1;
	m_thread_num = 1;
	m_seq_num = 1;
	//fscanf_s(file,"%s%lf%d",buffer,&mSub,&miK);
	//fclose(file);
	
	m_thread = NULL; 
	CEdit *pEdit = (CEdit*)m_select.GetWindow(GW_CHILD);	
	pEdit->SetReadOnly(TRUE);
	pEdit->SetWindowText(TEXT("ICM"));
	m_selectValue = "ICM";
	m_select.AddString(TEXT("K-means"));
	m_select.AddString(TEXT("K-means1"));
	m_select.AddString(TEXT("GMM"));
	m_select.AddString(TEXT("GMM1"));
	m_select.AddString(TEXT("SLIC"));
	m_select.AddString(TEXT("ICM"));
	m_select.AddString(TEXT("ICM_Eigen"));
	m_select.AddString(TEXT("Test"));
	m_select.AddString(TEXT("LeftK"));
	CEdit *pEdit1 = (CEdit*)muti_thread_c.GetWindow(GW_CHILD);	
	pEdit1->SetReadOnly(TRUE);
	pEdit1->SetWindowText(TEXT("���߳�"));
	muti_thread = "���߳�";
	muti_thread_c.AddString("���߳�");
	muti_thread_c.AddString("���߳�");
	//showDlg = new ShowPicture(this);
	//showDlg->Create(IDD_DIALOG1);
	//showDlg->ShowWindow(SW_HIDE);
	FILE *file1 = NULL;

	int err = fopen_s(&file1,SAVEPATH,"r");
	if(!err){
		char buf[128];
		ReadDouble(file1,mlfRate);
		ReadInt(file1,miK);
		ReadString(file1,buf,128);
		m_selectValue = buf;
		ReadString(file1,buf,128);
		muti_thread = buf;
		ReadString(file1,buf,128);
		m_imgPath = buf;
		ReadInt(file1,m_thread_num);
		ReadInt(file1,m_seq_num);
		fclose(file1);
	}
	UpdateData(false);
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
 
	CString prePath = m_imgPath;
    if (dlgFile.DoModal())
    {
        m_imgPath = dlgFile.GetPathName();
    }
	if(m_imgPath == prePath)
		return ;	

	mTextPath.SetWindowText(m_imgPath);
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
	mPic.Load(m_imgPath);
	if (mPic.IsNull())
    {
		MessageBox(TEXT("����λͼʧ�ܣ�"),TEXT("��ʾ"),MB_OK);
		return;
    }
	m_btStart.EnableWindow(false);
	m_btStart.SetWindowText(TEXT("����ת��..."));
	//showDlg->ShowWindow(true);
	//showDlg->setImg(&img);
	img = mPic;
	fenClass = vector<int>(img.GetWidth()*img.GetHeight());
	param *pparam = new param;
	pparam->k = miK;
	pparam->des = &img;
	pparam->res = &mPic;
	pparam->path = m_imgPath;
	pparam->parent = this;
	pparam->rate = mlfRate;
	pparam->step = mStep;
	pparam->sub = mSub;
	pparam->type = m_selectValue;
	pparam->pos = CPoint(0,100);
	setPath(*pparam);
	//toDo(pparam);
	ts=clock();
	if(muti_thread == "���߳�")
		run(pparam);
	else if(muti_thread == "���߳�")
	{
		toDo(pparam);
	}
	//AfxBeginThread(run,pparam);	
	//CreateThread(NULL, 0, run,pparam, 0, NULL);
	/*unsigned *ret = new unsigned;
	*ret = 0;
	_beginthreadex(NULL,0,run,&pparam,0,ret);
	delete ret;*/
}

UINT  CMFC���ԡ�Dlg::run(LPVOID pParam)
{
	//Sleep(10000);
	param *pa = (param*)pParam;
	time_t ts,te;
	ts = clock();
	if(pa->type == "K-means")
	{
		KMeans(*pa->res,*pa->des,pa->k,pa->rate,pa->step,pa->sub);
	}
	else if(pa->type == "K-means1")
	{
		KMeans1(*pa->res,*pa->des,pa->k,pa->rate);
	}
	else if(pa->type == "GMM")
	{
		GMM(*pa->res,*pa->des,pa->k,pa->rate);
	}
	else if(pa->type == "GMM1")
	{
		GMM1(*pa->res,*pa->des,pa->k,pa->rate);
	}
	else if(pa->type == "SLIC")
	{
		int ret = SLIC(*pa->res,*pa->des,pa->k,pa->rate);
	}
	else if(pa->type == "Test")
	{
		Test(*pa->res,*pa->des,pa->k,pa->path);
	}
	else if(pa->type == "LeftK")
	{
		LeftK(*pa->res,*pa->des,pa->k,pa->path);
	}
	else if(pa->type == "ICM")
	{
		ICM(*pa->res,*pa->des,pa->k,pa->rate);
	}else if(pa->type == "ICM_Eigen")
	{
		AfxMessageBox("wait");
		return 0;
	}
	te = clock();

	double span=difftime(te,ts);
	pa->t =span;
	pa->parent->PostMessage(WM_MESSOK,(UINT)pa,0);
	CString str;
	str.Format("Done! time:%.0lfms",span);
	AfxMessageBox(str);
	return 0;
} 

UINT CMFC���ԡ�Dlg::run1(LPVOID pParam)
{
	param *pa = (param*)pParam;
	vector<vector<float>> *lab = NULL;
	int num;
		if(pa->type == "K-means")
		{
			KMeans(*pa->res,*pa->des,pa->k,pa->rate,pa->step,pa->sub);
			//sprintf_s(buffer,1024,"_%d_%.2lf_%.2lf_%.2lf",pa->k,pa->rate,pa->step,pa->sub);
		}
		else if(pa->type == "K-means1")
		{
			KMeans1(*pa->res,*pa->des,pa->k,pa->rate);
			//sprintf_s(buffer,1024,"_%d_%.2lf",pa->k,pa->rate);
		}
		else if(pa->type == "GMM")
		{
			GMM(*pa->res,*pa->des,pa->k,pa->rate);
			//sprintf_s(buffer,1024,"_%d_%.2lf",pa->k,pa->rate);
		}
		else if(pa->type == "GMM1")
		{
			GMM1(*pa->res,*pa->des,pa->k,pa->rate);
			//sprintf_s(buffer,1024,"_%d_%.2lf",pa->k,pa->rate);
		}
		else if(pa->type == "SLIC")
		{
			int ret = SLIC(*pa->res,*pa->des,pa->k,pa->rate);
			//sprintf_s(buffer,1024,"_%d_%.2lf",pa->k,pa->rate);
		}
		else if(pa->type == "Test")
		{
			Test(*pa->res,*pa->des,pa->k,pa->path);
			//sprintf_s(buffer,1024,"_%d",pa->k);
		}
		else if(pa->type == "LeftK")
		{
			LeftK(*pa->res,*pa->des,pa->k,pa->path);
			//(buffer,1024,"_%d",pa->k);
		}
		else if(pa->type == "ICM")
		{
			ICM(*pa->res,*pa->des,pa->k,pa->rate);
			//(buffer,1024,"_%d",pa->k);
		}
	pa->parent->PostMessage(WM_MESSOK1,(UINT)pa,0);
	return 0;
}

UINT CMFC���ԡ�Dlg::run2(LPVOID pParam)
{
	param *pa = (param*)pParam;
	vector<MutiSEQ> *lab = pa->p;
	int len = pa->p->size();
	for(int i = pa->thread_id ; i < len ; i+= pa->off)
	{
		MutiSEQ &thisLab = (*lab)[i];
		if(pa->type == "ICM")
			ICM(thisLab.lab,thisLab.fenClass_c,thisLab.width,thisLab.height,thisLab.k,pa->rate);
		else if(pa->type == "GMM")
			GMM2(thisLab.lab,thisLab.fenClass_c,thisLab.width,thisLab.height,thisLab.k,pa->rate);
		else if(pa->type == "K-means")
			KMeans(thisLab.lab,thisLab.fenClass_c,thisLab.width,thisLab.height,thisLab.k,pa->rate);
		else {
			AfxMessageBox("���㷨�ݲ�֧�ֶ��̣߳�");
		}
			
	}
	pa->parent->PostMessage(WM_MESSOK2,(UINT)pa,0);
	return 1;
}
void CMFC���ԡ�Dlg::toDo(param *pa)
{
	
	UpdateData(true);
	st = clock();
	int width = pa->res->GetWidth();
	int height = pa->res->GetHeight();
	int sz = width * height;
	int num = m_seq_num;
	int minK = 4;
	int thread_num = m_thread_num;
	int iK ;
	int S = sqrt(1.0 * sz / num) + 0.5;
	int wNum = width / S ;
	int hNum = height / S ;
	if(width % S > S / 2.0)
		wNum++;
	if(height % S > S / 2.0)
		hNum++;
	int oneThreadNum ;
	if(wNum == 0)
		wNum = 1;
	if(hNum == 0)
		hNum = 1;
	num = wNum * hNum;
	iK = pa->k / num;
	oneThreadNum = num / thread_num;
	vector<MutiSEQ> *lab =new vector<MutiSEQ>(num);
	
	byte * blt = (byte *)(pa->res->GetBits());
	int plt = pa->res->GetPitch();
	Image2Lab(*pa->res,Lab);
	
	cou = 0;
	allCou = thread_num;
	baseK = 0;
	int more = num % thread_num;
	if(iK < minK)
		iK = minK;
	for(int i = 0 ; i< wNum ; i++)
	{
		for(int j = 0 ; j < hNum ; j++)
		{
			int baseX = i * S;
			int baseY = j * S;
			int w = S;
			int h = S;
			int pos = i * hNum + j;
			if(width - baseX < S / 2.0 * 3)
			{
				w = width - baseX;
			}
			if(height - baseY < S / 2.0 * 3)
			{
				h = height -baseY;
			}
			int size = w * h;
			(*lab)[pos].lab = vector<vector<float>>(3,vector<float>(size));
			for(int ii = 0 ; ii < w ; ii ++)
			{
				for(int jj = 0 ;jj < h ;jj ++)
				{
					int x = baseX + ii;
					int y = baseY + jj;
					int oppos = ii * h + jj;
					int relpos = x * height + y;
					(*lab)[pos].lab[0][oppos] = Lab[0][relpos];
					(*lab)[pos].lab[1][oppos] = Lab[1][relpos];
					(*lab)[pos].lab[2][oppos] = Lab[2][relpos];
					(*lab)[pos].col = j;
					(*lab)[pos].row = i;
					(*lab)[pos].width = w;
					(*lab)[pos].height = h;
					(*lab)[pos].k = iK;
				}
			}
		}
	}
	int cou1 = 0;
	
	for(int i = 0; i < thread_num ; i++)
	{
		param * pap= new param(*pa);
		pap->thread_id = i;
		pap->off = thread_num;
		pap->res = NULL;
		pap->des = NULL;
		pap->k = iK;
		pap->p = lab;
		pap->S = S;
		thread t(run2,pap);
		t.detach();
	}
}

void CMFC���ԡ�Dlg::toDo1(param *pa)
{
	int width = pa->res->GetWidth();
	int height = pa->res->GetHeight();
	int num=9;
	int subWidth = width/3;
	int subHeight = height/3;
	int widthNum = width/subWidth;
	int heightNum = height/subHeight;
	int k = pa->k / num +1;
	cou=0;
	allCou=num;
	char buf[1024];
	for(int i=0;i<widthNum;i++)
	{
		for(int j=0;j<heightNum;j++)
		{
			CPoint pos = CPoint(i*subWidth,j*subHeight);
			int nWidth = subWidth;
			int nHeight = subHeight;
			if(i == widthNum-1){
				nWidth = width - pos.x;
			}
			if(j == heightNum -1){
				nHeight = height - pos.y;
			}
			CImage *res = new CImage;
			CImage *des = new CImage;
			GetSubImg(*pa->res,*res,pos,nWidth,nHeight);
			//sprintf_s(buf,1024,"%s,%d,%d",savePath.GetBuffer(savePath.GetLength()),pos.x,pos.y);
			//res->Save(buf);
			//AfxMessageBox(buf);
			param *pap = new param;
			*pap=*pa;
			pap->res=res;
			pap->des=des;
			pap->k=k;
			pap->pos=pos;
			
			using std::thread;
			std::thread t(run1,pap);
			t.detach();
		}
	}
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

	pa->des->Save(savePath);
	delete pa;
	m_btStart.SetWindowText(TEXT("��ʼת��"));
	m_btStart.EnableWindow(true);
	return 0;
}
LRESULT CMFC���ԡ�Dlg::OnMessOK1(WPARAM wparam,LPARAM rparam)
{
	param * pa = (param*)wparam;
	CImageDC imgDC(img);
	char buf[1024];
	CRect rect(pa->pos.x,pa->pos.y,pa->des->GetWidth()+pa->pos.x,pa->des->GetHeight()+pa->pos.y);
	POINT p;
	p.x=0;
	p.y=0;
	pa->des->BitBlt(imgDC,rect,p);
	cou++;
	//sprintf_s(buf,1024,"%d",cou);
	//pa->des->Save(GetPath(savePath,buf));
	delete pa->res;
	delete pa->des;
	delete pa;
	if(cou >= allCou)
	{
		img.Save(savePath);
		m_btStart.SetWindowText(TEXT("��ʼת��"));
		m_btStart.EnableWindow(true);
		te=clock();
		double span=difftime(te,ts);
		CString str;
		str.Format("Done! time:%.0lfms",span);
		MessageBox(str);
	}
	return 0;
}
LRESULT  CMFC���ԡ�Dlg::OnMessOK2(WPARAM wParam,LPARAM lParam)
{
	param * pa = (param*)wParam;
	int width = img.GetWidth();
	int height = img.GetHeight();
	int sz = width * height;
	vector<MutiSEQ> *lab = pa->p;
	byte * blt = (byte *)(img.GetBits());
	int plt = img.GetPitch();
	int len = pa->p->size();
	for(int i = pa->thread_id ; i < len;i+= pa->off)
	{
		MutiSEQ &seq = (*lab)[i];
		int len = seq.lab[0].size();
		int baseX = seq.row * pa->S;
		int baseY = seq.col * pa->S;
		int baseC = baseK;
		baseK+=seq.k;
		for(int j = 0 ; j < len;j++)
		{
			int opx = j / seq.height;
			int opy = j % seq.height;
			int x = baseX + opx;
			int y = baseY + opy;
			int pos = x*height + y;
			fenClass[pos] = seq.fenClass_c[j] + baseC;
		}
	}
	cou++;
	if(cou == allCou)
	{
		time_t et = clock();
		//MergeClassByColor(Lab,fenClass,width,height,baseK);
		DrawBorder1(img,fenClass,width,height);
		//DrawBorderWidthAvgColor(img,fenClass,width,height);
		img.Save(savePath);
		m_btStart.SetWindowText(TEXT("��ʼת��"));
		m_btStart.EnableWindow(true);
		delete pa;
		double span=difftime(et,st);
		CString str;
		str.Format("Done! time:%.0lfms",span);
		MessageBox(str);
	}
	return 0;
}
void CMFC���ԡ�Dlg::setPath(param & pparam)
{
	char buffer[1024];
	if(m_selectValue == "K-means")
	{
		sprintf_s(buffer,1024,"_%d_%.2lf_%.2lf_%.2lf",pparam.k,pparam.rate,pparam.step,pparam.sub);
	}
	else if(m_selectValue == "K-means1")
	{
		sprintf_s(buffer,1024,"_%d_%.2lf",pparam.k,pparam.rate);
	}
	else if(pparam.type == "GMM")
	{
		sprintf_s(buffer,1024,"_%d_%.2lf",pparam.k,pparam.rate);
	}
	else if(pparam.type == "GMM1")
	{
		sprintf_s(buffer,1024,"_%d_%.2lf",pparam.k,pparam.rate);
	}
	else if(pparam.type == "SLIC")
	{
		sprintf_s(buffer,1024,"_%d_%.2lf",pparam.k,pparam.rate);
	}
	else if(pparam.type == "Test")
	{
		sprintf_s(buffer,1024,"_%d",pparam.k);
	}
	else if(pparam.type == "LeftK")
	{
		sprintf_s(buffer,1024,"_%d",pparam.k);
	}
	else if(pparam.type == "ICM")
	{
		sprintf_s(buffer,1024,"_%d",pparam.k);
	}
	CString path = pparam.path;
	int i;
	for(i=path.GetLength();i>0;i--)
	{
		if(path[i]=='.')
		{
			break;
		}
	}
	CString left =  path.Left(i)+pparam.type+CString(buffer);
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
	savePath = path;
}

void CMFC���ԡ�Dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData(true);
	FILE *file = NULL;
	int err = fopen_s(&file,SAVEPATH,"w");
	char * p = m_selectValue.GetBuffer(m_selectValue.GetLength());
	char * th = muti_thread.GetBuffer(muti_thread.GetLength());
	char * imgPath = m_imgPath.GetBuffer(m_imgPath.GetLength());
	if(!err){
		WriteDouble(file,mlfRate);
		WriteInt(file,miK);
		WriteString(file,p);
		WriteString(file,th);
		WriteString(file,imgPath);
		WriteInt(file,m_thread_num);
		WriteInt(file,m_seq_num);
		/*fprintf_s(file,"%lf %d %s",mlfRate,miK,m_selectValue.GetBuffer(m_selectValue.GetLength()));*/
		fclose(file);
	}
	CDialogEx::OnClose();
}
