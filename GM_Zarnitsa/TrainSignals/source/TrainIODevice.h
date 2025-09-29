#pragma once
#include "utf8_to_cp1251.h"
#include "params.h"
#include "UnivCon3_2.h"
#include "UnivCon3_7.h"
#include <Windows.h>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <regex>

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

	std::unordered_map<std::string, ControlItem> VarTable;
};

class TrainIODevice
{
public:
	TrainIODevice();
	~TrainIODevice();

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
	void LoadConfig();
	void DeviceThreadFunc();
	void DataExchangeInputs();
	void DataExchangeOutputs();

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

	std::unique_ptr<CUnivCon> m_UnivConv;
	CUnivCon::Signals m_Signals;

	CRITICAL_SECTION m_CriticalSection{};
	std::thread m_DeviceThread{};
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

	static CUnivCon::ConfigState ParseConfig(const char* key,const char* value);
};

