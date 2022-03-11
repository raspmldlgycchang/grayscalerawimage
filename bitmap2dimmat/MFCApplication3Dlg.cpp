#define _CRT_SECURE_NO_WARNINGS

// MFCApplication3Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CMFCApplication3Dlg 대화 상자



CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCApplication3Dlg 메시지 처리기

BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CClientDC dc(this);
	mdcOffScreen.CreateCompatibleDC(&dc);
	CString defaultPath = "C:\\Users\\홍성희\\Desktop\\kykim\\Programming\\이미지뷰어만들기2주차목요일\\picSpace1\\universe.bmp";
	if (bmpFilePath.IsEmpty())
		bmpFilePath = defaultPath;
	hBmp = LoadImage(NULL, bmpFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	bmpOffScreen.Attach(hBmp);
	h_bmpOffScreen = GetCurrentObject(mdcOffScreen, OBJ_BITMAP);
	mdcOffScreen.SelectObject(hBmp);
	GetObject(hBmp, sizeof(BITMAP), &m_bmp_info);
	CImage dest_image;
	dest_image.Create(m_bmp_info.bmWidth, m_bmp_info.bmHeight, m_bmp_info.bmBitsPixel);
	BITMAP dest_bmp_info;
	GetObject((HBITMAP)dest_image, sizeof(BITMAP), &dest_bmp_info);
	//혹은
	/*int copy_size = m_bmp_info.bmWidth * m_bmp_info.bmHeight * m_bmp_info.bmBitsPixel;
	int image_size = copy_size;
	GetBitmapBits(bmpOffScreen, copy_size, dest_bmp_info.bmBits);
	char* p_temp = new char[image_size];
	GetBitmapBits(bmpOffScreen, image_size, p_temp);
	int line_size = m_bmp_info.bmWidth * m_bmp_info.bmBitsPixel;
	char* p_bmp_pattern = (char*)dest_bmp_info.bmBits;
	for (int y = m_bmp_info.bmHeight - 1; y >= 0; y--) {
		memcpy(p_bmp_pattern, p_temp + y * line_size, line_size);
		p_bmp_pattern += line_size;
	}
	delete[] p_temp;*/
	int copy_size = m_bmp_info.bmWidth * m_bmp_info.bmHeight * m_bmp_info.bmBitsPixel;
	int image_size = copy_size;
	GetBitmapBits(bmpOffScreen, copy_size, dest_bmp_info.bmBits);
	char* p_temp = new char[image_size];
	GetBitmapBits(bmpOffScreen, image_size, p_temp);
	int line_size = m_bmp_info.bmWidth * m_bmp_info.bmBitsPixel;

	h_dc = CreateCompatibleDC(NULL);
	SelectObject(h_dc, bmpOffScreen);
	BitBlt(dest_image.GetDC(), 0, 0, m_bmp_info.bmWidth, m_bmp_info.bmHeight, h_dc, 0, 0,SRCCOPY);
	unsigned char** raw_image;
	raw_image = new unsigned char* [873];
	for (unsigned int i = 0; i < m_bmp_info.bmHeight; i++) {
		*(raw_image + i) = new unsigned char[21601];
		memset(*(raw_image + i), 0, line_size + 1);
	}
	for (int y = m_bmp_info.bmHeight - 1; y > -0; y--) {
		for (int x = 0; x < m_bmp_info.bmWidth; x++) {
			raw_image[y][x] = *(p_temp + y * line_size + x);
		}
	}
	FILE* p_file;
	fopen_s(&p_file,"pointsave.dat", "wb");
	fwrite(raw_image, sizeof(unsigned char) * image_size, 1, p_file);

	if (p_file != NULL) {
		/*for (int y = m_bmp_info.bmHeight - 1; y > -0; y--) {
			for (int x = 0; x < line_size; x++) {*/
		//	}
		//	//GetDlgItem(IDC_STATIC1 + m_bmp_info.bmHeight - 1 - y)->SetWindowTextA(temp);
		//}
	}
	
	dest_image.ReleaseDC();
	DeleteDC(h_dc);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication3Dlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		dc.BitBlt(0,0, m_bmp_info.bmWidth, m_bmp_info.bmHeight, &mdcOffScreen, 0,0, SRCCOPY);

		//CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

