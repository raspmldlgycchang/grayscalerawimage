
// MFCApplication3Dlg.h: 헤더 파일
//

#pragma once


// CMFCApplication3Dlg 대화 상자
class CMFCApplication3Dlg : public CDialogEx
{
private:
	CDC mdcOffScreen;
	BITMAP m_bmp_info;
	CBitmap bmpOffScreen;
	HANDLE hBmp;
	CString bmpFilePath;
	HGDIOBJ h_bmpOffScreen;
	CImage dest_image;
	HDC h_dc;
	char m_len;
	CString total_str;

public:
	CMFCApplication3Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CListBox m_data_list;
};
