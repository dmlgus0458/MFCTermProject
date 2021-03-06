
// MFCProjectDlg.h: 헤더 파일
//

#pragma once


// CMFCProjectDlg 대화 상자
class CMFCProjectDlg : public CDialogEx
{
private:
	CImage m_card_list[19];	//카드이미지 리스트
	char m_game_table[36];	//카드 테이블
	char m_front_back = 1;	//카드의 앞뒤를 확인
	char m_card_choice = -1; //카드의 선택을 확인
// 생성입니다.
public:
	CMFCProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	CStatic m_timer;
	CStatic m_score;
	CStatic m_hintflag;
	CStatic m_combo_level;
	int m_count = 60;//게임 제한시간 설정
	int m_hint_flag = 10;//힌트횟수 설정
	int m_total_score;//전체점수
	int m_current_score;//현재점수
	int m_cardcount = 0;//카드 짝을 맞춘 개수
	int m_card_score = 0;
	int m_combo = 0;
	int m_combo_score = 0;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
};
