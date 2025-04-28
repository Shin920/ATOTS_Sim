#pragma once
#include "SingletonBase.h"

template <typename T>
class TextTable : public SingletonBase<T>
{
private:
	typedef map<int, const CHAR*>			TextList;
	typedef map<int, const CHAR*>::iterator	TextListIter;
	TextList txtList;

protected:
	void AddText(int val, const CHAR* txt);
	virtual void Init() = 0;

public:
	const CHAR* GetText(int val);
};

template <typename T>
const CHAR* TextTable<T>::GetText(int val)
{
	TextListIter it = txtList.find(val);
	if (it != txtList.end()) return it->second;

	return nullptr;
}

class TextTablePacketHSAck : public TextTable<TextTablePacketHSAck>
{
public:
	TextTablePacketHSAck();
protected:
	virtual void Init() override;
};

class TextTablePacketHSRej : public TextTable<TextTablePacketHSRej>
{
public:
	TextTablePacketHSRej();
protected:
	virtual void Init() override;
};


class TextTablePacketJP : public TextTable<TextTablePacketJP>
{
public:
	TextTablePacketJP();
protected:
	virtual void Init() override;
};

class TextTablePacketSP : public TextTable<TextTablePacketSP>
{
public:
	TextTablePacketSP();
protected:
	virtual void Init() override;
};

class TextTablePacketSTRAck : public TextTable<TextTablePacketSTRAck>
{
public:
	TextTablePacketSTRAck();
protected:
	virtual void Init() override;
};