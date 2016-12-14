//String.h

#ifndef _STRING_H
#define _STRING_H

typedef signed long int Long;


class String{

public:

	String(Long capacity = 64000);
	String(const String& source);
	~String();
	Long Store(Long index, char object);
	Long Store(Long index, char(*object));
	Long Insert(Long index, char object);
	Long Insert(Long index, char(*object));
	Long AppendFromRear(char object);
	Long AppendFromRear(char (*object));
	Long Modify(Long index, char object);
	Long Delete(Long index);
	Long BackSpaceDelete(Long index);
	Long Erase(Long startIndex, Long endIndex);
	////////////////////////////////////////////////
	//ÇÑ±Û
	Long HangeulWrite(Long index, char(*object));
	Long HangeulInsert(Long index, char(*object));
	Long HangeulDelete(Long index);
	///////////////////////////////////////////////
	char& GetAt(Long index);
	Long Position(Long index);
	void Clear();
	String& operator=(const String& source);
	char& operator [](Long index);
	char* operator +(Long index);
	Long GetCapacity() const;
	Long GetLength() const;
	Long GetCurrent() const;
	char* GetString();


private:
	char (*front);
	Long capacity;
	Long length;
	Long current;


};

inline Long String::GetCapacity() const{
	return this->capacity;
}


inline Long String::GetLength() const{
	return this->length;
}


inline Long String::GetCurrent() const{
	return this->current;
}


inline char* String::GetString(){
	Long i = 0;
	char(*temp);

	temp = new char[this->length + 1];

	while (i <= this->length){
		temp[i] = this->front[i];
		i++;
	}

	return temp;
}


#endif //_STRING_H