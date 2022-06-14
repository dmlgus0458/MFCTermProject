
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
	//게임에 쓸 카드 이미지의 순서를 실제 게임 테이블에 쓸 m_game_table 배열에 저장한다
	for (int i = 0; i < 36; i++) {
		m_game_table[i] = i % 18;	//같은 카드를 짝을 맞추는 게임이기 때문에 0~17까지 전체 카드를 2번 저장해준다
	}
	srand((unsigned int)time(NULL));//rand함수 쓸 때 srand함수를 같이 써주지 않으면 '랜덤'의 패턴이 똑같이 발생해서 모든게임에 카드 배치가 같아진다
	int first_choice, second_choice, temp;//카드를 섞기위한 변수 선언
	//카드의 순서를 섞어주지 않으면 순서대로 나열되어 있기 때문에 섞어준다
	for (int i = 0; i < 35; i++) {	//카드를 36번 섞는다
		first_choice = rand() % 36;	//0~35 까지 랜덤한 수가 들어간다
		second_choice = rand() % 36;
		temp = m_game_table[first_choice];
		m_game_table[first_choice] = m_game_table[second_choice];
		m_game_table[second_choice] = temp;
	}
}

void CMFCProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DISP, m_timer);
	DDX_Control(pDX, ID_SCORE, m_score);
	DDX_Control(pDX, ID_hintflag, m_hintflag);
	DDX_Control(pDX, IDC_COMBO, m_combo_level);
}

BEGIN_MESSAGE_MAP(CMFCProjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCProjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CMFCProjectDlg::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
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
	//m_Progress_time.SetRange(0, 180);
	//m_Progress_time.SetPos(180);
	//SetTimer(3, 1000, NULL);
	CString str;
	//프로젝트안 res 폴더에 있는 카드이미지 들을 불러온다
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
		//화면에 카드를 그리는 부분
		CPaintDC dc(this);
		char index;
		for (int i = 0; i < 36; i++) {
			index = m_game_table[i] + 1;
			if (index == 0) continue;
			if (m_front_back == 0) index = 0;
			m_card_list[index].Draw(dc, (i % 6) * 46, (i / 6) * 66);
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

	if (point.x < 46 * 6 && point.y < 66 * 6)
	{
		int x = point.x / 46;
		int y = point.y / 66;
		char num = x + y * 6;

		char index = m_game_table[num] + 1;

		if (index == 0) return;

		CClientDC dc(this);
		m_card_list[index].Draw(dc, x * 46, y * 66);

		if (m_card_choice == -1) m_card_choice = num;//아직 클릭이 되지 않았을때
		else
		{
			if (m_card_choice != num)//카드가 선택되었을때
			{
				if (m_game_table[m_card_choice] == m_game_table[num])//첫번째 카드와 두번째 카드가 같을때
				{
					m_count += 5;
					m_game_table[m_card_choice] = -1;
					m_game_table[num] = -1;
					m_cardcount++;
					m_combo++;
					m_combo_score += m_combo * 1;
					m_card_score = m_cardcount * 5;
					m_current_score = m_card_score + m_combo_score;//현재점수 = 카드 스코어 + 콤보 스코어
					m_total_score = m_current_score + m_count * 10;//전체점수 = 현재점수 + 남은시간 * 10
					
				}
				else{
					m_count --;
					m_combo = 0;
				}
				m_card_choice = -1;
				m_front_back = 1;
				SetTimer(1, 300, NULL);
			}
		}
	}
	if (m_cardcount == 18) {//모든 카드가 뒤집힐경우 타이머가 멈추고 게임에 승리한다
		KillTimer(2);
		if (MessageBox(L"게임에 이겼습니다.", L"축하합니다.", MB_OK) == IDOK) {
			CString score;
			score.Format(_T("당신의 점수는 %d점 입니다."), m_total_score);
			AfxMessageBox(score);
			OnBnClickedCancel();
		}
	}
	CString current_score;
	current_score.Format(_T("점수:%d"), m_current_score);
	m_score.SetWindowTextW(current_score);
	CString current_combo;
	current_combo.Format(_T("콤보x %d"), m_combo);
	m_combo_level.SetWindowTextW(current_combo);
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
		CString lefttime;
		lefttime.Format(_T("남은시간:%d"), m_count--);
		if (m_count >= -1)
			m_timer.SetWindowTextW(lefttime);
		else
			KillTimer(2);
	}
	/*if (nIDEvent == 3)
{
	int pos = m_Progress_time.GetPos();
	if (pos > 0)
	{
		pos--;
		m_Progress_time.SetPos(pos);
	}
}*/
	if (m_count == -1) {
		if (MessageBox(L"제한시간이 초과 되었습니다. 게임을 종료합니다.", L"시간초과", MB_ICONSTOP) == IDOK) {
			OnBnClickedCancel();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMFCProjectDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_front_back) return;
	else {
		if (m_hint_flag > 0) {
			m_front_back = 1;
			Invalidate();
			SetTimer(1, 2000, NULL);
			m_hint_flag--;
		}
		else {
			MessageBox(L"더이상 힌트를 사용할수 없습니다.", MB_OK);
			return;
		}
	}
	CString current_hint;
	current_hint.Format(_T("남은힌트수:%d"), m_hint_flag);
	m_hintflag.SetWindowTextW(current_hint);
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


int CMFCProjectDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	long style = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
	style &= ~WS_CAPTION;
	::SetWindowLong(this->m_hWnd, GWL_STYLE, style);
	return 0;
}
