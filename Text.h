//Text.h

#ifndef _TEXT_H
#define _TEXT_H

#include "String.h"


class Text{

public:

	Text();
	~Text();
	Text(const Text& source);
	Long Write(Long index, char object);
	//////////////////////////////////////////////////
	//한글
	Long HangeulWrite(Long index, char(*object));
	Long HangeulComplete(Long index, char(*object));
	/////////////////////////////////////////////////
	Long Delete(Long index);
	Long BackSpace(Long index);
	Long Erase(Long startRow, Long startColumn);

	Long Paste(Long index, char(*object));
	char* Copy(Long startRow, Long startColumn);
	char* Cut(Long startRow, Long startColumn);

	void Search(char(*object), Long *(*indexes), Long *count);
	Long Modify(Long index, char object);
	Long Position(Long row, Long column);
	Long GetRowLength(Long row);
	void Clear();

	Text& operator=(const Text& source);

	
	Long GetRow() const;
	Long GetCurrent() const;
	Long GetColumn() const;
	Long GetLength() const; //Edit OnPaint에서 사용
	Long GetCapacity() const;
	Long GetMaxRow(); //추가
	char* GetString();
	char& GetAt(Long index); //Edit OnPaint에서 사용

private:

	String string;
	Long row;
	Long column;
	Long current;

};

inline Long Text::GetLength() const{
	return this->string.GetLength();
}

inline Long Text::GetCapacity() const{
	return this->string.GetCapacity();
}


inline Long  Text::GetRow() const{

	return this->row;
}

inline Long Text::GetCurrent() const{

	return this->current;
}

inline Long Text::GetColumn() const{

	return this->column;
}

inline char* Text::GetString(){

	return this->string.GetString();
}

#endif //_TEXT_H