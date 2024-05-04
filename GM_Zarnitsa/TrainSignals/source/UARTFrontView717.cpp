#include "UARTFrontView717.h"

UARTFrontView717::UARTFrontView717()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif

	InitializeCriticalSection(&m_CriticalSection);
}

UARTFrontView717::~UARTFrontView717()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif

	DeleteCriticalSection(&m_CriticalSection);
}

int UARTFrontView717::start(int port)
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	int openResult = openCOMPort(port);
	if (openResult)
		return openResult;
	m_PortNumber = port;

	setupArrays();
	loadCalibartions();

	m_DeviceThread = std::thread(&UARTFrontView717::deviceThreadFunc, this);
	m_DeviceThread.detach();

	return 0;
}

void UARTFrontView717::stop(bool force)
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	if (!m_Connected)
		return;

	m_ThreadStop = true;
	m_ThreadForceStop = force;
}

void UARTFrontView717::loadCalibartions(bool printCalib)
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	readStopcraneCalibrations();
	readKM013Calibrations();

	readTCCalibrations();
	readNMCalibrations();
	readTMCalibrations();
	readKiloVoltmeterCalibrations();
	readAmmeterCalibrations();
	readBattVoltmerCalibrations();

	if (printCalib)
	{
		PRINT_MSG("Stopcrane:\n");
		PRINT_MSG("    Off = %d\n", m_StopcraneCalib.m_Off);
		PRINT_MSG("    On  = %d\n", m_StopcraneCalib.m_On);

		PRINT_MSG("KM013:\n");
		PRINT_MSG("    Pos1 = %d\n", m_KM013Calib.m_Pos1);
		PRINT_MSG("    Pos2 = %d\n", m_KM013Calib.m_Pos2);
		PRINT_MSG("    Pos3 = %d\n", m_KM013Calib.m_Pos3);
		PRINT_MSG("    Pos4 = %d\n", m_KM013Calib.m_Pos4);
		PRINT_MSG("    Pos5 = %d\n", m_KM013Calib.m_Pos5);
		PRINT_MSG("    Pos6 = %d\n", m_KM013Calib.m_Pos6);
		PRINT_MSG("    Pos7 = %d\n", m_KM013Calib.m_Pos7);

		PRINT_MSG("TC:\n");
		PRINT_MSG("    Min = %d\n", m_TCCalib.m_Min);
		PRINT_MSG("    Max = %d\n", m_TCCalib.m_Max);

		PRINT_MSG("NM:\n");
		PRINT_MSG("    Min = %d\n", m_NMCalib.m_Min);
		PRINT_MSG("    Max = %d\n", m_NMCalib.m_Max);

		PRINT_MSG("TM:\n");
		PRINT_MSG("    Min = %d\n", m_TMCalib.m_Min);
		PRINT_MSG("    Max = %d\n", m_TMCalib.m_Max);

		PRINT_MSG("KiloVoltmeter:\n");
		PRINT_MSG("    Min = %d\n", m_KiloVoltmeterCalib.m_Min);
		PRINT_MSG("    Max = %d\n", m_KiloVoltmeterCalib.m_Max);

		PRINT_MSG("Ammeter:\n");
		PRINT_MSG("    Min = %d\n", m_AmmeterCalib.m_Min);
		PRINT_MSG("    Max = %d\n", m_AmmeterCalib.m_Max);

		PRINT_MSG("BattVoltmeter:\n");
		PRINT_MSG("    Min = %d\n", m_BattVoltmeterCalib.m_Min);
		PRINT_MSG("    Max = %d\n", m_BattVoltmeterCalib.m_Max);
	}
}

bool UARTFrontView717::isConnected()
{
	return m_Connected;
}

int UARTFrontView717::getPortNumber()
{
	return m_PortNumber;
}

CRITICAL_SECTION* UARTFrontView717::getCriticalSection()
{
	return &m_CriticalSection;
}

int UARTFrontView717::openCOMPort(int port)
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	if (port < 1 || port > 254)
	{
		PRINT_MSG_ERROR("Port number %d not in range 1-254!", port);
		return 0x255;
	}

	char portPath[7];
	wsprintf(portPath, "COM%d", port);
	m_hPort = CreateFile(portPath, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

	if (m_hPort == INVALID_HANDLE_VALUE)
	{
		PRINT_MSG_ERROR("Fail to open COM%d\n", port);
		return -1;
	}

	DCB dcb;
	BOOL success;

	success = GetCommState(m_hPort, &dcb);
	if (!success)
	{
		PRINT_MSG_ERROR("GetCommState failed.\n");
		destroyHandle();
		return 0x01;
	}

	dcb.BaudRate = 115200;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	success = SetCommState(m_hPort, &dcb);
	if (!success)
	{
		PRINT_MSG_ERROR("SetCommState failed.\n");
		destroyHandle();
		return 0x02;
	}

	success = SetCommMask(m_hPort, EV_TXEMPTY);
	if (!success)
	{
		PRINT_MSG_ERROR("SetCommMask failed.\n");
		destroyHandle();
		return 0x03;
	}

	COMMTIMEOUTS timeouts;
	success = GetCommTimeouts(m_hPort, &timeouts);
	if (!success)
	{
		PRINT_MSG_ERROR("GetCommTimeouts failed.\n");
		destroyHandle();
		return 0x04;
	}

	timeouts.ReadIntervalTimeout = 5;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.ReadTotalTimeoutConstant = 5;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 5;
	success = SetCommTimeouts(m_hPort, &timeouts);
	if (!success)
	{
		PRINT_MSG_ERROR("SetCommTimeouts failed.\n");
		destroyHandle();
		return 0x05;
	}

	success = SetupComm(m_hPort, 1000, 1000);
	if (!success)
	{
		PRINT_MSG_ERROR("SetupComm failed.\n");
		destroyHandle();
		return 0x06;
	}

	static byte cmdDetect[2]{0x00,0x80};
	byte answerBuf{};

	PurgeComm(m_hPort, PURGE_RXCLEAR | PURGE_TXCLEAR);
	Sleep(10);

	PRINT_MSG("Send mirror command...\n");
	WriteFile(m_hPort, &cmdDetect, 2, nullptr, nullptr);
	ReadFile(m_hPort, &answerBuf, 1, nullptr, nullptr);
	PRINT_MSG_DBG("Answer = %02X\n", answerBuf);
	if (answerBuf != 0x66)
	{
		PRINT_MSG_ERROR("Wrong answer.\n");
		destroyHandle();
		return 0x10;
	}

	return 0;
}

void UARTFrontView717::setupArrays()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	// Количество контроллеров
	int nControllers = 14;

	// Количество стрелочных приборов на контроллер
	m_Config.arrArrows.reset(new int[nControllers]());
	m_Config.arrArrows[2] = 3;
	m_Config.arrArrows[3] = 3;

	// Массив конфигурации 7-ми сегментного индикатора скорости для метро (через дешифратор К514ИД2)
	m_Config.arr7SegDec.reset(new SevenDecSignals[nControllers]());
	m_Config.arr7SegDec[8].port[0] = 1;

	// Массив конфигурации текстовых дисплеев. Число, отличное от нуля, обозначает кол-во символов в одной строке дисплея.
	m_Config.arrTextDisplaySize.reset(new int[nControllers]());
	m_Config.arrTextDisplaySize[12] = 20;
	m_Config.arrTextDisplaySize[13] = 24;

	// Главный конфигурационный массив
	int nPins = 24 * nControllers;
	m_Config.arrPins.reset(new int[nPins]());

	// 1.1
	m_Config.arrPins[0] = NotUsed; // 
	m_Config.arrPins[1] = NotUsed; // 
	m_Config.arrPins[2] = NotUsed; // 
	m_Config.arrPins[3] = NotUsed; // 
	m_Config.arrPins[4] = NotUsed; // 
	m_Config.arrPins[5] = NotUsed; // 
	m_Config.arrPins[6] = NotUsed; // 
	m_Config.arrPins[7] = InputADC; // Стопкран

	// 1.2
	m_Config.arrPins[8] = Input; // УНЧ     
	m_Config.arrPins[9] = Input; // Контроль ЭС
	m_Config.arrPins[10] = Input; // Контр громког
	m_Config.arrPins[11] = Input; // Радио
	m_Config.arrPins[12] = Input; // Программа 2
	m_Config.arrPins[13] = Input; // Программа 1
	m_Config.arrPins[14] = Input; // Закрытие дверей
	m_Config.arrPins[15] = Input; // ВПР

	// 1.3
	m_Config.arrPins[16] = Input; // Вкл. мотор компрессора
	m_Config.arrPins[17] = Input; // БПСН
	m_Config.arrPins[18] = Input; // Аварийное освещение
	m_Config.arrPins[19] = Input; // Компрессор резервный
	m_Config.arrPins[20] = Input; // АРС 13V
	m_Config.arrPins[21] = Input; // АСНП: Вверх
	m_Config.arrPins[22] = Input; // АСНП: Вниз
	m_Config.arrPins[23] = Input; // АСНП: Меню

	// 2.1
    m_Config.arrPins[24] = InputADC; // КМ-013
	m_Config.arrPins[25] = Input; // КВ-70: 10-8
	m_Config.arrPins[26] = Input; // КВ-70: Реверс вкл
	m_Config.arrPins[27] = Input; // КВ-70: Выбег
	m_Config.arrPins[28] = Input; // КВ-70: Реверс назад
	m_Config.arrPins[29] = Input; // КВ-70: У2-2
	m_Config.arrPins[30] = Input; // КВ-70: У2-20Б
	m_Config.arrPins[31] = Input; // КВ-70: У2-3

	// 2.2
	m_Config.arrPins[32] = Input; // АРС
	m_Config.arrPins[33] = Input; // АЛС
	m_Config.arrPins[34] = Input; // АРС-Р
	m_Config.arrPins[35] = Input; // Дешифратор
	m_Config.arrPins[36] = Input; // Осв. салона
	m_Config.arrPins[37] = Input; // Осв. кабины
	m_Config.arrPins[38] = Input; // Осв. пульта
	m_Config.arrPins[39] = Input; // Вспомогательный поезд

	// 2.3
	m_Config.arrPins[40] = Input; // Рез. закрытие дверей
	m_Config.arrPins[41] = Input; // Вкл. БВ
	m_Config.arrPins[42] = Input; // Двери левые (правая)
	m_Config.arrPins[43] = Input; // Двери левые (левая)
	m_Config.arrPins[44] = Input; // Выбор дверей (левые-0, правые-1)
	m_Config.arrPins[45] = Input; // Бдительность (левая)
	m_Config.arrPins[46] = Input; // Бдительность (правая)
	m_Config.arrPins[47] = Input; // АВУ

	// 3.1
	m_Config.arrPins[48] = Input; // Двери торцевые (1 гр.)
	m_Config.arrPins[49] = Input; // Вентиляция кабины
	m_Config.arrPins[50] = Input; // Вкл. авар. сигн. (2 гр.)
	m_Config.arrPins[51] = Input; // Защита преобразователя
	m_Config.arrPins[52] = Input; // Сигнализация
	m_Config.arrPins[53] = Input; // Звонок
	m_Config.arrPins[54] = Input; // Откл БВ
	m_Config.arrPins[55] = Input; // Вентиль №1

	// 3.2
	m_Config.arrPins[56] = Input; // Пуск резервный
	m_Config.arrPins[57] = Input; // Аварийный ход
	m_Config.arrPins[58] = Input; // Фары
	m_Config.arrPins[59] = Input; // ВУС
	m_Config.arrPins[60] = Input; // Авариные двери (тумблер)
	m_Config.arrPins[61] = Input; // Аварийный ход (тумблер)
	m_Config.arrPins[62] = Input; // ВКСТ
	m_Config.arrPins[63] = Input; // Двери прав

	// 3.3
	m_Config.arrPins[64] = Input; // КРУ1
	m_Config.arrPins[65] = Input; // КРУ2
	m_Config.arrPins[66] = NotUsed; // 
	m_Config.arrPins[67] = Input; // АСОТП: Кнопка 1
	m_Config.arrPins[68] = Input; // АСОТП: Кнопка 2
	m_Config.arrPins[69] = Input; // АСОТП: Кнопка 3
	m_Config.arrPins[70] = Input; // АСОТП: Кнопка 4
	m_Config.arrPins[71] = NotUsed; // 

	// 4.1
	m_Config.arrPins[72] = Input; // Помощник: Закрытие дверей
	m_Config.arrPins[73] = Input; // Помощник: Левые двери
	m_Config.arrPins[74] = Input; // Помощник: Программа 1
	m_Config.arrPins[75] = Input; // Помощник: Программа 2
	m_Config.arrPins[76] = Input; // Педаль бдительности
	m_Config.arrPins[77] = Input; // Пневмосигнал
	m_Config.arrPins[78] = NotUsed; // 
	m_Config.arrPins[79] = NotUsed; // 

	// 4.2
	m_Config.arrPins[80] = NotUsed; // 
	m_Config.arrPins[81] = NotUsed; // 
	m_Config.arrPins[82] = NotUsed; // 
	m_Config.arrPins[83] = NotUsed; // 
	m_Config.arrPins[84] = Input; // Управление (предохранитель)
	m_Config.arrPins[85] = Input; // Батарея (предохранитель)
	m_Config.arrPins[86] = Input; // Без подписи (предохранитель)
	m_Config.arrPins[87] = Input; // Преобразователь (предохранитель)

	// 4.3
	m_Config.arrPins[88] = Input; // Освещение1 (предохранитель)
	m_Config.arrPins[89] = Input; // Освещение2 (предохранитель)
	m_Config.arrPins[90] = Input; // УАВА
	m_Config.arrPins[91] = NotUsed; // 
	m_Config.arrPins[92] = NotUsed; // 
	m_Config.arrPins[93] = NotUsed; // 
	m_Config.arrPins[94] = NotUsed; // 
	m_Config.arrPins[95] = NotUsed; // 

	// 5.1
	m_Config.arrPins[96] = Input; // РЦ1
	m_Config.arrPins[97] = Input; // Батареи
	m_Config.arrPins[98] = Input; // УОС
	m_Config.arrPins[99] = Input; // Втр прижат
	m_Config.arrPins[100] = Input; // Весь состав
	m_Config.arrPins[101] = Input; // 1 половина
	m_Config.arrPins[102] = Input; // 2 половина
	m_Config.arrPins[103] = NotUsed; // 

	// 5.2
	m_Config.arrPins[104] = Input; // Разобщ кран кран машиниста
	m_Config.arrPins[105] = Input; // Стояночный тормоз
	m_Config.arrPins[106] = Input; // ЭПВ-АРС
	m_Config.arrPins[107] = NotUsed; // 
	m_Config.arrPins[108] = NotUsed; // 
	m_Config.arrPins[109] = NotUsed; // 
	m_Config.arrPins[110] = NotUsed; // 
	m_Config.arrPins[111] = NotUsed; // 

	// 5.3
	m_Config.arrPins[112] = Input; // A54-in
	m_Config.arrPins[113] = Input; // ВУ-in
	m_Config.arrPins[114] = Input; // A10-in
	m_Config.arrPins[115] = Input; // A53-in
	m_Config.arrPins[116] = Input; // A49-in
	m_Config.arrPins[117] = Input; // A27-in
	m_Config.arrPins[118] = Input; // AC1-in
	m_Config.arrPins[119] = Input; // A21-in

	// 6.1
	m_Config.arrPins[120] = Input; // A26-in
	m_Config.arrPins[121] = Input; // AP63-in
	m_Config.arrPins[122] = Input; // A17-in
	m_Config.arrPins[123] = Input; // A44-in
	m_Config.arrPins[124] = Input; // A45-in
	m_Config.arrPins[125] = Input; // A11-in
	m_Config.arrPins[126] = Input; // A71-in
	m_Config.arrPins[127] = Input; // A41-in

	// 6.2
	m_Config.arrPins[128] = Input; // A74-in
	m_Config.arrPins[129] = Input; // A73-in
	m_Config.arrPins[130] = Input; // A79-in
	m_Config.arrPins[131] = Input; // A42-in
	m_Config.arrPins[132] = Input; // A46-in
	m_Config.arrPins[133] = Input; // A47-in
	m_Config.arrPins[134] = Input; // AB1-in
	m_Config.arrPins[135] = Input; // A29-in

	// 6.3
	m_Config.arrPins[136] = Input; // A76-in
	m_Config.arrPins[137] = Input; // A48-in
	m_Config.arrPins[138] = Input; // A56-in
	m_Config.arrPins[139] = Input; // A65-in
	m_Config.arrPins[140] = Input; // A25-in
	m_Config.arrPins[141] = Input; // A30-in
	m_Config.arrPins[142] = Input; // A1-in
	m_Config.arrPins[143] = Input; // A20-in

	// 7.1
	m_Config.arrPins[144] = Input; // A32-in
	m_Config.arrPins[145] = Input; // A13-in
	m_Config.arrPins[146] = Input; // A43-in
	m_Config.arrPins[147] = Input; // A31-in
	m_Config.arrPins[148] = Input; // A77-in
	m_Config.arrPins[149] = Input; // A78-in
	m_Config.arrPins[150] = Input; // ВБД
	m_Config.arrPins[151] = Input; // A75-in
	
	// 7.2
	m_Config.arrPins[152] = Input; // A22-in
	m_Config.arrPins[153] = Input; // A8-in
	m_Config.arrPins[154] = Input; // A28-in
	m_Config.arrPins[155] = Input; // A38-in
	m_Config.arrPins[156] = Input; // A14-in
	m_Config.arrPins[157] = Input; // A39-in
	m_Config.arrPins[158] = Input; // A6-in
	m_Config.arrPins[159] = Input; // A70-in
	
	// 7.3
	m_Config.arrPins[160] = Input; // A4-in
	m_Config.arrPins[161] = Input; // A5-in
	m_Config.arrPins[162] = Input; // A2-in
	m_Config.arrPins[163] = Input; // A3-in
	m_Config.arrPins[164] = Input; // A50-in
	m_Config.arrPins[165] = Input; // A52-in
	m_Config.arrPins[166] = Input; // A40-in
	m_Config.arrPins[167] = Input; // A80-in
	

	// 8.1
	m_Config.arrPins[168] = Input; // A66-in
	m_Config.arrPins[169] = Input; // A18-in
	m_Config.arrPins[170] = Input; // A24-in
	m_Config.arrPins[171] = Input; // A19-in
	m_Config.arrPins[172] = Input; // A37-in
	m_Config.arrPins[173] = Input; // A51-in
	m_Config.arrPins[174] = Input; // A12-in
	m_Config.arrPins[175] = Input; // A16-in
	
	// 8.2	
	m_Config.arrPins[176] = Input; // A68-in
	m_Config.arrPins[177] = Input; // A72-in
	m_Config.arrPins[178] = Input; // A7-in
	m_Config.arrPins[179] = Input; // A9-in
	m_Config.arrPins[180] = Input; // A57-in
	m_Config.arrPins[181] = Input; // A81-in
	m_Config.arrPins[182] = Input; // A82-in
	m_Config.arrPins[183] = Input; // A15-in
	
	// 8.3
	m_Config.arrPins[184] = Input; // AB6-in
	m_Config.arrPins[185] = Input; // A83-in
	m_Config.arrPins[186] = Input; // A33-in (АИС)
	m_Config.arrPins[187] = Input; // AB3-in
	m_Config.arrPins[188] = NotUsed; // TODO: УППС
	m_Config.arrPins[189] = NotUsed; //
	m_Config.arrPins[190] = NotUsed; // 
	m_Config.arrPins[191] = NotUsed; // 
	
	// 9.1
	m_Config.arrPins[192] = NotUsed; // Индикатор скорости
	m_Config.arrPins[193] = NotUsed; // Индикатор скорости
	m_Config.arrPins[194] = NotUsed; // Индикатор скорости
	m_Config.arrPins[195] = NotUsed; // Индикатор скорости
	m_Config.arrPins[196] = NotUsed; // Индикатор скорости
	m_Config.arrPins[197] = NotUsed; // Индикатор скорости
	m_Config.arrPins[198] = NotUsed; // Индикатор скорости
	m_Config.arrPins[199] = NotUsed; // Индикатор скорости
	
	// 9.2
	m_Config.arrPins[200] = Output; // ОЧ
	m_Config.arrPins[201] = Output; // 0
	m_Config.arrPins[202] = Output; // 40
	m_Config.arrPins[203] = Output; // 60
	m_Config.arrPins[204] = Output; // 70
	m_Config.arrPins[205] = Output; // 80
	m_Config.arrPins[206] = Output; // ЛСД
	m_Config.arrPins[207] = NotUsed; // 
	
	// 9.3
	m_Config.arrPins[208] = Output; // ЛХ"РК"
	m_Config.arrPins[209] = Output; // РП
	m_Config.arrPins[210] = Output; // ЛСН
	m_Config.arrPins[211] = Output; // ЛЭКК
	m_Config.arrPins[212] = Output; // ЛКВЦ
	m_Config.arrPins[213] = Output; // ЛН
	m_Config.arrPins[214] = Output; // РС
	m_Config.arrPins[215] = Output; // ЛКВД
	
	// 10.1
	m_Config.arrPins[216] = NotUsed; // 
	m_Config.arrPins[217] = NotUsed; // 
	m_Config.arrPins[218] = NotUsed; // 
	m_Config.arrPins[219] = Output; // ЛВД
	m_Config.arrPins[220] = Output; // ЛКТ
	m_Config.arrPins[221] = Output; // ЛСТ
	m_Config.arrPins[222] = NotUsed; // 
	m_Config.arrPins[223] = NotUsed; // 
	
	// 10.2
	m_Config.arrPins[224] = NotUsed; // 
	m_Config.arrPins[225] = NotUsed; // 
	m_Config.arrPins[226] = NotUsed; // 
	m_Config.arrPins[227] = Output; // Двери левые (левая) светодиод
	m_Config.arrPins[228] = Output; // Двери левые (правая) светодиод
	m_Config.arrPins[229] = Output; // РП светодиод
	m_Config.arrPins[230] = Output; // Защита преобраз. светодиод
	m_Config.arrPins[231] = Output; // Лампа контроля невключения вентиляции
	
	// 10.3
	m_Config.arrPins[232] = Output; // ЛСП
	m_Config.arrPins[233] = Output; // АВУ
	m_Config.arrPins[234] = Output; // ЛКВП
	m_Config.arrPins[235] = Output; // Пневмотормоз
	m_Config.arrPins[236] = Output; // ИСТ
	m_Config.arrPins[237] = Output; // Двери правые
	m_Config.arrPins[238] = NotUsed; // 
	m_Config.arrPins[239] = NotUsed; // 
	
	// 11.1
	m_Config.arrPins[240] = Output; // ВУ-o
	m_Config.arrPins[241] = Output; // A54-o
	m_Config.arrPins[242] = Output; // A53-o
	m_Config.arrPins[243] = Output; // A10-o
	m_Config.arrPins[244] = Output; // A27-o
	m_Config.arrPins[245] = Output; // A49-o
	m_Config.arrPins[246] = Output; // A21-o
	m_Config.arrPins[247] = Output; // AC-1-o
	
	// 11.2
	m_Config.arrPins[248] = Output; // AP63-o
	m_Config.arrPins[249] = Output; // A26-o
	m_Config.arrPins[250] = Output; // A44-o
	m_Config.arrPins[251] = Output; // A17-o
	m_Config.arrPins[252] = Output; // A11-o
	m_Config.arrPins[253] = Output; // A45-o
	m_Config.arrPins[254] = Output; // A41-o
	m_Config.arrPins[255] = Output; // A71-o
	
	// 11.3
	m_Config.arrPins[256] = Output; // A73-o
	m_Config.arrPins[257] = Output; // A74-o
	m_Config.arrPins[258] = Output; // A42-o
	m_Config.arrPins[259] = Output; // A79-o
	m_Config.arrPins[260] = Output; // A47-o
	m_Config.arrPins[261] = Output; // A46-o
	m_Config.arrPins[262] = Output; // A29-o
	m_Config.arrPins[263] = Output; // AB1-o
	
	// 12.1
	m_Config.arrPins[264] = Output; // A48-o
	m_Config.arrPins[265] = Output; // A76-o
	m_Config.arrPins[266] = Output; // A65-o
	m_Config.arrPins[267] = Output; // A56-o
	m_Config.arrPins[268] = Output; // A30-o
	m_Config.arrPins[269] = Output; // A25-o
	m_Config.arrPins[270] = Output; // A20-o
	m_Config.arrPins[271] = Output; // A1-o
	
	// 12.2
	m_Config.arrPins[272] = Output; // A13-o
	m_Config.arrPins[273] = Output; // A32-o
	m_Config.arrPins[274] = Output; // A31-o
	m_Config.arrPins[275] = Output; // A43-o
	m_Config.arrPins[276] = Output; // A78-o
	m_Config.arrPins[277] = Output; // A77-o
	m_Config.arrPins[278] = Output; // A75-o
	m_Config.arrPins[279] = NotUsed; // 
	
	// 12.3
	m_Config.arrPins[280] = NotUsed; // 
	m_Config.arrPins[281] = NotUsed; // 
	m_Config.arrPins[282] = Output; // АСОТП: LED 4
	m_Config.arrPins[283] = Output; // АСОТП: LED 3
	m_Config.arrPins[284] = Output; // АСОТП: LED 2
	m_Config.arrPins[285] = Output; // АСОТП: LED 1
	m_Config.arrPins[286] = Output; // АСОТП: НЕИСПР.
	m_Config.arrPins[287] = Output; // АСОТП: ПОЖАР !
	
	// 13.1
	m_Config.arrPins[288] = Output; // A8-o
	m_Config.arrPins[289] = Output; // A22-o
	m_Config.arrPins[290] = Output; // A38-o
	m_Config.arrPins[291] = Output; // A28-o
	m_Config.arrPins[292] = Output; // A39-o
	m_Config.arrPins[293] = Output; // A14-o
	m_Config.arrPins[294] = Output; // A70-o
	m_Config.arrPins[295] = Output; // A6-o
	
	// 13.2
	m_Config.arrPins[296] = Output; // A5-o
	m_Config.arrPins[297] = Output; // A4-o
	m_Config.arrPins[298] = Output; // A3-o
	m_Config.arrPins[299] = Output; // A2-o
	m_Config.arrPins[300] = Output; // A52-o
	m_Config.arrPins[301] = Output; // A50-o
	m_Config.arrPins[302] = Output; // A80-o
	m_Config.arrPins[303] = Output; // A40-o
	
	// 13.3
	m_Config.arrPins[304] = Output; // A18-o
	m_Config.arrPins[305] = Output; // A66-o
	m_Config.arrPins[306] = Output; // A19-o
	m_Config.arrPins[307] = Output; // A24-o
	m_Config.arrPins[308] = Output; // A51-o
	m_Config.arrPins[309] = Output; // A37-o
	m_Config.arrPins[310] = Output; // A16-o
	m_Config.arrPins[311] = Output; // A12-o
	
	// 14.1
	m_Config.arrPins[312] = Output; // A72-o
	m_Config.arrPins[313] = Output; // A68-o
	m_Config.arrPins[314] = Output; // A9-o
	m_Config.arrPins[315] = Output; // A7-o
	m_Config.arrPins[316] = Output; // A81-o
	m_Config.arrPins[317] = Output; // A57-o
	m_Config.arrPins[318] = Output; // A15-o
	m_Config.arrPins[319] = Output; // A82-o
	
	// 14.2
	m_Config.arrPins[320] = Output; // A83-o
	m_Config.arrPins[321] = Output; // AB6-o
	m_Config.arrPins[322] = Output; // AB3-o
	m_Config.arrPins[323] = Output; // A33-o
	m_Config.arrPins[324] = NotUsed; // 
	m_Config.arrPins[325] = NotUsed; // 
	m_Config.arrPins[326] = NotUsed; // 
	m_Config.arrPins[327] = NotUsed; // 
	
	// 14.3
	m_Config.arrPins[328] = Output; // Клапан 1
	m_Config.arrPins[329] = Output; // Клапан 2
	m_Config.arrPins[330] = NotUsed; // 
	m_Config.arrPins[331] = Output; // Клапан 3
	m_Config.arrPins[332] = NotUsed; // 
	m_Config.arrPins[333] = NotUsed; // 
	m_Config.arrPins[334] = NotUsed; // 
	m_Config.arrPins[335] = NotUsed; // 
	
	// Считаем размеры массивов
	// Количество ADC
	m_Config.arrADCPerController.reset(new int[nControllers]());
	int nADC = 0;
	for (int i = 0; i < nPins; i++)
	{
		int i_Controller = i / 24;
		if (m_Config.arrPins[i] == InputADC)
		{
			nADC++;
			m_Config.arrADCPerController[i_Controller]++;
		}
	}

	// Количество стрелочных приборов
	int nArrows = 0;
	for (int c = 0; c < nControllers; c++)
	{
		nArrows += m_Config.arrArrows[c];
	}

	// Размер массива сигналов 7SegDec
	int n7SegDec = 0;
	for (int c = 0; c < nControllers; c++)
	{
		for (int p = 0; p < 3; p++)
		{
			n7SegDec += m_Config.arr7SegDec[c].port[p];
		}
	}

	// Количество подключенных текстовых дисплеев
	int nTextDisplays = 0;
	for (int c = 0; c < nControllers; c++)
	{
		if (m_Config.arrTextDisplaySize[c] > 0)
			nTextDisplays++;
	}

	m_Config.nControllers = nControllers;
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
	m_Signals.arrTextDisplay.reset(new TextDisplaySignals[nTextDisplays]());


	// Задание типа Integer для переменных
	m_NW2VarTableInput.VarTable["ControllerPosition"].type = 3;
	m_NW2VarTableInput.VarTable["ReverserPosition"].type = 3;
	m_NW2VarTableInput.VarTable["KRUPosition"].type = 3;
	m_NW2VarTableInput.VarTable["CranePosition"].type = 3;

	PRINT_MSG_DBG("m_Config.nControllers = %d\n", nControllers);
	PRINT_MSG_DBG("m_Signals.arrInput.count = %d\n", nPins);
	PRINT_MSG_DBG("m_Signals.arrOutput.count = %d\n", nPins);
	PRINT_MSG_DBG("m_Signals.arrADC.count = %d\n", nADC);
	PRINT_MSG_DBG("m_Signals.arrArrow.count = %d\n", nArrows);
	PRINT_MSG_DBG("m_Signals.arr7SegDec.count = %d\n", n7SegDec);
	PRINT_MSG_DBG("m_Signals.arrTextDisplay.count = %d\n", nTextDisplays);
}

int UARTFrontView717::setupDevice()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	int nInputBytes = 3 * m_Config.nControllers;
	int nOutputBytes = 3 * m_Config.nControllers;
	int nUARTBytes = 0;

	int nConfigBytes = m_Config.nControllers * 7 + 2;
	std::unique_ptr<byte[]> arrConfigBytes = std::make_unique<byte[]>(nConfigBytes);

	arrConfigBytes[0] = (byte)(nConfigBytes - 2);
	arrConfigBytes[1] = 0x81;

	int iByte = 0;
	for (int iController = 0; iController < m_Config.nControllers; iController++)
	{
		for (int p = 0; p < 24; p++)
		{
			int iPin = iController * 24 + p;
			iByte = iPin / 4 + 2;

			arrConfigBytes[iByte + iController] |= (byte)((m_Config.arrPins[iPin] << 6) >> ((iPin % 4) * 2));

			if (m_Config.arrPins[iPin] == InputADC)
				nInputBytes+=2;
		}

		byte nUARTBytesPerController = 0;
		if (m_Config.arrArrows[iController] > 0)
			nUARTBytesPerController += (byte)(m_Config.arrArrows[iController] * 3 + 1);

		if (m_Config.arrTextDisplaySize[iController] > 0)
			nUARTBytesPerController += (byte)(m_Config.arrTextDisplaySize[iController] * 2 + 6);

		arrConfigBytes[iByte + iController + 1] = nUARTBytesPerController;
		nUARTBytes += nUARTBytesPerController;
	}

	m_Data.nInputBytes = nInputBytes;
	m_Data.nOutputBytes = nOutputBytes;
	m_Data.nUARTBytes = nUARTBytes;

	m_Data.arrInputBytes.reset(new byte[nInputBytes + 3]());
	m_Data.arrOutputBytes.reset(new byte[nOutputBytes + 2]());
	m_Data.arrUARTBytes.reset(new byte[nUARTBytes + 2]());

	PRINT_MSG_DBG("m_Data.arrInputBytes.count = %d\n", nInputBytes + 3);
	PRINT_MSG_DBG("m_Data.arrOutputBytes.count = %d\n", nOutputBytes + 2);
	PRINT_MSG_DBG("m_Data.arrUARTBytes.count = %d\n", nUARTBytes + 2);

	PRINT_MSG("Send configuration...\n");
	int ttl = 20;
	while (--ttl)
	{
		Sleep(100);

		// Отправляем конфигурацию на контроллеры
		WriteFile(m_hPort, arrConfigBytes.get(), nConfigBytes, nullptr, nullptr);

		COMSTAT comstat;
		DWORD errors;
		comstat.cbInQue = 0;
		ClearCommError(m_hPort, &errors, &comstat);

		DWORD bytesToRead = comstat.cbInQue;
		PRINT_MSG_DBG("ttl = %d, bytesToRead = %d\n", ttl,bytesToRead);
		if (bytesToRead == 2)
		{
			byte buffACK[2]{};
			ReadFile(m_hPort, &buffACK, bytesToRead, nullptr, nullptr);
			

			PRINT_MSG_DBG("buffACK = {0x%02X, 0x%02X};\n", buffACK[0], buffACK[1]);
			if ((buffACK[0] == 0x01) && (buffACK[1] == 0x8F))
			{
				PurgeComm(m_hPort, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(100);
				m_Connected = true;
				return 0;
			}
		}
	}
	PRINT_MSG_ERROR("Couldn't configure controllers, exit.\n");

	m_Signals.arrInput.release();
	m_Signals.arrOutput.release();
	m_Signals.arrADC.release();
	m_Signals.arrArrow.release();
	m_Signals.arr7SegDec.release();
	m_Signals.arrTextDisplay.release();

	m_Data.arrInputBytes.release();
	m_Data.arrOutputBytes.release();
	m_Data.arrUARTBytes.release();

	destroyHandle();
	return 0x20;
}

void UARTFrontView717::readSignalsDevice()
{
	static byte cmdRead[]{0x00, 0x85};
	WriteFile(m_hPort,&cmdRead, 2, nullptr, nullptr);

	auto inBytes = m_Data.arrInputBytes.get();
	ReadFile(m_hPort, inBytes, m_Data.nInputBytes + 3, nullptr, nullptr);
	PurgeComm(m_hPort, PURGE_RXCLEAR);

	if ((inBytes[0] != 0x0B) || (inBytes[1] != 0x0B) || (inBytes[2] != m_Data.nInputBytes))
		return;

	int i_Byte = 3;
	int i_ADCSignals = 0;

	for (int i_Controller = 0; i_Controller < m_Config.nControllers; i_Controller++)
	{
		if (m_Config.arrADCPerController[i_Controller] > 0)
		{
			for (int i_ADC = 0; i_ADC < m_Config.arrADCPerController[i_Controller]; i_ADC++)
			{
				m_Signals.arrADC[i_ADCSignals] = inBytes[i_Byte++] << 8;
				m_Signals.arrADC[i_ADCSignals++] |= inBytes[i_Byte++];
			}
		}
		
		for (int i_Pin = 0; i_Pin < 24; i_Pin++)
		{
			int i_InputSignal = i_Controller * 24 + i_Pin;
			m_Signals.arrInput[i_InputSignal] = ((inBytes[i_Byte] << (i_Pin % 8)) & 0x80) >> 7;
			if (i_Pin == 7 || i_Pin == 15 || i_Pin == 23)
				i_Byte++;
		}
	}
}

void UARTFrontView717::writeSignalsDevice()
{
	auto outBytes = m_Data.arrOutputBytes.get();
	outBytes[0] = (byte)m_Data.nOutputBytes;
	outBytes[1] = 0x83;

	for (int i_OutputSignals = 0; i_OutputSignals < m_Signals.nPins; i_OutputSignals++)
	{
		if ((i_OutputSignals % 8) == 0)
			outBytes[(i_OutputSignals / 8) + 2] = 0;
		outBytes[(i_OutputSignals / 8) + 2] |= (byte)(m_Signals.arrOutput[i_OutputSignals] << 7) >> (i_OutputSignals % 8);
	}

	int i_7SegDecSignals = 0;
	for (int i_Controller = 0; i_Controller < m_Config.nControllers; i_Controller++)
	{
		for (int i_Port = 0; i_Port < 3; i_Port++)
		{
			if (m_Config.arr7SegDec[i_Controller].port[i_Port])
			{
				outBytes[i_Controller * 3 + i_Port + 2] = convertIntTo7DecSegByte(m_Signals.arr7SegDec[i_7SegDecSignals++]);
			}
		}
	}

	WriteFile(m_hPort, outBytes, m_Data.nOutputBytes + 2, nullptr, nullptr);
	Sleep(10);
}

void UARTFrontView717::writeUARTDevice()
{
	if (!m_Data.nUARTBytes)
		return;

	auto uartBytes = m_Data.arrUARTBytes.get();

	uartBytes[0] = (byte)m_Data.nUARTBytes;
	uartBytes[1] = 0x87;

	int i_Byte = 2;
	int i_Arrow = 0;
	int i_TextDisplay = 0;
	for (int i_Controller = 0; i_Controller < m_Config.nControllers; i_Controller++)
	{
		if (m_Config.arrArrows[i_Controller] > 0)
		{
			uartBytes[i_Byte++] = 0x41; // 'A' aka Arrows
			for (int i = 0; i < m_Config.arrArrows[i_Controller]; i++)
			{
				uartBytes[i_Byte++] = (byte)(i + 1);
				uartBytes[i_Byte++] = (byte)((m_Signals.arrArrow[i_Arrow] & 0xFF00) >> 8);
				uartBytes[i_Byte++] = (byte)(m_Signals.arrArrow[i_Arrow++] & 0xFF);
			}
		}

		if (m_Config.arrTextDisplaySize[i_Controller] > 0)
		{
			auto display = &m_Signals.arrTextDisplay[i_TextDisplay++];
			uartBytes[i_Byte++] = 0x54; // 'T' aka Text
			uartBytes[i_Byte++] = 0x42; // 'B' aka Begin
			uartBytes[i_Byte++] = (byte)((display->ledOn << 4) | display->on);

			int nTextByte = (m_Config.arrTextDisplaySize[i_Controller] * 2);
			uartBytes[i_Byte++] = (byte)nTextByte;
			for (int i_TextByte = 0; i_TextByte < nTextByte; i_TextByte++)
			{
				uartBytes[i_Byte++] = display->text[i_TextByte];
			}

			uartBytes[i_Byte++] = 0x54; // 'T' aka Text
			uartBytes[i_Byte++] = 0x45; // 'E' aka End
		}
	}

	WriteFile(m_hPort, uartBytes, m_Data.nUARTBytes + 2, nullptr, nullptr);
	Sleep(10);
}

void UARTFrontView717::writeShutdownDevice()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	PRINT_MSG("Send shutdown data...\n");
	// Отправка UART
	if (m_Data.nUARTBytes > 0)
	{
		m_Data.arrUARTBytes[0] = (byte)m_Data.nUARTBytes;
		m_Data.arrUARTBytes[1] = 0x87;

		int iUARTByte = 2;
		for (int c = 0; c < m_Config.nControllers; c++)
		{
			if (m_Config.arrArrows[c] > 0)
			{
				m_Data.arrUARTBytes[iUARTByte++] = 0x41; // 'A' aka Arrows
				for (int i = 0; i < m_Config.arrArrows[c]; i++)
				{
					m_Data.arrUARTBytes[iUARTByte++] = (byte)(i + 1);
					m_Data.arrUARTBytes[iUARTByte++] = 0x00;
					m_Data.arrUARTBytes[iUARTByte++] = 0x00;
				}
			}

			if (m_Config.arrTextDisplaySize[c] > 0)
			{
				m_Data.arrUARTBytes[iUARTByte++] = 0x54; // 'T' aka Text
				m_Data.arrUARTBytes[iUARTByte++] = 0x42; // 'B' aka Begin

				m_Data.arrUARTBytes[iUARTByte++] = 0x00; // Экран выключен
				m_Data.arrUARTBytes[iUARTByte++] = (byte)(m_Config.arrTextDisplaySize[c] * 2);

				for (int iTextByte = 0; iTextByte < (m_Config.arrTextDisplaySize[c] * 2); iTextByte++)
				{
					m_Data.arrUARTBytes[iUARTByte++] = 0x00;
				}

				m_Data.arrUARTBytes[iUARTByte++] = 0x54; // 'T' aka Text
				m_Data.arrUARTBytes[iUARTByte++] = 0x45; // 'E' aka End
			}
		}

		Sleep(500);
		for (int i = 0; i < 5; i++)
		{
			WriteFile(m_hPort, m_Data.arrUARTBytes.get(), (DWORD)(m_Data.nUARTBytes + 2), nullptr, nullptr);
			Sleep(100);
		}
	}

	// Отправка сигналов
	DWORD disableBytesSize = m_Config.nControllers * 7 + 2;
	std::unique_ptr<byte[]> disableBytes = std::make_unique<byte[]>(disableBytesSize);
	disableBytes[0] = (byte)(disableBytesSize - 2);
	disableBytes[1] = 0x81;

	Sleep(100);
	WriteFile(m_hPort, disableBytes.get(), disableBytesSize, nullptr, nullptr);
}

void UARTFrontView717::deviceThreadFunc()
{
	m_ThreadRunning = true;
	m_ThreadStop = false;
	m_ThreadForceStop = false;

	int setupResult = setupDevice();
	if (setupResult)
	{
		m_ThreadStop = true;
		m_ThreadForceStop = true;
	}
	else
	{
		PRINT_MSG("Successfully connected!\n");
	}

	while (!m_ThreadStop)
	{
		// Входные сигналы
		readSignalsDevice();
		dataExchangeInputs();

		// Выходные сигналы
		dataExchangeOutputs();
		Sleep(15);
		writeSignalsDevice(); // Sleep(10);
		writeUARTDevice();    // Sleep(10);
		Sleep(10);
	}

	if (!m_ThreadForceStop)
		writeShutdownDevice();

	destroyHandle();

	m_NW2VarTableInput.VarTable.clear();
	m_NW2VarTableOutput.VarTable.clear();

	m_ThreadRunning = false;
	PRINT_MSG("Thread working end.\n");
}

void UARTFrontView717::dataExchangeInputs()
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

	inTable["EmergencyBrakeValve"].val = !adcStopcrane(m_Signals.arrADC[0]);
	inTable["CranePosition"].val = adcKM013(m_Signals.arrADC[1]) * 1000;

	LeaveCriticalSection(&m_CriticalSection);
}

void UARTFrontView717::dataExchangeOutputs()
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
	m_Signals.arrArrow[0] = stepTC(m_NW2VarTableOutput.GetPackedRatio("BCPressure"));
	m_Signals.arrArrow[1] = stepNM(m_NW2VarTableOutput.GetPackedRatio("TLPressure"));
	m_Signals.arrArrow[2] = stepTM(m_NW2VarTableOutput.GetPackedRatio("BLPressure"));
	m_Signals.arrArrow[3] = stepAmmeter(m_NW2VarTableOutput.GetPackedRatio("EnginesCurrent"));
	m_Signals.arrArrow[4] = stepKiloVoltmeter(m_NW2VarTableOutput.GetPackedRatio("EnginesVoltage"));
	m_Signals.arrArrow[5] = stepBattVoltmeter(m_NW2VarTableOutput.GetPackedRatio("BatteryVoltage"));

	// Скоростемер
	m_Signals.arr7SegDec[0] = int(m_NW2VarTableOutput.GetPackedRatio("Speed") * 100.0f);

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

void UARTFrontView717::destroyHandle()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	PRINT_MSG_DBG("Destroy handle\n");

	if (m_hPort != INVALID_HANDLE_VALUE)
		PurgeComm(m_hPort, PURGE_TXCLEAR | PURGE_RXCLEAR);

	CloseHandle(m_hPort);
	m_PortNumber = -1;
	m_Connected = false;
	m_hPort = INVALID_HANDLE_VALUE;
}

bool UARTFrontView717::adcStopcrane(int adc)
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

int UARTFrontView717::adcKM013(int adc)
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

int UARTFrontView717::stepTC(float value)
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

int UARTFrontView717::stepNM(float value)
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

int UARTFrontView717::stepTM(float value)
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

int UARTFrontView717::stepKiloVoltmeter(float value)
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

int UARTFrontView717::stepAmmeter(float value)
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

int UARTFrontView717::stepBattVoltmeter(float value)
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

void UARTFrontView717::readStopcraneCalibrations()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	m_StopcraneCalib.m_Off = GetPrivateProfileInt("Stopcrane", "Off", 0, CALIBRATIONS_FILE);
	m_StopcraneCalib.m_On = GetPrivateProfileInt("Stopcrane", "On", 1, CALIBRATIONS_FILE);
}

void UARTFrontView717::readKM013Calibrations()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	m_KM013Calib.m_Pos1 = GetPrivateProfileInt("KM013", "Pos1", 0, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos2 = GetPrivateProfileInt("KM013", "Pos2", 1, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos3 = GetPrivateProfileInt("KM013", "Pos3", 2, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos4 = GetPrivateProfileInt("KM013", "Pos4", 3, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos5 = GetPrivateProfileInt("KM013", "Pos5", 4, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos6 = GetPrivateProfileInt("KM013", "Pos6", 5, CALIBRATIONS_FILE);
	m_KM013Calib.m_Pos7 = GetPrivateProfileInt("KM013", "Pos7", 6, CALIBRATIONS_FILE);
}

void UARTFrontView717::readTCCalibrations()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	m_TCCalib.m_Min = GetPrivateProfileInt("TC", "Min", 0, CALIBRATIONS_FILE);
	m_TCCalib.m_Max = GetPrivateProfileInt("TC", "Max", 6, CALIBRATIONS_FILE);
}

void UARTFrontView717::readNMCalibrations()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	m_NMCalib.m_Min = GetPrivateProfileInt("NM", "Min", 0, CALIBRATIONS_FILE);
	m_NMCalib.m_Max = GetPrivateProfileInt("NM", "Max", 16, CALIBRATIONS_FILE);
}

void UARTFrontView717::readTMCalibrations()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	m_TMCalib.m_Min = GetPrivateProfileInt("TM", "Min", 0, CALIBRATIONS_FILE);
	m_TMCalib.m_Max = GetPrivateProfileInt("TM", "Max", 16, CALIBRATIONS_FILE);
}

void UARTFrontView717::readKiloVoltmeterCalibrations()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	m_KiloVoltmeterCalib.m_Min = GetPrivateProfileInt("KiloVoltmeter", "Min", 0, CALIBRATIONS_FILE);
	m_KiloVoltmeterCalib.m_Max = GetPrivateProfileInt("KiloVoltmeter", "Max", 1000, CALIBRATIONS_FILE);
}

void UARTFrontView717::readAmmeterCalibrations()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	m_AmmeterCalib.m_Min = GetPrivateProfileInt("Ammeter", "Min", 0, CALIBRATIONS_FILE);
	m_AmmeterCalib.m_Max = GetPrivateProfileInt("Ammeter", "Max", 1000, CALIBRATIONS_FILE);
}

void UARTFrontView717::readBattVoltmerCalibrations()
{
#ifdef SHOW_CONSOLE
	printf("%s\n", __FUNCSIG__);
#endif
	m_BattVoltmeterCalib.m_Min = GetPrivateProfileInt("BattVoltmeter", "Min", 0, CALIBRATIONS_FILE);
	m_BattVoltmeterCalib.m_Max = GetPrivateProfileInt("BattVoltmeter", "Max", 150, CALIBRATIONS_FILE);
}

static byte byte7DecSeg[] = { 0x00,0x08,0x01,0x09,0x02,0x0A,0x03,0x0B,0x04,0x0C };
byte UARTFrontView717::convertIntTo7DecSegByte(int number)
{
	number = number % 100;

	int number1 = number % 10;
	int number10 = (number - number1) / 10;

	return (byte7DecSeg[number10] | (byte7DecSeg[number1] << 4));
}
