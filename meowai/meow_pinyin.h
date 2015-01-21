#pragma once
class MeowPinyin
{
public:
	MeowPinyin();
	~MeowPinyin();

	static void subsplit(WCHAR * input);
	static void split(WCHAR * input);
	static void printsplit(WCHAR * input);
};