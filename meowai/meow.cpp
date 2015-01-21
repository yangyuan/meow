#include "meow.h"
#include "meow_pinyin.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 2) {
		MeowPinyin::split(argv[1]);
	}
	else {

		MeowPinyin * mp = new MeowPinyin();
		mp->Solute(L"shurufawenan");

		MeowPinyin::split(L"wenan");
		MeowPinyin::split(L"shurufa");
		MeowPinyin::split(L"shuo");
		MeowPinyin::split(L"fangui");
		MeowPinyin::split(L"xianhoushangxia");
		MeowPinyin::split(L"xchua");
		MeowPinyin::split(L"hehhehhehe");
		MeowPinyin::split(L"wenan");
		MeowPinyin::split(L"yunan");
		MeowPinyin::split(L"zzzzzzzhhhhhhhhh");
	}
	
	return 0;
}
