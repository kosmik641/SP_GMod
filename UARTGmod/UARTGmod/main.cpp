#include "main.h"
#define xCont Glob::mainContainer
std::map<std::string, std::map<std::string, double>> xCont;
CRITICAL_SECTION Glob::UARTFrontViewClient::m_SignalCriticalSection;
bool Glob::debugInfo = false;

LUA_FUNCTION_STATIC(API_ReceiveBtnTable)
{
	if (!Glob::UARTFrontViewClient::threadWorking()) return 0;
	LUA->CheckType(1, GarrysMod::Lua::Type::Table);
	LUA->CheckString(2);
	LUA->CheckString(3);
	LUA->PushNil();
	while (LUA->Next(1) != 0) {
		xCont["Output"][LUA->GetString(-2)] = LUA->GetNumber(-1);
		LUA->Pop();
	}
	Glob::UTF8_2_1251(LUA->GetString(2), (char*)Glob::ASNPText, 96);
	Glob::UTF8_2_1251(LUA->GetString(3), (char*)Glob::ASOTPText, 80);
	Glob::UARTFrontViewClient::Metro81717Signals::outSignals();
	return 0;
}

LUA_FUNCTION_STATIC(API_SendBtnTable)
{
	if (!Glob::UARTFrontViewClient::threadWorking()) return 0;
	Glob::UARTFrontViewClient::Metro81717Signals::inSignals();

	LUA->CreateTable();
	for (auto it = xCont["Input"].begin(); it != xCont["Input"].end(); ++it) {
		LUA->PushString((it->first).c_str());
		LUA->PushNumber(it->second);
		LUA->RawSet(-3);
	}
	return 1;
}

LUA_FUNCTION_STATIC(API_SendBtnTableByKey)
{
	LUA->CheckString(1);
	LUA->CheckString(2);
	bool locking = false;
	if (Glob::UARTFrontViewClient::threadWorking()) {
		EnterCriticalSection(&(Glob::UARTFrontViewClient::m_SignalCriticalSection));
		locking = true;
	}
	const char* str1 = LUA->GetString(1);
	const char* str2 = LUA->GetString(2);
	int retVal = xCont[str1][str2];
	if (locking) LeaveCriticalSection(&(Glob::UARTFrontViewClient::m_SignalCriticalSection));
	LUA->PushNumber(retVal);
	return 1;
}

LUA_FUNCTION_STATIC(API_InitUART)
{
	if (Glob::UARTFrontViewClient::threadWorking()) return 0;
	int RetVal = -10;
	LUA->CheckNumber(1);
	int portNumber = LUA->GetNumber(1);
	RetVal = Glob::UARTFrontViewClient::Metro81717Signals::startSignals(portNumber);
	LUA->PushNumber(RetVal);
	return 1;
}

LUA_FUNCTION_STATIC(API_Shutdown)
{
	if (!Glob::UARTFrontViewClient::threadWorking()) return 0;
	Glob::UARTFrontViewClient::Metro81717Signals::stopSignals();
	return 0;
}

LUA_FUNCTION_STATIC(API_GetThreadStatus) {
	LUA->PushBool(Glob::UARTFrontViewClient::threadWorking());
	return 1;
}

LUA_FUNCTION_STATIC(API_CalibrateADC) {
	const char* ADC = "ADC";
	const char* Step = "Step";

	LUA->CheckString(1);
	LUA->CheckNumber(2);
	LUA->CheckNumber(3);
	LUA->CheckNumber(4);
	const char* configType = LUA->GetString(1);
	int configTypeNum = LUA->GetNumber(2);
	int configTypePos = LUA->GetNumber(3);
	int configVal = LUA->GetNumber(4);
	int maxPos = 1;
	int* configPtr;
	bool locking = false;
	if (Glob::UARTFrontViewClient::threadWorking()) {
		EnterCriticalSection(&Glob::UARTFrontViewClient::m_SignalCriticalSection);
		locking = true;
	}
	if (!strcmp(configType,ADC)) {
		switch (configTypeNum)
		{
		case 1:
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Adc1c;
			break;
		case 2:
			maxPos = 6;
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Adc2c;
			break;
		case 3:
			maxPos = 6;
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Adc3c;
			break;
		default:
			printf("Enter number in range from 1 to 3\n");
			if (locking) LeaveCriticalSection(&(Glob::UARTFrontViewClient::m_SignalCriticalSection));
			return 0;
			break;
		}
	}
	else if (!strcmp(configType, Step)) {
		switch (configTypeNum)
		{
		case 1:
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Step1c;
			break;
		case 2:
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Step2c;
			break;
		case 3:
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Step3c;
			break;
		case 4:
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Step4c;
			break;
		case 5:
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Step5c;
			break;
		case 6:
			configPtr = Glob::UARTFrontViewClient::Metro81717Signals::configValues.Step6c;
			break;
		default:
			printf("Enter number in range from 1 to 6\n");
			if (locking) LeaveCriticalSection(&(Glob::UARTFrontViewClient::m_SignalCriticalSection));
			return 0;
			break;
		}
	}
	else {
		printf("Not found calibration type (ADC|Step)\n");
		if (locking) LeaveCriticalSection(&(Glob::UARTFrontViewClient::m_SignalCriticalSection));
		return 0;
	}
	
	if (configTypePos > maxPos) {
		printf("Enter number in range from 0 to %d\n", maxPos);
		if (locking) LeaveCriticalSection(&(Glob::UARTFrontViewClient::m_SignalCriticalSection));
		return 0;
	}

	if (configVal > -1) {
		int oldVal = configPtr[configTypePos];
		if (configVal > 27999) configVal = 27999;
		configPtr[configTypePos] = configVal;
		printf("+++       UART: %s%dc[%d]: %d -> %d\n", configType, configTypeNum, configTypePos, oldVal, configPtr[configTypePos]);
	}
	else printf("Current value of %s%dc[%d]: %d\n", configType, configTypeNum, configTypePos, configPtr[configTypePos]);
	if (locking) LeaveCriticalSection(&(Glob::UARTFrontViewClient::m_SignalCriticalSection));
	return 0;
}

LUA_FUNCTION_STATIC(API_ChangeDebugState) {
	if (!Glob::UARTFrontViewClient::threadWorking()) return 0;
	EnterCriticalSection(&Glob::UARTFrontViewClient::m_SignalCriticalSection);
	Glob::debugInfo = !Glob::debugInfo;
	LeaveCriticalSection(&(Glob::UARTFrontViewClient::m_SignalCriticalSection));
	return 0;
}

LUA_FUNCTION(API_Version)
{
	char date_str[30];
	sprintf_s(date_str, "Bulded %s %s", __TIME__, __DATE__);
	LUA->PushString(date_str);
	return 1;
}

// Called when the module opens
GMOD_MODULE_OPEN()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->CreateTable();
	PushCLua(API_InitUART, "Initialize");		// int UART.Initialize()
	PushCLua(API_ReceiveBtnTable, "Send");		// UART.Send(table)
	PushCLua(API_SendBtnTable, "Get");			// table UART.Get()
	PushCLua(API_SendBtnTableByKey, "GetByKey");// int UART.GetByKey(string,string)
	PushCLua(API_GetThreadStatus, "Status");	// bool UART.Status()
	PushCLua(API_Shutdown, "Shutdown");			// UART.Shutdown()
	PushCLua(API_Shutdown, "Shutdown");			// UART.Shutdown()
	PushCLua(API_CalibrateADC, "SetADC");			// UART.ADC(int,int,int,int)
	PushCLua(API_ChangeDebugState, "DebugADC");	// UART.DebugADC()
	PushCLua(API_Version, "Version");			// string UART.Version()
	LUA->SetField(-2, "UART");
	LUA->Pop();

	return 0;
}
// Called when the module closes
GMOD_MODULE_CLOSE()
{
	LUA->PushNil();
	LUA->SetField(GarrysMod::Lua::INDEX_GLOBAL, "UART");
	return 0;
}

int Glob::UTF8_2_1251(const char* utf8, char* windows1251, size_t n)
{
	int i = 0;
	int j = 0;
	for (; i < (int)n && utf8[i] != 0; ++i) {
		char prefix = utf8[i];
		char suffix = utf8[i + 1];
		if ((prefix & 0x80) == 0) {
			windows1251[j] = (char)prefix;
			++j;
		}
		else if ((~prefix) & 0x20) {
			int first5bit = prefix & 0x1F;
			first5bit <<= 6;
			int sec6bit = suffix & 0x3F;
			int unicode_char = first5bit + sec6bit;
			if (unicode_char >= 0x410 && unicode_char <= 0x44F) {
				windows1251[j] = (char)(unicode_char - 0x350);
			}
			else if (unicode_char >= 0x80 && unicode_char <= 0xFF) {
				windows1251[j] = (char)(unicode_char);
			}
			else if (unicode_char >= 0x402 && unicode_char <= 0x403) {
				windows1251[j] = (char)(unicode_char - 0x382);
			}
			else {
				int count = sizeof(g_letters) / sizeof(Letter);
				for (int k = 0; k < count; ++k) {
					if (unicode_char == g_letters[k].unicode) {
						windows1251[j] = g_letters[k].win1251;
						goto NEXT_LETTER;
					}
				}
				// can't convert this char
				printf("ERROR: Cant convert char �%d\n",j);
				return 0;
			}
		NEXT_LETTER:
			++i;
			++j;
		}
		else {
			// can't convert this chars
			printf("ERROR: Cant convert input string\n");
			return 0;
		}
	}
	windows1251[j] = 0;
	return 1;
}