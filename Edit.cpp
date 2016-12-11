//Edit.cpp
#include "Edit.h"
#include "Text.h"
#include <imm.h>
#include <afxmsg_.h>

#define IsSHIFTPressed() ( 0x8000 ==(GetKeyState(VK_SHIFT) & 0x8000 ))
#define IsCTRLPressed()  ( 0x8000 ==(GetKeyState(VK_CONTROL) & 0x8000 ))

#pragma warning(disable:4996)
#pragma comment(lib, "Imm32")

BEGIN_MESSAGE_MAP(Edit, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_MESSAGE(WM_IME_COMPOSITION, OnHangeulComposition)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

Edit::Edit(){
	
}

Edit::~Edit(){

}


int Edit::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);
	//정수부분 초기화
	this->x = 0;
	this->y = 0;
	this->current = 0;
	this->startX = -1;
	this->startY = -1;
	this->startIndex = 0;
	//카피 한것들을 저장할 곳을 초기화
	this->copyString = NULL;
	this->startCopy = false;
	this->isClicked = false;

	//블락을 만들어주는것을 초기화 시켜줌
	this->startMakeBlock = false;
	this->bComp = false;
	//텍스트 할당
	this->text = new Text;

	//카렛 설정 + 카렛 보여주기
	CreateSolidCaret(2, 15);
	SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
	ShowCaret();

	return 0;
}


void Edit::OnPaint(){
	//그림 그리기 준비
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);
	//폰트를 설정함
	CFont myfont;
	myfont.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	CFont *pOldFont=pDC->SelectObject(&myfont);

	Long i = 0;
	Long j = 0;
	Long textX = 0;
	Long textY = 0;

//글자를 보여주는 작업
	char(*line) = NULL;
	line = new char[256];

	//text의 length만큼 반복한다.
	while (i < this->text->GetLength()){
		//리턴 캐리지나 뉴라인이 아니면
		if (this->text->GetAt(i) != '\r' && this->text->GetAt(i) != '\n'){
			//line에 계속 넣어준다.
			line[j] = this->text->GetAt(i);
			line[j + 1] = '\0';
			j++;
		}

		//줄이 바뀌면
		else if (this->text->GetAt(i) == '\r'){
			j = 0;
		}

		else if (this->text->GetAt(i) == '\n'){
			delete[] line;
			line = new char[256];
			line[0] = '\0';
			textY += 15;
		}
	
		//한 줄씩 보여준다.
		pDC->TextOut(textX, textY, CString(line));
		i++;
	}

	//블락으로 엮어줄 캐릭터
	char(*block) = NULL;
	block = new char[256];

//블럭을 보여주는 작업
	if (this->startMakeBlock == true && this->startIndex != this->current){
		Long index = this->startIndex;
		Long currentIndex = this->current;
		Long temp = 0;
		i = 0;
		textX = 0;
		textY = 0;

		//색깔을 반전으로 입혀준다.
		pDC->SetBkColor(RGB(0, 0, 250));
		pDC->SetTextColor(RGB(255, 255, 255));

		//앞에서부터 블락을 만들었을 때
		if (index < currentIndex){
			//블럭의 시작 좌표를 구한다.
			while (i < index){
				if (this->text->GetAt(i) == '\r'){
					textY += 15;
				}
				else if (this->text->GetAt(i) == '\n'){
					textX = 0;
				}
				else if (this->text->GetAt(i) != '\r' && this->text->GetAt(i) != '\n'){
					textX += 6;
				}
				i++;
			}

			j = 0;
			//블럭이 생기는 곳 까지
			while (index < currentIndex){

				if (this->text->GetAt(index) != '\r' && this->text->GetAt(index) != '\n'){
					block[j] = this->text->GetAt(index);
					block[j + 1] = '\0';
					j++;
					pDC->TextOut(textX, textY, CString(block));
				}

				else if (this->text->GetAt(index) == '\r'){
					delete[] block;
					block = new char[256];
					j = 0;
					textY += 15;
					textX = 0;
				}

				index++;
			}
		}
		//뒤에서부터 블락을 만들었을 때
		else{
			//블럭의 시작 좌표를 구한다.
			while (i < currentIndex){
				if (this->text->GetAt(i) == '\r'){
					textY += 15;
				}
				else if (this->text->GetAt(i) == '\n'){
					textX = 0;
				}
				else if (this->text->GetAt(i) != '\r' && this->text->GetAt(i) != '\n'){
					textX += 6;
				}
				i++;
			}

			j = 0;
			//블럭이 생기는 곳 까지
			while (currentIndex < index){

				if (this->text->GetAt(currentIndex) != '\r' && this->text->GetAt(currentIndex) != '\n'){
					block[j] = this->text->GetAt(currentIndex);
					block[j + 1] = '\0';
					j++;
					pDC->TextOut(textX, textY, CString(block));
				}

				else if (this->text->GetAt(currentIndex) == '\r'){
					delete[] block;
					block = new char[256];
					j = 0;
					textY += 15;
					textX = 0;
				}

				currentIndex++;
			}
		}
	
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString text;
	text.Format("x자표:(%d), y좌표:(%d), edit현재위치:(%d), 스타트X:(%d), 스타트Y:(%d), 스타트인덱스:(%d)", 
		this->x, this->y, this->current, this->startX, this->startY, this->startIndex);
	pDC->TextOut(200, 300, text);
	CString text2;
	text2.Format("text사용량:(%d), text할당량:(%d), text현재위치:(%d), text행:(%d), text열:(%d)", 
		this->text->GetLength(), this->text->GetCapacity(), this->text->GetCurrent(), this->text->GetRow(), this->text->GetColumn());
	pDC->TextOut(200, 350, text2);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ShowCaret();
	
	ReleaseDC(pDC);

	EndPaint(&Ps);

	if (line != NULL){
		delete[] line;
	}

	if (block != NULL){
		delete[] block;
	}
}


void Edit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	
	switch (nChar) {
	case VK_UP:
		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);;
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			if (this->y > 0) {
				this->y -= 1;
			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);
			}
			//String 이동
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret 이동
			if (this->y > 0) {
				this->y -= 1;				
			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);				
			}
			//String 이동
			this->startX = this->x;
			this->startY = this->y;
			this->startIndex = this->text->Position(this->y, this->x);;

			this->current = this->text->Position(this->y, this->x);
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case VK_DOWN:
		if (IsSHIFTPressed()== TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);
				this->startMakeBlock = true;
				this->startCopy = true;			
			}
			//Caret 이동
			if (this->y < this->text->GetMaxRow()) {
				this->y += 1;//String 이동
			this->current = this->text->Position(this->y, this->x);
			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);//String 이동
			this->current = this->text->Position(this->y, this->x);
			}			
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret 이동
			if (this->y < this->text->GetMaxRow()) {
				this->y += 1;				
			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);				
			}
			//String 이동
			this->startX = this->x;
			this->startY = this->y;
			this->startIndex = this->text->Position(this->y, this->x);;

			this->current = this->text->Position(this->y, this->x);
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case VK_LEFT:
		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);;
				this->startMakeBlock = true;
				this->startCopy = true;
			}
	

			if (this->x > 0) {
				this->x -= 1;
			}
			else if (this->x == 0 && this->y > 0) {
				this->y -= 1;
				this->x = this->text->GetRowLength(this->y);
			}
			
			//String 이동
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret 이동
			if (this->x > 0) {
				this->x -= 1;
			}else if (this->x == 0 && this->y > 0) {
				this->y -= 1;
				this->x = this->text->GetRowLength(this->y);
			}
			//String 이동
			this->startX = this->x;
			this->startY = this->y;
			this->startIndex = this->text->Position(this->y, this->x);;

			this->current = this->text->Position(this->y, this->x);
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case VK_RIGHT:
		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->x);;
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			if (this->x < this->text->GetRowLength(this->y)) {
				this->x += 1;
			}
			else if (this->x == this->text->GetRowLength(this->y) && this->y < this->text->GetMaxRow()) {
				this->y += 1;
				this->x = 0;
			}
			//String 이동
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret 이동
			if (this->x < this->text->GetRowLength(this->y)) {
				this->x += 1;
			}
			else if (this->x == this->text->GetRowLength(this->y) && this->y < this->text->GetMaxRow()) {
				this->y +=1;
				this->x = 0;
			}
			//String 이동
			this->startX = this->x;
			this->startY = this->y;
			this->startIndex = this->text->Position(this->y, this->x);;

			this->current = this->text->Position(this->y, this->x);
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case 'A':
		if (IsCTRLPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startMakeBlock = true;

				this->startY = 0;
				this->startX = 0;
				this->startIndex = 0;

				this->y = this->text->GetMaxRow();
				this->x = this->text->GetRowLength(this->y);

				this->startMakeBlock = true;
				this->startCopy = true;

				this->current = this->text->Position(this->y, this->x);

				SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
				this->Invalidate();

			}
		}

		
		break;
	case 'C':
		if (IsCTRLPressed() == TRUE) {
			//if (this->startCopy == true) {
				if (this->copyString != NULL) {
					delete[] this->copyString;
				}
				this->copyString = this->text->Copy(this->startY, this->startX);
				this->y = this->text->GetRow();
				this->x = this->text->GetColumn();
				this->current = this->text->Position(this->y, this->x);
			//}
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case 'X':
		if (IsCTRLPressed() == TRUE) {
			//if (this->startCopy == true) {
				if (this->copyString != NULL) {
					delete[] this->copyString;
				}
				this->copyString = this->text->Cut(this->startY, this->startX);
				this->y = this->text->GetRow();
				this->x = this->text->GetColumn();
				this->current = this->text->Position(this->y, this->x);

				this->startY = this->y;
				this->startX = this->x;
				this->startIndex = this->text->Position(this->y, this->x);
				this->startMakeBlock = false;
			}
		//}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case 'V':
		if (IsCTRLPressed() == TRUE) {
			if (this->copyString != NULL) {
				if (this->startMakeBlock == true) {
					this->x = this->startX;
					this->y = this->startY;

					this->current = this->text->Position(this->y, this->x);
					this->startMakeBlock = false;
					this->startCopy = false;

				}
				else {
					this->current = this->text->Paste(this->current, this->copyString);
					this->y = this->text->GetRow();
					this->x = this->text->GetColumn();

					//this->startY = this->y;
					//this->startX = this->x;
					this->startIndex = this->text->Position(this->y, this->x);
				}
			}
			
		}

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case VK_HOME:
		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startY = this->y;
				this->startIndex = this->text->Position(this->y, this->startX);;
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			else {
				this->startMakeBlock = false;
				this->startCopy = false;
			}
		}
		this->x = 0;
		this->current = this->text->Position(this->y, this->x);

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case VK_END:
		if (IsSHIFTPressed() == TRUE) {
			if (this->startMakeBlock == false) {
				this->startX = this->x;
				this->startIndex = this->text->Position(this->y, this->startX);;
				this->startMakeBlock = true;
				this->startCopy = true;
			}
			else {
				this->startMakeBlock = false;
				this->startCopy = false;
			}
		}
		this->x = this->text->GetRowLength(this->y);
		this->current = this->text->Position(this->y, this->x);

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();

		break;
	case VK_BACK:
		if (this->isClicked == false && this->startMakeBlock == false) {
			
			this->current = this->text->BackSpace(this->current);
		
			
			this->y = this->text->GetRow();
			this->x = this->text->GetColumn();

		}
		else if (this->isClicked == false && this->startMakeBlock == true) {
			this->current = this->text->Erase(this->startY, this->startX);
			this->y = this->text->GetRow();
			this->x = this->text->GetColumn();
			this->startMakeBlock = FALSE;
		}

		this->current = this->text->Position(this->y, this->x);
		
		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		break;
	case VK_DELETE:
		if (this->isClicked == false && this->startMakeBlock == false) {
		
			this->text->Delete(this->current);
		
		}
		else if (this->isClicked == false && this->startMakeBlock == true) {
			this->text->Erase(this->startY, this->startX);

			this->y = this->text->GetRow();
			this->x = this->text->GetColumn();

			this->startMakeBlock = false;
		}

		this->current = this->text->Position(this->y, this->x);
				
		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		this->Invalidate();
		break;
	default:
		break;
	}

}


void Edit::OnKeyUP(UINT nChar, UINT nRepCnet, UINT nFlags) {

}


void Edit::OnChar(UINT nChar, UINT nRepcnt, UINT nFlags){

	switch (nChar) {
	case VK_RETURN:
		if (this->startMakeBlock == true) {
			this->text->Erase(this->startY, this->startX);

			this->y = this->text->GetRow();
			this->x = this->text->GetColumn();

			this->startMakeBlock = false;
		}

		this->text->Write(this->text->GetCurrent(), '\r');
		this->text->Write(this->text->GetCurrent(), '\n');
		this->x = 0;
		(this->y)++;
		this->current = this->text->Position(this->y, this->x);

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
		ShowCaret();
		this->Invalidate();
		break;
	case VK_BACK:
		break;
	
	//글자가 들어감
	default:
		if (TRUE != IsCTRLPressed()) {
			if (this->startMakeBlock == true) {
				this->startMakeBlock = false;
			}
			if (this->startCopy == true) {
				this->startCopy = false;
			}
			this->text->Write(this->text->GetCurrent(), nChar);
			(this->x)++;
			this->current = this->text->Position(this->y, this->x);
			
			this->bComp = false;

			SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
			this->Invalidate();
		}
		break;
	}
	
	
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//한글 입력
////////////////////////////////////////////////////////////////////////////////////////////////

LONG Edit::OnHangeulComposition(UINT wParam, LPARAM lParam){

	DWORD letterLength = 0; //composition 길이
	char *letterComposition = NULL; //composition 메모리
	LPARAM compositionFlag = (LPARAM)lParam;
	HIMC hIMC;

	//기본 IME를 취득한다.
	hIMC = ImmGetContext(GetSafeHwnd());
	if (hIMC != NULL){

		//문자를 조립중일 때
		if (compositionFlag == 24600){

			//composition의 길이를 구한다.
			letterLength = ImmGetCompositionString(hIMC, GCS_COMPSTR, NULL, 0);
			letterComposition = new char[letterLength + 1];
			ImmGetCompositionString(hIMC, GCS_COMPSTR, letterComposition, letterLength);
			ImmReleaseContext(GetSafeHwnd(), hIMC);
			
			if (this->startMakeBlock == true) {
				this->startMakeBlock = false;
			}
			if (this->startCopy == true) {
				this->startCopy = false;
			}
			
			if (this->bComp == true){
				this->text->Delete(this->text->GetCurrent());
				this->text->Delete(this->text->GetCurrent());
			}

			if (letterLength == 0){
				this->bComp = false;
			}
			else{
				this->bComp = true;
			}

			this->current = this->text->HangeulWrite(this->text->GetCurrent(), letterComposition);
			
			//조립이 어떻게 됐는지 알려주기위해서
			this->process = compositionFlag;

		}
		
		//조립이 완료되면
		else if (compositionFlag == 2048){
		letterLength = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);
		letterComposition = new char[letterLength + 1];
		ImmGetCompositionString(hIMC, GCS_RESULTSTR, letterComposition, letterLength);
		ImmReleaseContext(GetSafeHwnd(), hIMC);

		if (this->startMakeBlock == true) {
			this->startMakeBlock = false;
		}
		if (this->startCopy == true) {
			this->startCopy = false;
		}

		if (this->bComp == true){
			this->text->Delete(this->text->GetCurrent());
			this->text->Delete(this->text->GetCurrent());
		}

		this->bComp = false;

		this->current = this->text->HangeulComplete(this->text->GetCurrent(), letterComposition);
		this->x = this->x + 2;

		this->current = this->text->Position(this->y, this->x);

		SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));

		//조립이 어떻게 됐는지 알려주기위해서
		this->process = compositionFlag;
		}


		this->Invalidate();

		if (letterComposition != NULL){
			delete[] letterComposition;
		}

	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//한글 입력
////////////////////////////////////////////////////////////////////////////////////////////////


void Edit::OnLButtonDown(UINT nFlags, CPoint point){
//마우스 왼쪽 버튼을 눌렀을 때

	//1.1 x와 y값 좌표에 맞게 구한다.
	this->x = point.x/6;
	this->y = point.y/15;
	
	//1.2 그 줄 안을 클릭 하지 않았을 떄
	 //1.2.1. 행의 마지막 위치에 y를 위치시킨다. 
	 if(this->y > this->text->GetMaxRow()){
		 this->y = this->text->GetMaxRow();
	 }
	 //1.2.2. 마지막행의 마지막 위치에 x를 위치시킨다.
	 if( this->x > this->text->GetRowLength(this->y)){
		 this->x = this->text->GetRowLength(this->y);
	 }

	this->startX = this->x;
	this->startY = this->y;
	
	//1.3. 좌표를 시작위치와 현재위치로 변환시킨다.
	this->startIndex = this->text->Position(this->startY, this->startX);
	this->current = this->text->Position(this->y, this->x);

	this->startMakeBlock = true;
	this->isClicked = true;

	SetCaretPos(CPoint((this->x)*6, (this->y)*15));
	this->Invalidate();
	
}


void Edit::OnMouseMove(UINT nFlags, CPoint point){

	if(this->startMakeBlock == true && this->isClicked == true){
		this->x = point.x/6;
		this->y = point.y/15;

		if (this->y > this->text->GetMaxRow()) {
			this->y = this->text->GetMaxRow();
		}
		if (this->x > this->text->GetRowLength(this->y)) {
			this->x = this->text->GetRowLength(this->y);
		}

		SetCaretPos(CPoint((this->x)*6, (this->y)*15));
		this->Invalidate();
	}
}


void Edit::OnLButtonUp(UINT nFlags, CPoint point) {
//마우스 왼쪽 버튼을 뗐을 때

	//1.1 x와 y값 좌표에 맞게 구한다.
	this->x = point.x / 6;
	this->y = point.y / 15;

	//1.2 그 줄 안을 클릭 하지 않았을 떄
	 //1.2.1. 행의 마지막 위치에 y를 위치시킨다. 
	if (this->y > this->text->GetMaxRow()){
		this->y = this->text->GetMaxRow();
	}
	 //1.2.2. 마지막행의 마지막 위치에 x를 위치시킨다.
	if (this->x > this->text->GetRowLength(this->y)){
		this->x = this->text->GetRowLength(this->y);
	}

	//1.3 마우스의 커서가 움직였으면
	if (this->x == this->startX && this->y == this->startY) {
		this->startCopy = false;
		this->startMakeBlock = false;
		this->startIndex = this->text->Position(this->y, this->x);		
	}
	//1.4 마우스 커서가 움직이지 않았으면
	else if (this->startMakeBlock == true && this->isClicked == true) {
		this->startCopy = true;
	}

	this->current = this->text->Position(this->y, this->x);
	this->isClicked = false;
	
	SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
	this->Invalidate();
}


void Edit::OnClose() {


	if (this->text != NULL) {

		delete this->text;
	}
	
	CFrameWnd::OnClose();
}

