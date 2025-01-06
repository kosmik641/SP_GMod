#pragma once
#include "utf8_to_cp1251.h"
#include "params.h"
#include <windows.h>
#include <memory>
#include <string>
#include <thread>
#include <map>

#define minmax(_val,_min,_max) max(_min, min(_val, _max))

class NW2VarTable
{
public:
	struct ControlItem {
		int val = 0;
		int type = 1;
	};

	NW2VarTable() = default;
	~NW2VarTable() = default;

	void SetPackedRatio(const std::string& idx,float val) {
		VarTable[idx].val = int(val * 1000.0f);
	};
	float GetPackedRatio(const std::string& idx) {
		return VarTable[idx].val * 0.001f;
	};

	std::map<std::string, ControlItem> VarTable;
};

class UARTFrontView717
{
public:
	UARTFrontView717();
	~UARTFrontView717();

	int Start(int port);
	void Stop(bool force = false);

	void LoadSleepTimings();
	void LoadCalibartions();

	bool IsConnected();
	int GetPortNumber();

	CRITICAL_SECTION* GetCriticalSection();

	NW2VarTable m_NW2VarTableInput;
	NW2VarTable m_NW2VarTableOutput;
	std::string m_ASOTPText;
	std::string m_ASNPText;
private:
	enum ConfigState {
		NotUsed = 0,
		Input,
		Output,
		InputADC
	};
	int OpenCOMPort(int port);
	void SetupArrays();
	void DeviceThreadFunc();
	int SetupDevice();
	void ReadSignalsDevice();
	void WriteSignalsDevice();
	void WriteUARTDevice();
	void WriteShutdownDevice();
	void DataExchangeInputs();
	void DataExchangeOutputs();

	void DestroyHandle();

	bool CreateCalibrationsFile();

	bool ADCStopcrane(int adc);
	int ADCKM013(int adc);

	int StepTC(float value);
	int StepNM(float value);
	int StepTM(float value);

	int StepKiloVoltmeter(float value);
	int StepAmmeter(float value);
	int StepBattVoltmeter(float value);

	void ReadSleepTimes();

	void ReadStopcraneCalibrations();
	void ReadKM013Calibrations();

	void ReadTCCalibrations();
	void ReadNMCalibrations();
	void ReadTMCalibrations();

	void ReadKiloVoltmeterCalibrations();
	void ReadAmmeterCalibrations();
	void ReadBattVoltmerCalibrations();

	static byte ConvertIntTo7DecSegByte(int number);

	HANDLE m_hPort = INVALID_HANDLE_VALUE;
	CRITICAL_SECTION m_CriticalSection{};
	std::thread m_DeviceThread{};
	int m_PortNumber = -1;
	bool m_Connected = false;
	bool m_ThreadRunning = false;
	bool m_ThreadStop = true;
	bool m_ThreadForceStop = false;

	struct SleepTimes
	{
		DWORD afterRead = 5;
		DWORD afterWriteSignals = 25;
		DWORD afterWriteUART = 30;
		DWORD afterAll = 30;
	} m_sleepTimes;

	struct SevenDecSignals
	{
		int port[3]{};
	};

	struct TextDisplaySignals
	{
		int on = 0;    // Включен экран
		int ledOn = 0; // Включена подсветка.
		char text[50]{};
	};

	struct Configuration {
		int nControllers = 0; // Количество контроллеров
		
		std::unique_ptr<int[]> arrPins = nullptr; // Массив конфигурации пинов
		std::unique_ptr<int[]> arrArrows = nullptr; // Количество стрелочных приборов на контроллер
		std::unique_ptr<SevenDecSignals[]> arr7SegDec = nullptr; // Массив конфигурации 7-ми сегментного индикатора скорости для метро (через дешифратор К514ИД2)
		std::unique_ptr<int[]> arrTextDisplaySize = nullptr; // Массив конфигурации текстовых дисплеев.

		std::unique_ptr<int[]> arrADCPerController = nullptr;
	} m_Config;

	struct Data {
		int nInputBytes = 0;
		int nOutputBytes = 0;
		int nUARTBytes = 0;

		std::unique_ptr<byte[]> arrInputBytes = nullptr; // Массив для приема данных
		std::unique_ptr<byte[]> arrOutputBytes = nullptr; // Массив для отправки данных
		std::unique_ptr<byte[]> arrUARTBytes = nullptr; // Массив для отправки UART данных
	} m_Data;

	struct Signals {
		int nPins = 0; // Размер массива конфигурации пинов
		int nADC = 0; // Количество АЦП
		int nArrows = 0; // Количество стрелочных сигналов
		int n7SegDec = 0; // Размер массива сигналов 7SegDec
		int nTextDisplays = 0; // Количество текстовых дисплеев

		std::unique_ptr<int[]> arrInput = nullptr;
		std::unique_ptr<int[]> arrOutput = nullptr;
		std::unique_ptr<int[]> arrADC = nullptr;
		std::unique_ptr<int[]> arrArrow = nullptr;
		std::unique_ptr<int[]> arr7SegDec = nullptr;
		std::unique_ptr<TextDisplaySignals[]> arrTextDisplay = nullptr;
	} m_Signals;

	struct StopcraneCalibrate
	{
		int m_Off = 0;
		int m_On = 1;
	} m_StopcraneCalib;

	struct KM013Calibrate
	{
		unsigned int m_Pos1 = 0;
		unsigned int m_Pos2 = 1;
		unsigned int m_Pos3 = 2;
		unsigned int m_Pos4 = 3;
		unsigned int m_Pos5 = 4;
		unsigned int m_Pos6 = 5;
		unsigned int m_Pos7 = 6;
	} m_KM013Calib;

	struct ArrowCalibrate
	{
		unsigned int m_Min = 0;
		unsigned int m_Max = 1;
	} m_TCCalib, m_NMCalib, m_TMCalib,
		m_KiloVoltmeterCalib, m_AmmeterCalib,
		m_BattVoltmeterCalib;
};

