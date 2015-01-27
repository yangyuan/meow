#include "meow.h"
#include "meow_cli.h"
#include "meow_pinyin.h"

void ConsoleProc(INPUT_RECORD input) {

}

int _tmain(int argc, _TCHAR* argv[])
{

	MeowPinyin * mp = new MeowPinyin();
	if (argc == 2) {
		mp->Solute(argv[1]);
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




	SetConsoleTitle(TEXT("Meow Test Console"));

	HANDLE handle_sih = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE handle_soh = GetStdHandle(STD_OUTPUT_HANDLE);

	while (true) {
		DWORD numEvents = 0;
		DWORD numEventsRead = 0;
		GetNumberOfConsoleInputEvents(handle_sih, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(handle_sih, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				ConsoleProc(eventBuffer[i]);
			}
			delete[] eventBuffer;
		}
	}

	return 0;
}
