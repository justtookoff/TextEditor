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
	//정수부분
	Long x;
	Long y;
	Long current;
	Long startX;
	Long startY;
	Long startIndex;
	//카피 한것을 저장
	char(*copyString);

	bool isClicked;
	bool startCopy;
	//블럭을 위한
	bool startMakeBlock;
	bool bComp;
	//조립중인지 조립이 끝났는지
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
