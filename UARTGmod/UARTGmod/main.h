#pragma once
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include "process.h"
#include "Interface.h"

#define PushCLua( _function, _name ) LUA->PushCFunction(_function); LUA->SetField(-2, _name);

namespace Glob {
	extern std::map<std::string, std::map<std::string, double>> mainContainer;
	extern byte ASNPText[50];
	extern byte ASOTPText[50];

	int UTF8_2_1251(const char* utf8, char* windows1251, size_t n);
	struct Letter {
		char win1251;
		int unicode;
	};
	static Letter g_letters[] = {
		{0x82, 0x201A}, // SINGLE LOW-9 QUOTATION MARK
		{0x83, 0x0453}, // CYRILLIC SMALL LETTER GJE
		{0x84, 0x201E}, // DOUBLE LOW-9 QUOTATION MARK
		{0x85, 0x2026}, // HORIZONTAL ELLIPSIS
		{0x86, 0x2020}, // DAGGER
		{0x87, 0x2021}, // DOUBLE DAGGER
		{0x88, 0x20AC}, // EURO SIGN
		{0x89, 0x2030}, // PER MILLE SIGN
		{0x8A, 0x0409}, // CYRILLIC CAPITAL LETTER LJE
		{0x8B, 0x2039}, // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
		{0x8C, 0x040A}, // CYRILLIC CAPITAL LETTER NJE
		{0x8D, 0x040C}, // CYRILLIC CAPITAL LETTER KJE
		{0x8E, 0x040B}, // CYRILLIC CAPITAL LETTER TSHE
		{0x8F, 0x040F}, // CYRILLIC CAPITAL LETTER DZHE
		{0x90, 0x0452}, // CYRILLIC SMALL LETTER DJE
		{0x91, 0x2018}, // LEFT SINGLE QUOTATION MARK
		{0x92, 0x2019}, // RIGHT SINGLE QUOTATION MARK
		{0x93, 0x201C}, // LEFT DOUBLE QUOTATION MARK
		{0x94, 0x201D}, // RIGHT DOUBLE QUOTATION MARK
		{0x95, 0x2022}, // BULLET
		{0x96, 0x2013}, // EN DASH
		{0x97, 0x2014}, // EM DASH
		{0x99, 0x2122}, // TRADE MARK SIGN
		{0x9A, 0x0459}, // CYRILLIC SMALL LETTER LJE
		{0x9B, 0x203A}, // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
		{0x9C, 0x045A}, // CYRILLIC SMALL LETTER NJE
		{0x9D, 0x045C}, // CYRILLIC SMALL LETTER KJE
		{0x9E, 0x045B}, // CYRILLIC SMALL LETTER TSHE
		{0x9F, 0x045F}, // CYRILLIC SMALL LETTER DZHE
		{0xA0, 0x00A0}, // NO-BREAK SPACE
		{0xA1, 0x040E}, // CYRILLIC CAPITAL LETTER SHORT U
		{0xA2, 0x045E}, // CYRILLIC SMALL LETTER SHORT U
		{0xA3, 0x0408}, // CYRILLIC CAPITAL LETTER JE
		{0xA4, 0x00A4}, // CURRENCY SIGN
		{0xA5, 0x0490}, // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
		{0xA6, 0x00A6}, // BROKEN BAR
		{0xA7, 0x00A7}, // SECTION SIGN
		{0xA8, 0x0401}, // CYRILLIC CAPITAL LETTER IO
		{0xA9, 0x00A9}, // COPYRIGHT SIGN
		{0xAA, 0x0404}, // CYRILLIC CAPITAL LETTER UKRAINIAN IE
		{0xAB, 0x00AB}, // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
		{0xAC, 0x00AC}, // NOT SIGN
		{0xAD, 0x00AD}, // SOFT HYPHEN
		{0xAE, 0x00AE}, // REGISTERED SIGN
		{0xAF, 0x0407}, // CYRILLIC CAPITAL LETTER YI
		{0xB0, 0x00B0}, // DEGREE SIGN
		{0xB1, 0x00B1}, // PLUS-MINUS SIGN
		{0xB2, 0x0406}, // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
		{0xB3, 0x0456}, // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
		{0xB4, 0x0491}, // CYRILLIC SMALL LETTER GHE WITH UPTURN
		{0xB5, 0x00B5}, // MICRO SIGN
		{0xB6, 0x00B6}, // PILCROW SIGN
		{0xB7, 0x00B7}, // MIDDLE DOT
		{0xB8, 0x0451}, // CYRILLIC SMALL LETTER IO
		{0xB9, 0x2116}, // NUMERO SIGN
		{0xBA, 0x0454}, // CYRILLIC SMALL LETTER UKRAINIAN IE
		{0xBB, 0x00BB}, // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
		{0xBC, 0x0458}, // CYRILLIC SMALL LETTER JE
		{0xBD, 0x0405}, // CYRILLIC CAPITAL LETTER DZE
		{0xBE, 0x0455}, // CYRILLIC SMALL LETTER DZE
		{0xBF, 0x0457} // CYRILLIC SMALL LETTER YI
	};
	
	namespace UARTFrontViewClient {
		struct IndicatorSignals
		{
			int map[3][2];
		};
		struct textDisplaySignals
		{
			int DisplayOn;
			int DisplayLedOn;
			byte textBytes[50];
		};

		int COM_Open(int a_portNumber, int a_portRate);
		void COM_Close();
		void setThreadEnded(bool a_threadEnded);
		bool threadWorking();
		void __cdecl ThreadFunction(void* pParams);
		void updateUART();
		bool ConfigSignals();
		void shutdown();
		byte Convert_Int_Number_To_8_Output_Signals(int Number);
		bool Convert_Float_Number_To_Byte_Digits(float Number, int Number_Of_Digits, int Number_Of_Digits_After_Point_User, byte* Digits);

		extern HANDLE hSerial;
		extern CRITICAL_SECTION m_SignalCriticalSection;
		extern bool m_isInitFinish, m_isThreadWorking, m_isTthreadEnded;

		extern int* ADC_Signals;
		extern int* Input_Signals;
		extern int* Output_Signals;
		extern int* Arrow_Signals;
		extern float* Indicator_Signals;
		extern int* SAUT_Signals;

		extern int Number_Of_Controllers, Nmb_Of_ADC_Signals, Nmb_Of_Arrow_Signals, Nmb_Of_Indicator_Signals, Nmb_Of_SAUT_Signals, Nmb_Of_7SegDec_Signals, Nmb_Of_Text_Displays;
		extern std::vector<int> Config_Arrow_Signals, Config_SAUT_Signals, Config_Text_Signals;
		extern std::vector<IndicatorSignals> Config_Indicator_Signals;
		extern int Config_7SegDec_Signals[20][3];
		extern int Config_Signals_Length;
		extern int* Config_Signals;
		extern int* _7SegDec_Signals;
		extern textDisplaySignals* TextDisplaysSignalsStruct;

		extern int Nmb_Of_Input_Bytes, Nmb_Of_Output_Bytes, Nmb_Of_Arrow_Bytes, Nmb_Of_SAUT_Bytes, Nmb_Of_Indicator_Bytes, Nmb_Of_Text_Bytes;
		extern std::vector<int> Nmb_Of_ADC_Signals_Per_Contr, Nmb_Of_Arrow_Bytes_Per_Contr, Nmb_Of_Indicator_Bytes_Per_Contr, Nmb_Of_SAUT_Bytes_Per_Contr;
		extern std::vector<int> Nmb_Of_UART_Bytes_Per_Contr, Nmb_Of_Text_Bytes_Per_Controller;
		extern int Nmb_Of_Input_Signals, Nmb_Of_Output_Signals;
		extern int Nmb_Of_Config_Bytes, Nmb_Of_UART_Bytes;

		extern byte* Config_Bytes;
		extern byte* ACK_Bytes;
		extern byte* Input_Bytes;
		extern byte* Output_Bytes;
		extern byte* UART_Bytes;
		namespace Metro81717Signals {
			struct CircuitBrakersOn {
				CircuitBrakersOn();

				int A1;
				int A2;
				int A3;
				int A4;
				int A5;
				int A6;
				int A7;
				int A8;
				int A9;
				int A10;
				int A11;
				int A12;
				int A13;
				int A14;
				int A15;
				int A16;
				int A17;
				int A18;
				int A19;
				int A20;
				int A21;
				int A22;
				int A24;
				int A25;
				int A26;
				int A27;
				int A28;
				int A29;
				int A30;
				int A31;
				int A32;
				int AIS;
				int A37;
				int A38;
				int A39;
				int A40;
				int A41;
				int A42;
				int A43;
				int A44;
				int A45;
				int A46;
				int A47;
				int A48;
				int A49;
				int A50;
				int A51;
				int A52;
				int A53;
				int A54;
				int A56;
				int A57;
				int A65;
				int A66;
				int A68;
				int A70;
				int A71;
				int A72;
				int A73;
				int A74;
				int A75;
				int A76;
				int A77;
				int A78;
				int A79;
				int A80;
				int A81;
				int AB4;
				int AB5;
				int A84;
				int AB1;
				int AB3;
				int AB6;
				int AC1;
				int AP63;
			};
			struct CircuitBrakersOff {
				CircuitBrakersOff();

				int A1;
				int A2;
				int A3;
				int A4;
				int A5;
				int A6;
				int A7;
				int A8;
				int A9;
				int A10;
				int A11;
				int A12;
				int A13;
				int A14;
				int A15;
				int A16;
				int A17;
				int A18;
				int A19;
				int A20;
				int A21;
				int A22;
				int A24;
				int A25;
				int A26;
				int A27;
				int A28;
				int A29;
				int A30;
				int A31;
				int A32;
				int AIS;
				int A37;
				int A38;
				int A39;
				int A40;
				int A41;
				int A42;
				int A43;
				int A44;
				int A45;
				int A46;
				int A47;
				int A48;
				int A49;
				int A50;
				int A51;
				int A52;
				int A53;
				int A54;
				int A56;
				int A57;
				int A65;
				int A66;
				int A68;
				int A70;
				int A71;
				int A72;
				int A73;
				int A74;
				int A75;
				int A76;
				int A77;
				int A78;
				int A79;
				int A80;
				int A81;
				int AB4;
				int AB5;
				int AB1;
				int AB3;
				int AB6;
				int AC1;
				int AP63;
				int VU;
			};
			struct InHardwareSignals {
				InHardwareSignals();

				int asotpButton1;
				int asotpButton2;
				int asotpButton3;
				int asotpButton4;
				int automaticBrakeCutOffValve;
				int automaticBrakeDisablerSwitch;
				int ARS13V;
				int BVoff;
				int BVreturn;
				int cabinVentillationSwitch;
				int circuitSplitterSwitch;
				int controlES;
				int controlSpeaker;
				int decoderSwitch;
				int doorCloseSwitch2;
				int doorsChooseSwitch;
				int emergencyDoorSwitch;
				int emergencyTractionButton;
				int emergencyTractionSwitch;
				int emergencyVentillationSwitch;
				int endDoorsSwitch;
				int epvArsCutOffValve;
				int faultsSignallingButton;
				int first1Program;
				int first2Program;
				int headlights;
				int informerButtonDown;
				int informerButtonMenu;
				int informerButtonUp;
				int inverterProtectionSwitch;
				int K3;
				int KALS;
				int KARS;
				int KARSR;
				int KOLD1;
				int KOLD2;
				int KORD;
				int KRMK;
				int KRP;
				int KRU;
				int KRZD;
				int leftDoorOpenSwitch2;
				int lightingCabin;
				int lightingPanel;
				int lightingSalon;
				int mainController;
				int manometerCutOffValve;
				int pantographSwitch;
				int parkingBrakeControlSwitch;
				int parkingBrakeCutOffValve;
				int pneumSignalCutOffValve;
				int pneumSignalPedal;
				int powerSupply;
				int protectionBattery;
				int protectionControlCircuits;
				int protectionInverter;
				int protectionLighting1;
				int protectionLighting2;
				int protectionMasterSwitch;
				int radioInformer;
				int RBmain;
				int RBres;
				int reverseController;
				int safetyPedal;
				int safetyStair;
				int second1Program;
				int second2Program;
				int stopCrane;
				int tap313;
				int UAVAButton;
				int UNCHSwitch;
				int UOS;
				int valveOne;
				int VAVU;
				int VB;
				int VBD;
				int VMK;
				int VPR;
				int VPSwitch;
				int VUSSwitch;
				int VZD;
			};
			struct OutHardwareSignals {
				OutHardwareSignals();

				int arrowAmmeter;
				int arrowBatteryVoltage;
				int arrowBrakeCylinderPressure;
				int arrowBrakePipePressure;
				int arrowContactVoltage;
				int arrowFeedPipePressure;
				int LAB;
				int lampAsotp4;
				int lampAsotp3;
				int lampAsotp2;
				int lampAsotp1;
				int lampAsotpFault;
				int lampAsotpFire;
				int lampAvu;
				int lampBv;
				int lampHeaterControl;
				int lampInformerLeft;
				int lampInformerMiddle;
				int lampInformerRight;
				int lampInverterProtection;
				int lampIST;
				int lampLeftDoors1;
				int lampLeftDoors2;
				int lampPneumBrakes;
				int lampRightDoors;
				int LAV;
				int LAV1;
				int LKVP;
				int UAVAEmergencyBrakeValve;
				int UAVAFeedValve;
				int UAVAServiceBrakeValve;
				std::string textAsotp;
				std::string textInformer;
				int displayInformer;
				int displayAsotp;
				int vfact;
				int L0CH;
				int L00;
				int L40;
				int L60;
				int L70;
				int L80;
				int LSD;
				int LHRK;
				int LRP;
				int LSN;
				int LEKK;
				int LKVC;
				int LN;
				int LRS;
				int LKVD;
				int LVD;
				int LKT;
				int LST;
			};

			int startSignals(int a_port);
			void InitConfArrays();
			void update();
			void stopSignals();

			void inSignals();
			void outSignals();

			auto Adc1(int adc) -> int const; // Стоп-кран
			auto Adc2(int adc) -> int const; // Кран машиниста
			auto Adc3(int adc) -> int const; // Контроллер машиниста

			auto Step1(double step) -> int const; // Тормозной цилиндр
			auto Step2(double step) -> int const; // Напорная магистраль
			auto Step3(double step) -> int const; // Тормозная магистраль
			auto Step4(double step) -> int const; // Амперметр
			auto Step5(double step) -> int const; // Киловольтметр
			auto Step6(double step) -> int const; // Акк. батарея

			extern CircuitBrakersOn circuitBrakersOn;
			extern CircuitBrakersOff circuitBrakersOff;
			extern InHardwareSignals inHardwareSignals;
			extern OutHardwareSignals outHardwareSignals;
		}
		
	}
}
