//Edit.h
#ifndef _EDIT_H
#define _EDIT_H
#include <afxwin.h>

typedef signed long int Long;

class Text;
class Edit : public CFrameWnd {

public:
	
	Edit();
	~Edit();
	

public:

	Text *text;

private:
	//�����κ�
	Long x;
	Long y;
	Long current;
	Long startX;
	Long startY;
	Long startIndex;
	//ī�� �Ѱ��� ����
	char(*copyString);

	bool isClicked;
	bool startCopy;
	//���� ����
	bool startMakeBlock;
	bool bComp;
	//���������� ������ ��������
	LPARAM process;

protected :
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUP(UINT nChar, UINT nRepCnet, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg LONG OnHangeulComposition(UINT wParam, LPARAM lParam);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

};

#endif // !_EDIT_H
