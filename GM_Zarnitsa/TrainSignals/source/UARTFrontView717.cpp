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

int UARTFrontView717::Start(int port)
{
	PRINT_FUNCSIG;

	int model = GetPrivateProfileIntA("Configuration", "Model", 0, CONFIG_FILE);

	// Создаем объект CUnivCon
	switch (model)
	{
	case 32:
		PRINT_MSG_DBG("Select model 32\n");
		m_UnivConv.reset(new CUnivCon3_2());
		break;
	case 37:
		PRINT_MSG_DBG("Select model 37\n");
		m_UnivConv.reset(new CUnivCon3_7());
		break;
	default:
		PRINT_MSG_ERROR("Model %d not implemented\n", model);
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
	ReadControllerCalibrations();

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

	PRINT_MSG_DBG("Controller:\n");
	PRINT_MSG_DBG("    PosX3  = %d\n", m_ControllerCalib.m_PosX3);
	PRINT_MSG_DBG("    PosX2  = %d\n", m_ControllerCalib.m_PosX2);
	PRINT_MSG_DBG("    PosX1  = %d\n", m_ControllerCalib.m_PosX1);
	PRINT_MSG_DBG("    Pos0   = %d\n", m_ControllerCalib.m_Pos0);
	PRINT_MSG_DBG("    PosT1  = %d\n", m_ControllerCalib.m_PosT1);
	PRINT_MSG_DBG("    PosT1a = %d\n", m_ControllerCalib.m_PosT1a);
	PRINT_MSG_DBG("    PosT2  = %d\n", m_ControllerCalib.m_PosT2);

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
	inTable["A17"].val = m_Signals.arrInput[0];
	inTable["A11"].val = m_Signals.arrInput[1];
	inTable["A26"].val = m_Signals.arrInput[2];
	inTable["A44"].val = m_Signals.arrInput[3];
	inTable["AS1"].val = m_Signals.arrInput[4];
	inTable["AR63"].val = m_Signals.arrInput[5];
	inTable["A49"].val = m_Signals.arrInput[6];
	inTable["A21"].val = m_Signals.arrInput[7];

	inTable["A10"].val = m_Signals.arrInput[16];
	inTable["A27"].val = m_Signals.arrInput[17];
	inTable["A54"].val = m_Signals.arrInput[18];
	inTable["A53"].val = m_Signals.arrInput[19];
	inTable["A76"].val = m_Signals.arrInput[20];
	inTable["A84"].val = m_Signals.arrInput[21];
	inTable["AV1"].val = m_Signals.arrInput[22];
	inTable["A48"].val = m_Signals.arrInput[23];

	inTable["A46"].val = m_Signals.arrInput[32];
	inTable["A29"].val = m_Signals.arrInput[33];
	//inTable["A79"].val = m_Signals.arrInput[34]; // Not implemented
	inTable["A47"].val = m_Signals.arrInput[35];
	inTable["A74"].val = m_Signals.arrInput[36];
	inTable["A42"].val = m_Signals.arrInput[37];
	inTable["A71"].val = m_Signals.arrInput[38];
	inTable["A73"].val = m_Signals.arrInput[39];

	inTable["A45"].val = m_Signals.arrInput[48];
	inTable["A41"].val = m_Signals.arrInput[49];
	//inTable["A77"].val = m_Signals.arrInput[50]; // Not implemented
	inTable["A75"].val = m_Signals.arrInput[51];
	inTable["A43"].val = m_Signals.arrInput[52];
	//inTable["A78"].val = m_Signals.arrInput[53]; // Not implemented
	inTable["A32"].val = m_Signals.arrInput[54];
	inTable["A31"].val = m_Signals.arrInput[55];

	inTable["A1"].val = m_Signals.arrInput[64];
	inTable["A13"].val = m_Signals.arrInput[65];
	inTable["A25"].val = m_Signals.arrInput[66];
	inTable["A20"].val = m_Signals.arrInput[67];
	inTable["A56"].val = m_Signals.arrInput[68];
	inTable["A30"].val = m_Signals.arrInput[69];
	inTable["A65"].val = m_Signals.arrInput[71];

	inTable["A3"].val = m_Signals.arrInput[80];
	inTable["A2"].val = m_Signals.arrInput[81];
	inTable["A5"].val = m_Signals.arrInput[82];
	inTable["A4"].val = m_Signals.arrInput[83];
	inTable["A70"].val = m_Signals.arrInput[84];
	inTable["A6"].val = m_Signals.arrInput[85];
	inTable["A39"].val = m_Signals.arrInput[86];
	inTable["A14"].val = m_Signals.arrInput[87];

	inTable["A38"].val = m_Signals.arrInput[96];
	inTable["A28"].val = m_Signals.arrInput[97];
	inTable["A8"].val = m_Signals.arrInput[98];
	inTable["A22"].val = m_Signals.arrInput[99];
	inTable["A16"].val = m_Signals.arrInput[100];
	inTable["A12"].val = m_Signals.arrInput[101];
	inTable["A51"].val = m_Signals.arrInput[102];
	inTable["A37"].val = m_Signals.arrInput[103];

	inTable["A19"].val = m_Signals.arrInput[112];
	inTable["A24"].val = m_Signals.arrInput[113];
	inTable["A18"].val = m_Signals.arrInput[114];
	inTable["A66"].val = m_Signals.arrInput[115];
	inTable["A80"].val = m_Signals.arrInput[116];
	inTable["A40"].val = m_Signals.arrInput[117];
	inTable["A52"].val = m_Signals.arrInput[118];
	inTable["A50"].val = m_Signals.arrInput[119];

	inTable["AV3"].val = m_Signals.arrInput[128];
	inTable["AIS"].val = m_Signals.arrInput[129];
	//inTable["A83"].val = m_Signals.arrInput[130]; // Not implemented
	inTable["AV6"].val = m_Signals.arrInput[131];
	inTable["A15"].val = m_Signals.arrInput[132];
	//inTable["A82"].val = m_Signals.arrInput[133]; // Not implemented
	inTable["A81"].val = m_Signals.arrInput[134];
	inTable["A57"].val = m_Signals.arrInput[135];

	inTable["A9"].val = m_Signals.arrInput[144];
	inTable["A7"].val = m_Signals.arrInput[145];
	inTable["A72"].val = m_Signals.arrInput[146];
	inTable["A68"].val = m_Signals.arrInput[147];

	inTable["RC1"].val = m_Signals.arrInput[160];
	inTable["VB"].val = m_Signals.arrInput[161];
	inTable["UOS"].val = m_Signals.arrInput[165] == 0;
	//inTable["UAVA"].val = m_Signals.arrInput[167];

	inTable["VMK"].val = m_Signals.arrInput[168];
	inTable["BPSNon"].val = m_Signals.arrInput[169];
	//inTable["L_Emer"].val = m_Signals.arrInput[170]; // Not implemented
	inTable["RezMK"].val = m_Signals.arrInput[174];
	inTable["ARS13"].val = m_Signals.arrInput[175];

	inTable["R_UNch"].val = m_Signals.arrInput[176];
	inTable["R_ZS"].val = m_Signals.arrInput[177];
	inTable["R_G"].val = m_Signals.arrInput[178];
	inTable["R_Radio"].val = m_Signals.arrInput[179];
	inTable["R_Program1"].val = m_Signals.arrInput[180];
	inTable["R_Program2"].val = m_Signals.arrInput[181];
	inTable["VUD1"].val = m_Signals.arrInput[182];
	inTable["R_VPR"].val = m_Signals.arrInput[183];

	inTable["KRZD"].val = m_Signals.arrInput[184];
	inTable["VozvratRP"].val = m_Signals.arrInput[185];
	inTable["KDL"].val = m_Signals.arrInput[186];
	inTable["KDLR"].val = m_Signals.arrInput[187];
	inTable["DoorSelect"].val = m_Signals.arrInput[188];

	inTable["KVT"].val = m_Signals.arrInput[192];
	inTable["KVTR"].val = m_Signals.arrInput[193];
	inTable["VZ1"].val = m_Signals.arrInput[194];
	inTable["V13"].val = m_Signals.arrInput[195];
	inTable["OtklAVU"].val = m_Signals.arrInput[196];
	inTable["OtklBV"].val = m_Signals.arrInput[197];
	inTable["V11"].val = m_Signals.arrInput[198];
	inTable["V12"].val = m_Signals.arrInput[199];

	inTable["ARS"].val = m_Signals.arrInput[200];
	inTable["ALS"].val = m_Signals.arrInput[201];
	inTable["ARSR"].val = m_Signals.arrInput[202];
	inTable["ConverterProtection"].val = m_Signals.arrInput[203];
	inTable["KSN"].val = m_Signals.arrInput[204];
	inTable["Ring"].val = m_Signals.arrInput[205];
	inTable["OVT"].val = m_Signals.arrInput[206];

	inTable["ALSFreq"].val = m_Signals.arrInput[208];
	inTable["L_1"].val = m_Signals.arrInput[209];
	inTable["L_2"].val = m_Signals.arrInput[210];
	inTable["L_3"].val = m_Signals.arrInput[211];
	inTable["VP"].val = m_Signals.arrInput[212];
	inTable["KRUPosition"].val = m_Signals.arrInput[214] ? 2 : m_Signals.arrInput[213] ? 1 : 0;

	inTable["KRP"].val = m_Signals.arrInput[216];
	inTable["KAH"].val = m_Signals.arrInput[217];
	inTable["KDP"].val = m_Signals.arrInput[218];
	inTable["L_4"].val = m_Signals.arrInput[219];
	inTable["VUS"].val = m_Signals.arrInput[220];
	inTable["VAD"].val = m_Signals.arrInput[221];
	inTable["VAH"].val = m_Signals.arrInput[222];
	inTable["HornState"].val = m_Signals.arrInput[223];

	inTable["R_ASNPDown"].val = m_Signals.arrInput[233];
	inTable["R_ASNPMenu"].val = m_Signals.arrInput[234];
	inTable["R_ASNPUp"].val = m_Signals.arrInput[235];

	inTable["ReverserPosition"].val = m_Signals.arrInput[242] ? 0 : (m_Signals.arrInput[243] ? 2 : 1); // 0 - Назад, 2 - Вперед
	inTable["PB"].val = m_Signals.arrInput[244];

	inTable["IGLA1"].val = m_Signals.arrInput[248];
	//inTable["IGLA2"].val = m_Signals.arrInput[]; // No button
	//inTable["IGLA3"].val = m_Signals.arrInput[]; // No button
	inTable["IGLA4"].val = m_Signals.arrInput[249];

	inTable["DriverValveDisconnect"].val = m_Signals.arrInput[291] == 0;
	inTable["ParkingBrake"].val = m_Signals.arrInput[292] == 0;
	inTable["EPK"].val = m_Signals.arrInput[293] == 0;

	inTable["VUD2"].val = m_Signals.arrInput[302] == 0;
	inTable["R_Program1H"].val = m_Signals.arrInput[303];
	inTable["VDL"].val = m_Signals.arrInput[310] == 0;
	inTable["R_Program2H"].val = m_Signals.arrInput[311];

	inTable["EmergencyBrakeValve"].val = !ADCStopcrane(m_Signals.arrADC[0]);
	inTable["CranePosition"].val =  ADCKM013(m_Signals.arrADC[1]) * 1000;
	inTable["ControllerPosition"].val = ADCController(m_Signals.arrADC[2]);

	LeaveCriticalSection(&m_CriticalSection);
}

void UARTFrontView717::DataExchangeOutputs()
{
	EnterCriticalSection(&m_CriticalSection);

	// Индикация
	auto& outTable = m_NW2VarTableOutput.VarTable;
	m_Signals.arrOutput[171] = outTable["VD"].val;
	m_Signals.arrOutput[172] = outTable["KT"].val;
	m_Signals.arrOutput[173] = outTable["ST"].val;

	m_Signals.arrOutput[252] = outTable["IGLA:Error"].val;
	m_Signals.arrOutput[254] = outTable["IGLA:Fire"].val;

	// Конфигурация с 7SegDec работает неправильно (?)
	int speed = outTable["LUDS"].val ? int(m_NW2VarTableOutput.GetPackedRatio("Speed") * 100.0f) : -1;
	byte speedVal = CUnivCon::ConvertIntTo7DecSegByte(speed);
	m_Signals.arrOutput[256] = (speedVal & 0x01) > 0;
	m_Signals.arrOutput[257] = (speedVal & 0x02) > 0;
	m_Signals.arrOutput[258] = (speedVal & 0x04) > 0;
	m_Signals.arrOutput[259] = (speedVal & 0x08) > 0;
	m_Signals.arrOutput[260] = (speedVal & 0x10) > 0;
	m_Signals.arrOutput[261] = (speedVal & 0x20) > 0;
	m_Signals.arrOutput[262] = (speedVal & 0x40) > 0;
	m_Signals.arrOutput[263] = (speedVal & 0x80) > 0;

	m_Signals.arrOutput[264] = outTable["AR04"].val;
	m_Signals.arrOutput[265] = outTable["AR0"].val;
	m_Signals.arrOutput[266] = outTable["AR40"].val;
	m_Signals.arrOutput[267] = outTable["AR60"].val;
	m_Signals.arrOutput[268] = outTable["AR70"].val;
	m_Signals.arrOutput[269] = outTable["AR80"].val;
	m_Signals.arrOutput[270] = outTable["SD"].val;
	
	m_Signals.arrOutput[272] = outTable["HRK"].val;
	m_Signals.arrOutput[273] = outTable["RP"].val;
	m_Signals.arrOutput[274] = outTable["SN"].val;
	m_Signals.arrOutput[275] = outTable["GLIB"].val;
	m_Signals.arrOutput[276] = outTable["KVC"].val;
	m_Signals.arrOutput[277] = outTable["LN"].val;
	m_Signals.arrOutput[278] = outTable["RS"].val;
	m_Signals.arrOutput[279] = outTable["KVD"].val;

	m_Signals.arrOutput[280] = outTable["GreenRP"].val;
	m_Signals.arrOutput[281] = outTable["DoorsLeftL"].val;
	m_Signals.arrOutput[282] = outTable["DoorsLeftL"].val;
	m_Signals.arrOutput[283] = outTable["L1"].val;
	m_Signals.arrOutput[284] = outTable["LSP"].val;
	m_Signals.arrOutput[285] = outTable["AVU"].val;
	m_Signals.arrOutput[286] = outTable["LKVP"].val;
	m_Signals.arrOutput[287] = outTable["RZP"].val;

	m_Signals.arrOutput[288] = outTable["PN"].val;
	m_Signals.arrOutput[289] = outTable["DoorsRightL"].val;
	m_Signals.arrOutput[290] = outTable["BrW"].val;

	// Стрелочная индикация
	m_Signals.arrArrow[0] = StepBattVoltmeter(m_NW2VarTableOutput.GetPackedRatio("BatteryVoltage"));
	m_Signals.arrArrow[1] = StepNM(m_NW2VarTableOutput.GetPackedRatio("TLPressure"));
	m_Signals.arrArrow[2] = StepTM(m_NW2VarTableOutput.GetPackedRatio("BLPressure"));
	m_Signals.arrArrow[3] = StepTC(m_NW2VarTableOutput.GetPackedRatio("BCPressure"));
	m_Signals.arrArrow[4] = StepKiloVoltmeter(m_NW2VarTableOutput.GetPackedRatio("EnginesVoltage"));
	m_Signals.arrArrow[5] = StepAmmeter(m_NW2VarTableOutput.GetPackedRatio("EnginesCurrent"));

	// Дисплеи
	// АСНП
	m_Signals.arrTextDisplay[0].on = m_Signals.arrTextDisplay[0].ledOn = (outTable["ASNP:State"].val != 0);
	convertUtf8ToCp1251(m_ASNPText.c_str(), m_Signals.arrTextDisplay[0].text, 48);

	// АСОТП
	int asotpState = outTable["IGLA:State"].val;

	static bool asotpDisplayOn = true;
	if (asotpState == 2)
		asotpDisplayOn = (outTable["IGLA:Standby"].val == 0) || outTable["IGLA:ShowTime"].val;
	else 
		asotpDisplayOn = (asotpState != -2);

	m_Signals.arrTextDisplay[1].on = m_Signals.arrTextDisplay[1].ledOn = asotpDisplayOn;
	convertUtf8ToCp1251(m_ASOTPText.c_str(), m_Signals.arrTextDisplay[1].text, 40);

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

int UARTFrontView717::ADCController(int adc)
{
	int retVal = 429;

	int pos_1 = m_ControllerCalib.m_PosT2;
	int pos_2 = m_ControllerCalib.m_PosT1a;
	int pos_3 = m_ControllerCalib.m_PosT1;
	int pos_4 = m_ControllerCalib.m_Pos0;
	int pos_5 = m_ControllerCalib.m_PosX1;
	int pos_6 = m_ControllerCalib.m_PosX2;
	int pos_7 = m_ControllerCalib.m_PosX3;

	if (adc <= (pos_1 + pos_2) / 2)
		retVal = 0; // T2
	else if (adc > (pos_1 + pos_2) / 2 && adc <= (pos_2 + pos_3) / 2)
		retVal = 143; // T1a
	else if (adc > (pos_2 + pos_3) / 2 && adc <= (pos_3 + pos_4) / 2)
		retVal = 286; // T1
	else if (adc > (pos_3 + pos_4) / 2 && adc <= (pos_4 + pos_5) / 2)
		retVal = 429; // 0
	else if (adc > (pos_4 + pos_5) / 2 && adc <= (pos_5 + pos_6) / 2)
		retVal = 571; // X1
	else if (adc > (pos_5 + pos_6) / 2 && adc <= (pos_6 + pos_7) / 2)
		retVal = 714; // X2
	else if (adc > (pos_6 + pos_7) / 2)
		retVal = 857; // X3

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

void UARTFrontView717::ReadControllerCalibrations()
{
	PRINT_FUNCSIG;

	m_ControllerCalib.m_PosX3 = GetPrivateProfileInt("Controller", "PosX3", 6, CALIBRATIONS_FILE);
	m_ControllerCalib.m_PosX2 = GetPrivateProfileInt("Controller", "PosX2", 5, CALIBRATIONS_FILE);
	m_ControllerCalib.m_PosX1 = GetPrivateProfileInt("Controller", "PosX1", 4, CALIBRATIONS_FILE);
	m_ControllerCalib.m_Pos0 = GetPrivateProfileInt("Controller", "Pos0", 3, CALIBRATIONS_FILE);
	m_ControllerCalib.m_PosT1 = GetPrivateProfileInt("Controller", "PosT1", 2, CALIBRATIONS_FILE);
	m_ControllerCalib.m_PosT1a = GetPrivateProfileInt("Controller", "PosT1a", 1, CALIBRATIONS_FILE);
	m_ControllerCalib.m_PosT2 = GetPrivateProfileInt("Controller", "PosT2", 0, CALIBRATIONS_FILE);
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