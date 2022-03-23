#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

// MFCApplication3Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"
#include <string>
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
	DDX_Control(pDX, IDC_DATA_LIST, m_data_list);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication3Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication3Dlg::OnBnClickedButton2)
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

	CDC* p_dc = GetDC();//ReleaseDC(p_dc)로 제거

	mdcOffScreen.CreateCompatibleDC(p_dc);
	
	CString defaultPath = "비트맵파일";
	if (bmpFilePath.IsEmpty())
		bmpFilePath = defaultPath;

	hBmp = LoadImage(NULL, bmpFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	bmpOffScreen.Attach(hBmp);
	h_bmpOffScreen = GetCurrentObject(mdcOffScreen, OBJ_BITMAP);
	mdcOffScreen.SelectObject(hBmp);
	GetObject(hBmp, sizeof(BITMAP), &m_bmp_info);
	mdcOffScreen.SelectObject(&bmpOffScreen);
	//bmpOffScreen.GetBitmap(&m_bmp_info);//읽은 비트맵의 정보를 저장한다
	GetObject(h_bmpOffScreen, sizeof(BITMAP), &m_bmp_info);
	CImage dest_image;
	CBitmap* pBitmap=CBitmap::FromHandle(dest_image);
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
	
	
	dest_image.ReleaseDC();
	DeleteDC(h_dc);
	ReleaseDC(p_dc);
	mdcOffScreen.DeleteDC();
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



void CMFCApplication3Dlg::OnBnClickedButton1()
{
	int length = 24;
	FILE* p_file = NULL;
	wchar_t temp[24];
	int line = 1;
	CString str;
	bmpFilePath = L"cpp";
	if (0 == fopen_s(&p_file, bmpFilePath, "wb")) {
		while (length == 24) {
			total_str = "";//total_str.Empty();
			length = fread(temp, 1, 24, p_file);
			if (length > 0) {
				total_str.Format("%06d:  ", line++);

				for (int i = 0; i < 24; i++) {
					str.Format("%02d ", temp[i]);
					total_str += str;
				}
				/*wchar_t* unicode_data = (wchar_t*)total_str.GetBuffer(total_str.GetLength());
				total_str.Replace("\r\n", "\n");
				int len = WideCharToMultiByte(CP_ACP, 0, unicode_data, -1, NULL, 0, NULL, NULL);
				char* p = new char[len];
				WideCharToMultiByte(CP_ACP, 0, unicode_data, -1, p, len, NULL, NULL);
				fwrite(total_str, length, 1, p_file);
				delete[] p;
				delete[] unicode_data;*/
				m_data_list.InsertString(-1,total_str);

			}
		}
		fclose(p_file);
	}
}



void CMFCApplication3Dlg::OnBnClickedButton2()
{
	size_t cn;

	wchar_t* unicode_data = (wchar_t*)total_str.GetBuffer(total_str.GetLength());

	FILE* p_write_file = NULL;
	if (0 == fopen_s(&p_write_file, "jin.dat", "wb")) {
		total_str.Replace("\r\n", "\n");
		/*LPCSTR lpcstr = (LPCSTR)(LPCSTR)total_str;
		char* text = (char*)lpcstr;
		mbstowcs_s(&cn, unicode_data, length, text, length);*/
		//unicode_data = total_str.GetBuffer(total_str.GetLength());
		int len = WideCharToMultiByte(CP_ACP, 0, unicode_data, -1, NULL, 0, NULL, NULL);
		char* p = new char[len];
		WideCharToMultiByte(CP_ACP, 0, unicode_data, -1, p, len, NULL, NULL);
		fwrite(p, len, 1, p_write_file);
		delete[] p;
		fclose(p_write_file);
	}
	//fwrite(raw_image, sizeof(unsigned char) * image_size, 1, p_file);

	delete[] unicode_data;
}
