#pragma once
#include <unordered_map>
#include "SingletonBase.h"
class FontManager : public SingletonBase<FontManager>
{
	typedef map<const TCHAR*, CFont*>			FontList;
	typedef map<const TCHAR*, CFont*>::iterator	FontListIter;
	FontList fontList;

protected:
public:
	~FontManager();
	void InsertFont(const TCHAR* txt, CFont* font);
	CFont* GetFont(const TCHAR* key);
};

