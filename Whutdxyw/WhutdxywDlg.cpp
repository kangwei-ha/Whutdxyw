
// WhutdxywDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Whutdxyw.h"
#include "WhutdxywDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWhutdxywDlg 对话框



CWhutdxywDlg::CWhutdxywDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WHUTDXYW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWhutdxywDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_XUEHAO, m_editXuehao);
	DDX_Control(pDX, IDC_EDIT_MIMA, m_editMima);
	DDX_Control(pDX, IDC_LIST1, m_userList);
}

BEGIN_MESSAGE_MAP(CWhutdxywDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CWhutdxywDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CWhutdxywDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_DELETEROW, &CWhutdxywDlg::OnBnClickedButtonDeleterow)
END_MESSAGE_MAP()


// CWhutdxywDlg 消息处理程序

BOOL CWhutdxywDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWhutdxywDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWhutdxywDlg::OnPaint()
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
HCURSOR CWhutdxywDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWhutdxywDlg::OnBnClickedButtonAdd()
{
	m_enterVec.lock();

	int currentCount = m_userList.GetItemCount();

	CString xuehao;
	m_editXuehao.GetWindowTextW(xuehao);
	CString mima;
	m_editMima.GetWindowTextW(mima);

	CString xuhaocstr;
	xuhaocstr.Format(_T("%d"), currentCount + 1);

	int nRow = m_userList.InsertItem(currentCount, xuhaocstr);
	m_userList.SetItemText(nRow, 1, xuehao);
	m_userList.SetItemText(nRow, 2, L"");
	m_userList.SetItemText(nRow, 3, L"");
	m_userList.SetItemText(nRow, 4, L"");
	m_userList.SetItemText(nRow, 5, L"正在加载");

	dxyw::Student* stu = new dxyw::Student();
	stu->SetInfo(nRow, WCharToChar(xuehao), WCharToChar(mima));
	stu->Register(std::bind(&CWhutdxywDlg::SetValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	stu->DoAll();

	m_studyStudents.push_back(stu);

	m_enterVec.unlock();
}


void CWhutdxywDlg::OnBnClickedButtonRefresh()
{
	m_enterVec.lock();

	for (int i = 0; i < m_studyStudents.size(); i++) {
		if (!m_studyStudents[i]->Refresh()) {
			dxyw::Student* stu = new dxyw::Student();
			m_studyStudents[i]->GetInfo(stu);
			stu->DoAll();
			delete m_studyStudents[i];
			m_studyStudents[i] = stu;
		}
	}

	m_enterVec.unlock();
}


void CWhutdxywDlg::OnBnClickedButtonDeleterow()
{
	m_enterVec.lock();

	CString str;
	for (int i = 0; i < m_userList.GetItemCount(); i++)
	{
		if (m_userList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			for (int j = i + 1; j < m_userList.GetItemCount(); j++)
			{
				CString itemText = m_userList.GetItemText(j, 0);

				int xuhaoo = _ttoi(itemText);
				itemText.Format(_T("%d"), xuhaoo - 1);

				m_userList.SetItemText(j, 0, itemText);

				m_studyStudents[j]->SubNum();
			}

			delete m_studyStudents[i];
			m_studyStudents.erase(m_studyStudents.begin() + i);
			m_userList.DeleteItem(i); //根据索引删除

			break;
		}
	}

	m_enterVec.unlock();
}

int CWhutdxywDlg::Init() {
#ifdef _DEBUG
	AllocConsole();
	SetConsoleTitle(L"debug console");
	freopen("CONOUT$", "w", stdout);
#endif

	/*
		初始化列表显示
	*/
	//为列表视图控件添加全行选中和栅格风格
	m_userList.SetExtendedStyle(m_userList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_SHOWSELALWAYS);
	//为列表视图控件添加列

	CRect temRect;
	m_userList.GetViewRect(&temRect);
	//temRect.Width();

	m_userList.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 50, /*表格宽度*/ 0);
	m_userList.InsertColumn(1, _T("学号"), LVCFMT_CENTER, 120, 1);
	m_userList.InsertColumn(2, _T("姓名"), LVCFMT_CENTER, 80, 2);
	m_userList.InsertColumn(3, _T("在线时长"), LVCFMT_CENTER, 80, 3);
	m_userList.InsertColumn(4, _T("是否考试"), LVCFMT_CENTER, 80, 4);
	m_userList.InsertColumn(5, _T("当前状态"), LVCFMT_CENTER, 160, 5);

	std::thread t(&CWhutdxywDlg::AutoRefresh, this);
	t.detach();

	return 1;
}

void CWhutdxywDlg::OnCancel()
{
	int mBR;
	mBR = MessageBoxA(this->GetSafeHwnd(), "是否关闭窗口？", "提示", MB_OKCANCEL);
	if (mBR == IDOK) {

		PostQuitMessage(0);
		CDialog::OnCancel();
	}
}

int CWhutdxywDlg::SetValue(int r, int c, CString content) {
	m_userList.SetItemText(r, c, content);

	return 1;
}

int CWhutdxywDlg::AutoRefresh() {
	while (true)
	{
		OnBnClickedButtonRefresh();
		Sleep(AutoRefreshTime);
	}
}
