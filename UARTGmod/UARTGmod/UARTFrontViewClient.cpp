#include "main.h"
#define UART UARTFrontViewClient
using namespace Glob;

#pragma region Variables
HANDLE UART::hSerial;
bool UART::m_isInitFinish, UART::m_isThreadWorking, UART::m_isTthreadEnded;

int* UART::ADC_Signals;
int* UART::Input_Signals;
int* UART::Output_Signals;
int* UART::Arrow_Signals;
float* UART::Indicator_Signals;
int* UART::SAUT_Signals;

int UART::Number_Of_Controllers, UART::Nmb_Of_ADC_Signals, UART::Nmb_Of_Arrow_Signals, UART::Nmb_Of_Indicator_Signals, UART::Nmb_Of_SAUT_Signals, UART::Nmb_Of_7SegDec_Signals, UART::Nmb_Of_Text_Displays;
std::vector<int> UART::Config_Arrow_Signals, UART::Config_SAUT_Signals, UART::Config_Text_Signals;
std::vector<UART::IndicatorSignals> UART::Config_Indicator_Signals;
int UART::Config_7SegDec_Signals[20][3];
int UART::Config_Signals_Length;
int* UART::Config_Signals;
int* UART::_7SegDec_Signals;
UART::textDisplaySignals* UART::TextDisplaysSignalsStruct;

int UART::Nmb_Of_Input_Bytes, UART::Nmb_Of_Output_Bytes, UART::Nmb_Of_Arrow_Bytes, UART::Nmb_Of_SAUT_Bytes, UART::Nmb_Of_Indicator_Bytes, UART::Nmb_Of_Text_Bytes;
std::vector<int> UART::Nmb_Of_ADC_Signals_Per_Contr, UART::Nmb_Of_Arrow_Bytes_Per_Contr, UART::Nmb_Of_Indicator_Bytes_Per_Contr, UART::Nmb_Of_SAUT_Bytes_Per_Contr;
std::vector<int> UART::Nmb_Of_UART_Bytes_Per_Contr, UART::Nmb_Of_Text_Bytes_Per_Controller;
int UART::Nmb_Of_Input_Signals, UART::Nmb_Of_Output_Signals;
int UART::Nmb_Of_Config_Bytes, UART::Nmb_Of_UART_Bytes;

byte* UART::Config_Bytes;
byte* UART::ACK_Bytes;
byte* UART::Input_Bytes;
byte* UART::Output_Bytes;
byte* UART::UART_Bytes;
#pragma endregion

int UARTFrontViewClient::COM_Open(int a_portNumber, int a_portRate)
{
	wchar_t port_numb_str[14];

	wsprintfW(port_numb_str, L"\\\\.\\COM%d", a_portNumber);
	hSerial = CreateFile(port_numb_str, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

	if (hSerial == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	else
	{
		DCB DCB_;
		int Sys = GetCommState(hSerial, &DCB_);

		if (Sys == 0)
		{
			return -11;
		}

		DCB_.BaudRate = (DWORD)a_portRate;
		DCB_.ByteSize = 8;
		DCB_.Parity = NOPARITY;
		DCB_.StopBits = ONESTOPBIT;
		Sys = SetCommState(hSerial, &DCB_);

		if (Sys == 0)
		{
			return -12;
		}

		Sys = SetCommMask(hSerial, EV_TXEMPTY);
		if (Sys == 0)
		{
			return -13;
		}

		COMMTIMEOUTS Timeout;
		Sys = GetCommTimeouts(hSerial, &Timeout);
		if (Sys == 0)
		{
			return -14;
		}
		Timeout.ReadIntervalTimeout = MAXDWORD;
		Timeout.ReadTotalTimeoutMultiplier = 0;
		Timeout.ReadTotalTimeoutConstant = 100;
		Timeout.WriteTotalTimeoutMultiplier = 0;
		Timeout.WriteTotalTimeoutConstant = 1000;
		Sys = SetCommTimeouts(hSerial, &Timeout);
		if (Sys == 0)
		{
			return -15;
		}

		SetupComm(hSerial, 1000, 1000);

		byte* Detect_Command = new byte[2];
		Detect_Command[0] = 0x00;
		Detect_Command[1] = 0x80;

		byte* Answer = new byte[1];
		byte Right_Answer = 0x66;

		DWORD Wr_Len = 0;
		DWORD Read_Len = 0;

		WriteFile(hSerial, Detect_Command, 2, &Wr_Len, NULL);

		ReadFile(hSerial, Answer, 1, &Read_Len, NULL);

		Read_Len = 0;

		if (Answer[0] == Right_Answer)
		{
			return 0;
		}
		else
		{
			PurgeComm(hSerial, PURGE_TXCLEAR | PURGE_RXCLEAR);
			CloseHandle(hSerial);
			hSerial = NULL;
			return -2;
		}
	}
}

void UARTFrontViewClient::COM_Close()
{
	if (!((hSerial == INVALID_HANDLE_VALUE) || (hSerial == NULL)))
	{
		PurgeComm(hSerial, PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(hSerial);
		hSerial = NULL;
	}
}

void UARTFrontViewClient::setThreadEnded(bool a_threadEnded)
{
	m_isTthreadEnded = a_threadEnded;
}

bool UARTFrontViewClient::threadWorking()
{
	return m_isThreadWorking;
}

void __cdecl UARTFrontViewClient::ThreadFunction(void* pParams)
{
	while (threadWorking())
	{
		Glob::UARTFrontViewClient::Metro81717Signals::update();
		Sleep(10);
	}
	setThreadEnded(true);
	_endthread();
}

void UARTFrontViewClient::updateUART()
{
	byte* Read_Command = new byte[2];
	Read_Command[0] = 0x00;
	Read_Command[1] = 0x85;

	DWORD Wr_Len = 0;
	DWORD Read_Len = 0;

	Sleep(30);
	WriteFile(hSerial, Read_Command, 2, &Wr_Len, NULL);

#pragma region Reading_Bytes

	ReadFile(hSerial, Input_Bytes, (DWORD)(Nmb_Of_Input_Bytes + 3), &Read_Len, NULL);
	PurgeComm(hSerial, PURGE_RXCLEAR);

	if ((Input_Bytes[0] == 0x0B) && (Input_Bytes[1] == 0x0B) && (Input_Bytes[2] == Nmb_Of_Input_Bytes))   // Если эти байты удовлетворяют протоколу, работаем с ними
	{
		int Input_Bytes_Index = 3;
		int Input_Signals_Index = 0;
		int ADC_Signals_Index = 0;

		for (int Cntr_Nmb = 0; Cntr_Nmb < Number_Of_Controllers; Cntr_Nmb++)
		{
			for (int ADC_Nmb = 0; ADC_Nmb < Nmb_Of_ADC_Signals_Per_Contr[(size_t)Cntr_Nmb]; ADC_Nmb++)
			{
				ADC_Signals[ADC_Signals_Index] = Input_Bytes[Input_Bytes_Index++] << 8;
				ADC_Signals[ADC_Signals_Index++] |= Input_Bytes[Input_Bytes_Index++];
			}

			for (int Pin_Nmb = 0; Pin_Nmb < 24; Pin_Nmb++)
			{
				Input_Signals[Input_Signals_Index++] = ((Input_Bytes[Input_Bytes_Index] << (Pin_Nmb % 8)) & 0x80) >> 7;
				if (Pin_Nmb == 7 || Pin_Nmb == 15 || Pin_Nmb == 23)
					Input_Bytes_Index++;
			}
		}
	}

#pragma endregion

#pragma region Writing_Bytes

	// Готовим исходящие байты
	Output_Bytes[0] = (byte)Nmb_Of_Output_Bytes;
	Output_Bytes[1] = 0x83;

	for (int Output_Signals_Index = 0; Output_Signals_Index < Nmb_Of_Output_Signals; Output_Signals_Index++)
	{
		if ((Output_Signals_Index % 8) == 0)
			Output_Bytes[(Output_Signals_Index / 8) + 2] = 0;
		Output_Bytes[(Output_Signals_Index / 8) + 2] |= (byte)(Output_Signals[Output_Signals_Index] << 7) >> (Output_Signals_Index % 8);
	}

	int _7SegDec_Signals_Index = 0;

	for (int Cntr_Nmb = 0; Cntr_Nmb < Number_Of_Controllers; Cntr_Nmb++)
	{
		for (int Port_Nmb = 0; Port_Nmb < 3; Port_Nmb++)
		{
			if (Config_7SegDec_Signals[(size_t)Cntr_Nmb][Port_Nmb] == 1)
			{
				Output_Bytes[Cntr_Nmb * 3 + Port_Nmb + 2] = Convert_Int_Number_To_8_Output_Signals(_7SegDec_Signals[_7SegDec_Signals_Index++]);
			}
		}
	}

	WriteFile(hSerial, Output_Bytes, (DWORD)(Nmb_Of_Output_Bytes + 2), &Wr_Len, NULL);

#pragma endregion

#pragma region Writing_UART_Bytes

	if (Nmb_Of_UART_Bytes > 0)
	{
		// Готовим байты, выводимые контроллером через UART
		UART_Bytes[0] = (byte)Nmb_Of_UART_Bytes;
		UART_Bytes[1] = 0x87;

		int Arrow_Signals_Index = 0;
		int Indicator_Signals_Counter = 0;
		int SAUT_Signals_Counter = 0;
		int Text_Display_Counter = 0;
		int UART_Bytes_Counter = 2;

		for (size_t Cntr_Nmb = 0; Cntr_Nmb < (size_t)Number_Of_Controllers; Cntr_Nmb++)
		{
			if (Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Nmb] > 0)
			{
				UART_Bytes[UART_Bytes_Counter++] = 0x41;//"A" aka ARROWS
				for (int i = 0; i < Config_Arrow_Signals[Cntr_Nmb]; i++)
				{
					UART_Bytes[UART_Bytes_Counter++] = (byte)(i + 1);
					UART_Bytes[UART_Bytes_Counter++] = (byte)((Arrow_Signals[Arrow_Signals_Index] & 0xFF00) >> 8);
					UART_Bytes[UART_Bytes_Counter++] = (byte)(Arrow_Signals[Arrow_Signals_Index++] & 0xFF);
				}
			}

			if (Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Nmb] > 0)
			{
				for (int Ind_Nmb = 0; Ind_Nmb < 3; Ind_Nmb++)
				{
					if (Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0] > 0)
					{
						UART_Bytes[UART_Bytes_Counter++] = 0x49;//"I" aka INDICATORS
						UART_Bytes[UART_Bytes_Counter++] = (byte)(Ind_Nmb + 1);
						UART_Bytes[UART_Bytes_Counter++] = (byte)Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0];

						byte* Digits = new byte[(size_t)Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0]];

						Convert_Float_Number_To_Byte_Digits(Indicator_Signals[Indicator_Signals_Counter++], Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0], Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][1], Digits);

						for (int Indicator_Bytes_Index = 0; Indicator_Bytes_Index < Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0]; Indicator_Bytes_Index++)
						{
							UART_Bytes[UART_Bytes_Counter++] = Digits[Indicator_Bytes_Index];
						}

						UART_Bytes[UART_Bytes_Counter++] = 0xFF;
					}
				}
			}

			if (Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Nmb] > 0)
			{
				UART_Bytes[UART_Bytes_Counter++] = 0x4C;//"L" aka LEDS
				UART_Bytes[UART_Bytes_Counter++] = (byte)((SAUT_Signals[SAUT_Signals_Counter] << 2) | (SAUT_Signals[SAUT_Signals_Counter + 1] << 1) | SAUT_Signals[SAUT_Signals_Counter + 2]);
				SAUT_Signals_Counter += 3;
			}

			if (Nmb_Of_Text_Bytes_Per_Controller[Cntr_Nmb] > 0)
			{
				UART_Bytes[UART_Bytes_Counter++] = 0x54;// "T" aka TEXT
				UART_Bytes[UART_Bytes_Counter++] = 0x42;// "B" aka BEGIN
				if (TextDisplaysSignalsStruct[Text_Display_Counter].DisplayOn == 1)
				{
					if (TextDisplaysSignalsStruct[Text_Display_Counter].DisplayLedOn == 1)
						UART_Bytes[UART_Bytes_Counter++] = 0x11;// Включены экран и подсветка.
					else
						UART_Bytes[UART_Bytes_Counter++] = 0x01;// Включен только экран.
				}
				else
				{
					UART_Bytes[UART_Bytes_Counter++] = 0x00;// Выключен экран.
				}
				int temp = (Config_Text_Signals[Cntr_Nmb] * 2);
				UART_Bytes[UART_Bytes_Counter++] = (byte)temp;
				for (int Text_Byte_Index = 0; Text_Byte_Index < (Config_Text_Signals[Cntr_Nmb] * 2); Text_Byte_Index++)
				{
					UART_Bytes[UART_Bytes_Counter++] = TextDisplaysSignalsStruct[Text_Display_Counter].textBytes[Text_Byte_Index];
				}

				UART_Bytes[UART_Bytes_Counter++] = 0x54;// "T" aka TEXT
				UART_Bytes[UART_Bytes_Counter++] = 0x45;// "E" aka END

				Text_Display_Counter++;
			}
		}

		Sleep(10);// Задержка для завершения предыдущей команды
		WriteFile(hSerial, UART_Bytes, (DWORD)(Nmb_Of_UART_Bytes + 2), &Wr_Len, NULL);
	}
#pragma endregion
}

byte UARTFrontViewClient::Convert_Int_Number_To_8_Output_Signals(int Number)
{
	Number = Number % 100;

	byte tmp = 0;

	int Edinici = Number % 10;
	int Desyatki = (Number - Edinici) / 10;

	switch (Desyatki)
	{
	case 1:
		tmp |= 0x08;
		break;
	case 2:
		tmp |= 0x01;
		break;
	case 3:
		tmp |= 0x09;
		break;
	case 4:
		tmp |= 0x02;
		break;
	case 5:
		tmp |= 0x0A;
		break;
	case 6:
		tmp |= 0x03;
		break;
	case 7:
		tmp |= 0x0B;
		break;
	case 8:
		tmp |= 0x04;
		break;
	case 9:
		tmp |= 0x0C;
		break;
	}

	switch (Edinici)
	{
	case 1:
		tmp |= 0x80;
		break;
	case 2:
		tmp |= 0x10;
		break;
	case 3:
		tmp |= 0x90;
		break;
	case 4:
		tmp |= 0x20;
		break;
	case 5:
		tmp |= 0xA0;
		break;
	case 6:
		tmp |= 0x30;
		break;
	case 7:
		tmp |= 0xB0;
		break;
	case 8:
		tmp |= 0x40;
		break;
	case 9:
		tmp |= 0xC0;
		break;
	}

	return tmp;
}

bool UARTFrontViewClient::Convert_Float_Number_To_Byte_Digits(float Number, int Number_Of_Digits, int Number_Of_Digits_After_Point_User, byte* Digits)
{
	int Nmb_Of_Dig_After_Point = Number_Of_Digits_After_Point_User;
	double Max_Int_Designator = pow((double)10, Number_Of_Digits);

	// Если число за пределами возможностей отображения экрана, выходим ничего не сделав
	if ((int)(Number / Max_Int_Designator) != 0)
	{
		return false;
	}

	// Проверяем хватит ли цифр, для обозначения всей дробной части, если нет-уменьшаем ее и снова проверяем
	bool success = false;
	while (!success)
	{
		Max_Int_Designator = pow((double)10, (int)(Number_Of_Digits - Nmb_Of_Dig_After_Point));

		if ((int)(Number / Max_Int_Designator) == 0)
		{
			success = true;
		}
		else
		{
			Nmb_Of_Dig_After_Point--;
		}
	}

	for (int i = 0; i < Number_Of_Digits; i++)
	{
		if (i < Nmb_Of_Dig_After_Point)
		{
			double var1 = (double)pow((double)10, (Nmb_Of_Dig_After_Point - i));
			int var2 = (int)floor(Number * (int)var1 + 0.5);
			Digits[Number_Of_Digits - i - 1] = (byte)((var2) % 10);
		}
		else if ((i == Nmb_Of_Dig_After_Point) && (Nmb_Of_Dig_After_Point != 0))
		{
			Digits[Number_Of_Digits - i - 1] = (byte)((byte)((int)Number % 10) | 0x80);
		}
		else
		{
			double var1 = pow((double)10, (i - Nmb_Of_Dig_After_Point + 1));

			double numerator;

			if (i == 0)
				numerator = (int)floor(Number + 0.5) % (int)var1;
			else
				numerator = (int)Number % (int)var1;

			double designator = (int)pow((double)10, (i - Nmb_Of_Dig_After_Point));

			Digits[Number_Of_Digits - i - 1] = (byte)((int)(numerator / designator));
		}
	}

	for (int i = 0; i < Number_Of_Digits - 1; i++)
	{
		if (Digits[i] == 0)
		{
			Digits[i] = 0x0A;
		}
		else
			break;
	}

	return true;
}

bool UARTFrontViewClient::ConfigSignals()
{
	Nmb_Of_Input_Bytes = 0;
	Nmb_Of_Output_Bytes = 0;

	Nmb_Of_Arrow_Bytes = 0;
	Nmb_Of_SAUT_Bytes = 0;
	Nmb_Of_Indicator_Bytes = 0;
	Nmb_Of_Text_Bytes = 0;

	Nmb_Of_ADC_Signals_Per_Contr.resize((size_t)Number_Of_Controllers);
	Nmb_Of_Arrow_Bytes_Per_Contr.resize((size_t)Number_Of_Controllers);
	Nmb_Of_Indicator_Bytes_Per_Contr.resize((size_t)Number_Of_Controllers);
	Nmb_Of_SAUT_Bytes_Per_Contr.resize((size_t)Number_Of_Controllers);
	Nmb_Of_UART_Bytes_Per_Contr.resize((size_t)Number_Of_Controllers);
	Nmb_Of_Text_Bytes_Per_Controller.resize((size_t)Number_Of_Controllers);

	for (int Cntr_Nmb = 0; Cntr_Nmb < Number_Of_Controllers; Cntr_Nmb++)
	{
		Nmb_Of_ADC_Signals_Per_Contr[(size_t)Cntr_Nmb] = 0;
		Nmb_Of_Arrow_Bytes_Per_Contr[(size_t)Cntr_Nmb] = 0;
		Nmb_Of_Indicator_Bytes_Per_Contr[(size_t)Cntr_Nmb] = 0;
		Nmb_Of_SAUT_Bytes_Per_Contr[(size_t)Cntr_Nmb] = 0;
		Nmb_Of_UART_Bytes_Per_Contr[(size_t)Cntr_Nmb] = 0;
		Nmb_Of_Text_Bytes_Per_Controller[(size_t)Cntr_Nmb] = 0;
	}

	Nmb_Of_Input_Signals = 0;
	Nmb_Of_Output_Signals = 0;

	Nmb_Of_Config_Bytes = Number_Of_Controllers * 7 + 2;

	Config_Bytes = new byte[(size_t)Nmb_Of_Config_Bytes]();

	Config_Bytes[0] = (byte)(Nmb_Of_Config_Bytes - 2);
	Config_Bytes[1] = 0x81;

	size_t Config_Signals_Counter = 0;
	size_t Config_Bytes_Counter = 0;
	size_t Cntr_Counter = 0;

	for (int i = 0; i < (Config_Signals_Length + Number_Of_Controllers); i++)
	{
		if ((i + 1) % 25 != 0)      //24 бита отвечают за конфигурирование пинов контроллера, на шаге, кратном 25, записываем байт-показатель количества байт, передаваемых данным контроллером по UART
		{
			Config_Signals_Counter = i - Cntr_Counter;
			Config_Bytes_Counter = Config_Signals_Counter / 4 + 2;
			Config_Bytes[Config_Bytes_Counter + Cntr_Counter] |= (byte)((Config_Signals[Config_Signals_Counter] << 6) >> ((Config_Signals_Counter % 4) * 2));

			if (Config_Signals[Config_Signals_Counter] == 3)
			{
				Nmb_Of_Input_Bytes += 2;
				Nmb_Of_ADC_Signals_Per_Contr[Config_Signals_Counter / 24] += 1;
			}
		}

		else
		{
			Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Counter] = (Config_Arrow_Signals[Cntr_Counter] == 0) ? 0 : Config_Arrow_Signals[(size_t)Cntr_Counter] * 3 + 1;

			for (int Ind_Nmb = 0; Ind_Nmb < 3; Ind_Nmb++)
			{
				Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Counter] += (Config_Indicator_Signals[Cntr_Counter].map[Ind_Nmb][0] == 0) ? 0 : (Config_Indicator_Signals[Cntr_Counter].map[Ind_Nmb][0] + 4);
			}
			Nmb_Of_Indicator_Bytes += Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Counter];

			Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Counter] = Config_SAUT_Signals[Cntr_Counter] * 2;

			Nmb_Of_Text_Bytes_Per_Controller[Cntr_Counter] = (Config_Text_Signals[Cntr_Counter] == 0) ? 0 : (Config_Text_Signals[Cntr_Counter] * 2 + 6);

			Nmb_Of_UART_Bytes_Per_Contr[Cntr_Counter] = Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Counter] + Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Counter]
				+ Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Counter] + Nmb_Of_Text_Bytes_Per_Controller[Cntr_Counter];

			Config_Bytes[Config_Bytes_Counter + Cntr_Counter + 1] = (byte)(Nmb_Of_UART_Bytes_Per_Contr[Cntr_Counter]);

			Nmb_Of_Arrow_Bytes += Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Counter];
			Nmb_Of_SAUT_Bytes += Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Counter];
			Nmb_Of_Text_Bytes += Nmb_Of_Text_Bytes_Per_Controller[Cntr_Counter];

			Cntr_Counter++;
		}
	}

	int ms_delay = 0;
	DWORD Wr_Len = 0;
	DWORD Read_Len = 0;
	while (true/*ms_delay<21*/)
	{
		if (ms_delay > 20)
		{
			//Устройство не получается cконфигурировать
			m_isThreadWorking = false;
			return false;
		}

		COMSTAT comstat;
		DWORD temp; // заглушка
		WriteFile(hSerial, Config_Bytes, (DWORD)Nmb_Of_Config_Bytes, &Wr_Len, NULL);   // Конфигурируем контроллеры
		Sleep(100);
		comstat.cbInQue = 0;
		ClearCommError(hSerial, &temp, &comstat);
		DWORD bytesToRead = comstat.cbInQue;

		if (bytesToRead == 2) //Проверяем пришло ли подтверждение конфигурации
		{
			ACK_Bytes = new byte[2];//+2 на всякий случай
			ReadFile(hSerial, ACK_Bytes, bytesToRead, &Read_Len, NULL);

			if ((ACK_Bytes[0] == 0x01) && (ACK_Bytes[1] == 0x8F))
			{
				delete[]ACK_Bytes;
				break;
			}
			delete[]ACK_Bytes;
		}

		ms_delay++;
	}

	Nmb_Of_Input_Bytes += 3 * Number_Of_Controllers;
	Nmb_Of_Output_Bytes += 3 * Number_Of_Controllers;
	Nmb_Of_Input_Signals = 24 * Number_Of_Controllers;
	Nmb_Of_Output_Signals = 24 * Number_Of_Controllers;

	Nmb_Of_UART_Bytes = Nmb_Of_Arrow_Bytes + Nmb_Of_SAUT_Bytes + Nmb_Of_Indicator_Bytes + Nmb_Of_Text_Bytes;

	//Определяем размеры всех используемых массивов
	Input_Bytes = new byte[size_t(Nmb_Of_Input_Bytes + 3)]();
	Output_Bytes = new byte[size_t(Nmb_Of_Output_Bytes + 2)]();
	UART_Bytes = new byte[size_t(Nmb_Of_UART_Bytes + 2)]();

	ADC_Signals = new int[(size_t)Nmb_Of_ADC_Signals]();
	Input_Signals = new int[(size_t)Nmb_Of_Input_Signals]();
	Output_Signals = new int[(size_t)Nmb_Of_Output_Signals]();
	Arrow_Signals = new int[(size_t)Nmb_Of_Arrow_Signals]();
	Indicator_Signals = new float[(size_t)Nmb_Of_Indicator_Signals]();
	SAUT_Signals = new int[(size_t)Nmb_Of_SAUT_Signals]();
	_7SegDec_Signals = new int[(size_t)Nmb_Of_7SegDec_Signals]();
	TextDisplaysSignalsStruct = new textDisplaySignals[(size_t)Nmb_Of_Text_Displays]();

	return true;
}

void UARTFrontViewClient::shutdown()
{
	DWORD Wr_Len = 0;

#pragma region Writing_UART_Bytes
	if (Nmb_Of_UART_Bytes > 0)
	{
		// Готовим байты, выводимые контроллером через UART
		UART_Bytes[0] = (byte)Nmb_Of_UART_Bytes;
		UART_Bytes[1] = 0x87;

		int UART_Bytes_Counter = 2;

		for (size_t Cntr_Nmb = 0; Cntr_Nmb < (size_t)Number_Of_Controllers; Cntr_Nmb++)
		{
			if (Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Nmb] > 0)
			{
				UART_Bytes[UART_Bytes_Counter++] = 0x41;//"A" aka ARROWS
				for (int i = 0; i < Config_Arrow_Signals[Cntr_Nmb]; i++)
				{
					UART_Bytes[UART_Bytes_Counter++] = (byte)(i + 1);
					UART_Bytes[UART_Bytes_Counter++] = 0x00;
					UART_Bytes[UART_Bytes_Counter++] = 0x00;
				}
			}

			if (Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Nmb] > 0)
			{
				for (int Ind_Nmb = 0; Ind_Nmb < 3; Ind_Nmb++)
				{
					if (Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0] > 0)
					{
						UART_Bytes[UART_Bytes_Counter++] = 0x49;//"I" aka INDICATORS
						UART_Bytes[UART_Bytes_Counter++] = (byte)(Ind_Nmb + 1);
						UART_Bytes[UART_Bytes_Counter++] = (byte)Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0];

						for (int Indicator_Bytes_Index = 0; Indicator_Bytes_Index < Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0]; Indicator_Bytes_Index++)
						{
							UART_Bytes[UART_Bytes_Counter++] = 0x0A;
						}

						UART_Bytes[UART_Bytes_Counter++] = 0xFF;
					}
				}
			}

			if (Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Nmb] > 0)
			{
				UART_Bytes[UART_Bytes_Counter++] = 0x4C;//"L" aka LEDS
				UART_Bytes[UART_Bytes_Counter++] = 0x00;
			}

			if (Nmb_Of_Text_Bytes_Per_Controller[Cntr_Nmb] > 0)
			{
				UART_Bytes[UART_Bytes_Counter++] = 0x54;// "T" aka TEXT
				UART_Bytes[UART_Bytes_Counter++] = 0x42;// "B" aka BEGIN

				UART_Bytes[UART_Bytes_Counter++] = 0x00;// Выключен экран.

				UART_Bytes[UART_Bytes_Counter++] = (byte)(Config_Text_Signals[Cntr_Nmb] * 2);

				for (int Text_Byte_Index = 0; Text_Byte_Index < (Config_Text_Signals[Cntr_Nmb] * 2); Text_Byte_Index++)
				{
					UART_Bytes[UART_Bytes_Counter++] = 0x00;
				}

				UART_Bytes[UART_Bytes_Counter++] = 0x54;// "T" aka TEXT
				UART_Bytes[UART_Bytes_Counter++] = 0x45;// "E" aka END
			}
		}

		Sleep(400);// Задержка для завершения предыдущей команды
		for (int i = 0; i < 5; i++)
		{
			WriteFile(hSerial, UART_Bytes, (DWORD)(Nmb_Of_UART_Bytes + 2), &Wr_Len, NULL);
			Sleep(100);
		}

	}
#pragma endregion

#pragma region Deconfiguration

	byte* Disable_Bytes = new byte[(size_t)(Number_Of_Controllers * 7 + 2)];

	for (int i = 2; i < (Number_Of_Controllers * 7 + 2); i++)
	{
		Disable_Bytes[i] = 0;
	}

	Disable_Bytes[0] = (byte)(Number_Of_Controllers * 7);
	Disable_Bytes[1] = 0x81;

	Sleep(200);
	WriteFile(hSerial, Disable_Bytes, (DWORD)(Number_Of_Controllers * 7 + 2), &Wr_Len, NULL);

#pragma endregion

	//		MessageBox(0, L"OutSignals", L"Inside the DLL", MB_OK);	
}