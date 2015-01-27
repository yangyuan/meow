#include "meow.h"
#include "meow_cli.h"
#include "meow_pinyin.h"

void ConsoleProc(INPUT_RECORD input) {

}

int _tmain(int argc, _TCHAR* argv[])
{
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
