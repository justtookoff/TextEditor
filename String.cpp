//String.cpp

#include "String.h"

String::String(Long capacity){

	this->front = new char[capacity];
	this->front[0] = '\0';
	this->capacity = capacity;
	this->length = 0;
	this->current = 0;
}


String::String(const String& source)
	:front(source.front){

	this->capacity = source.capacity;
	this->length = source.length;
	this->current = source.current;
}


String::~String(){

	if (this->front != 0){
		delete[] this->front;
	}
}


Long String::Store(Long index, char object){

	this->front[index] = object;
	this->length += 1;
	this->front[index + 1] = '\0';
	this->current = this->length;

	return this->current;
}
////////////////////////////////////////////////////////
//한글
///////////////////////////////////////////////////////
Long String::HangeulStore(Long index, char object){
	this->front[index] = object;
	
	return index;
}

void String::HangeulLength(){
	this->length += 2;
}


Long String::HangeulInsertTwoByte(Long index){
	Long i = 0;
	char(*temp) = new char[64000];

	while (i < index){
		temp[i] = this->front[i];
		i++;
	}

	index = index + 2;

	while (i < this->GetLength() + 1){
		temp[index] = this->front[i];
		index++;
		i++;
	}

	if (this->front != 0){
		delete[] this->front;
	}

	this->front = temp;

	if (temp != 0){
		delete[] temp;
	}

	return index;
}


Long String::HangeulInsert(Long index, char object){
	Long i;

	i = this->length - 1;
	while (i >= index){
		this->front[i + 1] = this->front[i];
		i--;
	}

	this->front[index] = object;
	//this->length += 1;
	this->current = index + 1;
	//this->front[this->length] = '\0';

	return this->current;
}
////////////////////////////////////////////////////////
//한글
///////////////////////////////////////////////////////

Long String::Store(Long index, char(*objects)){
	Long i = 0;
	Long j = 0;
	char(*temp) = 0;

	while (objects[i] != '\0'){
		i++;
	}

	if (this->length + i + 1 > this->capacity){
		temp = new char[this->length + i + 256];
		
		while (j < this->length){
			temp[j] = this->front[j];
			j++;
		}
		
		if (this->front != 0){
			delete[] this->front;
		}

		this->front = temp;
		this->capacity = this->length + i + 256;
		j = 0;
	}

	while (j < i){
		this->front[index + j] = objects[j];
		this->length += 1;

		j++;
	}

	this->front[index + j] = '\0';
	this->current = this->length;

	return this->current;

}


Long String::Insert(Long index, char object){
	Long i;

	i = this->length - 1;
	while (i >= index){
		this->front[i + 1] = this->front[i];
		i--;
	}

	this->front[index] = object;
	this->length += 1;
	this->current = index + 1;
	this->front[this->length] = '\0';

	return this->current;
}


Long String::Insert(Long index, char(*object)){

	Long i = 0;
	char(*temp);
	Long j = 0;
	Long k;

	while (object[i] != '\0'){

		i++;
	}

	if (this->length + i + 1 > this->capacity){

		temp = new char[this->length + i + 256];


		while (j <= this->length){

			temp[j] = this->front[j];
			j++;
		}

		if (this->front != 0){

			delete[]this->front;
		}
		this->front = temp;

		this->capacity = this->length + i + 256;
	}

	this->front[this->length] = 0;
	j = this->length;

	while (j >= index){
	
		this->front[j + i] = this->front[j];
		j--;
	}
	j = 0;
	k = index;

	while (j < i){
	
		this->front[k] = object[j];
		j++;
		k++;
	}

	this->length = this->length + i;
	this->front[this->length] = '\0';
	this->current = index + i;

	return this->current;
}


Long String::AppendFromRear(char object){
	Long i = 0;
	char(*temp);

	if (this->length + 1 >= this->capacity){
		temp = new char[this->capacity + 256];
		
		while (i < this->length){
			temp[i] = this->front[i];
			i++;
		}

		if (this->front != 0){
			delete[] this->front;
		}

		this->front = temp;
		this->capacity = this->capacity + 256;
	}

	this->front[this->length] = object;
	this->length += 1;
	this->front[this->length] = '\0';

	this->current = this->length;

	return this->current;
}


Long String::AppendFromRear(char(*objects)){
	Long i = 0;
	char(*temp);
	Long count = 0;

	while (objects[i] != '\0'){
		count++;
		i++;
	}

	if (this->length + count + 1 >= this->capacity){
		i = 0;
		temp = new char[this->length + count + 256];

		while (i < this->length){
			temp[i] = this->front[i];
			i++;
		}

		if (this->front != 0){
			delete[] this->front;
		}

		this->front = temp;
		this->capacity = this->length + count + 256;
	}

	i = 0;
	while (i < count){
		this->front[this->length] = objects[i];
		this->length += 1;
		i++;
	}

	this->front[this->length] = '\0';
	this->current = this->length;

	return this->current;

}


Long String::Modify(Long index, char object){
	this->front[index] = object;
	this->current = index + 1;

	return this->current;
}


Long String::Delete(Long index){
	Long i = index;
	while (i <= this->length){
		this->front[i] = this->front[i + 1];
		i++;
	}
	this->length -= 1;
	this->current = index;

	return this->current;
}


Long String::BackSpaceDelete(Long index){

	
	Long i = index;


	if (index != 0){

	
		while (i <= this->length){
	
			this->front[i - 1] = this->front[i];
	
			i++;
	
		}

	this->length -= 1;
	this->current = index - 1;
	}
	else{

	this->current = 0;
	}

	return this->current;
	
}


Long String::Erase(Long startIndex, Long endIndex){
	Long i;
	Long j;
	Long tempIndex;

	this->current = endIndex;

	if (startIndex > endIndex){
		tempIndex = startIndex;
		startIndex = endIndex;
		endIndex = tempIndex;
	}

	i = startIndex;
	j = endIndex;

	while (j <= this->length){
		this->front[i] = this->front[j];
		i++;
		j++;
	}

	this->length = this->length - (endIndex - startIndex);
	this->current = startIndex;

	return this->current;
}



//LinearSearchDuplicate는 잠시보류

char& String::GetAt(Long index){

	return this->front[index];
}

Long String::Position(Long index){
	this->current = index;
	return this->current;
}

void String::Clear(){

	this->front[0] = '\0';
	this->current = 0;
	this->length = 0;
}


String& String::operator=(const String& source){
	Long i = 0;
	if (this->front != 0){

		delete[]this->front;
		this->front = 0;
	}

	this->front = new char[source.capacity];
	this->capacity = source.capacity;
	while (i < source.length){

		this->front[i] = source.front[i];
		i++;
	}
	this->length = source.length;
	this->current = source.current;
	return *this;
}


char& String::operator [](Long index){
	return this->front[index];
}


char* String::operator +(Long index){
	return this->front + index;
}





