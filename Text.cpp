//Text.cpp

#include "Text.h"
Text::Text(){
	
	this->row = 0;
	this->column = 0;
	this->current = 0;
	
}

Text::Text(const Text& source){
	
	this->row = source.row;
	this->column = source.column;
	this->current = source.current;

}

Text::~Text(){

}


Long Text::Write(Long index, char object){

	if (index == this->string.GetLength()){
	
		if (this->string.GetLength() + 1 >= this->string.GetCapacity()){
		
			this->current = this->string.AppendFromRear(object);
		}
		else{
		
			this->current = this->string.Store(index, object);
		
		}

	}

	else if (index < this->string.GetLength()){
		this->current = this->string.Insert(index, object);
	}


	this->column++; //열을 늘려준다.

	if (object == '\n'){ //newline을 만나면
	
		this->row++;  //행을 증가시킨다.
		this->column = 0; //열을 초기화시킨다.
	}
	else if (object == '\r'){ //리턴캐리지를 만나면
	
		this->column--; //열을 감소시킨다.
	}

	return this->current;
}



Long Text::Delete(Long index){
	if (index < this->string.GetLength()){
		if (this->string.GetAt(index) == '\r'){
			this->current = this->string.Delete(index);

		}
		this->current = this->string.Delete(index);
	}
	return this->current;
}


Long Text::BackSpace(Long index){

	Long i = 0;
	Long count = 0;

	if (this->string.GetAt(index - 1) == '\n' && this->string.GetAt(index - 2) == '\r'){

		this->current = this->string.BackSpaceDelete(index);
		this->current = this->string.BackSpaceDelete(index - 1);
		this->row--;



		while (i < this->current && this->row != count){

			if (this->string.GetAt(i) == '\n'){

				count++;
			}
			i++;
		}


		while (i < this->current){

			this->column++;
			i++;
		}
	}
	else{
	
		this->current = this->string.BackSpaceDelete(index);
		this->column -= 1;
	}

	return this->current;
}


Long Text::Erase(Long startRow, Long startColumn){

	Long index = 0;
	Long count = 0;
	
	while (index < this->string.GetLength() && startRow != count){
	
		if (this->string.GetAt(index) == '\n'){
		
			count++;
		}
		index++;
	}
	index = index + startColumn;

	this->current = this->string.Erase(index, this->current);

	if (this->row > startRow){
	
		this->row = startRow;
		this->column = startColumn;
	}
	else if (this->row == startRow && this->column > startColumn){
	
		this->column = startColumn;
	}

	return this->current;
}
////////////////////////////////////////////////////////////////////////////////////
//한글
////////////////////////////////////////////////////////////////////////////////////
Long Text::HangeulWrite(Long index, char(*object)){
	Long i = 0;
	Long count = 0;

	while (object[i] != '\0'){

		count++;
		i++;
	}

	if (index == this->string.GetLength()){

	
		this->current = this->string.HangeulWrite(index, object);
		
	}
	else if (index < this->string.GetLength()){
		this->current = this->string.HangeulInsert(index, object);
	}

	/*if (this->GetLength() < this->GetCapacity() - 1){
		this->current = this->string.HangeulWrite(index, object);
	}*/

	return this->current;
}

Long Text::HangeulPaste(Long index, char(*object)){

	Long i = 0;
	Long count = 0;

	while (object[i] != '\0'){

		count++;
		i++;
	}

	if (index == this->string.GetLength()){

		if (this->string.GetLength() + count + 1 >= this->string.GetCapacity()){

			this->current = this->string.AppendFromRear(object);
		}
		else{

			this->current = this->string.Store(index, object);
		}
	}
	else if (index < this->string.GetLength()){
		this->current = this->string.HangeulInsert(index, object);
	}

	i = 0;

	while (object[i] != '\0'){

		this->column++;
		if (object[i] == '\n'){

			this->row++;
			this->column = 0;
		}
		else if (object[i] == '\r'){
			this->column--;

		}
		i++;
	}

	return this->current;
}

Long Text::HangeulDelete(Long index){
	if (index < this->string.GetLength()){
		if (this->string.GetAt(index) == '\r'){
			this->current = this->string.HangeulDelete(index);

		}
		this->current = this->string.HangeulDelete(index);
	}
	return this->current;
}
////////////////////////////////////////////////////////////////////////////////////
//한글
////////////////////////////////////////////////////////////////////////////////////

Long Text::Paste(Long index, char(*object)){

	Long i = 0;
	Long count = 0;



	while (object[i] != '\0'){
	
		count++;
		i++;
	}

	if (index == this->string.GetLength()){
	
		if (this->string.GetLength() + count + 1 >= this->string.GetCapacity()){
		
			this->current = this->string.AppendFromRear(object);
		}
		else{
		
			this->current = this->string.Store(index, object);
		}
	}
	else if (index < this->string.GetLength()){
		this->current = this->string.Insert(index, object);
	}

	i = 0;

	while (object[i] != '\0'){
	
		this->column++;
		if (object[i] == '\n'){
		
			this->row++;
			this->column = 0;
		}
		else if(object[i]=='\r'){
			this->column--;
		
		}
		i++;
	}

	return this->current;
}


char* Text::Copy(Long startRow, Long startColumn){
	Long i = 0;
	Long j;
	Long flag = 0;
	Long index = 0;
	char(*temp)=0;

	while (i < this->string.GetLength() && flag != startRow){
		if (this->string.GetAt(i) == '\n'){
			flag++;
		}
		index++;
		i++;
	}

	index = index + startColumn;

	if (this->current > index) {
		temp = new char[this->current - index + 1];
		i = 0;
		j = index;
		while (j < this->current) {
			temp[i] = this->string.GetAt(j);
			i++;
			j++;
		}
	}

	else if (this->current < index) {
		temp = new char[index - this->current + 1];
		i = 0;
		j = this->current;
		while (j < index) {
			temp[i] = this->string.GetAt(j);
			i++;
			j++;
		}
	}

	temp[i] = '\0';

	return temp;
}

char* Text::Cut(Long startRow, Long startColumn){
	Long i = 0;
	Long j;
	Long flag = 0;
	Long index = 0;
	char(*temp)=0;

	while (i < this->string.GetLength() && flag != startRow){
		if (this->string.GetAt(i) == '\n'){
			flag++;
		}
		index++;
		i++;
	}

	index = index + startColumn;

	if (this->current > index) {
		temp = new char[this->current - index + 1];
		i = 0;
		j = index;
		while (j < this->current) {
			temp[i] = this->string.GetAt(j);
			i++;
			j++;
		}
	}

	else if (this->current < index) {
		temp = new char[index - this->current + 1];
		i = 0;
		j = this->current;
		while (j < index) {
			temp[i] = this->string.GetAt(j);
			i++;
			j++;
		}
	}

	temp[i] = '\0';

	this->current = this->string.Erase(index, this->current);

	if (this->row > startRow) {

		this->row = startRow;
		this->column = startColumn;
	}
	else if (this->row == startRow && this->column > startColumn) {

		this->column = startColumn;
	}

	return temp;
}



void Text::Search(char(*object), Long *(*indexes), Long *count){

	//this->string.LinearSearchDuplicate(object,indexes,count,compare)



}


Long Text::Modify(Long index, char object){

	this->current=this->string.Modify(index, object);

	this->column++;
	return this->current;

}


Long Text::Position(Long row, Long column){

	Long current = 0;
	Long i = 0;
	Long flag = 0;

	while (i < this->string.GetLength() && flag != row){
	
		if (this->string.GetAt(i) == '\n'){
		
			flag++;
		}
		current++;
		i++;
	}
	i = 0;

	while (i < column){
	
		current++;
		i++;
	}
	//this->current = current;
	this->current = this->string.Position(current);
	this->row = row;
	this->column = column;

	return this->current;
}


Long Text::GetRowLength(Long row){
	Long i = 0;
	Long count = 0;
	Long length = 0;

	while (i < this->string.GetLength() && row != count){
		
		if (this->string.GetAt(i) == '\n'){
			count++;
		}

		i++;
	}

	while (this->string.GetAt(i) != '\r' && this->string.GetAt(i) != '\0'){
		length++;
		i++;
	}

	return length;
}


void Text::Clear(){

	this->string.Clear();
	this->row = 0;
	this->column = 0;
	this->current = 0;
}

Text& Text::operator=(const Text& source){

	this->row = source.row;
	this->column = source.column;
	this->current = source.current;

	return *this;
}

char& Text::GetAt(Long index){
	return this->string.GetAt(index);
}


Long Text::GetMaxRow(){
	Long i = 0;
	Long count = 0;

	while( i < this->string.GetLength()){
		if(this->string.GetAt(i) == '\n'){
			count++;
		}
		i++;
	}
	return count;
}
