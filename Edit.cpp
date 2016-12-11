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
	//�����κ� �ʱ�ȭ
	this->x = 0;
	this->y = 0;
	this->current = 0;
	this->startX = -1;
	this->startY = -1;
	this->startIndex = 0;
	//ī�� �Ѱ͵��� ������ ���� �ʱ�ȭ
	this->copyString = NULL;
	this->startCopy = false;
	this->isClicked = false;

	//����� ������ִ°��� �ʱ�ȭ ������
	this->startMakeBlock = false;
	this->bComp = false;
	//�ؽ�Ʈ �Ҵ�
	this->text = new Text;

	//ī�� ���� + ī�� �����ֱ�
	CreateSolidCaret(2, 15);
	SetCaretPos(CPoint((this->x) * 6, (this->y) * 15));
	ShowCaret();

	return 0;
}


void Edit::OnPaint(){
	//�׸� �׸��� �غ�
	PAINTSTRUCT Ps;
	CDC *pDC = BeginPaint(&Ps);
	//��Ʈ�� ������
	CFont myfont;
	myfont.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����ü");
	CFont *pOldFont=pDC->SelectObject(&myfont);

	Long i = 0;
	Long j = 0;
	Long textX = 0;
	Long textY = 0;

//���ڸ� �����ִ� �۾�
	char(*line) = NULL;
	line = new char[256];

	//text�� length��ŭ �ݺ��Ѵ�.
	while (i < this->text->GetLength()){
		//���� ĳ������ �������� �ƴϸ�
		if (this->text->GetAt(i) != '\r' && this->text->GetAt(i) != '\n'){
			//line�� ��� �־��ش�.
			line[j] = this->text->GetAt(i);
			line[j + 1] = '\0';
			j++;
		}

		//���� �ٲ��
		else if (this->text->GetAt(i) == '\r'){
			j = 0;
		}

		else if (this->text->GetAt(i) == '\n'){
			delete[] line;
			line = new char[256];
			line[0] = '\0';
			textY += 15;
		}
	
		//�� �پ� �����ش�.
		pDC->TextOut(textX, textY, CString(line));
		i++;
	}

	//������� ������ ĳ����
	char(*block) = NULL;
	block = new char[256];

//���� �����ִ� �۾�
	if (this->startMakeBlock == true && this->startIndex != this->current){
		Long index = this->startIndex;
		Long currentIndex = this->current;
		Long temp = 0;
		i = 0;
		textX = 0;
		textY = 0;

		//������ �������� �����ش�.
		pDC->SetBkColor(RGB(0, 0, 250));
		pDC->SetTextColor(RGB(255, 255, 255));

		//�տ������� ����� ������� ��
		if (index < currentIndex){
			//���� ���� ��ǥ�� ���Ѵ�.
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
			//���� ����� �� ����
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
		//�ڿ������� ����� ������� ��
		else{
			//���� ���� ��ǥ�� ���Ѵ�.
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
			//���� ����� �� ����
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
	text.Format("x��ǥ:(%d), y��ǥ:(%d), edit������ġ:(%d), ��ŸƮX:(%d), ��ŸƮY:(%d), ��ŸƮ�ε���:(%d)", 
		this->x, this->y, this->current, this->startX, this->startY, this->startIndex);
	pDC->TextOut(200, 300, text);
	CString text2;
	text2.Format("text��뷮:(%d), text�Ҵ緮:(%d), text������ġ:(%d), text��:(%d), text��:(%d)", 
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
			//String �̵�
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret �̵�
			if (this->y > 0) {
				this->y -= 1;				
			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);				
			}
			//String �̵�
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
			//Caret �̵�
			if (this->y < this->text->GetMaxRow()) {
				this->y += 1;//String �̵�
			this->current = this->text->Position(this->y, this->x);
			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);//String �̵�
			this->current = this->text->Position(this->y, this->x);
			}			
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret �̵�
			if (this->y < this->text->GetMaxRow()) {
				this->y += 1;				
			}
			if (this->x > this->text->GetRowLength(this->y)) {
				this->x = this->text->GetRowLength(this->y);				
			}
			//String �̵�
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
			
			//String �̵�
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret �̵�
			if (this->x > 0) {
				this->x -= 1;
			}else if (this->x == 0 && this->y > 0) {
				this->y -= 1;
				this->x = this->text->GetRowLength(this->y);
			}
			//String �̵�
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
			//String �̵�
			this->current = this->text->Position(this->y, this->x);
		}
		else {
			this->startMakeBlock = false;
			this->startCopy = false;
			//Caret �̵�
			if (this->x < this->text->GetRowLength(this->y)) {
				this->x += 1;
			}
			else if (this->x == this->text->GetRowLength(this->y) && this->y < this->text->GetMaxRow()) {
				this->y +=1;
				this->x = 0;
			}
			//String �̵�
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
	
	//���ڰ� ��
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
//�ѱ� �Է�
////////////////////////////////////////////////////////////////////////////////////////////////

LONG Edit::OnHangeulComposition(UINT wParam, LPARAM lParam){

	DWORD letterLength = 0; //composition ����
	char *letterComposition = NULL; //composition �޸�
	LPARAM compositionFlag = (LPARAM)lParam;
	HIMC hIMC;

	//�⺻ IME�� ����Ѵ�.
	hIMC = ImmGetContext(GetSafeHwnd());
	if (hIMC != NULL){

		//���ڸ� �������� ��
		if (compositionFlag == 24600){

			//composition�� ���̸� ���Ѵ�.
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
			
			//������ ��� �ƴ��� �˷��ֱ����ؼ�
			this->process = compositionFlag;

		}
		
		//������ �Ϸ�Ǹ�
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

		//������ ��� �ƴ��� �˷��ֱ����ؼ�
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
//�ѱ� �Է�
////////////////////////////////////////////////////////////////////////////////////////////////


void Edit::OnLButtonDown(UINT nFlags, CPoint point){
//���콺 ���� ��ư�� ������ ��

	//1.1 x�� y�� ��ǥ�� �°� ���Ѵ�.
	this->x = point.x/6;
	this->y = point.y/15;
	
	//1.2 �� �� ���� Ŭ�� ���� �ʾ��� ��
	 //1.2.1. ���� ������ ��ġ�� y�� ��ġ��Ų��. 
	 if(this->y > this->text->GetMaxRow()){
		 this->y = this->text->GetMaxRow();
	 }
	 //1.2.2. ���������� ������ ��ġ�� x�� ��ġ��Ų��.
	 if( this->x > this->text->GetRowLength(this->y)){
		 this->x = this->text->GetRowLength(this->y);
	 }

	this->startX = this->x;
	this->startY = this->y;
	
	//1.3. ��ǥ�� ������ġ�� ������ġ�� ��ȯ��Ų��.
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
//���콺 ���� ��ư�� ���� ��

	//1.1 x�� y�� ��ǥ�� �°� ���Ѵ�.
	this->x = point.x / 6;
	this->y = point.y / 15;

	//1.2 �� �� ���� Ŭ�� ���� �ʾ��� ��
	 //1.2.1. ���� ������ ��ġ�� y�� ��ġ��Ų��. 
	if (this->y > this->text->GetMaxRow()){
		this->y = this->text->GetMaxRow();
	}
	 //1.2.2. ���������� ������ ��ġ�� x�� ��ġ��Ų��.
	if (this->x > this->text->GetRowLength(this->y)){
		this->x = this->text->GetRowLength(this->y);
	}

	//1.3 ���콺�� Ŀ���� ����������
	if (this->x == this->startX && this->y == this->startY) {
		this->startCopy = false;
		this->startMakeBlock = false;
		this->startIndex = this->text->Position(this->y, this->x);		
	}
	//1.4 ���콺 Ŀ���� �������� �ʾ�����
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

