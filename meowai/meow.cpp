#include "meow.h"
#include "meow_pinyin.h"




int _tmain(int argc, _TCHAR* argv[])
{
	MeowPinyin * mp = new MeowPinyin();
	if (argc == 2) {
		mp->Solute(argv[1], true);
	}
	else {
		mp->Solute(L"shuassadsavsdcidsaiudisa");
		mp->PrintCandidate();
		mp->Solute(L"sf");
		mp->PrintCandidate();
		mp->Solute(L"shuruf");
		mp->PrintCandidate();
		mp->Solute(L"shuf");
		mp->PrintCandidate();
		mp->Solute(L"ss");
		mp->PrintCandidate();
		mp->Solute(L"ssi");
		mp->PrintCandidate();
		mp->Solute(L"shous");
		mp->PrintCandidate();
		mp->Solute(L"xian");
		mp->PrintCandidate();
		mp->Solute(L"shurufa");
		mp->PrintCandidate();
		mp->Solute(L"wenan");
		mp->PrintCandidate();
		mp->Solute(L"quanpin");
		mp->PrintCandidate();
		mp->Solute(L"fangui");
		mp->PrintCandidate();
		mp->Solute(L"shuo");
		mp->PrintCandidate();
		mp->Solute(L"xianxixixananan");
		mp->PrintCandidate();
		mp->Solute(L"shurufawenan");
		mp->Solute(L"wenan");
		mp->Solute(L"shuo");
		mp->Solute(L"fangui");
		mp->Solute(L"xianhoushangxia");
		mp->Solute(L"xchua");
		mp->Solute(L"hehhehhehe");
		mp->Solute(L"hengggggodageziyeshihaai");
		mp->Solute(L"zzzzzzzhhhhhhhhh");
	}
	
	return 0;
}
