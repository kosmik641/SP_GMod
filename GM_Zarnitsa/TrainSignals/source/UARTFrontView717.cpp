#include "UARTFrontView717.h"

UARTFrontView717::UARTFrontView717()
{
	PRINT_FUNCSIG;

	InitializeCriticalSection(&m_CriticalSection);
}

UARTFrontView717::~UARTFrontView717()
{
	PRINT_FUNCSIG;

	DeleteCriticalSection(&m_CriticalSection);
}

int UARTFrontView717::Start(int port,int version)
{
	PRINT_FUNCSIG;

	// Создаем объект CUnivCon
	switch (version)
	{
	case 32:
		PRINT_MSG_DBG("Select version 32\n");
		m_UnivConv.reset(new CUnivCon3_2());
		break;
	default:
		PRINT_MSG_ERROR("Version %d not implemented\n",version);
		return -1;
	}

	// Подключение к контроллерам
	CUnivCon::ErrorCode openError = m_UnivConv->Start(port);
	switch (openError)
	{
	case CUnivCon::E_SUCCESS:
		PRINT_MSG("Connected to COM%d\n", port);
		break;
	case CUnivCon::E_GETCOMMSTATE:
		PRINT_MSG_ERROR("GetCommState failed.\n");
		return openError;
	case CUnivCon::E_SETCOMMSTATE:
		PRINT_MSG_ERROR("SetCommMask failed.\n");
		return openError;
	case CUnivCon::E_SETCOMMMASK:
		PRINT_MSG_ERROR("GetCommTimeouts failed.\n");
		return openError;
	case CUnivCon::E_GETCOMMTIMEOUTS:
		PRINT_MSG_ERROR("SetCommTimeouts failed.\n");
		return openError;
	case CUnivCon::E_SETCOMMTIMEOUTS:
		PRINT_MSG_ERROR("SetupComm failed.\n");
		return openError;
	case CUnivCon::E_SETUPCOMM:
		return openError;
	case CUnivCon::E_CONNECTFAIL:
		PRINT_MSG_ERROR("Wrong answer.\n");
		return openError;
	case CUnivCon::E_WRONGPORT:
		PRINT_MSG_ERROR("Port number %d not in range 1-254!", port);
		return openError;
	case CUnivCon::E_HANDLEERROR:
		PRINT_MSG_ERROR("Fail to open COM%d\n", port);
		return openError;
	default:
		PRINT_MSG_ERROR("Unknown error (%d)\n", openError);
		return openError;
	}

	// Загрузка конфигураций
	LoadConfig();
	LoadSleepTimings();
	LoadCalibartions();

	// Запуск потока
	m_DeviceThread = std::thread(&UARTFrontView717::DeviceThreadFunc, this);
	m_DeviceThread.detach();

	return 0;
}

void UARTFrontView717::Stop(bool force)
{
	PRINT_FUNCSIG;

	if (!IsConnected())
		return;

	m_ThreadStop = true;
	m_ThreadForceStop = force;
}

void UARTFrontView717::LoadSleepTimings()
{
	PRINT_FUNCSIG;

	DWORD dwAttrib = GetFileAttributes(SLEEPTIMINGS_FILE);
	if (dwAttrib == INVALID_FILE_ATTRIBUTES)
	{
		PRINT_MSG_ERROR("Sleep timings file %s not found!\n", SLEEPTIMINGS_FILE);
		return;
	}

	ReadSleepTimes();

	PRINT_MSG_DBG("Sleep timings:\n");
	PRINT_MSG_DBG("    AfterRead = %u ms\n",m_sleepTimes.afterRead);
	PRINT_MSG_DBG("    AfterWriteSignals = %u ms\n",m_sleepTimes.afterWriteSignals);
	PRINT_MSG_DBG("    AfterWriteUART = %u ms\n",m_sleepTimes.afterWriteUART);
	PRINT_MSG_DBG("    AfterAll = %u ms\n",m_sleepTimes.afterAll);
}

void UARTFrontView717::LoadCalibartions()
{
	PRINT_FUNCSIG;
	
	DWORD dwAttrib = GetFileAttributes(CALIBRATIONS_FILE);
	if (dwAttrib == INVALID_FILE_ATTRIBUTES)
		CreateCalibrationsFile();

	ReadStopcraneCalibrations();
	ReadKM013Calibrations();

	ReadTCCalibrations();
	ReadNMCalibrations();
	ReadTMCalibrations();
	ReadKiloVoltmeterCalibrations();
	ReadAmmeterCalibrations();
	ReadBattVoltmerCalibrations();

	PRINT_MSG_DBG("Stopcrane:\n");
	PRINT_MSG_DBG("    Off = %d\n", m_StopcraneCalib.m_Off);
	PRINT_MSG_DBG("    On  = %d\n", m_StopcraneCalib.m_On);

	PRINT_MSG_DBG("KM013:\n");
	PRINT_MSG_DBG("    Pos1 = %d\n", m_KM013Calib.m_Pos1);
	PRINT_MSG_DBG("    Pos2 = %d\n", m_KM013Calib.m_Pos2);
	PRINT_MSG_DBG("    Pos3 = %d\n", m_KM013Calib.m_Pos3);
	PRINT_MSG_DBG("    Pos4 = %d\n", m_KM013Calib.m_Pos4);
	PRINT_MSG_DBG("    Pos5 = %d\n", m_KM013Calib.m_Pos5);
	PRINT_MSG_DBG("    Pos6 = %d\n", m_KM013Calib.m_Pos6);
	PRINT_MSG_DBG("    Pos7 = %d\n", m_KM013Calib.m_Pos7);

	PRINT_MSG_DBG("TC:\n");
	PRINT_MSG_DBG("    Min = %d\n", m_TCCalib.m_Min);
	PRINT_MSG_DBG("    Max = %d\n", m_TCCalib.m_Max);

	PRINT_MSG_DBG("NM:\n");
	PRINT_MSG_DBG("    Min = %d\n", m_NMCalib.m_Min);
	PRINT_MSG_DBG("    Max = %d\n", m_NMCalib.m_Max);

	PRINT_MSG_DBG("TM:\n");
	PRINT_MSG_DBG("    Min = %d\n", m_TMCalib.m_Min);
	PRINT_MSG_DBG("    Max = %d\n", m_TMCalib.m_Max);

	PRINT_MSG_DBG("KiloVoltmeter:\n");
	PRINT_MSG_DBG("    Min = %d\n", m_KiloVoltmeterCalib.m_Min);
	PRINT_MSG_DBG("    Max = %d\n", m_KiloVoltmeterCalib.m_Max);

	PRINT_MSG_DBG("Ammeter:\n");
	PRINT_MSG_DBG("    Min = %d\n", m_AmmeterCalib.m_Min);
	PRINT_MSG_DBG("    Max = %d\n", m_AmmeterCalib.m_Max);

	PRINT_MSG_DBG("BattVoltmeter:\n");
	PRINT_MSG_DBG("    Min = %d\n", m_BattVoltmeterCalib.m_Min);
	PRINT_MSG_DBG("    Max = %d\n", m_BattVoltmeterCalib.m_Max);
}

bool UARTFrontView717::IsConnected()
{
	if (m_UnivConv == nullptr)
		return false;
	return m_UnivConv->IsConnected();
}

int UARTFrontView717::GetPortNumber()
{
	if (m_UnivConv == nullptr)
		return -1;
	return m_UnivConv->GetPortNumber();
}

CRITICAL_SECTION* UARTFrontView717::GetCriticalSection()
{
	return &m_CriticalSection;
}

void UARTFrontView717::LoadConfig()
{
	PRINT_FUNCSIG;

	CUnivCon::Configuration& config = m_UnivConv->m_Config;

	// Количество контроллеров
	byte nControllers = GetPrivateProfileIntA("Configuration", "NumberOfControllers", 0, CONFIG_FILE) % 256;
	config.nControllers = nControllers;
	if (nControllers == 0)
		return;

	static char ini_key[64]{};
	static char ini_value[64]{};

	// Количество стрелочных приборов на контроллер
	int nArrows = 0;
	config.arrArrows.reset(new int[nControllers]());
	for (int i = 0; i < nControllers; i++)
	{
		wsprintf(ini_key, "Cntr%d", i);
		config.arrArrows[i] = GetPrivateProfileIntA("Arrows", ini_key, 0, CONFIG_FILE);
		nArrows += config.arrArrows[i];
	}

	// Массив конфигурации 7-ми сегментного индикатора скорости для метро (через дешифратор К514ИД2)
	int n7SegDec = 0;
	config.arr7SegDec.reset(new CUnivCon::SevenDecSignals[nControllers]());
	for (int i = 0; i < nControllers; i++)
	{
		for (int p = 0; p < 3; p++)
		{
			wsprintf(ini_key, "Cntr%dPort%d", i, p);
			config.arr7SegDec[i].port[p] = GetPrivateProfileIntA("7SegDec", ini_key, 0, CONFIG_FILE) > 0;
			n7SegDec += config.arr7SegDec[i].port[p];
		}
	}

	// Массив конфигурации текстовых дисплеев. Число, отличное от нуля, обозначает кол-во символов в одной строке дисплея.
	int nTextDisplays = 0;
	config.arrTextDisplaySize.reset(new int[nControllers]());
	for (int i = 0; i < nControllers; i++)
	{
		wsprintf(ini_key, "Cntr%d", i);
		config.arrTextDisplaySize[i] = GetPrivateProfileIntA("TextDisplay", ini_key, 0, CONFIG_FILE);
		nTextDisplays += (config.arrTextDisplaySize[i] > 0);
	}

	// Количество ADC на контроллер
	int nADC = 0;
	config.arrADCPerController.reset(new int[nControllers]());

	// Главный конфигурационный массив
	int nPins = 24 * nControllers;
	config.arrPins.reset(new int[nPins]());
	for (int i = 0; i < nPins; i++)
	{
		wsprintfA(ini_key, "Pin%d", i);
		GetPrivateProfileStringA("Pins", ini_key, "NotUsed", ini_value, sizeof(ini_value), CONFIG_FILE);
		config.arrPins[i] = ParseConfig(ini_key, ini_value);

		int i_Controller = i / 24;
		if (config.arrPins[i] == CUnivCon::InputADC)
		{
			nADC++;
			config.arrADCPerController[i_Controller]++;
		}
	}

	m_Signals.nPins = nPins;
	m_Signals.nADC = nADC;
	m_Signals.nArrows = nArrows;
	m_Signals.n7SegDec = n7SegDec;
	m_Signals.nTextDisplays = nTextDisplays;

	// Инициализируем массивы для сигналов
	m_Signals.arrInput.reset(new int[nPins]());
	m_Signals.arrOutput.reset(new int[nPins]());
	m_Signals.arrADC.reset(new int[nADC]());
	m_Signals.arrArrow.reset(new int[nArrows]());
	m_Signals.arr7SegDec.reset(new int[n7SegDec]());
	m_Signals.arrTextDisplay.reset(new CUnivCon::TextDisplaySignals[nTextDisplays]());

	// Задание типа Integer для переменных
	m_NW2VarTableInput.VarTable["ControllerPosition"].type = 3;
	m_NW2VarTableInput.VarTable["ReverserPosition"].type = 3;
	m_NW2VarTableInput.VarTable["KRUPosition"].type = 3;
	m_NW2VarTableInput.VarTable["CranePosition"].type = 3;

	PRINT_MSG_DBG("config.nControllers = %d\n", nControllers);
	PRINT_MSG_DBG("m_Signals.arrInput:		size = %d\n", nPins);
	PRINT_MSG_DBG("m_Signals.arrOutput		size = %d\n", nPins);
	PRINT_MSG_DBG("m_Signals.arrADC			size = %d\n", nADC);
	PRINT_MSG_DBG("m_Signals.arrArrow		size = %d\n", nArrows);
	PRINT_MSG_DBG("m_Signals.arr7SegDec		size = %d\n", n7SegDec);
	PRINT_MSG_DBG("m_Signals.arrTextDisplay size = %d\n", nTextDisplays);
}

void UARTFrontView717::DeviceThreadFunc()
{
	m_ThreadRunning = true;
	m_ThreadStop = false;
	m_ThreadForceStop = false;

	// Конфигурируем устройство
	int setupResult = m_UnivConv->Setup();
	if (setupResult != CUnivCon::E_SUCCESS)
	{
		PRINT_MSG("Configuration failed (0x%02X)\n", setupResult);
		m_ThreadStop = true;
		m_ThreadForceStop = true;
	}
	else
	{
		PRINT_MSG("Successfully connected!\n");
	}

	while (!m_ThreadStop)
	{
		// Входящие сигналы
		m_UnivConv->ReadSignals(m_Signals);
		DataExchangeInputs();
		Sleep(m_sleepTimes.afterRead);
		
		// Исходящие сигналы
		DataExchangeOutputs();
		m_UnivConv->WriteSignals(m_Signals);
		Sleep(m_sleepTimes.afterWriteSignals);
		m_UnivConv->WriteUARTData(m_Signals);
		Sleep(m_sleepTimes.afterWriteUART);

		Sleep(m_sleepTimes.afterAll);
	}

	if (!m_ThreadForceStop)
		m_UnivConv->Stop();

	m_NW2VarTableInput.VarTable.clear();
	m_NW2VarTableOutput.VarTable.clear();

	m_ThreadRunning = false;
	PRINT_MSG("Thread working end.\n");
}

void UARTFrontView717::DataExchangeInputs()
{
	EnterCriticalSection(&m_CriticalSection);

	auto& inTable = m_NW2VarTableInput.VarTable;
	inTable["R_UNch"].val = m_Signals.arrInput[8];
	inTable["R_ZS"].val = m_Signals.arrInput[9];
	inTable["R_G"].val = m_Signals.arrInput[10];
	inTable["R_Radio"].val = m_Signals.arrInput[11];
	inTable["R_Program2"].val = m_Signals.arrInput[12];
	inTable["R_Program1"].val = m_Signals.arrInput[13];
	inTable["VUD1"].val = m_Signals.arrInput[14];
	inTable["R_VPR"].val = m_Signals.arrInput[15];
	inTable["VMK"].val = m_Signals.arrInput[16];
	inTable["BPSNon"].val = m_Signals.arrInput[17];
	//inTable["L_Emer"].val = m_Signals.arrInput[18]; // Not implemented
	inTable["RezMK"].val = m_Signals.arrInput[19];
	inTable["ARS13"].val = m_Signals.arrInput[20];
	inTable["R_ASNPUp"].val = m_Signals.arrInput[21];
	inTable["R_ASNPDown"].val = m_Signals.arrInput[22];
	inTable["R_ASNPMenu"].val = m_Signals.arrInput[23];

	static int controllerPos = 0;
	int controllerPosBit =
		(m_Signals.arrInput[25] << 4) | 
		(m_Signals.arrInput[27] << 3) | 
		(m_Signals.arrInput[29] << 2) | 
		(m_Signals.arrInput[30] << 1) | 
		(m_Signals.arrInput[31]);

	switch (controllerPosBit)
	{
	case 0x05:
		controllerPos = 857; // Х3
		break;
	case 0x04:
		controllerPos = 714; // Х2
		break;
	case 0x00:
		controllerPos = 571; // Х1
		break;
	case 0x08:
		controllerPos = 429; // 0
		break;
	case 0x02:
		controllerPos =	286; // Т1
		break;
	case 0x06:
		controllerPos = 143; // Т1А
		break;
	case 0x16:
		controllerPos = 0; // Т2
		break;
	default:
		break;
	}

	inTable["ReverserPosition"].val = m_Signals.arrInput[26] ? (m_Signals.arrInput[28] ? 0 : 2) : 1; // 0 - Назад, 2 - Вперед
	inTable["ControllerPosition"].val = controllerPos;

	inTable["ARS"].val = m_Signals.arrInput[32];
	inTable["ALS"].val = m_Signals.arrInput[33];
	inTable["ARSR"].val = m_Signals.arrInput[34];
	inTable["ALSFreq"].val = m_Signals.arrInput[35];
	inTable["L_1"].val = m_Signals.arrInput[36];
	inTable["L_2"].val = m_Signals.arrInput[37];
	inTable["L_3"].val = m_Signals.arrInput[38];
	inTable["VP"].val = m_Signals.arrInput[39];
	inTable["KRZD"].val = m_Signals.arrInput[40];
	inTable["VozvratRP"].val = m_Signals.arrInput[41];
	inTable["KDL"].val = m_Signals.arrInput[42];
	inTable["KDLR"].val = m_Signals.arrInput[43];
	inTable["DoorSelect"].val = m_Signals.arrInput[44];
	inTable["KVT"].val = m_Signals.arrInput[45];
	inTable["KVTR"].val = m_Signals.arrInput[46];
	inTable["OtklAVU"].val = m_Signals.arrInput[47];
	inTable["V11"].val = m_Signals.arrInput[48];
	inTable["V13"].val = m_Signals.arrInput[49];
	inTable["V12"].val = m_Signals.arrInput[50];
	inTable["ConverterProtection"].val = m_Signals.arrInput[51];
	inTable["KSN"].val = m_Signals.arrInput[52];
	inTable["Ring"].val = m_Signals.arrInput[53];
	inTable["OtklBV"].val = m_Signals.arrInput[54];
	inTable["VZ1"].val = m_Signals.arrInput[55];
	inTable["KRP"].val = m_Signals.arrInput[56];
	inTable["KAH"].val = m_Signals.arrInput[57];
	inTable["L_4"].val = m_Signals.arrInput[58];
	inTable["VUS"].val = m_Signals.arrInput[59];
	inTable["VAD"].val = m_Signals.arrInput[60];
	inTable["VAH"].val = m_Signals.arrInput[61];
	inTable["VKST"].val = m_Signals.arrInput[62];
	inTable["KDP"].val = m_Signals.arrInput[63];

	inTable["KRUPosition"].val = m_Signals.arrInput[64] + m_Signals.arrInput[65];

	inTable["IGLA1"].val = m_Signals.arrInput[67];
	inTable["IGLA2"].val = m_Signals.arrInput[68];
	inTable["IGLA3"].val = m_Signals.arrInput[69];
	inTable["IGLA4"].val = m_Signals.arrInput[70];

	inTable["VUD2"].val = m_Signals.arrInput[72];
	inTable["VDL"].val = m_Signals.arrInput[73];
	inTable["R_Program1H"].val = m_Signals.arrInput[74];
	inTable["R_Program2H"].val = m_Signals.arrInput[75];
	inTable["PB"].val = m_Signals.arrInput[76];
	inTable["HornState"].val = m_Signals.arrInput[77];

	//inTable["UAVA"].val = (m_Signals.arrInput[90] == 0);

	inTable["RC1"].val = m_Signals.arrInput[96];
	inTable["VB"].val = m_Signals.arrInput[97];
	inTable["UOS"].val = m_Signals.arrInput[98];

	inTable["DriverValveDisconnect"].val = (m_Signals.arrInput[104] == 0);
	inTable["ParkingBrake"].val = (m_Signals.arrInput[105] == 0);
	inTable["EPK"].val = (m_Signals.arrInput[106] == 0);

	inTable["A54"].val = m_Signals.arrInput[112];
	inTable["A84"].val = m_Signals.arrInput[113];
	inTable["A10"].val = m_Signals.arrInput[114];
	inTable["A53"].val = m_Signals.arrInput[115];
	inTable["A49"].val = m_Signals.arrInput[116];
	inTable["A27"].val = m_Signals.arrInput[117];
	inTable["AS1"].val = m_Signals.arrInput[118];
	inTable["A21"].val = m_Signals.arrInput[119];
	inTable["A26"].val = m_Signals.arrInput[120];
	inTable["AR63"].val= m_Signals.arrInput[121];
	inTable["A17"].val = m_Signals.arrInput[122];
	inTable["A44"].val = m_Signals.arrInput[123];
	inTable["A45"].val = m_Signals.arrInput[124];
	inTable["A11"].val = m_Signals.arrInput[125];
	inTable["A71"].val = m_Signals.arrInput[126];
	inTable["A41"].val = m_Signals.arrInput[127];
	inTable["A74"].val = m_Signals.arrInput[128];
	inTable["A73"].val = m_Signals.arrInput[129];
	//inTable["A79"].val = m_Signals.arrInput[130]; // Not implemented
	inTable["A42"].val = m_Signals.arrInput[131];
	inTable["A46"].val = m_Signals.arrInput[132];
	inTable["A47"].val = m_Signals.arrInput[133];
	inTable["AV1"].val = m_Signals.arrInput[134];
	inTable["A29"].val = m_Signals.arrInput[135];
	inTable["A76"].val = m_Signals.arrInput[136];
	inTable["A48"].val = m_Signals.arrInput[137];
	inTable["A56"].val = m_Signals.arrInput[138];
	inTable["A65"].val = m_Signals.arrInput[139];
	inTable["A25"].val = m_Signals.arrInput[140];
	inTable["A30"].val = m_Signals.arrInput[141];
	inTable["A1"].val  = m_Signals.arrInput[142];
	inTable["A20"].val = m_Signals.arrInput[143];
	inTable["A32"].val = m_Signals.arrInput[144];
	inTable["A13"].val = m_Signals.arrInput[145];
	inTable["A43"].val = m_Signals.arrInput[146];
	inTable["A31"].val = m_Signals.arrInput[147];
	//inTable["A77"].val = m_Signals.arrInput[148]; // Not implemented
	//inTable["A78"].val = m_Signals.arrInput[149]; // Not implemented
	inTable["VBD"].val = m_Signals.arrInput[150];
	inTable["A75"].val = m_Signals.arrInput[151];

	inTable["A22"].val = m_Signals.arrInput[152];
	inTable["A8"].val  = m_Signals.arrInput[153];
	inTable["A28"].val = m_Signals.arrInput[154];
	inTable["A38"].val = m_Signals.arrInput[155];
	inTable["A14"].val = m_Signals.arrInput[156];
	inTable["A39"].val = m_Signals.arrInput[157];
	inTable["A6"].val  = m_Signals.arrInput[158];
	inTable["A70"].val = m_Signals.arrInput[159];
	inTable["A4"].val  = m_Signals.arrInput[160];
	inTable["A5"].val  = m_Signals.arrInput[161];
	inTable["A2"].val  = m_Signals.arrInput[162];
	inTable["A3"].val  = m_Signals.arrInput[163];
	inTable["A50"].val = m_Signals.arrInput[164];
	inTable["A52"].val = m_Signals.arrInput[165];
	inTable["A40"].val = m_Signals.arrInput[166];
	inTable["A80"].val = m_Signals.arrInput[167];
	inTable["A66"].val = m_Signals.arrInput[168];
	inTable["A18"].val = m_Signals.arrInput[169];
	inTable["A24"].val = m_Signals.arrInput[170];
	inTable["A19"].val = m_Signals.arrInput[171];
	inTable["A37"].val = m_Signals.arrInput[172];
	inTable["A51"].val = m_Signals.arrInput[173];
	inTable["A12"].val = m_Signals.arrInput[174];
	inTable["A16"].val = m_Signals.arrInput[175];
	inTable["A68"].val = m_Signals.arrInput[176];
	inTable["A72"].val = m_Signals.arrInput[177];
	inTable["A7"].val  = m_Signals.arrInput[178];
	inTable["A9"].val  = m_Signals.arrInput[179];
	inTable["A57"].val = m_Signals.arrInput[180];
	inTable["A81"].val = m_Signals.arrInput[181];
	//inTable["A82"].val = m_Signals.arrInput[182]; // Not implemented
	inTable["A15"].val = m_Signals.arrInput[183];
	inTable["AV6"].val = m_Signals.arrInput[184];
	//inTable["A83"].val = m_Signals.arrInput[185]; // Not implemented
	inTable["AIS"].val = m_Signals.arrInput[186];
	inTable["AV3"].val = m_Signals.arrInput[187];
	//inTable["UPPS_On"].val = m_Signals.arrInput[188]; // TODO

	inTable["EmergencyBrakeValve"].val = !ADCStopcrane(m_Signals.arrADC[0]);
	inTable["CranePosition"].val = ADCKM013(m_Signals.arrADC[1]) * 1000;

	LeaveCriticalSection(&m_CriticalSection);
}

void UARTFrontView717::DataExchangeOutputs()
{
	EnterCriticalSection(&m_CriticalSection);

	// Индикация
	auto& outTable = m_NW2VarTableOutput.VarTable;
	m_Signals.arrOutput[200] = outTable["AR04"].val;
	m_Signals.arrOutput[201] = outTable["AR0"].val;
	m_Signals.arrOutput[202] = outTable["AR40"].val;
	m_Signals.arrOutput[203] = outTable["AR60"].val;
	m_Signals.arrOutput[204] = outTable["AR70"].val;
	m_Signals.arrOutput[205] = outTable["AR80"].val;
	m_Signals.arrOutput[206] = outTable["SD"].val;

	m_Signals.arrOutput[208] = outTable["HRK"].val;
	m_Signals.arrOutput[209] = outTable["RP"].val;
	m_Signals.arrOutput[210] = outTable["SN"].val;
	m_Signals.arrOutput[211] = outTable["GLIB"].val;
	m_Signals.arrOutput[212] = outTable["KVC"].val;
	m_Signals.arrOutput[213] = outTable["LN"].val;
	m_Signals.arrOutput[214] = outTable["RS"].val;
	m_Signals.arrOutput[215] = outTable["KVD"].val;

	m_Signals.arrOutput[219] = outTable["VD"].val;
	m_Signals.arrOutput[220] = outTable["KT"].val;
	m_Signals.arrOutput[221] = outTable["ST"].val;
	
	m_Signals.arrOutput[227] = outTable["DoorsLeftL"].val;
	m_Signals.arrOutput[228] = outTable["DoorsLeftL"].val;
	m_Signals.arrOutput[229] = outTable["GreenRP"].val;
	m_Signals.arrOutput[230] = outTable["RZP"].val;
	m_Signals.arrOutput[231] = outTable["L1"].val;
	m_Signals.arrOutput[232] = outTable["RZP"].val;
	m_Signals.arrOutput[233] = outTable["AVU"].val;
	m_Signals.arrOutput[234] = outTable["LKVP"].val;
	m_Signals.arrOutput[235] = outTable["PN"].val;
	m_Signals.arrOutput[236] = outTable["ISTLamp"].val;
	m_Signals.arrOutput[237] = outTable["DoorsRightL"].val;

	m_Signals.arrOutput[282] = outTable["IGLA:ButtonL4"].val;
	m_Signals.arrOutput[283] = outTable["IGLA:ButtonL3"].val;
	m_Signals.arrOutput[284] = outTable["IGLA:ButtonL2"].val;
	m_Signals.arrOutput[285] = outTable["IGLA:ButtonL1"].val;
	m_Signals.arrOutput[286] = outTable["IGLA:Error"].val;
	m_Signals.arrOutput[287] = outTable["IGLA:Fire"].val;

	// Стрелочная индикация
	m_Signals.arrArrow[0] = StepTC(m_NW2VarTableOutput.GetPackedRatio("BCPressure"));
	m_Signals.arrArrow[1] = StepNM(m_NW2VarTableOutput.GetPackedRatio("TLPressure"));
	m_Signals.arrArrow[2] = StepTM(m_NW2VarTableOutput.GetPackedRatio("BLPressure"));
	m_Signals.arrArrow[3] = StepAmmeter(m_NW2VarTableOutput.GetPackedRatio("EnginesCurrent"));
	m_Signals.arrArrow[4] = StepKiloVoltmeter(m_NW2VarTableOutput.GetPackedRatio("EnginesVoltage"));
	m_Signals.arrArrow[5] = StepBattVoltmeter(m_NW2VarTableOutput.GetPackedRatio("BatteryVoltage"));

	// Скоростемер
	m_Signals.arr7SegDec[0] = outTable["LUDS"].val ? int(m_NW2VarTableOutput.GetPackedRatio("Speed") * 100.0f) : -1;

	// Дисплеи
	// АСОТП
	int asotpState = outTable["IGLA:State"].val;

	static bool asotpDisplayOn = true;
	if (asotpState == 2)
		asotpDisplayOn = (outTable["IGLA:Standby"].val == 0) || outTable["IGLA:ShowTime"].val;
	else 
		asotpDisplayOn = (asotpState != -2);

	m_Signals.arrTextDisplay[0].on = m_Signals.arrTextDisplay[0].ledOn = asotpDisplayOn;
	convertUtf8ToCp1251(m_ASOTPText.c_str(), m_Signals.arrTextDisplay[0].text, 40);
	
	// АСНП
	m_Signals.arrTextDisplay[1].on = m_Signals.arrTextDisplay[1].ledOn = (outTable["ASNP:State"].val != 0);
	convertUtf8ToCp1251(m_ASNPText.c_str(), m_Signals.arrTextDisplay[1].text, 48);

	LeaveCriticalSection(&m_CriticalSection);
}

bool UARTFrontView717::CreateCalibrationsFile()
{
	auto hFile = CreateFile(CALIBRATIONS_FILE, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	CloseHandle(hFile);
	return true;
}

bool UARTFrontView717::ADCStopcrane(int adc)
{
	bool retVal = false;

	int m_Off = m_StopcraneCalib.m_Off;
	int m_On = m_StopcraneCalib.m_On;

	if (adc >= m_On)
		retVal = true;
	else if (adc <= m_Off)
		retVal = false;

	return retVal;
}

int UARTFrontView717::ADCKM013(int adc)
{
	int retVal = 7;

	int pos_1 = m_KM013Calib.m_Pos1;
	int pos_2 = m_KM013Calib.m_Pos2;
	int pos_3 = m_KM013Calib.m_Pos3;
	int pos_4 = m_KM013Calib.m_Pos4;
	int pos_5 = m_KM013Calib.m_Pos5;
	int pos_6 = m_KM013Calib.m_Pos6;
	int pos_7 = m_KM013Calib.m_Pos7;

	if (adc <= (pos_1 + pos_2) / 2)
		retVal = 1;
	else if (adc > (pos_1 + pos_2) / 2 && adc <= (pos_2 + pos_3) / 2)
		retVal = 2;
	else if (adc > (pos_2 + pos_3) / 2 && adc <= (pos_3 + pos_4) / 2)
		retVal = 3;
	else if (adc > (pos_3 + pos_4) / 2 && adc <= (pos_4 + pos_5) / 2)
		retVal = 4;
	else if (adc > (pos_4 + pos_5) / 2 && adc <= (pos_5 + pos_6) / 2)
		retVal = 5;
	else if (adc > (pos_5 + pos_6) / 2 && adc <= (pos_6 + pos_7) / 2)
		retVal = 6;
	else if (adc > (pos_6 + pos_7) / 2)
		retVal = 7;

	return retVal;
}

int UARTFrontView717::StepTC(float value)
{
	int m_Min = m_TCCalib.m_Min;
	int m_Max = m_TCCalib.m_Max;

	float delta1Atm = float(m_Max - m_Min);
	int retVal = m_Min + int(value * delta1Atm);

	if (m_Max > m_Min)
		return minmax(retVal, m_Min, m_Max);
	else
		return minmax(retVal, m_Max, m_Min);
}

int UARTFrontView717::StepNM(float value)
{
	int m_Min = m_NMCalib.m_Min;
	int m_Max = m_NMCalib.m_Max;

	float delta1Atm = float(m_Max - m_Min);
	int retVal = m_Min + int(value * delta1Atm);

	if (m_Max > m_Min)
		return minmax(retVal, m_Min, m_Max);
	else
		return minmax(retVal, m_Max, m_Min);
}

int UARTFrontView717::StepTM(float value)
{
	int m_Min = m_TMCalib.m_Min;
	int m_Max = m_TMCalib.m_Max;

	float delta1Atm = float(m_Max - m_Min);
	int retVal = m_Min + int(value * delta1Atm);

	if (m_Max > m_Min)
		return minmax(retVal, m_Min, m_Max);
	else
		return minmax(retVal, m_Max, m_Min);
}

int UARTFrontView717::StepKiloVoltmeter(float value)
{
	int m_Min = m_KiloVoltmeterCalib.m_Min;
	int m_Max = m_KiloVoltmeterCalib.m_Max;

	float delta1V = float(m_Max - m_Min);
	int retVal = m_Min + int(value * delta1V);

	if (m_Max > m_Min)
		return minmax(retVal, m_Min, m_Max);
	else
		return minmax(retVal, m_Max, m_Min);
}

int UARTFrontView717::StepAmmeter(float value)
{
	int m_Min = m_AmmeterCalib.m_Min;
	int m_Max = m_AmmeterCalib.m_Max;

	float delta01A = float(m_Max - m_Min);
	int retVal = m_Min + int(value * delta01A);

	if (m_Max > m_Min)
		return minmax(retVal, m_Min, m_Max);
	else
		return minmax(retVal, m_Max, m_Min);
}

int UARTFrontView717::StepBattVoltmeter(float value)
{
	int m_Min = m_BattVoltmeterCalib.m_Min;
	int m_Max = m_BattVoltmeterCalib.m_Max;

	float delta1V = float(m_Max - m_Min);
	int retVal = m_Min + int(value * delta1V);

	if (m_Max > m_Min)
		return minmax(retVal, m_Min, m_Max);
	else
		return minmax(retVal, m_Max, m_Min);
}

void UARTFrontView717::ReadSleepTimes()
{
	m_sleepTimes.afterRead = GetPrivateProfileInt("Sleep", "AfterRead", 5, SLEEPTIMINGS_FILE);
	m_sleepTimes.afterWriteSignals = GetPrivateProfileInt("Sleep", "AfterWriteSignals", 25, SLEEPTIMINGS_FILE);
	m_sleepTimes.afterWriteUART = GetPrivateProfileInt("Sleep", "AfterWriteUART", 30, SLEEPTIMINGS_FILE);
	m_sleepTimes.afterAll = GetPrivateProfileInt("Sleep", "AfterAll", 30, SLEEPTIMINGS_FILE);
}

void UARTFrontView717::ReadStopcraneCalibrations()
{
	PRINT_FUNCSIG;

	m_StopcraneCalib.m_Off = GetPrivateProfileInt("Stopcrane", "Off", 0, CALIBRATIONS_FILE);
	m_StopcraneCalib.m_On = GetPrivateProfileInt("Stopcrane", "On", 1, CALIBRATIONS_FILE);
}

void UARTFrontView717::ReadKM013Calibrations()
{
	PRINT_FUNCSIG;

	m_KM013Calib.m_Pos1 = GetPrivateProfileInt("KM013", "Pos1", 0, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos2 = GetPrivateProfileInt("KM013", "Pos2", 1, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos3 = GetPrivateProfileInt("KM013", "Pos3", 2, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos4 = GetPrivateProfileInt("KM013", "Pos4", 3, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos5 = GetPrivateProfileInt("KM013", "Pos5", 4, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos6 = GetPrivateProfileInt("KM013", "Pos6", 5, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos7 = GetPrivateProfileInt("KM013", "Pos7", 6, CALIBRATIONS_FILE);
}

void UARTFrontView717::ReadTCCalibrations()
{
	PRINT_FUNCSIG;

	m_TCCalib.m_Min = GetPrivateProfileInt("TC", "Min", 0, CALIBRATIONS_FILE);
	m_TCCalib.m_Max = GetPrivateProfileInt("TC", "Max", 6, CALIBRATIONS_FILE);
}

void UARTFrontView717::ReadNMCalibrations()
{
	PRINT_FUNCSIG;

	m_NMCalib.m_Min = GetPrivateProfileInt("NM", "Min", 0, CALIBRATIONS_FILE);
	m_NMCalib.m_Max = GetPrivateProfileInt("NM", "Max", 16, CALIBRATIONS_FILE);
}

void UARTFrontView717::ReadTMCalibrations()
{
	PRINT_FUNCSIG;

	m_TMCalib.m_Min = GetPrivateProfileInt("TM", "Min", 0, CALIBRATIONS_FILE);
	m_TMCalib.m_Max = GetPrivateProfileInt("TM", "Max", 16, CALIBRATIONS_FILE);
}

void UARTFrontView717::ReadKiloVoltmeterCalibrations()
{
	PRINT_FUNCSIG;

	m_KiloVoltmeterCalib.m_Min = GetPrivateProfileInt("KiloVoltmeter", "Min", 0, CALIBRATIONS_FILE);
	m_KiloVoltmeterCalib.m_Max = GetPrivateProfileInt("KiloVoltmeter", "Max", 1000, CALIBRATIONS_FILE);
}

void UARTFrontView717::ReadAmmeterCalibrations()
{
	PRINT_FUNCSIG;

	m_AmmeterCalib.m_Min = GetPrivateProfileInt("Ammeter", "Min", 0, CALIBRATIONS_FILE);
	m_AmmeterCalib.m_Max = GetPrivateProfileInt("Ammeter", "Max", 1000, CALIBRATIONS_FILE);
}

void UARTFrontView717::ReadBattVoltmerCalibrations()
{
	PRINT_FUNCSIG;

	m_BattVoltmeterCalib.m_Min = GetPrivateProfileInt("BattVoltmeter", "Min", 0, CALIBRATIONS_FILE);
	m_BattVoltmeterCalib.m_Max = GetPrivateProfileInt("BattVoltmeter", "Max", 150, CALIBRATIONS_FILE);
}

CUnivCon::ConfigState UARTFrontView717::ParseConfig(const char* key, const char* value)
{
	auto val = std::regex_replace(value, std::regex(R"(^\s*(.*?)\s*;.*$)"), "$1");
	if (val == "0" || val == "NotUsed")
		return CUnivCon::NotUsed;
	else if (val == "1" || val == "Input")
		return CUnivCon::Input;
	else if (val == "2" || val == "Output")
		return CUnivCon::Output;
	else if (val == "3" || val == "InputADC")
		return CUnivCon::InputADC;

	PRINT_MSG_ERROR("Unknown setting in %s: \"%s\"!\n", key, val.c_str());
	return CUnivCon::NotUsed;
}