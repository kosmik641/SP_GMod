#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Interface.h"
#include <windows.h>
#include <string>

//#include <fcntl.h>
//#include <io.h>
//void CreateConsole()
//{
//	int hConHandle = 0;
//	HANDLE lStdHandle = 0;
//  FILE* fp = 0;
//	AllocConsole();
//	freopen("CON", "w", stdout);
//	SetConsoleTitle(L"UART Console");
//	HWND hwnd = ::GetConsoleWindow();
//	if (hwnd != NULL)
//	{
//		HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
//		if (hMenu != NULL)
//		{
//			DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
//			DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
//			DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
//		}
//	}
//	lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
//	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
//	fp = _fdopen(hConHandle, "w");
//	*stdout = *fp;
//	setvbuf(stdout, NULL, _IONBF, 0);
//}

/*
В return функции возвращается количество возвращаемых в Lua объектов
*/

#define PushCLua( _function, _name ) LUA->PushCFunction(_function); LUA->SetField(-2, _name);

using namespace GarrysMod::Lua;
HANDLE m_HCOM;
int comState = -10;

int Version(lua_State* state)
{
	char date_str[30];
	sprintf_s(date_str, "Bulded %s %s",__TIME__,__DATE__);
	LUA->PushString(date_str);
	return 1;
}

int GetCOMState(lua_State* state)
{
	LUA->PushNumber(comState);
	return 1;
}

int WriteByte(lua_State* state)
{
	unsigned int Bytes_To_Send = LUA->GetNumber(2); // Количество входящих байт
	byte* Input_Bytes = new byte[Bytes_To_Send + 2];
	if (LUA->IsType(1, Type::TABLE)) // Проверяем, что первый параметр - таблица
	{
		unsigned int Input_Bytes_Index = 0;
		LUA->PushNil(); // Первый ключ
		// Функция lua_next перебирает все пары "ключ"-"значение" в таблице,
		// Вторым параметром указывается индекс в стеке, по которому расположен массив (таблица Lua)

		while (LUA->Next(1) != 0) {
			// в паре "ключ" находится по индексу -2, "значение" находится по индексу -1
			Input_Bytes[Input_Bytes_Index++] = (byte)LUA->GetNumber(-1);
			LUA->Pop(1);// освобождает стек для следующей итерации
		}
	}
	
	DWORD WrBytes = 0;
	WriteFile(m_HCOM, Input_Bytes,(DWORD)Bytes_To_Send, &WrBytes, NULL);
	delete[] Input_Bytes;

	return 0;
}

int ReadByte(lua_State* state)
{
	int Nmb_Of_Input_Bytes = LUA->GetNumber(1);
	byte* Input_Bytes = new byte[Nmb_Of_Input_Bytes + 2]();
	
	DWORD Read_Len = 0;
	ReadFile(m_HCOM, Input_Bytes, Nmb_Of_Input_Bytes, &Read_Len, NULL);
	PurgeComm(m_HCOM, PURGE_RXCLEAR);

	LUA->Top();
	LUA->CreateTable();
	for (int i = 0; i < Nmb_Of_Input_Bytes; i++) {
		LUA->PushNumber(i); // Кладем индекс
		LUA->PushNumber(Input_Bytes[i]); // Значение по индексу
		LUA->RawSet(-3); // Закидываем в таблицу
	}
	
	delete[] Input_Bytes;

	return 1;
}

int StartCOM(lua_State* state)
{
	int a_portNumber = LUA->GetNumber(1);

	// UARTFrontViewClient.cpp
	wchar_t port_numb_str[14];

	wsprintfW(port_numb_str, L"\\\\.\\COM%d", a_portNumber);
	m_HCOM = CreateFile(port_numb_str, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	Sleep(250);
	if (m_HCOM == INVALID_HANDLE_VALUE)
	{
		comState = -1;
		LUA->PushNumber(comState);
		return 1;
	}
	else
	{
		DCB DCB_;
		int Sys = GetCommState(m_HCOM, &DCB_);

		if (Sys == 0)
		{
			comState = -2;
			LUA->PushNumber(comState);
			return 1;
		}

		DCB_.BaudRate = (DWORD)115200;
		DCB_.ByteSize = 8;
		DCB_.Parity = NOPARITY;
		DCB_.StopBits = ONESTOPBIT;
		Sys = SetCommState(m_HCOM, &DCB_);

		if (Sys == 0)
		{
			comState = -3;
			LUA->PushNumber(comState);
			return 1;
		}

		SetCommMask(m_HCOM, EV_TXEMPTY);

		COMMTIMEOUTS Timeout;
		GetCommTimeouts(m_HCOM, &Timeout);
		Timeout.ReadIntervalTimeout = MAXDWORD;
		Timeout.ReadTotalTimeoutMultiplier = 0;
		Timeout.ReadTotalTimeoutConstant = 100;
		Timeout.WriteTotalTimeoutMultiplier = 0;
		Timeout.WriteTotalTimeoutConstant = 1000;
		SetCommTimeouts(m_HCOM, &Timeout);

		SetupComm(m_HCOM, 1000, 1000);

		byte* Detect_Command = new byte[2];
		Detect_Command[0] = 0x00;
		Detect_Command[1] = 0x80;

		byte* Answer = new byte[1];
		byte Right_Answer = 0x66;

		DWORD Wr_Len = 0;
		DWORD Read_Len = 0;

		WriteFile(m_HCOM, Detect_Command, 2, &Wr_Len, NULL);

		ReadFile(m_HCOM, Answer, 1, &Read_Len, NULL);
		Read_Len = 0;

		if (Answer[0] == Right_Answer)
		{
			//CreateConsole();
			//printf("====== Connected to COM%d ======\n", a_portNumber);
			comState = 0;
			LUA->PushNumber(comState);
			return 1;
		}
		else
		{
			PurgeComm(m_HCOM, PURGE_TXCLEAR | PURGE_RXCLEAR);
			CloseHandle(m_HCOM);
			m_HCOM = NULL;
			comState = -4;
			LUA->PushNumber(comState);
			return 1;
		}
	}

	LUA->PushNumber(comState);
	return 1;
}

int StopCOM(lua_State* state)
{
	if (!((m_HCOM == INVALID_HANDLE_VALUE) || (m_HCOM == NULL)))
	{
		PurgeComm(m_HCOM, PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(m_HCOM);
		m_HCOM = NULL;
		comState = -10;
	}
	return 0;
}

// Called when the module opens
GMOD_MODULE_OPEN()
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->CreateTable();
			PushCLua(Version, "Version"); // string UART.Version()
			PushCLua(StartCOM, "StartCOM"); // int UART.StartCOM(port)
			PushCLua(StopCOM, "StopCOM"); // int UART.StopCOM()
			PushCLua(WriteByte, "WriteByte"); // UART.WriteByte(table Bytes, int nBytes)
			PushCLua(ReadByte, "ReadByte"); // string UART.ReadByte(int bytesToRead)
			PushCLua(GetCOMState, "GetCOMState"); // int UART.GetCOMState()
		LUA->SetField(-2, "UART");
	LUA->Pop();

	//CreateConsole();
	
	return 0;
}
// Called when the module closes
GMOD_MODULE_CLOSE()
{
	PurgeComm(m_HCOM, PURGE_TXCLEAR | PURGE_RXCLEAR);
	CloseHandle(m_HCOM);
	m_HCOM = NULL;
	return 0;
}