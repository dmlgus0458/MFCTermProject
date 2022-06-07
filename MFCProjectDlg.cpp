
// MFCProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCProject.h"
#include "MFCProjectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCProjectDlg 대화 상자



CMFCProjectDlg::CMFCProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (int i = 0; i < 36; i++) {
		m_game_table[i] = i % 18;	
	}

	srand((unsigned int)time(NULL));

	int first_choice, second_choice, temp;

	for (int i = 0; i < 35; i++) {
		first_choice = rand() % 36;	
		second_choice = rand() % 36;

		temp = m_game_table[first_choice];
		m_game_table[first_choice] = m_game_table[second_choice];
		m_game_table[second_choice] = temp;
	}
}

void CMFCProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DISP, m_staticDisp);
}

BEGIN_MESSAGE_MAP(CMFCProjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCProjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CMFCProjectDlg::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMFCProjectDlg 메시지 처리기

BOOL CMFCProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	CString str;

	for (int i = 0; i < 19; i++) {		
		str.Format(L"res/%03d.bmp", i);
		m_card_list[i].Load(str);
	}
	SetTimer(1, 2000, NULL);
	SetTimer(2, 1000, NULL);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CPaintDC dc(this);
		char index;	

		for (int i = 0; i < 36; i++) {
			index = m_game_table[i] + 1;	
										
			if (index == 0) continue;	

			if (m_front_back == 0) index = 0;	

			m_card_list[index].Draw(dc, (i % 6) * 36, (i / 6) * 56);
		}
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCProjectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
	if (m_front_back) return;		

	if (point.x < 36 * 6 && point.y < 56 * 6)
	{
		int x = point.x / 36;
		int y = point.y / 56;
		char num = x + y * 6;

		char index = m_game_table[num] + 1;

		if (index == 0) return;	

		CClientDC dc(this);
		m_card_list[index].Draw(dc, x * 36, y * 56);

		if (m_card_choice == -1) m_card_choice = num;	
		else
		{
			if (m_card_choice != num)
			{
				if (m_game_table[m_card_choice] == m_game_table[num])
				{
					m_game_table[m_card_choice] = -1;
					m_game_table[num] = -1;
				}
				m_card_choice = -1;

				m_front_back = 1;		

				SetTimer(1, 300, NULL);

			}
		}
	}
}


void CMFCProjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		KillTimer(1);	
		m_front_back = 0;	
		Invalidate();	
	}
	if (nIDEvent == 2)
	{
		CString sTxt;
		sTxt.Format(_T("%d"), m_count--);
		if (m_count >= -1)
			m_staticDisp.SetWindowTextW(sTxt);
		else
			KillTimer(2);
	}
	if(m_count == -1){
	int check = MessageBox(L"제한시간이 초과 되었습니다", L"시간초과", MB_OK);
	if (check == IDOK) {
		OnBnClickedCancel();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
	CString sTxt;
}

void CMFCProjectDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_front_back) return;	
	else{
		m_front_back = 1;
		Invalidate();
		SetTimer(1, 800, NULL);
	}
}
void CMFCProjectDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


BOOL CMFCProjectDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(0, 100, 50));

	return TRUE;
}
