#include "pch.h"
#include "FontManager.h"

FontManager::~FontManager()
{
	for (pair<const TCHAR* const, CFont*>& it : fontList)
	{
		delete it.second;
	}
}

void FontManager::InsertFont(const TCHAR* txt, CFont* font)
{
	fontList.insert(make_pair(txt, font));
}

CFont* FontManager::GetFont(const TCHAR* key)
{
	FontListIter it = fontList.find(key);
	if (it != fontList.end()) return it->second;

	return nullptr;
}
