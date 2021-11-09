#include "main.h"
//using namespace Glob;
#define M717 Glob::UARTFrontViewClient::Metro81717Signals
#define Input mainContainer["Input"]
#define Output mainContainer["Output"]

int const iNanSignal = 0;
int const notSignal = 0;

byte Glob::ASNPText[50];
byte Glob::ASOTPText[50];
M717::CircuitBrakersOn M717::circuitBrakersOn;
M717::CircuitBrakersOff M717::circuitBrakersOff;
M717::InHardwareSignals M717::inHardwareSignals;
M717::OutHardwareSignals M717::outHardwareSignals;
M717::ConfigValues M717::configValues;

void M717::readAdc1Config()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("stopcrane.txt");

	for (int i = 0; i < 2; i++) {
		std::getline(infile, STRING);
		configValues.Adc1c[i] = std::stoi(STRING);
	}
}

void M717::readAdc2Config()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("km013.txt");

	for (int i = 0; i < 7; i++) {
		std::getline(infile, STRING);
		configValues.Adc2c[i] = std::stoi(STRING);
	}
}

void M717::readAdc3Config()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("kv70.txt");

	for (int i = 0; i < 7; i++) {
		std::getline(infile, STRING);
		configValues.Adc3c[i] = std::stoi(STRING);
	}
}

void M717::readTCConfig()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("ArrowTC.txt");

	for (int i = 0; i < 2; i++) {
		std::getline(infile, STRING);
		configValues.Step1c[i] = std::stoi(STRING);
	}
}

void M717::readNMConfig()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("ArrowNM.txt");

	for (int i = 0; i < 2; i++) {
		std::getline(infile, STRING);
		configValues.Step2c[i] = std::stoi(STRING);
	}
}

void M717::readTMConfig()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("ArrowTM.txt");

	for (int i = 0; i < 2; i++) {
		std::getline(infile, STRING);
		configValues.Step3c[i] = std::stoi(STRING);
	}
}

void M717::readAmpermeterConfig()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("ArrowAmper.txt");

	for (int i = 0; i < 2; i++) {
		std::getline(infile, STRING);
		configValues.Step4c[i] = std::stoi(STRING);
	}
}

void M717::readKiloVoltmeterConfig()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("ArrowKiloVolt.txt");

	for (int i = 0; i < 2; i++) {
		std::getline(infile, STRING);
		configValues.Step5c[i] = std::stoi(STRING);
	}
}

void M717::readVoltmeterConfig()
{
	std::string STRING;
	std::ifstream infile;
	infile.open("ArrowVolt.txt");

	for (int i = 0; i < 2; i++) {
		std::getline(infile, STRING);
		configValues.Step6c[i] = std::stoi(STRING);
	}
}



int M717::startSignals(int a_port)
{
	int retVal = COM_Open(a_port, 115200);
	if (retVal != 0)
	{
		//wchar_t msg[65];
		//wsprintfW(msg, L"Не удалось подключиться к порту №%d\n(Ошибка: %d)", a_port, retVal);
		//MessageBox(NULL, msg, L"Ошибка подключения", 0);
		return retVal;
	}

	// чтение файлов с калибровочными данными
	//readAdc1Config();
	readAdc2Config();
	readAdc3Config();

	readTCConfig();
	readNMConfig();
	readTMConfig();
	readAmpermeterConfig();
	readKiloVoltmeterConfig();
	readVoltmeterConfig();

	InitConfArrays();
	ConfigSignals();

	InitializeCriticalSection(&m_SignalCriticalSection);
	m_isThreadWorking = true;
	m_isInitFinish = true;
	m_isTthreadEnded = false;
	_beginthread(ThreadFunction, 0, (&mainContainer));
	return retVal;
}

void M717::stopSignals()
{
	m_isThreadWorking = false;

	while (m_isTthreadEnded == false)
		Sleep(100);

	DeleteCriticalSection(&m_SignalCriticalSection);

	shutdown();

	COM_Close();

	delete[] ADC_Signals;
	delete[] Input_Signals;
	delete[] Output_Signals;
	delete[] Arrow_Signals;
	delete[] Config_Signals;
	delete[] Config_Bytes;
	delete[] Input_Bytes;
	delete[] Output_Bytes;
	delete[] UART_Bytes;
	delete[] Indicator_Signals;
	delete[] SAUT_Signals;
	delete[] _7SegDec_Signals;
	delete[] TextDisplaysSignalsStruct;
}

void M717::InitConfArrays()
{
	// Количество контроллеров
	Number_Of_Controllers = 14;
	// Общее количество АЦП
	Nmb_Of_ADC_Signals = 3;
	// Размер массива сигналов стрелок
	Nmb_Of_Arrow_Signals = 6;
	// Размер массива сигналов индикаторам
	Nmb_Of_Indicator_Signals = 0;
	// Размер массива сигналов САУТ
	Nmb_Of_SAUT_Signals = 0;
	// Размер массива сигналов 7SegDec
	Nmb_Of_7SegDec_Signals = 1;
	// Количество подключенных текстовых дисплеев
	Nmb_Of_Text_Displays = 2;
	// Количество стрелочных приборов на контроллер
	Config_Arrow_Signals.resize((size_t)Number_Of_Controllers);
	Config_Arrow_Signals[0] = 0;
	Config_Arrow_Signals[1] = 0;
	Config_Arrow_Signals[2] = 3;
	Config_Arrow_Signals[3] = 3;
	Config_Arrow_Signals[4] = 0;
	Config_Arrow_Signals[5] = 0;
	Config_Arrow_Signals[6] = 0;
	Config_Arrow_Signals[7] = 0;
	Config_Arrow_Signals[8] = 0;
	Config_Arrow_Signals[9] = 0;
	Config_Arrow_Signals[10] = 0;
	Config_Arrow_Signals[11] = 0;
	Config_Arrow_Signals[12] = 0;
	Config_Arrow_Signals[13] = 0;
	// Массив конфигурации приборов индикации
	Config_Indicator_Signals.resize((size_t)Number_Of_Controllers);
	Config_Indicator_Signals[0].map[0][0] = 0;
	Config_Indicator_Signals[0].map[0][1] = 0;
	Config_Indicator_Signals[0].map[1][0] = 0;
	Config_Indicator_Signals[0].map[1][1] = 0;
	Config_Indicator_Signals[0].map[2][0] = 0;
	Config_Indicator_Signals[0].map[2][1] = 0;
	Config_Indicator_Signals[1].map[0][0] = 0;
	Config_Indicator_Signals[1].map[0][1] = 0;
	Config_Indicator_Signals[1].map[1][0] = 0;
	Config_Indicator_Signals[1].map[1][1] = 0;
	Config_Indicator_Signals[1].map[2][0] = 0;
	Config_Indicator_Signals[1].map[2][1] = 0;
	Config_Indicator_Signals[2].map[0][0] = 0;
	Config_Indicator_Signals[2].map[0][1] = 0;
	Config_Indicator_Signals[2].map[1][0] = 0;
	Config_Indicator_Signals[2].map[1][1] = 0;
	Config_Indicator_Signals[2].map[2][0] = 0;
	Config_Indicator_Signals[2].map[2][1] = 0;
	Config_Indicator_Signals[3].map[0][0] = 0;
	Config_Indicator_Signals[3].map[0][1] = 0;
	Config_Indicator_Signals[3].map[1][0] = 0;
	Config_Indicator_Signals[3].map[1][1] = 0;
	Config_Indicator_Signals[3].map[2][0] = 0;
	Config_Indicator_Signals[3].map[2][1] = 0;
	Config_Indicator_Signals[4].map[0][0] = 0;
	Config_Indicator_Signals[4].map[0][1] = 0;
	Config_Indicator_Signals[4].map[1][0] = 0;
	Config_Indicator_Signals[4].map[1][1] = 0;
	Config_Indicator_Signals[4].map[2][0] = 0;
	Config_Indicator_Signals[4].map[2][1] = 0;
	Config_Indicator_Signals[5].map[0][0] = 0;
	Config_Indicator_Signals[5].map[0][1] = 0;
	Config_Indicator_Signals[5].map[1][0] = 0;
	Config_Indicator_Signals[5].map[1][1] = 0;
	Config_Indicator_Signals[5].map[2][0] = 0;
	Config_Indicator_Signals[5].map[2][1] = 0;
	Config_Indicator_Signals[6].map[0][0] = 0;
	Config_Indicator_Signals[6].map[0][1] = 0;
	Config_Indicator_Signals[6].map[1][0] = 0;
	Config_Indicator_Signals[6].map[1][1] = 0;
	Config_Indicator_Signals[6].map[2][0] = 0;
	Config_Indicator_Signals[6].map[2][1] = 0;
	Config_Indicator_Signals[7].map[0][0] = 0;
	Config_Indicator_Signals[7].map[0][1] = 0;
	Config_Indicator_Signals[7].map[1][0] = 0;
	Config_Indicator_Signals[7].map[1][1] = 0;
	Config_Indicator_Signals[7].map[2][0] = 0;
	Config_Indicator_Signals[7].map[2][1] = 0;
	Config_Indicator_Signals[8].map[0][0] = 0;
	Config_Indicator_Signals[8].map[0][1] = 0;
	Config_Indicator_Signals[8].map[1][0] = 0;
	Config_Indicator_Signals[8].map[1][1] = 0;
	Config_Indicator_Signals[8].map[2][0] = 0;
	Config_Indicator_Signals[8].map[2][1] = 0;
	Config_Indicator_Signals[9].map[0][0] = 0;
	Config_Indicator_Signals[9].map[0][1] = 0;
	Config_Indicator_Signals[9].map[1][0] = 0;
	Config_Indicator_Signals[9].map[1][1] = 0;
	Config_Indicator_Signals[9].map[2][0] = 0;
	Config_Indicator_Signals[9].map[2][1] = 0;
	Config_Indicator_Signals[10].map[0][0] = 0;
	Config_Indicator_Signals[10].map[0][1] = 0;
	Config_Indicator_Signals[10].map[1][0] = 0;
	Config_Indicator_Signals[10].map[1][1] = 0;
	Config_Indicator_Signals[10].map[2][0] = 0;
	Config_Indicator_Signals[10].map[2][1] = 0;
	Config_Indicator_Signals[11].map[0][0] = 0;
	Config_Indicator_Signals[11].map[0][1] = 0;
	Config_Indicator_Signals[11].map[1][0] = 0;
	Config_Indicator_Signals[11].map[1][1] = 0;
	Config_Indicator_Signals[11].map[2][0] = 0;
	Config_Indicator_Signals[11].map[2][1] = 0;
	Config_Indicator_Signals[12].map[0][0] = 0;
	Config_Indicator_Signals[12].map[0][1] = 0;
	Config_Indicator_Signals[12].map[1][0] = 0;
	Config_Indicator_Signals[12].map[1][1] = 0;
	Config_Indicator_Signals[12].map[2][0] = 0;
	Config_Indicator_Signals[12].map[2][1] = 0;
	Config_Indicator_Signals[13].map[0][0] = 0;
	Config_Indicator_Signals[13].map[0][1] = 0;
	Config_Indicator_Signals[13].map[1][0] = 0;
	Config_Indicator_Signals[13].map[1][1] = 0;
	Config_Indicator_Signals[13].map[2][0] = 0;
	Config_Indicator_Signals[13].map[2][1] = 0;
	// Массив конфигурации приборов САУТ
	Config_SAUT_Signals.resize((size_t)Number_Of_Controllers);
	Config_SAUT_Signals[0] = 0;
	Config_SAUT_Signals[1] = 0;
	Config_SAUT_Signals[2] = 0;
	Config_SAUT_Signals[3] = 0;
	Config_SAUT_Signals[4] = 0;
	Config_SAUT_Signals[5] = 0;
	Config_SAUT_Signals[6] = 0;
	Config_SAUT_Signals[7] = 0;
	Config_SAUT_Signals[8] = 0;
	Config_SAUT_Signals[9] = 0;
	Config_SAUT_Signals[10] = 0;
	Config_SAUT_Signals[11] = 0;
	Config_SAUT_Signals[12] = 0;
	Config_SAUT_Signals[13] = 0;
	// Массив конфигурации 7-ми сегментного индикатора скорости для метро (через дешифратор К514ИД2)
	Config_7SegDec_Signals[0][0] = 0;
	Config_7SegDec_Signals[0][1] = 0;
	Config_7SegDec_Signals[0][2] = 0;
	Config_7SegDec_Signals[1][0] = 0;
	Config_7SegDec_Signals[1][1] = 0;
	Config_7SegDec_Signals[1][2] = 0;
	Config_7SegDec_Signals[2][0] = 0;
	Config_7SegDec_Signals[2][1] = 0;
	Config_7SegDec_Signals[2][2] = 0;
	Config_7SegDec_Signals[3][0] = 0;
	Config_7SegDec_Signals[3][1] = 0;
	Config_7SegDec_Signals[3][2] = 0;
	Config_7SegDec_Signals[4][0] = 0;
	Config_7SegDec_Signals[4][1] = 0;
	Config_7SegDec_Signals[4][2] = 0;
	Config_7SegDec_Signals[5][0] = 0;
	Config_7SegDec_Signals[5][1] = 0;
	Config_7SegDec_Signals[5][2] = 0;
	Config_7SegDec_Signals[6][0] = 0;
	Config_7SegDec_Signals[6][1] = 0;
	Config_7SegDec_Signals[6][2] = 0;
	Config_7SegDec_Signals[7][0] = 0;
	Config_7SegDec_Signals[7][1] = 0;
	Config_7SegDec_Signals[7][2] = 0;
	Config_7SegDec_Signals[8][0] = 1;
	Config_7SegDec_Signals[8][1] = 0;
	Config_7SegDec_Signals[8][2] = 0;
	Config_7SegDec_Signals[9][0] = 0;
	Config_7SegDec_Signals[9][1] = 0;
	Config_7SegDec_Signals[9][2] = 0;
	Config_7SegDec_Signals[10][0] = 0;
	Config_7SegDec_Signals[10][1] = 0;
	Config_7SegDec_Signals[10][2] = 0;
	Config_7SegDec_Signals[11][0] = 0;
	Config_7SegDec_Signals[11][1] = 0;
	Config_7SegDec_Signals[11][2] = 0;
	Config_7SegDec_Signals[12][0] = 0;
	Config_7SegDec_Signals[12][1] = 0;
	Config_7SegDec_Signals[12][2] = 0;
	Config_7SegDec_Signals[13][0] = 0;
	Config_7SegDec_Signals[13][1] = 0;
	Config_7SegDec_Signals[13][2] = 0;
	// Массив конфигурации текстовых дисплеев. Число, отличное от нуля, обозначает кол-во символов в одной строке дисплея.
	Config_Text_Signals.resize((size_t)Number_Of_Controllers);
	Config_Text_Signals[0] = 0;
	Config_Text_Signals[1] = 0;
	Config_Text_Signals[2] = 0;
	Config_Text_Signals[3] = 0;
	Config_Text_Signals[4] = 0;
	Config_Text_Signals[5] = 0;
	Config_Text_Signals[6] = 0;
	Config_Text_Signals[7] = 0;
	Config_Text_Signals[8] = 0;
	Config_Text_Signals[9] = 0;
	Config_Text_Signals[10] = 0;
	Config_Text_Signals[11] = 0;
	Config_Text_Signals[12] = 20;
	Config_Text_Signals[13] = 24;
	// Главный конфигурационный массив
	Config_Signals_Length = 24 * Number_Of_Controllers;
	Config_Signals = new int[(size_t)Config_Signals_Length];
	Config_Signals[0] = 0; // 
	Config_Signals[1] = 0; // 
	Config_Signals[2] = 0; // 
	Config_Signals[3] = 0; // 
	Config_Signals[4] = 0; // 
	Config_Signals[5] = 0; // 
	Config_Signals[6] = 0; // 
	Config_Signals[7] = 3; // Стоп-кран
	Config_Signals[8] = 1; // УНЧ
	Config_Signals[9] = 1; // Контроль ЭС
	Config_Signals[10] = 1; // Контр громког
	Config_Signals[11] = 1; // Радио
	Config_Signals[12] = 1; // 1-2 программы(прав)
	Config_Signals[13] = 1; // 1-2 программы(лев)
	Config_Signals[14] = 1; // Закр дверей
	Config_Signals[15] = 1; // Возвр РП?(тумблер без фиксации)
	Config_Signals[16] = 1; // Вкл мотор компр
	Config_Signals[17] = 1; // Блок питания
	Config_Signals[18] = 1; // Резерв
	Config_Signals[19] = 1; // Компрессор резерв
	Config_Signals[20] = 1; // АРС 13V
	Config_Signals[21] = 1; // Вверх(дисплей)
	Config_Signals[22] = 1; // Вниз(дисплей)
	Config_Signals[23] = 1; // Меню
	Config_Signals[24] = 3; // Кран маш
	Config_Signals[25] = 3; // Контр маш
	Config_Signals[26] = 0; // 
	Config_Signals[27] = 0; // 
	Config_Signals[28] = 0; // 
	Config_Signals[29] = 0; // 
	Config_Signals[30] = 0; // 
	Config_Signals[31] = 0; // 
	Config_Signals[32] = 1; // АРС
	Config_Signals[33] = 1; // АЛС
	Config_Signals[34] = 1; // АРС-Р
	Config_Signals[35] = 1; // Дешифратор
	Config_Signals[36] = 1; // Осв салона
	Config_Signals[37] = 1; // Осв кабины
	Config_Signals[38] = 1; // Осв пульта
	Config_Signals[39] = 1; // ВП
	Config_Signals[40] = 1; // Рез.закр.дв.
	Config_Signals[41] = 1; // Вкл БВ
	Config_Signals[42] = 1; // Дв лев справа
	Config_Signals[43] = 1; // Дв лев слева
	Config_Signals[44] = 1; // Пр-1, лев-0
	Config_Signals[45] = 1; // Бдительность лев
	Config_Signals[46] = 1; // Бдительность прав
	Config_Signals[47] = 1; // АВУ(инв)
	Config_Signals[48] = 1; // Двери торцевые
	Config_Signals[49] = 1; // Вентиляция кабины
	Config_Signals[50] = 1; // Вкл авар сигн
	Config_Signals[51] = 1; // Защита преобр
	Config_Signals[52] = 1; // Сигнализация
	Config_Signals[53] = 1; // Звонок
	Config_Signals[54] = 1; // Откл БВ
	Config_Signals[55] = 1; // Вентиль №1
	Config_Signals[56] = 1; // Пуск резерв
	Config_Signals[57] = 1; // Авар ход
	Config_Signals[58] = 1; // Фары
	Config_Signals[59] = 1; // ВУС
	Config_Signals[60] = 1; // Авар двери(тумблер)
	Config_Signals[61] = 1; // Авар ход(тумблер)
	Config_Signals[62] = 1; // ВКСТ
	Config_Signals[63] = 1; // Двери прав
	Config_Signals[64] = 1; // КРУ1
	Config_Signals[65] = 1; // КРУ2(горят оба)
	Config_Signals[66] = 1; // 
	Config_Signals[67] = 1; // АСОТП1
	Config_Signals[68] = 1; // АСОТП2
	Config_Signals[69] = 1; // АСОТП3
	Config_Signals[70] = 1; // АСОТП4
	Config_Signals[71] = 1; // 
	Config_Signals[72] = 1; // закр дверей
	Config_Signals[73] = 1; // левые двери
	Config_Signals[74] = 1; // 1 программа
	Config_Signals[75] = 1; // 2 программа
	Config_Signals[76] = 1; // Педаль бдительности
	Config_Signals[77] = 1; // Пневмосигнал
	Config_Signals[78] = 1; // Реверс наз
	Config_Signals[79] = 1; // Реверс вп
	Config_Signals[80] = 1; // 
	Config_Signals[81] = 1; // 
	Config_Signals[82] = 1; // 
	Config_Signals[83] = 1; // 
	Config_Signals[84] = 1; // Управление(предохранитель)
	Config_Signals[85] = 1; // Батарея(предохранитель)
	Config_Signals[86] = 1; // Без подписи(предохранитель)
	Config_Signals[87] = 1; // Преобразователь(предохранитель)
	Config_Signals[88] = 1; // Освещение1(предохранитель)
	Config_Signals[89] = 1; // Освещение2(предохранитель)
	Config_Signals[90] = 1; // уава
	Config_Signals[91] = 1; // 
	Config_Signals[92] = 1; // 
	Config_Signals[93] = 1; // уава(черкизово)
	Config_Signals[94] = 1; // 
	Config_Signals[95] = 1; // 
	Config_Signals[96] = 1; // РЦ1 вкл
	Config_Signals[97] = 1; // Батареи вкл
	Config_Signals[98] = 1; // УОС выкл
	Config_Signals[99] = 1; // Втр прижат
	Config_Signals[100] = 1; // Весь состав
	Config_Signals[101] = 1; // 1 пол
	Config_Signals[102] = 1; // 2 пол
	Config_Signals[103] = 1; // 
	Config_Signals[104] = 1; // Разобщ кран кран машиниста
	Config_Signals[105] = 1; // Стоян. тормоз
	Config_Signals[106] = 1; // ЭПВ-АРС
	Config_Signals[107] = 1; // Разобщ кран манометров
	Config_Signals[108] = 1; // Разобщ кран пневмосигнала
	Config_Signals[109] = 1; // 
	Config_Signals[110] = 1; // 
	Config_Signals[111] = 1; // 
	Config_Signals[112] = 1; // A54-in
	Config_Signals[113] = 1; // ВУ-in
	Config_Signals[114] = 1; // A10-in
	Config_Signals[115] = 1; // A53-in
	Config_Signals[116] = 1; // A49-in
	Config_Signals[117] = 1; // A27-in
	Config_Signals[118] = 1; // AC1-in
	Config_Signals[119] = 1; // A21-in
	Config_Signals[120] = 1; // A26-in
	Config_Signals[121] = 1; // AP63-in
	Config_Signals[122] = 1; // A17-in
	Config_Signals[123] = 1; // A44-in
	Config_Signals[124] = 1; // A45-in
	Config_Signals[125] = 1; // A11-in
	Config_Signals[126] = 1; // A71-in
	Config_Signals[127] = 1; // A41-in
	Config_Signals[128] = 1; // A74-in
	Config_Signals[129] = 1; // A73-in
	Config_Signals[130] = 1; // A79-in
	Config_Signals[131] = 1; // A42-in
	Config_Signals[132] = 1; // A46-in
	Config_Signals[133] = 1; // A47-in
	Config_Signals[134] = 1; // AB1-in
	Config_Signals[135] = 1; // A29-in
	Config_Signals[136] = 1; // A76-in
	Config_Signals[137] = 1; // A48-in
	Config_Signals[138] = 1; // A56-in
	Config_Signals[139] = 1; // A65-in
	Config_Signals[140] = 1; // A25-in
	Config_Signals[141] = 1; // A30-in
	Config_Signals[142] = 1; // A1-in
	Config_Signals[143] = 1; // A20-in
	Config_Signals[144] = 1; // A32-in
	Config_Signals[145] = 1; // A13-in
	Config_Signals[146] = 1; // A43-in
	Config_Signals[147] = 1; // A31-in
	Config_Signals[148] = 1; // A77-in
	Config_Signals[149] = 1; // A78-in
	Config_Signals[150] = 1; // 
	Config_Signals[151] = 1; // A75-in
	Config_Signals[152] = 1; // A22-in
	Config_Signals[153] = 1; // A8-in
	Config_Signals[154] = 1; // A28-in
	Config_Signals[155] = 1; // A38-in
	Config_Signals[156] = 1; // A14-in
	Config_Signals[157] = 1; // A39-in
	Config_Signals[158] = 1; // A6-in
	Config_Signals[159] = 1; // A70-in
	Config_Signals[160] = 1; // A4-in
	Config_Signals[161] = 1; // A5-in
	Config_Signals[162] = 1; // A2-in
	Config_Signals[163] = 1; // A3-in
	Config_Signals[164] = 1; // A50-in
	Config_Signals[165] = 1; // A52-in
	Config_Signals[166] = 1; // A40-in
	Config_Signals[167] = 1; // A80-in
	Config_Signals[168] = 1; // A66-in
	Config_Signals[169] = 1; // A18-in
	Config_Signals[170] = 1; // A24-in
	Config_Signals[171] = 1; // A19-in
	Config_Signals[172] = 1; // A37-in
	Config_Signals[173] = 1; // A51-in
	Config_Signals[174] = 1; // A12-in
	Config_Signals[175] = 1; // A16-in
	Config_Signals[176] = 1; // A68-in
	Config_Signals[177] = 1; // A72-in
	Config_Signals[178] = 1; // A7-in
	Config_Signals[179] = 1; // A9-in
	Config_Signals[180] = 1; // A57-in
	Config_Signals[181] = 1; // A81-in
	Config_Signals[182] = 1; // AB4-in
	Config_Signals[183] = 1; // A15-in
	Config_Signals[184] = 1; // AB6-in
	Config_Signals[185] = 1; // AB5-in
	Config_Signals[186] = 1; // AIS-in
	Config_Signals[187] = 1; // AB3-in
	Config_Signals[188] = 1; // 
	Config_Signals[189] = 1; // 
	Config_Signals[190] = 1; // 
	Config_Signals[191] = 1; // 
	Config_Signals[192] = 2; // 
	Config_Signals[193] = 2; // 
	Config_Signals[194] = 2; // 
	Config_Signals[195] = 2; // 
	Config_Signals[196] = 2; // 
	Config_Signals[197] = 2; // 
	Config_Signals[198] = 2; // 
	Config_Signals[199] = 2; // 
	Config_Signals[200] = 2; // 0Ч
	Config_Signals[201] = 2; // 0
	Config_Signals[202] = 2; // 40
	Config_Signals[203] = 2; // 60
	Config_Signals[204] = 2; // 70
	Config_Signals[205] = 2; // 80
	Config_Signals[206] = 2; // ЛСД
	Config_Signals[207] = 2; // 
	Config_Signals[208] = 2; // ЛХ"РК"
	Config_Signals[209] = 2; // РП
	Config_Signals[210] = 2; // ЛСН
	Config_Signals[211] = 2; // ЛЭКК
	Config_Signals[212] = 2; // ЛКВЦ
	Config_Signals[213] = 2; // ЛН
	Config_Signals[214] = 2; // РС
	Config_Signals[215] = 2; // ЛКВД
	Config_Signals[216] = 2; // 
	Config_Signals[217] = 2; // 
	Config_Signals[218] = 2; // 
	Config_Signals[219] = 2; // ЛВД
	Config_Signals[220] = 2; // ЛКТ
	Config_Signals[221] = 2; // ЛСТ
	Config_Signals[222] = 2; // 
	Config_Signals[223] = 2; // 
	Config_Signals[224] = 2; // 
	Config_Signals[225] = 2; // 
	Config_Signals[226] = 2; // 
	Config_Signals[227] = 2; // Двери левые слева светодиод
	Config_Signals[228] = 2; // Двери левые справа светодиод
	Config_Signals[229] = 2; // РП светодиод 
	Config_Signals[230] = 2; // ЗЗащита преобраз
	Config_Signals[231] = 2; // Резервный белый
	Config_Signals[232] = 2; // ЛСП
	Config_Signals[233] = 2; // АВУ
	Config_Signals[234] = 2; // ЛКВП
	Config_Signals[235] = 2; // Пневмотормоз
	Config_Signals[236] = 2; // ИСТ
	Config_Signals[237] = 2; // Двери правые
	Config_Signals[238] = 2; // 
	Config_Signals[239] = 2; // 
	Config_Signals[240] = 2; // ВУ-o
	Config_Signals[241] = 2; // A54-o
	Config_Signals[242] = 2; // A53-o
	Config_Signals[243] = 2; // A10-o
	Config_Signals[244] = 2; // A27-o
	Config_Signals[245] = 2; // A49-o
	Config_Signals[246] = 2; // A21-o
	Config_Signals[247] = 2; // AC-1-o
	Config_Signals[248] = 2; // AP63-o
	Config_Signals[249] = 2; // A26-o
	Config_Signals[250] = 2; // A44-o
	Config_Signals[251] = 2; // A17-o
	Config_Signals[252] = 2; // A11-o
	Config_Signals[253] = 2; // A45-o
	Config_Signals[254] = 2; // A41-o
	Config_Signals[255] = 2; // A71-o
	Config_Signals[256] = 2; // A73-o
	Config_Signals[257] = 2; // A74-o
	Config_Signals[258] = 2; // A42-o
	Config_Signals[259] = 2; // A79-o
	Config_Signals[260] = 2; // A47-o
	Config_Signals[261] = 2; // A46-o
	Config_Signals[262] = 2; // A29-o
	Config_Signals[263] = 2; // AB1-o
	Config_Signals[264] = 2; // A48-o
	Config_Signals[265] = 2; // A76-o
	Config_Signals[266] = 2; // A65-o
	Config_Signals[267] = 2; // A56-o
	Config_Signals[268] = 2; // A30-o
	Config_Signals[269] = 2; // A25-o
	Config_Signals[270] = 2; // A20-o
	Config_Signals[271] = 2; // A1-o
	Config_Signals[272] = 2; // A13-o
	Config_Signals[273] = 2; // A32-o
	Config_Signals[274] = 2; // A31-o
	Config_Signals[275] = 2; // A43-o
	Config_Signals[276] = 2; // A78-o
	Config_Signals[277] = 2; // A77-o
	Config_Signals[278] = 2; // A75-o
	Config_Signals[279] = 2; // 
	Config_Signals[280] = 2; // 
	Config_Signals[281] = 2; // 
	Config_Signals[282] = 2; // асотп 4
	Config_Signals[283] = 2; // асотп 3
	Config_Signals[284] = 2; // асотп 2
	Config_Signals[285] = 2; // асотп 1
	Config_Signals[286] = 2; // асотп неиспр
	Config_Signals[287] = 2; // асотп пожар 1
	Config_Signals[288] = 2; // A8-o
	Config_Signals[289] = 2; // A22-o
	Config_Signals[290] = 2; // A38-o
	Config_Signals[291] = 2; // A28-o
	Config_Signals[292] = 2; // A39-o
	Config_Signals[293] = 2; // A14-o
	Config_Signals[294] = 2; // A70-o
	Config_Signals[295] = 2; // A6-o
	Config_Signals[296] = 2; // A5-o
	Config_Signals[297] = 2; // A4-o
	Config_Signals[298] = 2; // A3-o
	Config_Signals[299] = 2; // A2-o
	Config_Signals[300] = 2; // A52-o
	Config_Signals[301] = 2; // A50-o
	Config_Signals[302] = 2; // A80-o
	Config_Signals[303] = 2; // A40-o
	Config_Signals[304] = 2; // A18-o
	Config_Signals[305] = 2; // A66-o
	Config_Signals[306] = 2; // A19-o
	Config_Signals[307] = 2; // A24-o
	Config_Signals[308] = 2; // A51-o
	Config_Signals[309] = 2; // A37-o
	Config_Signals[310] = 2; // A16-o
	Config_Signals[311] = 2; // A12-o
	Config_Signals[312] = 2; // A72-o
	Config_Signals[313] = 2; // A68-o
	Config_Signals[314] = 2; // A9-o
	Config_Signals[315] = 2; // A7-o
	Config_Signals[316] = 2; // A81-o
	Config_Signals[317] = 2; // A57-o
	Config_Signals[318] = 2; // A15-o
	Config_Signals[319] = 2; // AB4-o
	Config_Signals[320] = 2; // AB5-o
	Config_Signals[321] = 2; // AB6-o
	Config_Signals[322] = 2; // AB3-o
	Config_Signals[323] = 2; // AIS-o
	Config_Signals[324] = 2; // 
	Config_Signals[325] = 2; // 
	Config_Signals[326] = 2; // 
	Config_Signals[327] = 2; // 
	Config_Signals[328] = 2; // Клапан 1
	Config_Signals[329] = 2; // Клапан 2
	Config_Signals[330] = 2; // 
	Config_Signals[331] = 2; // Клапан 3
	Config_Signals[332] = 2; // 
	Config_Signals[333] = 2; // 
	Config_Signals[334] = 2; // 
	Config_Signals[335] = 2; // 
	/*
	* Конец сгенерированного фрагмента
	*/
}

int M717::Adc1(int adc)
{
	int res = 0;
	if (adc > configValues.Adc1c[0])
		res = 0;
	else if (adc < configValues.Adc1c[1])
		res = 1;

	return res;
}
int M717::Adc2(int adc)
{
	int res = 1;

	if (adc <= (configValues.Adc2c[0] + configValues.Adc2c[1]) / 2)
		res = 1;
	else if (adc > (configValues.Adc2c[0] + configValues.Adc2c[1]) / 2 && adc <= (configValues.Adc2c[1] + configValues.Adc2c[2]) / 2)
		res = 2;
	else if (adc > (configValues.Adc2c[1] + configValues.Adc2c[2]) / 2 && (adc <= (configValues.Adc2c[2] + configValues.Adc2c[3]) / 2))
		res = 3;
	else if (adc > (configValues.Adc2c[2] + configValues.Adc2c[3]) / 2 && (adc <= (configValues.Adc2c[3] + configValues.Adc2c[4]) / 2))
		res = 4;
	else if (adc > (configValues.Adc2c[3] + configValues.Adc2c[4]) / 2 && (adc <= (configValues.Adc2c[4] + configValues.Adc2c[5]) / 2))
		res = 5;
	else if (adc > (configValues.Adc2c[4] + configValues.Adc2c[5]) / 2 && (adc <= (configValues.Adc2c[5] + configValues.Adc2c[6]) / 2))
		res = 6;
	else if (adc > (configValues.Adc2c[5] + configValues.Adc2c[6]) / 2)
		res = 7;

	return res;
}
int M717::Adc3(int adc)
{
	int res = 0;
	if (adc <= (configValues.Adc3c[0] + configValues.Adc3c[1]) / 2)
		res = 3;
	else if (adc > (configValues.Adc3c[0] + configValues.Adc3c[1]) / 2 && adc <= (configValues.Adc3c[1] + configValues.Adc3c[2]) / 2)
		res = 2;
	else if (adc > (configValues.Adc3c[1] + configValues.Adc3c[2]) / 2 && (adc <= (configValues.Adc3c[2] + configValues.Adc3c[3]) / 2))
		res = 1;
	else if (adc > (configValues.Adc3c[2] + configValues.Adc3c[3]) / 2 && (adc <= (configValues.Adc3c[3] + configValues.Adc3c[4]) / 2))
		res = 0;
	else if (adc > (configValues.Adc3c[3] + configValues.Adc3c[4]) / 2 && (adc <= (configValues.Adc3c[4] + configValues.Adc3c[5]) / 2))
		res = 1;
	else if (adc > (configValues.Adc3c[4] + configValues.Adc3c[5]) / 2 && (adc <= (configValues.Adc3c[5] + configValues.Adc3c[6]) / 2))
		res = 2;
	else if (adc > (configValues.Adc3c[5] + configValues.Adc3c[6]) / 2)
		res = 3;
	return res;
}
int M717::Step1(double step) // Тормозной цилиндр
{
	int res = ((double)configValues.Step1c[1] - (double)configValues.Step1c[0]) * step / 6.0 + (double)configValues.Step1c[0];
	return res;
}
int M717::Step2(double step) // Напорная магистраль
{
	int res = ((double)configValues.Step2c[1] - (double)configValues.Step2c[0]) * step / 16.0 + (double)configValues.Step2c[0];
	return res;
}
int M717::Step3(double step) // Тормозная магистраль
{
	int res = ((double)configValues.Step3c[1] - (double)configValues.Step3c[0]) * step / 16.0 + (double)configValues.Step3c[0];
	return res;
}
int M717::Step4(double step) // Амперметр
{
	int res = ((double)configValues.Step4c[1] - (double)configValues.Step4c[0]) * step / 1000.0 + (double)configValues.Step4c[0];
	return res;
}
int M717::Step5(double step) // Киловольтметр
{
	int res = ((double)configValues.Step5c[1] - (double)configValues.Step5c[0]) * step / 1.0 + (double)configValues.Step5c[0];
	return res;
}
int M717::Step6(double step) // АКБ
{
	int res = ((double)configValues.Step6c[1] - (double)configValues.Step6c[0]) * step / 150.0 + (double)configValues.Step6c[0];
	return res;
}

void M717::update()
{
	EnterCriticalSection(&m_SignalCriticalSection);
	if (Arrow_Signals == nullptr)
	{
		LeaveCriticalSection(&m_SignalCriticalSection);
		return;
	}

	Arrow_Signals[0] = outHardwareSignals.arrowBrakeCylinderPressure;
	Arrow_Signals[1] = outHardwareSignals.arrowFeedPipePressure;
	Arrow_Signals[2] = outHardwareSignals.arrowBrakePipePressure;
	Arrow_Signals[3] = outHardwareSignals.arrowAmmeter;
	Arrow_Signals[4] = outHardwareSignals.arrowContactVoltage;
	Arrow_Signals[5] = outHardwareSignals.arrowBatteryVoltage;

	_7SegDec_Signals[0] = outHardwareSignals.vfact;

	bool hasReachedEnd = false;
	for (unsigned int i = 0u; i < 40u; i++) {
		if (!hasReachedEnd) {
			if (outHardwareSignals.textAsotp.size() == 0u) {
				hasReachedEnd = true;
			}
			else if (outHardwareSignals.textAsotp.size() == i) {
				hasReachedEnd = true;
			}
		}
		TextDisplaysSignalsStruct[0].textBytes[i] = outHardwareSignals.textAsotp[i];
	}
	TextDisplaysSignalsStruct[0].DisplayOn = outHardwareSignals.displayAsotp;
	TextDisplaysSignalsStruct[0].DisplayLedOn = outHardwareSignals.displayAsotp;

	hasReachedEnd = false;
	for (unsigned int i = 0u; i < 48u; i++) {
		if (!hasReachedEnd) {
			if (outHardwareSignals.textInformer.size() == 0u) {
				hasReachedEnd = true;
			}
			else if (outHardwareSignals.textInformer.size() == i) {
				hasReachedEnd = true;
			}
		}
		TextDisplaysSignalsStruct[1].textBytes[i] = outHardwareSignals.textInformer[i];
	}
	TextDisplaysSignalsStruct[1].DisplayOn = outHardwareSignals.displayInformer;
	TextDisplaysSignalsStruct[1].DisplayLedOn = outHardwareSignals.displayInformer;

	Output_Signals[200] = outHardwareSignals.L0CH; // 0Ч
	Output_Signals[201] = outHardwareSignals.L00; // 0
	Output_Signals[202] = outHardwareSignals.L40; // 40
	Output_Signals[203] = outHardwareSignals.L60; // 60
	Output_Signals[204] = outHardwareSignals.L70; // 70
	Output_Signals[205] = outHardwareSignals.L80; // 80
	Output_Signals[206] = outHardwareSignals.LSD; // ЛСД
	Output_Signals[208] = outHardwareSignals.LHRK; // ЛХ"РК"
	Output_Signals[209] = outHardwareSignals.LRP; // РП
	Output_Signals[210] = outHardwareSignals.LSN; // ЛСН
	Output_Signals[211] = outHardwareSignals.LEKK; // ЛЭКК
	Output_Signals[212] = outHardwareSignals.LKVC; // ЛКВЦ
	Output_Signals[213] = outHardwareSignals.LN; // ЛН
	Output_Signals[214] = outHardwareSignals.LRS; // РС
	Output_Signals[215] = outHardwareSignals.LKVD; // ЛКВД
	Output_Signals[219] = outHardwareSignals.LVD; // ЛВД
	Output_Signals[220] = outHardwareSignals.LKT; // ЛКТ
	Output_Signals[221] = outHardwareSignals.LST; // ЛСТ
	Output_Signals[227] = outHardwareSignals.lampLeftDoors1; // Двери левые слева светодиод
	Output_Signals[228] = outHardwareSignals.lampLeftDoors2; // Двери левые справа светодиод
	Output_Signals[229] = outHardwareSignals.lampBv; // РП светодиод
	Output_Signals[230] = outHardwareSignals.lampInverterProtection; // ЗЗащита преобраз
	//Output_Signals[231] = outHardwareSignals.; // Резервный белый
	Output_Signals[232] = outHardwareSignals.lampHeaterControl; // ЛСП
	Output_Signals[233] = outHardwareSignals.lampAvu; // АВУ
	Output_Signals[234] = outHardwareSignals.LKVP; // ЛКВП
	Output_Signals[235] = outHardwareSignals.lampPneumBrakes; // Пневмотормоз
	Output_Signals[236] = outHardwareSignals.lampIST; // ИСТ
	Output_Signals[237] = outHardwareSignals.lampRightDoors; // Двери правые
	Output_Signals[240] = circuitBrakersOff.VU; // ВУ-o
	Output_Signals[241] = circuitBrakersOff.A54; // A54-o
	Output_Signals[242] = circuitBrakersOff.A53; // A53-o
	Output_Signals[243] = circuitBrakersOff.A10; // A10-o
	Output_Signals[244] = circuitBrakersOff.A27; // A27-o
	Output_Signals[245] = circuitBrakersOff.A49; // A49-o
	Output_Signals[246] = circuitBrakersOff.A21; // A21-o
	Output_Signals[247] = circuitBrakersOff.AC1; // AC-1-o
	Output_Signals[248] = circuitBrakersOff.AP63; // AP63-o
	Output_Signals[249] = circuitBrakersOff.A26; // A26-o
	Output_Signals[250] = circuitBrakersOff.A44; // A44-o
	Output_Signals[251] = circuitBrakersOff.A17; // A17-o
	Output_Signals[252] = circuitBrakersOff.A11; // A11-o
	Output_Signals[253] = circuitBrakersOff.A45; // A45-o
	Output_Signals[254] = circuitBrakersOff.A41; // A41-o
	Output_Signals[255] = circuitBrakersOff.A71; // A71-o
	Output_Signals[256] = circuitBrakersOff.A73; // A73-o
	Output_Signals[257] = circuitBrakersOff.A74; // A74-o
	Output_Signals[258] = circuitBrakersOff.A42; // A42-o
	Output_Signals[259] = circuitBrakersOff.A79; // A79-o
	Output_Signals[260] = circuitBrakersOff.A47; // A47-o
	Output_Signals[261] = circuitBrakersOff.A46; // A46-o
	Output_Signals[262] = circuitBrakersOff.A29; // A29-o
	Output_Signals[263] = circuitBrakersOff.AB1; // AB1-o
	Output_Signals[264] = circuitBrakersOff.A48; // A48-o
	Output_Signals[265] = circuitBrakersOff.A76; // A76-o
	Output_Signals[266] = circuitBrakersOff.A65; // A65-o
	Output_Signals[267] = circuitBrakersOff.A56; // A56-o
	Output_Signals[268] = circuitBrakersOff.A30; // A30-o
	Output_Signals[269] = circuitBrakersOff.A25; // A25-o
	Output_Signals[270] = circuitBrakersOff.A20; // A20-o
	Output_Signals[271] = circuitBrakersOff.A1; // A1-o
	Output_Signals[272] = circuitBrakersOff.A13; // A13-o
	Output_Signals[273] = circuitBrakersOff.A32; // A32-o
	Output_Signals[274] = circuitBrakersOff.A31; // A31-o
	Output_Signals[275] = circuitBrakersOff.A43; // A43-o
	Output_Signals[276] = circuitBrakersOff.A78; // A78-o
	Output_Signals[277] = circuitBrakersOff.A77; // A77-o
	Output_Signals[278] = circuitBrakersOff.A75; // A75-o
	Output_Signals[282] = outHardwareSignals.lampAsotp4; // асотп 4
	Output_Signals[283] = outHardwareSignals.lampAsotp3; // асотп 3
	Output_Signals[284] = outHardwareSignals.lampAsotp2; // асотп 2
	Output_Signals[285] = outHardwareSignals.lampAsotp1; // асотп 1
	Output_Signals[286] = outHardwareSignals.lampAsotpFault; // асотп неиспр
	Output_Signals[287] = outHardwareSignals.lampAsotpFire; // асотп пожар 1
	Output_Signals[288] = circuitBrakersOff.A8; // A8-o
	Output_Signals[289] = circuitBrakersOff.A22; // A22-o
	Output_Signals[290] = circuitBrakersOff.A38; // A38-o
	Output_Signals[291] = circuitBrakersOff.A28; // A28-o
	Output_Signals[292] = circuitBrakersOff.A39; // A39-o
	Output_Signals[293] = circuitBrakersOff.A14; // A14-o
	Output_Signals[294] = circuitBrakersOff.A70; // A70-o
	Output_Signals[295] = circuitBrakersOff.A6; // A6-o
	Output_Signals[296] = circuitBrakersOff.A5; // A5-o
	Output_Signals[297] = circuitBrakersOff.A4; // A4-o
	Output_Signals[298] = circuitBrakersOff.A3; // A3-o
	Output_Signals[299] = circuitBrakersOff.A2; // A2-o
	Output_Signals[300] = circuitBrakersOff.A52; // A52-o
	Output_Signals[301] = circuitBrakersOff.A50; // A50-o
	Output_Signals[302] = circuitBrakersOff.A80; // A80-o
	Output_Signals[303] = circuitBrakersOff.A40; // A40-o
	Output_Signals[304] = circuitBrakersOff.A18; // A18-o
	Output_Signals[305] = circuitBrakersOff.A66; // A66-o
	Output_Signals[306] = circuitBrakersOff.A19; // A19-o
	Output_Signals[307] = circuitBrakersOff.A24; // A24-o
	Output_Signals[308] = circuitBrakersOff.A51; // A51-o
	Output_Signals[309] = circuitBrakersOff.A37; // A37-o
	Output_Signals[310] = circuitBrakersOff.A16; // A16-o
	Output_Signals[311] = circuitBrakersOff.A12; // A12-o
	Output_Signals[312] = circuitBrakersOff.A72; // A72-o
	Output_Signals[313] = circuitBrakersOff.A68; // A68-o
	Output_Signals[314] = circuitBrakersOff.A9; // A9-o
	Output_Signals[315] = circuitBrakersOff.A7; // A7-o
	Output_Signals[316] = circuitBrakersOff.A81; // A81-o
	Output_Signals[317] = circuitBrakersOff.A57; // A57-o
	Output_Signals[318] = circuitBrakersOff.A15; // A15-o
	Output_Signals[319] = circuitBrakersOff.AB4; // AB4-o
	Output_Signals[320] = circuitBrakersOff.AB5; // AB5-o
	Output_Signals[321] = circuitBrakersOff.AB6; // AB6-o
	Output_Signals[322] = circuitBrakersOff.AB3; // AB3-o
	Output_Signals[323] = circuitBrakersOff.AIS; // AIS-o
	Output_Signals[328] = outHardwareSignals.UAVAServiceBrakeValve; // Клапан 1
	Output_Signals[329] = outHardwareSignals.UAVAFeedValve; // Клапан 2
	Output_Signals[331] = outHardwareSignals.UAVAEmergencyBrakeValve; // Клапан 3

	LeaveCriticalSection(&m_SignalCriticalSection);

	UARTFrontViewClient::updateUART();

	EnterCriticalSection(&m_SignalCriticalSection);

	inHardwareSignals.stopCrane = 0;//Adc1(ADC_Signals[0]);
	inHardwareSignals.KM013 = Adc2(ADC_Signals[1]);
	inHardwareSignals.mainController = Adc3(ADC_Signals[2]);
	//inHardwareSignals.ADC3Value = ADC_Signals[2];
	inHardwareSignals.reverseController = Input_Signals[79] - Input_Signals[78];

	inHardwareSignals.UNCHSwitch = Input_Signals[8]; // УНЧ
	inHardwareSignals.controlES = Input_Signals[9]; // Контроль ЭС
	inHardwareSignals.controlSpeaker = Input_Signals[10]; // Контр громког
	inHardwareSignals.radioInformer = Input_Signals[11]; // Радио
	inHardwareSignals.second1Program = Input_Signals[12]; // 1-2 программы(прав)
	inHardwareSignals.first1Program = Input_Signals[13]; // 1-2 программы(лев)
	inHardwareSignals.VZD = Input_Signals[14]; // Закр дверей
	inHardwareSignals.VPR = Input_Signals[15]; // ВПР
	inHardwareSignals.VMK = Input_Signals[16]; // Вкл мотор компр
	inHardwareSignals.powerSupply = Input_Signals[17]; // Блок питания
	//Config_Signals[18] = 1; // Резерв
	inHardwareSignals.KRMK = Input_Signals[19]; // Компрессор резерв
	inHardwareSignals.ARS13V = Input_Signals[20]; // АРС 13V
	inHardwareSignals.informerButtonUp = Input_Signals[21]; // Вверх(дисплей)
	inHardwareSignals.informerButtonDown = Input_Signals[22]; // Вниз(дисплей)
	inHardwareSignals.informerButtonMenu = Input_Signals[23]; // Меню
	inHardwareSignals.KARS = Input_Signals[32]; // АРС
	inHardwareSignals.KALS = Input_Signals[33]; // АЛС
	inHardwareSignals.KARSR = Input_Signals[34]; // АРС-Р
	inHardwareSignals.decoderSwitch = Input_Signals[35]; // Дешифратор
	inHardwareSignals.lightingSalon = Input_Signals[36]; // Осв салона
	inHardwareSignals.lightingCabin = Input_Signals[37]; // Осв кабины
	inHardwareSignals.lightingPanel = Input_Signals[38]; // Осв пульта
	inHardwareSignals.VPSwitch = Input_Signals[39]; // ВП
	inHardwareSignals.KRZD = Input_Signals[40]; // Рез.закр.дв.
	inHardwareSignals.BVreturn = Input_Signals[41]; // Вкл БВ
	inHardwareSignals.KOLD1 = Input_Signals[42]; // Дв лев справа
	inHardwareSignals.KOLD2 = Input_Signals[43]; // Дв лев слева
	inHardwareSignals.doorsChooseSwitch = Input_Signals[44]; // Пр-1, лев-0
	inHardwareSignals.RBmain = Input_Signals[45]; // Бдительность лев
	inHardwareSignals.RBres = Input_Signals[46]; // Бдительность прав
	inHardwareSignals.VAVU = !Input_Signals[47]; // АВУ(инв)
	//Input_Signals[48]; // Двери торцевые
	inHardwareSignals.cabinVentillationSwitch = Input_Signals[49]; // Вентиляция кабины
	//Input_Signals[50]; // Вкл авар сигн
	inHardwareSignals.inverterProtectionSwitch = Input_Signals[51]; // Защита преобр
	inHardwareSignals.faultsSignallingButton = Input_Signals[52]; // Сигнализация неисправностей
	inHardwareSignals.K3 = Input_Signals[53]; // Звонок
	inHardwareSignals.BVoff = Input_Signals[54]; // Откл БВ
	inHardwareSignals.valveOne = Input_Signals[55]; // Вентиль №1
	inHardwareSignals.KRP = Input_Signals[56]; // Пуск резерв
	inHardwareSignals.emergencyTractionButton = Input_Signals[57]; // Авар ход
	inHardwareSignals.headlights = Input_Signals[58]; // Фары
	inHardwareSignals.VUSSwitch = Input_Signals[59]; // ВУС
	inHardwareSignals.emergencyDoorSwitch = Input_Signals[60]; // Авар двери(тумблер)
	inHardwareSignals.emergencyTractionSwitch = Input_Signals[61]; // Авар ход(тумблер)
	inHardwareSignals.parkingBrakeControlSwitch = Input_Signals[62]; // ВКСТ
	inHardwareSignals.KORD = Input_Signals[63]; // Двери прав
	inHardwareSignals.KRU = Input_Signals[64] + Input_Signals[65]; // КРУ2(горят оба)
	inHardwareSignals.asotpButton1 = Input_Signals[67]; // АСОТП1
	inHardwareSignals.asotpButton2 = Input_Signals[68]; // АСОТП2
	inHardwareSignals.asotpButton3 = Input_Signals[69]; // АСОТП3
	inHardwareSignals.asotpButton4 = Input_Signals[70]; // АСОТП4
	inHardwareSignals.doorCloseSwitch2 = Input_Signals[72]; // закрытие дверей (у помощника машиниста на стене)
	inHardwareSignals.leftDoorOpenSwitch2 = Input_Signals[73]; // левые двери (у помощника машиниста на стене)
	inHardwareSignals.first2Program = Input_Signals[74]; // 1 программа
	inHardwareSignals.second2Program = Input_Signals[75]; // 2 программа
	inHardwareSignals.safetyPedal = Input_Signals[76]; // Педаль безопасности
	inHardwareSignals.pneumSignalPedal = Input_Signals[77]; // Пневмосигнал
	inHardwareSignals.protectionControlCircuits = Input_Signals[84]; // Управление(предохранитель)
	inHardwareSignals.protectionBattery = Input_Signals[85]; // Батарея(предохранитель)
	inHardwareSignals.protectionMasterSwitch = Input_Signals[86]; // Без подписи(предохранитель)
	inHardwareSignals.protectionInverter = Input_Signals[87]; // Преобразователь(предохранитель)
	inHardwareSignals.protectionLighting1 = Input_Signals[88]; // Освещение1(предохранитель)
	inHardwareSignals.protectionLighting2 = Input_Signals[89]; // Освещение2(предохранитель)
	inHardwareSignals.UAVAButton = Input_Signals[90]; // уава кнопка
	//Config_Signals[93] = 1; // уава(черкизово)
	inHardwareSignals.circuitSplitterSwitch = Input_Signals[96]; // РЦ1 вкл
	inHardwareSignals.VB = Input_Signals[97]; // Батареи вкл
	inHardwareSignals.UOS = Input_Signals[98]; // УОС выкл
	inHardwareSignals.pantographSwitch = Input_Signals[99] + // ВТР прижат
		2 * Input_Signals[100] + // Весь состав
		3 * Input_Signals[101] + // 1 пол
		4 * Input_Signals[102]; // 2 пол
	inHardwareSignals.automaticBrakeCutOffValve = !Input_Signals[104]; // Разобщ. кран машиниста
	inHardwareSignals.parkingBrakeCutOffValve = !Input_Signals[105]; // Стоян. тормоз
	inHardwareSignals.epvArsCutOffValve = !Input_Signals[106]; // ЭПВ-АРС
	inHardwareSignals.manometerCutOffValve = !Input_Signals[107]; // Разобщ кран манометров
	inHardwareSignals.pneumSignalCutOffValve = !Input_Signals[108]; // Разобщ кран пневмосигнала
	circuitBrakersOn.A54 = Input_Signals[112]; // A54-in
	circuitBrakersOn.A84 = Input_Signals[113]; // ВУ-in
	circuitBrakersOn.A10 = Input_Signals[114]; // A10-in
	circuitBrakersOn.A53 = Input_Signals[115]; // A53-in
	circuitBrakersOn.A49 = Input_Signals[116]; // A49-in
	circuitBrakersOn.A27 = Input_Signals[117]; // A27-in
	circuitBrakersOn.AC1 = Input_Signals[118]; // AC1-in
	circuitBrakersOn.A21 = Input_Signals[119]; // A21-in
	circuitBrakersOn.A26 = Input_Signals[120]; // A26-in
	circuitBrakersOn.AP63 = Input_Signals[121]; // AP63-in
	circuitBrakersOn.A17 = Input_Signals[122]; // A17-in
	circuitBrakersOn.A44 = Input_Signals[123]; // A44-in
	circuitBrakersOn.A45 = Input_Signals[124]; // A45-in
	circuitBrakersOn.A11 = Input_Signals[125]; // A11-in
	circuitBrakersOn.A71 = Input_Signals[126]; // A71-in
	circuitBrakersOn.A41 = Input_Signals[127]; // A41-in
	circuitBrakersOn.A74 = Input_Signals[128]; // A74-in
	circuitBrakersOn.A73 = Input_Signals[129]; // A73-in
	circuitBrakersOn.A79 = Input_Signals[130]; // A79-in
	circuitBrakersOn.A42 = Input_Signals[131]; // A42-in
	circuitBrakersOn.A46 = Input_Signals[132]; // A46-in
	circuitBrakersOn.A47 = Input_Signals[133]; // A47-in
	circuitBrakersOn.AB1 = Input_Signals[134]; // AB1-in
	circuitBrakersOn.A29 = Input_Signals[135]; // A29-in
	circuitBrakersOn.A76 = Input_Signals[136]; // A76-in
	circuitBrakersOn.A48 = Input_Signals[137]; // A48-in
	circuitBrakersOn.A56 = Input_Signals[138]; // A56-in
	circuitBrakersOn.A65 = Input_Signals[139]; // A65-in
	circuitBrakersOn.A25 = Input_Signals[140]; // A25-in
	circuitBrakersOn.A30 = Input_Signals[141]; // A30-in
	circuitBrakersOn.A1 = Input_Signals[142]; // A1-in
	circuitBrakersOn.A20 = Input_Signals[143]; // A20-in
	circuitBrakersOn.A32 = Input_Signals[144]; // A32-in
	circuitBrakersOn.A13 = Input_Signals[145]; // A13-in
	circuitBrakersOn.A43 = Input_Signals[146]; // A43-in
	circuitBrakersOn.A31 = Input_Signals[147]; // A31-in
	circuitBrakersOn.A77 = Input_Signals[148]; // A77-in
	circuitBrakersOn.A78 = Input_Signals[149]; // A78-in
	circuitBrakersOn.A75 = Input_Signals[151]; // A75-in
	circuitBrakersOn.A22 = Input_Signals[152]; // A22-in
	circuitBrakersOn.A8 = Input_Signals[153]; // A8-in
	circuitBrakersOn.A28 = Input_Signals[154]; // A28-in
	circuitBrakersOn.A38 = Input_Signals[155]; // A38-in
	circuitBrakersOn.A14 = Input_Signals[156]; // A14-in
	circuitBrakersOn.A39 = Input_Signals[157]; // A39-in
	circuitBrakersOn.A6 = Input_Signals[158]; // A6-in
	circuitBrakersOn.A70 = Input_Signals[159]; // A70-in
	circuitBrakersOn.A4 = Input_Signals[160]; // A4-in
	circuitBrakersOn.A5 = Input_Signals[161]; // A5-in
	circuitBrakersOn.A2 = Input_Signals[162]; // A2-in
	circuitBrakersOn.A3 = Input_Signals[163]; // A3-in
	circuitBrakersOn.A50 = Input_Signals[164]; // A50-in
	circuitBrakersOn.A52 = Input_Signals[165]; // A52-in
	circuitBrakersOn.A40 = Input_Signals[166]; // A40-in
	circuitBrakersOn.A80 = Input_Signals[167]; // A80-in
	circuitBrakersOn.A66 = Input_Signals[168];// A66-in
	circuitBrakersOn.A18 = Input_Signals[169]; // A18-in
	circuitBrakersOn.A24 = Input_Signals[170]; // A24-in
	circuitBrakersOn.A19 = Input_Signals[171]; // A19-in
	circuitBrakersOn.A37 = Input_Signals[172]; // A37-in
	circuitBrakersOn.A51 = Input_Signals[173]; // A51-in
	circuitBrakersOn.A12 = Input_Signals[174]; // A12-in
	circuitBrakersOn.A16 = Input_Signals[175]; // A16-in
	circuitBrakersOn.A68 = Input_Signals[176]; // A68-in
	circuitBrakersOn.A72 = Input_Signals[177]; // A72-in
	circuitBrakersOn.A7 = Input_Signals[178]; // A7-in
	circuitBrakersOn.A9 = Input_Signals[179]; // A9-in
	circuitBrakersOn.A57 = Input_Signals[180]; // A57-in
	circuitBrakersOn.A81 = Input_Signals[181]; // A81-in
	circuitBrakersOn.AB4 = Input_Signals[182]; // AB4-in
	circuitBrakersOn.A15 = Input_Signals[183]; // A15-in
	circuitBrakersOn.AB6 = Input_Signals[184]; // AB6-in
	circuitBrakersOn.AB5 = Input_Signals[185]; // AB5-in
	circuitBrakersOn.AIS = Input_Signals[186]; // AIS-in
	circuitBrakersOn.AB3 = Input_Signals[187]; // AB3-in

	LeaveCriticalSection(&m_SignalCriticalSection);
}

void M717::inSignals()
{

	EnterCriticalSection(&m_SignalCriticalSection);

	Input["IGLA1"] = inHardwareSignals.asotpButton1;
	Input["IGLA2"] = inHardwareSignals.asotpButton2;
	Input["IGLA3"] = inHardwareSignals.asotpButton3;
	Input["IGLA4"] = inHardwareSignals.asotpButton4;
	Input["DriverValveDisconnect"] = inHardwareSignals.automaticBrakeCutOffValve;
	//Input["VOVT"] = 1;//inHardwareSignals.automaticBrakeDisablerSwitch;
	Input["OtklBV"] = inHardwareSignals.BVoff;//откл. БВ
	Input["VozvratRP"] = inHardwareSignals.BVreturn;//возврат БВ
	Input["V13"] = inHardwareSignals.cabinVentillationSwitch;
	Input["RC1"] = inHardwareSignals.circuitSplitterSwitch;
	Input["R_ZS"] = inHardwareSignals.controlES;
	Input["R_G"] = inHardwareSignals.controlSpeaker;
	Input["ALSFreq"] = inHardwareSignals.decoderSwitch;
	Input["VUD2"] = inHardwareSignals.doorCloseSwitch2;
	Input["DoorSelect"] = inHardwareSignals.doorsChooseSwitch;//выбор дверей
	Input["VAD"] = inHardwareSignals.emergencyDoorSwitch;
	Input["KAH"] = inHardwareSignals.emergencyTractionButton;
	Input["VAH"] = inHardwareSignals.emergencyTractionSwitch;
	//Input["EmergencyVentillationSwitch"] = inHardwareSignals.emergencyVentillationSwitch;
	//Input["EndDoorsSwitch"] = inHardwareSignals.endDoorsSwitch;
	Input["EPK"] = inHardwareSignals.epvArsCutOffValve; // кран ЭПВ-АРС
	Input["KSN"] = inHardwareSignals.faultsSignallingButton;//сигнализация неисправностей
	Input["R_Program1"] = inHardwareSignals.first1Program;
	Input["R_Program2"] = inHardwareSignals.second1Program;
	Input["R_Program1H"] = inHardwareSignals.first2Program;
	Input["R_Program2H"] = inHardwareSignals.second2Program;
	Input["L_4"] = inHardwareSignals.headlights;//Фары
	Input["R_ASNPDown"] = inHardwareSignals.informerButtonDown;
	Input["R_ASNPMenu"] = inHardwareSignals.informerButtonMenu;
	Input["R_ASNPUp"] = inHardwareSignals.informerButtonUp;
	Input["ConverterProtection"] = inHardwareSignals.inverterProtectionSwitch;
	Input["Ring"] = inHardwareSignals.K3;//звонок
	Input["ALS"] = inHardwareSignals.KALS;//АЛС
	Input["ARS"] = inHardwareSignals.KARS;//АРС
	Input["ARSR"] = inHardwareSignals.KARSR;//АРС-Р
	Input["KDLR"] = inHardwareSignals.KOLD1;//двери левые
	Input["KDL"] = inHardwareSignals.KOLD2;
	Input["KDP"] = inHardwareSignals.KORD;//двери правые
	Input["RezMK"] = inHardwareSignals.KRMK;//резервный МК
	Input["KRP"] = inHardwareSignals.KRP;//пуск резервный
	Input["KRUReverser"] = inHardwareSignals.KRU;//контроллер резервного управления
	Input["KRZD"] = inHardwareSignals.KRZD;//резервн закрытие дверей
	Input["VDL"] = inHardwareSignals.leftDoorOpenSwitch2;
	Input["L_2"] = inHardwareSignals.lightingCabin;
	Input["L_3"] = inHardwareSignals.lightingPanel;
	Input["L_1"] = inHardwareSignals.lightingSalon;//освещение салона
	Input["KV70Controller"] = inHardwareSignals.mainController;
	//Input["ManometerCutOffValve"] = inHardwareSignals.manometerCutOffValve;
	//Input["PantographSwitch"] = inHardwareSignals.pantographSwitch;
	//Input["VKST"] = inHardwareSignals.parkingBrakeControlSwitch;
	Input["ParkingBrake"] = inHardwareSignals.parkingBrakeCutOffValve;
	//Input["PneumSignalCutOffValve"] = inHardwareSignals.pneumSignalCutOffValve;
	Input["HornSignal"] = inHardwareSignals.pneumSignalPedal;
	Input["BPSNon"] = inHardwareSignals.powerSupply;
	//Input["ProtectionBattery"] = inHardwareSignals.protectionBattery;
	//Input["ProtectionControlCircuits"] = inHardwareSignals.protectionControlCircuits;
	//Input["ProtectionInverter"] = inHardwareSignals.protectionInverter;
	//Input["ProtectionLighting1"] = inHardwareSignals.protectionLighting1;
	//Input["ProtectionLighting2"] = inHardwareSignals.protectionLighting2;
	//Input["ProtectionMasterSwitch"] = inHardwareSignals.protectionMasterSwitch;
	Input["R_Radio"] = inHardwareSignals.radioInformer;
	Input["KVT"] = inHardwareSignals.RBmain;// бдителльность1
	Input["KVTR"] = inHardwareSignals.RBres;//бдительность2
	Input["KV70Reverser"] = inHardwareSignals.reverseController;
	Input["PB"] = inHardwareSignals.safetyPedal;
	Input["ARS13"] = inHardwareSignals.ARS13V;
	Input["EmergencyBrakeValve"] = inHardwareSignals.stopCrane;//стоп кран
	Input["KM013"] = inHardwareSignals.KM013;
	Input["UAVAC"] = inHardwareSignals.UAVAButton;//уава кран
	Input["R_UNch"] = inHardwareSignals.UNCHSwitch;
	Input["UOS"] = inHardwareSignals.UOS;//УОС
	Input["VZ1"] = inHardwareSignals.valveOne;
	Input["OtklAVU"] = inHardwareSignals.VAVU;//откл. АВУ
	Input["VB"] = inHardwareSignals.VB; // Переключатель батареи
	//Input["VBD"] = inHardwareSignals.VBD;//откл. бл. дверей
	Input["R_VPR"] = inHardwareSignals.VPR;
	Input["VMK"] = inHardwareSignals.VMK;//МК
	Input["VP"] = inHardwareSignals.VPSwitch;
	Input["VUS"] = inHardwareSignals.VUSSwitch;
	Input["VUD1"] = inHardwareSignals.VZD;//закрытие дверей

	Input["A1"] = circuitBrakersOn.A1;; // Ход
	Input["A2"] = circuitBrakersOn.A2; // Управление РК
	Input["A3"] = circuitBrakersOn.A3; // Ход 3
	Input["A4"] = circuitBrakersOn.A4; // Ход назад
	Input["A5"] = circuitBrakersOn.A5; // Ход вперед
	Input["A6"] = circuitBrakersOn.A6; // Тормоз
	Input["A7"] = circuitBrakersOn.A7;
	Input["A8"] = circuitBrakersOn.A8; // Вентиль замещения 2
	Input["A9"] = circuitBrakersOn.A9;
	Input["A10"] = circuitBrakersOn.A10; // МК
	Input["A11"] = circuitBrakersOn.A11; // освещение кабины и отсеков
	Input["A12"] = circuitBrakersOn.A12; // Резервное закрытие дверей
	Input["A13"] = circuitBrakersOn.A13;
	Input["A14"] = circuitBrakersOn.A14;
	Input["A15"] = circuitBrakersOn.A15;
	Input["A16"] = circuitBrakersOn.A16; // Закрытие дверей
	Input["A17"] = circuitBrakersOn.A17;
	Input["A18"] = circuitBrakersOn.A18; // Восстановление защиты
	Input["A19"] = circuitBrakersOn.A19;
	Input["A20"] = circuitBrakersOn.A20; // ЛК2, дифф. реле
	Input["A21"] = circuitBrakersOn.A21; // управление дверями
	Input["A22"] = circuitBrakersOn.A22; // МК
	Input["A24"] = circuitBrakersOn.A24; // контроль заряда
	Input["A25"] = circuitBrakersOn.A25;
	Input["A26"] = circuitBrakersOn.A26;
	Input["A27"] = circuitBrakersOn.A27; // освещение салона, кабины, стояночный тормоз, лампа РП
	Input["A28"] = circuitBrakersOn.A28;
	Input["A29"] = circuitBrakersOn.A29;
	Input["A30"] = circuitBrakersOn.A30; // СДРК
	Input["A31"] = circuitBrakersOn.A31; // Открытие вери левые
	Input["A32"] = circuitBrakersOn.A32; // Открытие двери правые
	Input["A37"] = circuitBrakersOn.A37;
	Input["A38"] = circuitBrakersOn.A38; // Сигнальная лампа
	Input["A39"] = circuitBrakersOn.A39; // Пуск вагона
	Input["A40"] = circuitBrakersOn.A40; // Тормоз АРС
	Input["A41"] = circuitBrakersOn.A41;
	Input["A42"] = circuitBrakersOn.A42; // АРС
	Input["A43"] = circuitBrakersOn.A43;
	Input["A44"] = circuitBrakersOn.A44; // КРУ, МК РУ
	Input["A45"] = circuitBrakersOn.A45;
	Input["A46"] = circuitBrakersOn.A46;
	Input["A47"] = circuitBrakersOn.A47;
	Input["A48"] = circuitBrakersOn.A48; // АРС-МП РПБ
	Input["A49"] = circuitBrakersOn.A49;
	Input["A50"] = circuitBrakersOn.A50; // контактор освещения салона
	Input["A51"] = circuitBrakersOn.A51;
	Input["A52"] = circuitBrakersOn.A52;
	Input["A53"] = circuitBrakersOn.A53; // КВЦ, аварийное освещение
	Input["A54"] = circuitBrakersOn.A54; // Управление поездом
	Input["A56"] = circuitBrakersOn.A56; // АКБ
	Input["A57"] = circuitBrakersOn.A57; // Ход РК
	Input["A65"] = circuitBrakersOn.A65; // основное освещение
	Input["A66"] = circuitBrakersOn.A66; // Отключение БВ
	Input["A68"] = circuitBrakersOn.A68;
	Input["A70"] = circuitBrakersOn.A70;
	Input["A71"] = circuitBrakersOn.A71;
	Input["A72"] = circuitBrakersOn.A72; // Вентиль синхронизации замещения 1
	Input["A73"] = circuitBrakersOn.A73; // Сигнализация неисправности
	Input["A74"] = circuitBrakersOn.A74; // Возврат защиты
	Input["A75"] = circuitBrakersOn.A75; // основное освещение
	Input["A76"] = circuitBrakersOn.A76;
	//Input["A77"] = circuitBrakersOn.A77;
	//Input["A78"] = circuitBrakersOn.A78;
	//Input["A79"] = circuitBrakersOn.A79; // АРС
	Input["A80"] = circuitBrakersOn.A80;
	Input["A81"] = circuitBrakersOn.A81;
	Input["AIS"] = circuitBrakersOn.AIS;
	Input["AV4"] = circuitBrakersOn.AB4;
	Input["AV5"] = circuitBrakersOn.AB5;
	Input["AV1"] = circuitBrakersOn.AB1;
	Input["AV3"] = circuitBrakersOn.AB3;
	Input["AV6"] = circuitBrakersOn.AB6;
	Input["AS1"] = circuitBrakersOn.AC1;
	Input["AR63"] = circuitBrakersOn.AP63;
	Input["A84"] = circuitBrakersOn.A84; // Выключатель управления

	LeaveCriticalSection(&m_SignalCriticalSection);
}

void M717::outSignals()
{
	if (mainContainer.empty())
		return;

	EnterCriticalSection(&m_SignalCriticalSection);

	outHardwareSignals.arrowAmmeter = Step4(Output["Ammeter"]);
	outHardwareSignals.arrowBatteryVoltage = Step6(Output["BatteryVoltage"]);
	outHardwareSignals.arrowBrakeCylinderPressure = Step1(Output["tcPressure"]);
	outHardwareSignals.arrowBrakePipePressure = Step3(Output["tmPressure"]);
	outHardwareSignals.arrowContactVoltage = Step5((double)Output["KVolt"] / 1000.0);
	outHardwareSignals.arrowFeedPipePressure = Step2(Output["nmPressure"]);
	outHardwareSignals.LAB = Output["LAB"];
	outHardwareSignals.lampAsotp1 = Output["LampAsotp1"];
	outHardwareSignals.lampAsotp2 = Output["LampAsotp2"];
	outHardwareSignals.lampAsotp3 = Output["LampAsotp3"];
	outHardwareSignals.lampAsotp4 = Output["LampAsotp4"];
	outHardwareSignals.lampAvu = Output["LAVU"];
	outHardwareSignals.lampBv = Output["LBV"];
	outHardwareSignals.lampHeaterControl = Output["LSP"];
	outHardwareSignals.lampInformerLeft = Output["InformerLampLeft"];
	outHardwareSignals.lampInformerMiddle = Output["InformerLampMiddle"];
	outHardwareSignals.lampInformerRight = Output["InformerLampRight"];
	outHardwareSignals.lampInverterProtection = Output["LampInverterProtection"];
	outHardwareSignals.lampIST = Output["LampParkingBrake"];
	outHardwareSignals.lampLeftDoors1 = Output["LDoorLeft"];
	outHardwareSignals.lampLeftDoors2 = Output["LDoorLeft"];
	outHardwareSignals.lampPneumBrakes = Output["LampPneumBrakes"];
	outHardwareSignals.lampRightDoors = Output["LDoorRight"];
	outHardwareSignals.LAV = Output["LAV"];
	outHardwareSignals.LAV1 = Output["LAV1"];
	outHardwareSignals.LKVP = Output["LKVP"];
	outHardwareSignals.UAVAEmergencyBrakeValve = Output["UAVAEmergency"];
	outHardwareSignals.UAVAFeedValve = Output["UAVAFeed"];
	outHardwareSignals.UAVAServiceBrakeValve = Output["UAVAService"];

	outHardwareSignals.L00 = Output["L00"];
	outHardwareSignals.LKVC = Output["LKVC"];
	outHardwareSignals.LRP = Output["LRP"];
	outHardwareSignals.LSN = Output["LSN"];
	outHardwareSignals.LVD = Output["LVD"];
	outHardwareSignals.LST = Output["LST"];
	outHardwareSignals.LHRK = Output["LHRK"];
	outHardwareSignals.lampLeftDoors1 = Output["LDoorLeft"];
	outHardwareSignals.lampRightDoors = Output["LDoorRight"];
	outHardwareSignals.lampBv = Output["LBV"];
	outHardwareSignals.L0CH = Output["LOCH"];
	outHardwareSignals.L40 = Output["L40"];
	outHardwareSignals.L60 = Output["L60"];
	outHardwareSignals.L70 = Output["L70"];
	outHardwareSignals.L80 = Output["L80"];
	outHardwareSignals.LRS = Output["LRV"];
	outHardwareSignals.LAB = Output["LAB"];
	outHardwareSignals.LKVD = Output["LKVD"];
	outHardwareSignals.LAV = Output["LAV"];
	outHardwareSignals.LAV1 = Output["LAV1"];

	outHardwareSignals.LSD = Output["LSD"];
	outHardwareSignals.lampPneumBrakes = Output["LampPneumBrakes"];
	outHardwareSignals.LKVP = Output["LKVP"];
	outHardwareSignals.LN = Output["LN"];
	outHardwareSignals.LEKK = Output["LEKK"];
	outHardwareSignals.LKT = Output["LKT"];
	outHardwareSignals.textInformer = (char*)Glob::ASNPText;
	outHardwareSignals.textAsotp = (char*)Glob::ASOTPText;
	outHardwareSignals.displayInformer = Output["InformerDisplay"];
	outHardwareSignals.displayAsotp = Output["AsotpDisplay"];
	outHardwareSignals.vfact = Output["vfact"];

	circuitBrakersOff.A1 = Output["A1off"];
	circuitBrakersOff.A2 = Output["A2off"];
	circuitBrakersOff.A3 = Output["A3off"];
	circuitBrakersOff.A4 = Output["A4off"];
	circuitBrakersOff.A5 = Output["A5off"];
	circuitBrakersOff.A6 = Output["A6off"];
	circuitBrakersOff.A7 = Output["A7off"];
	circuitBrakersOff.A8 = Output["A8off"];
	circuitBrakersOff.A9 = Output["A9off"];
	circuitBrakersOff.A10 = Output["A10off"];
	circuitBrakersOff.A11 = Output["A11off"];
	circuitBrakersOff.A12 = Output["A12off"];
	circuitBrakersOff.A13 = Output["A13off"];
	circuitBrakersOff.A14 = Output["A14off"];
	circuitBrakersOff.A15 = Output["A15off"];
	circuitBrakersOff.A16 = Output["A16off"];
	circuitBrakersOff.A17 = Output["A17off"];
	circuitBrakersOff.A18 = Output["A18off"];
	circuitBrakersOff.A19 = Output["A19off"];
	circuitBrakersOff.A20 = Output["A20off"];
	circuitBrakersOff.A21 = Output["A21off"];
	circuitBrakersOff.A22 = Output["A22off"];
	circuitBrakersOff.A24 = Output["A24off"];
	circuitBrakersOff.A25 = Output["A25off"];
	circuitBrakersOff.A26 = Output["A26off"];
	circuitBrakersOff.A27 = Output["A27off"];
	circuitBrakersOff.A28 = Output["A28off"];
	circuitBrakersOff.A29 = Output["A29off"];
	circuitBrakersOff.A30 = Output["A30off"];
	circuitBrakersOff.A31 = Output["A31off"];
	circuitBrakersOff.A32 = Output["A32off"];
	circuitBrakersOff.AIS = Output["AISoff"];
	circuitBrakersOff.A37 = Output["A37off"];
	circuitBrakersOff.A38 = Output["A38off"];
	circuitBrakersOff.A39 = Output["A39off"];
	circuitBrakersOff.A40 = Output["A40off"];
	circuitBrakersOff.A41 = Output["A41off"];
	circuitBrakersOff.A42 = Output["A42off"];
	circuitBrakersOff.A43 = Output["A43off"];
	circuitBrakersOff.A44 = Output["A44off"];
	circuitBrakersOff.A45 = Output["A45off"];
	circuitBrakersOff.A46 = Output["A46off"];
	circuitBrakersOff.A47 = Output["A47off"];
	circuitBrakersOff.A48 = Output["A48off"];
	circuitBrakersOff.A49 = Output["A49off"];
	circuitBrakersOff.A50 = Output["A50off"];
	circuitBrakersOff.A51 = Output["A51off"];
	circuitBrakersOff.A52 = Output["A52off"];
	circuitBrakersOff.A53 = Output["A53off"];
	circuitBrakersOff.A54 = Output["A54off"];
	circuitBrakersOff.A56 = Output["A56off"];
	circuitBrakersOff.A57 = Output["A57off"];
	circuitBrakersOff.A65 = Output["A65off"];
	circuitBrakersOff.A66 = Output["A66off"];
	circuitBrakersOff.A68 = Output["A68off"];
	circuitBrakersOff.A70 = Output["A70off"];
	circuitBrakersOff.A71 = Output["A71off"];
	circuitBrakersOff.A72 = Output["A72off"];
	circuitBrakersOff.A73 = Output["A73off"];
	circuitBrakersOff.A74 = Output["A74off"];
	circuitBrakersOff.A75 = Output["A75off"];
	circuitBrakersOff.A76 = Output["A76off"];
	circuitBrakersOff.A77 = Output["A77off"];
	circuitBrakersOff.A78 = Output["A78off"];
	circuitBrakersOff.A79 = Output["A79off"];
	circuitBrakersOff.A80 = Output["A80off"];
	circuitBrakersOff.A81 = Output["A81off"];
	circuitBrakersOff.AB4 = Output["AB4off"];
	circuitBrakersOff.AB5 = Output["AB5off"];
	circuitBrakersOff.AB1 = Output["AB1off"];
	circuitBrakersOff.AB3 = Output["AB3off"];
	circuitBrakersOff.AB6 = Output["AB6off"];
	circuitBrakersOff.AC1 = Output["AC1off"];
	circuitBrakersOff.AP63 = Output["AP63off"];
	circuitBrakersOff.VU = Output["VUoff"];

	LeaveCriticalSection(&m_SignalCriticalSection);
}

M717::CircuitBrakersOn::CircuitBrakersOn()
	: A1(notSignal)
	, A2(notSignal)
	, A3(notSignal)
	, A4(notSignal)
	, A5(notSignal)
	, A6(notSignal)
	, A7(notSignal)
	, A8(notSignal)
	, A9(notSignal)
	, A10(notSignal)
	, A11(notSignal)
	, A12(notSignal)
	, A13(notSignal)
	, A14(notSignal)
	, A15(notSignal)
	, A16(notSignal)
	, A17(notSignal)
	, A18(notSignal)
	, A19(notSignal)
	, A20(notSignal)
	, A21(notSignal)
	, A22(notSignal)
	, A24(notSignal)
	, A25(notSignal)
	, A26(notSignal)
	, A27(notSignal)
	, A28(notSignal)
	, A29(notSignal)
	, A30(notSignal)
	, A31(notSignal)
	, A32(notSignal)
	, AIS(notSignal)
	, A37(notSignal)
	, A38(notSignal)
	, A39(notSignal)
	, A40(notSignal)
	, A41(notSignal)
	, A42(notSignal)
	, A43(notSignal)
	, A44(notSignal)
	, A45(notSignal)
	, A46(notSignal)
	, A47(notSignal)
	, A48(notSignal)
	, A49(notSignal)
	, A50(notSignal)
	, A51(notSignal)
	, A52(notSignal)
	, A53(notSignal)
	, A54(notSignal)
	, A56(notSignal)
	, A57(notSignal)
	, A65(notSignal)
	, A66(notSignal)
	, A68(notSignal)
	, A70(notSignal)
	, A71(notSignal)
	, A72(notSignal)
	, A73(notSignal)
	, A74(notSignal)
	, A75(notSignal)
	, A76(notSignal)
	, A77(notSignal)
	, A78(notSignal)
	, A79(notSignal)
	, A80(notSignal)
	, A81(notSignal)
	, AB4(notSignal)
	, AB5(notSignal)
	, AB1(notSignal)
	, AB3(notSignal)
	, AB6(notSignal)
	, AC1(notSignal)
	, AP63(notSignal)
	, A84(notSignal)
{
}

M717::CircuitBrakersOff::CircuitBrakersOff()
	: A1(iNanSignal)
	, A2(iNanSignal)
	, A3(iNanSignal)
	, A4(iNanSignal)
	, A5(iNanSignal)
	, A6(iNanSignal)
	, A7(iNanSignal)
	, A8(iNanSignal)
	, A9(iNanSignal)
	, A10(iNanSignal)
	, A11(iNanSignal)
	, A12(iNanSignal)
	, A13(iNanSignal)
	, A14(iNanSignal)
	, A15(iNanSignal)
	, A16(iNanSignal)
	, A17(iNanSignal)
	, A18(iNanSignal)
	, A19(iNanSignal)
	, A20(iNanSignal)
	, A21(iNanSignal)
	, A22(iNanSignal)
	, A24(iNanSignal)
	, A25(iNanSignal)
	, A26(iNanSignal)
	, A27(iNanSignal)
	, A28(iNanSignal)
	, A29(iNanSignal)
	, A30(iNanSignal)
	, A31(iNanSignal)
	, A32(iNanSignal)
	, A37(iNanSignal)
	, A38(iNanSignal)
	, A39(iNanSignal)
	, A40(iNanSignal)
	, A41(iNanSignal)
	, A42(iNanSignal)
	, A43(iNanSignal)
	, A44(iNanSignal)
	, A45(iNanSignal)
	, A46(iNanSignal)
	, A47(iNanSignal)
	, A48(iNanSignal)
	, A49(iNanSignal)
	, A50(iNanSignal)
	, A51(iNanSignal)
	, A52(iNanSignal)
	, A53(iNanSignal)
	, A54(iNanSignal)
	, A56(iNanSignal)
	, A57(iNanSignal)
	, A65(iNanSignal)
	, A66(iNanSignal)
	, A68(iNanSignal)
	, A70(iNanSignal)
	, A71(iNanSignal)
	, A72(iNanSignal)
	, A73(iNanSignal)
	, A74(iNanSignal)
	, A75(iNanSignal)
	, A76(iNanSignal)
	, A77(iNanSignal)
	, A78(iNanSignal)
	, A79(iNanSignal)
	, A80(iNanSignal)
	, A81(iNanSignal)
	, AB4(iNanSignal)
	, AB5(iNanSignal)
	, AB1(iNanSignal)
	, AB3(iNanSignal)
	, AB6(iNanSignal)
	, AC1(iNanSignal)
	, AP63(iNanSignal)
	, AIS(iNanSignal)
	, VU(iNanSignal)
{
}

M717::InHardwareSignals::InHardwareSignals()
	: asotpButton1(notSignal)
	, asotpButton2(notSignal)
	, asotpButton3(notSignal)
	, asotpButton4(notSignal)
	, automaticBrakeCutOffValve(notSignal)
	, automaticBrakeDisablerSwitch(notSignal)
	, ARS13V(notSignal)
	, BVoff(notSignal)
	, BVreturn(notSignal)
	, cabinVentillationSwitch(notSignal)
	, circuitSplitterSwitch(notSignal)
	, controlES(notSignal)
	, controlSpeaker(notSignal)
	, decoderSwitch(notSignal)
	, doorCloseSwitch2(notSignal)
	, doorsChooseSwitch(notSignal)
	, emergencyDoorSwitch(notSignal)
	, emergencyTractionButton(notSignal)
	, emergencyTractionSwitch(notSignal)
	, emergencyVentillationSwitch(notSignal)
	, endDoorsSwitch(notSignal)
	, epvArsCutOffValve(notSignal)
	, faultsSignallingButton(notSignal)
	, first1Program(notSignal)
	, first2Program(notSignal)
	, headlights(notSignal)
	, informerButtonDown(notSignal)
	, informerButtonMenu(notSignal)
	, informerButtonUp(notSignal)
	, inverterProtectionSwitch(notSignal)
	, K3(notSignal)
	, KALS(notSignal)
	, KARS(notSignal)
	, KARSR(notSignal)
	, KOLD1(notSignal)
	, KOLD2(notSignal)
	, KORD(notSignal)
	, KRMK(notSignal)
	, KRP(notSignal)
	, KRU(notSignal)
	, KRZD(notSignal)
	, leftDoorOpenSwitch2(notSignal)
	, lightingCabin(notSignal)
	, lightingPanel(notSignal)
	, lightingSalon(notSignal)
	, mainController(notSignal)
	, manometerCutOffValve(notSignal)
	, pantographSwitch(notSignal)
	, parkingBrakeControlSwitch(notSignal)
	, parkingBrakeCutOffValve(notSignal)
	, pneumSignalCutOffValve(notSignal)
	, pneumSignalPedal(notSignal)
	, powerSupply(notSignal)
	, protectionBattery(notSignal)
	, protectionControlCircuits(notSignal)
	, protectionInverter(notSignal)
	, protectionLighting1(notSignal)
	, protectionLighting2(notSignal)
	, protectionMasterSwitch(notSignal)
	, radioInformer(notSignal)
	, RBmain(notSignal)
	, RBres(notSignal)
	, reverseController(notSignal)
	, safetyPedal(notSignal)
	, safetyStair(notSignal)
	, second1Program(notSignal)
	, second2Program(notSignal)
	, stopCrane(notSignal)
	, KM013(notSignal)
	, UAVAButton(notSignal)
	, UNCHSwitch(notSignal)
	, UOS(notSignal)
	, valveOne(notSignal)
	, VAVU(notSignal)
	, VB(notSignal)
	, VBD(notSignal)
	, VMK(notSignal)
	, VPR(notSignal)
	, VPSwitch(notSignal)
	, VUSSwitch(notSignal)
	, VZD(notSignal)
{
}

M717::OutHardwareSignals::OutHardwareSignals()
	: arrowAmmeter(iNanSignal)
	, arrowBatteryVoltage(iNanSignal)
	, arrowBrakeCylinderPressure(iNanSignal)
	, arrowBrakePipePressure(iNanSignal)
	, arrowContactVoltage(iNanSignal)
	, arrowFeedPipePressure(iNanSignal)
	, LAB(iNanSignal)
	, lampAsotp4(iNanSignal)
	, lampAsotp3(iNanSignal)
	, lampAsotp2(iNanSignal)
	, lampAsotp1(iNanSignal)
	, lampAsotpFault(iNanSignal)
	, lampAsotpFire(iNanSignal)
	, lampAvu(iNanSignal)
	, lampBv(iNanSignal)
	, lampHeaterControl(iNanSignal)
	, lampInformerLeft(iNanSignal)
	, lampInformerMiddle(iNanSignal)
	, lampInformerRight(iNanSignal)
	, lampInverterProtection(iNanSignal)
	, lampIST(iNanSignal)
	, lampLeftDoors1(iNanSignal)
	, lampLeftDoors2(iNanSignal)
	, lampPneumBrakes(iNanSignal)
	, lampRightDoors(iNanSignal)
	, LAV(iNanSignal)
	, LAV1(iNanSignal)
	, LKVP(iNanSignal)
	, UAVAEmergencyBrakeValve(iNanSignal)
	, UAVAFeedValve(iNanSignal)
	, UAVAServiceBrakeValve(iNanSignal)
	, textAsotp("")
	, textInformer("")
	, displayInformer(iNanSignal)
	, displayAsotp(iNanSignal)
	, vfact(iNanSignal)
	, L0CH(iNanSignal)
	, L00(iNanSignal)
	, L40(iNanSignal)
	, L60(iNanSignal)
	, L70(iNanSignal)
	, L80(iNanSignal)
	, LSD(iNanSignal)
	, LHRK(iNanSignal)
	, LRP(iNanSignal)
	, LSN(iNanSignal)
	, LEKK(iNanSignal)
	, LKVC(iNanSignal)
	, LN(iNanSignal)
	, LRS(iNanSignal)
	, LKVD(iNanSignal)
	, LVD(iNanSignal)
	, LKT(iNanSignal)
	, LST(iNanSignal)
{
}

M717::ConfigValues::ConfigValues()
	: Adc1c()
	, Adc2c()
	, Adc3c()
	, Step1c()
	, Step2c()
	, Step3c()
	, Step4c()
	, Step5c()
	, Step6c()
{
}