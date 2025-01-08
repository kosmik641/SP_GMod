#include "UnivCon3_7.h"

CUnivCon::ErrorCode CUnivCon3_7::Setup()
{
	PRINT_FUNCSIG;

	if (m_Config.nControllers == 0)
	{
		return E_BADCONFIG;
	}

	// Размер данных
	int nInputBytes = 3 * m_Config.nControllers; // Данные из контроллеров
	int nOutputBytes = 3 * m_Config.nControllers; // Данные в контроллер
	int nUARTBytes = 0; // Данные для UART устройств

	// Данные конфигурации
	int nConfigBytes = m_Config.nControllers * 7 + 5;
	std::unique_ptr<byte[]> arrConfigBytes = std::make_unique<byte[]>(nConfigBytes);

	arrConfigBytes[0] = 0x73;
	arrConfigBytes[1] = 0x02;
	arrConfigBytes[2] = 0xE4;
	arrConfigBytes[3] = m_Config.nControllers;
	arrConfigBytes[nConfigBytes - 1] = 0x65;

	int iByte = 0;
	for (int iController = 0; iController < m_Config.nControllers; iController++)
	{
		// Конфигурация пинов
		for (int p = 0; p < 24; p++)
		{
			int iPin = iController * 24 + p;
			iByte = iPin / 4 + 4;

			arrConfigBytes[iByte + iController] |= (byte)((m_Config.arrPins[iPin] << 6) >> ((iPin % 4) * 2));

			if (m_Config.arrPins[iPin] == InputADC)
				nInputBytes += 2;
		}

		// Количество данных UART для каждого контроллера
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

	m_Data.arrInputBytes.reset(new byte[nInputBytes + 4]());
	m_Data.arrOutputBytes.reset(new byte[nOutputBytes + 4]());
	m_Data.arrUARTBytes.reset(new byte[nUARTBytes + 4]());

	PRINT_MSG_DBG("m_Data.arrInputBytes:  size = %d\n", nInputBytes + 4);
	PRINT_MSG_DBG("m_Data.arrOutputBytes: size = %d\n", nOutputBytes + 4);
	PRINT_MSG_DBG("m_Data.arrUARTBytes:   size = %d\n", nUARTBytes + 4);

	PRINT_MSG_DBG("Send configuration...\n");
	int ttl = 20;
	ErrorCode confError;
	while (--ttl)
	{
		Sleep(200);
		PRINT_MSG_DBG("ttl = %d\n", ttl);
		confError = WriteConfiguration(arrConfigBytes.get(), nConfigBytes);
		if (confError == E_SUCCESS)
		{
			m_Connected = true;
			return E_SUCCESS;
		}
	}

	// Если сконфигурировать не удалось
	m_Data.arrInputBytes.release();
	m_Data.arrOutputBytes.release();
	m_Data.arrUARTBytes.release();
	DestroyHandle();

	return confError;
}

void CUnivCon3_7::Stop()
{
	PRINT_FUNCSIG;

	PRINT_MSG_DBG("Send shutdown data...\n");

	// Отправка UART
	if (m_Data.nUARTBytes > 0)
	{
		m_Data.arrUARTBytes[0] = 0x73;
		m_Data.arrUARTBytes[1] = 0x05;
		m_Data.arrUARTBytes[2] = (byte)m_Data.nUARTBytes;
		m_Data.arrUARTBytes[m_Data.nUARTBytes + 4 - 1] = 0x65;

		int iUARTByte = 3;
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
			WriteFile(m_hPort, m_Data.arrUARTBytes.get(), (DWORD)(m_Data.nUARTBytes + 4), nullptr, nullptr);
			Sleep(100);
		}
	}

	// Отправка сигналов
	std::unique_ptr<byte[]> disableBytes = std::make_unique<byte[]>(m_Data.nOutputBytes + 4);
	disableBytes[0] = 0x73;
	disableBytes[1] = 0x04;
	disableBytes[2] = m_Data.nOutputBytes;
	disableBytes[m_Data.nOutputBytes + 4 - 1] = 0x65;

	Sleep(100);
	WriteFile(m_hPort, disableBytes.get(), m_Data.nOutputBytes + 4, nullptr, nullptr);

	DestroyHandle();
}

void CUnivCon3_7::ReadSignals(Signals& signals)
{
	static byte cmdRead[]{ 0x73, 0x13, 0x01, 0x65 };
	WriteFile(m_hPort, &cmdRead, sizeof(cmdRead), nullptr, nullptr);

	auto inBytes = m_Data.arrInputBytes.get();
	ReadFile(m_hPort, inBytes, m_Data.nInputBytes + 4, nullptr, nullptr);
	PurgeComm(m_hPort, PURGE_RXCLEAR);

	if ((inBytes[0] != 0x73) || (inBytes[1] != 0x03) /* || (inBytes[2] != m_Data.nInputBytes) */ )
		return;

	int i_Byte = 3;
	int i_ADCSignals = 0;

	for (int i_Controller = 0; i_Controller < m_Config.nControllers; i_Controller++)
	{
		if (m_Config.arrADCPerController[i_Controller] > 0)
		{
			for (int i_ADC = 0; i_ADC < m_Config.arrADCPerController[i_Controller]; i_ADC++)
			{
				signals.arrADC[i_ADCSignals] = inBytes[i_Byte++] << 8;
				signals.arrADC[i_ADCSignals++] |= inBytes[i_Byte++];
			}
		}

		for (int i_Pin = 0; i_Pin < 24; i_Pin++)
		{
			int i_InputSignal = i_Controller * 24 + i_Pin;
			signals.arrInput[i_InputSignal] = (((inBytes[i_Byte] << (i_Pin % 8)) & 0x80) >> 7) == 0;
			if (i_Pin == 7 || i_Pin == 15 || i_Pin == 23)
				i_Byte++;
		}
	}
}

void CUnivCon3_7::WriteSignals(Signals& signals)
{
	auto outBytes = m_Data.arrOutputBytes.get();
	outBytes[0] = 0x73;
	outBytes[1] = 0x04;
	outBytes[2] = (byte)m_Data.nOutputBytes;
	outBytes[m_Data.nOutputBytes + 4 - 1] = 0x65;

	for (int i_OutputSignals = 0; i_OutputSignals < signals.nPins; i_OutputSignals++)
	{
		if ((i_OutputSignals % 8) == 0)
			outBytes[(i_OutputSignals / 8) + 3] = 0;
		outBytes[(i_OutputSignals / 8) + 3] |= (byte)(signals.arrOutput[i_OutputSignals] << 7) >> (i_OutputSignals % 8);
	}

	int i_7SegDecSignals = 0;
	for (int i_Controller = 0; i_Controller < m_Config.nControllers; i_Controller++)
	{
		for (int i_Port = 0; i_Port < 3; i_Port++)
		{
			if (m_Config.arr7SegDec[i_Controller].port[i_Port])
			{
				outBytes[i_Controller * 3 + i_Port + 3] = ConvertIntTo7DecSegByte(signals.arr7SegDec[i_7SegDecSignals++]);
			}
		}
	}

	WriteFile(m_hPort, outBytes, m_Data.nOutputBytes + 4, nullptr, nullptr);
}

void CUnivCon3_7::WriteUARTData(Signals& signals)
{
	if (!m_Data.nUARTBytes)
		return;

	auto uartBytes = m_Data.arrUARTBytes.get();

	uartBytes[0] = 0x73;
	uartBytes[1] = 0x05;
	uartBytes[2] = (byte)m_Data.nUARTBytes;
	uartBytes[m_Data.nUARTBytes + 4 - 1] = 0x65;

	int i_Byte = 3;
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
				uartBytes[i_Byte++] = (byte)((signals.arrArrow[i_Arrow] & 0xFF00) >> 8);
				uartBytes[i_Byte++] = (byte)(signals.arrArrow[i_Arrow++] & 0xFF);
			}
		}

		if (m_Config.arrTextDisplaySize[i_Controller] > 0)
		{
			auto display = &signals.arrTextDisplay[i_TextDisplay++];
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

	WriteFile(m_hPort, uartBytes, m_Data.nUARTBytes + 4, nullptr, nullptr);
}

CUnivCon::ErrorCode CUnivCon3_7::OpenCOMPort(int port)
{
	PRINT_FUNCSIG;

	if (port < 1 || port > 254)
	{
		return E_WRONGPORT;
	}

	char portPath[16]{};
	wsprintf(portPath, "COM%d", port);
	m_hPort = CreateFile(portPath, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);

	if (m_hPort == INVALID_HANDLE_VALUE)
	{
		return E_HANDLEERROR;
	}

	DCB dcb;
	BOOL success;

	success = GetCommState(m_hPort, &dcb);
	if (!success)
	{
		DestroyHandle();
		return E_GETCOMMSTATE;
	}

	dcb.BaudRate = 115200;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	success = SetCommState(m_hPort, &dcb);
	if (!success)
	{
		DestroyHandle();
		return E_SETCOMMSTATE;
	}

	success = SetCommMask(m_hPort, EV_TXEMPTY);
	if (!success)
	{
		DestroyHandle();
		return E_SETCOMMMASK;
	}

	COMMTIMEOUTS timeouts;
	success = GetCommTimeouts(m_hPort, &timeouts);
	if (!success)
	{
		DestroyHandle();
		return E_GETCOMMTIMEOUTS;
	}

	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 30;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1000;
	success = SetCommTimeouts(m_hPort, &timeouts);
	if (!success)
	{
		DestroyHandle();
		return E_SETCOMMTIMEOUTS;
	}

	success = SetupComm(m_hPort, 1000, 1000);
	if (!success)
	{
		DestroyHandle();
		return E_SETUPCOMM;
	}

	static const byte cmdDetect[]{ 0x73, 0x01, 0x79, 0x3F, 0x65 };
	static const byte answerCorrect[5]{ 0x73, 0x07, 0x01, 0x01, 0x65 };
	byte answerBuf[5]{};

	PurgeComm(m_hPort, PURGE_RXCLEAR | PURGE_TXCLEAR);
	Sleep(10);

	PRINT_MSG_DBG("Send mirror command...\n");
	WriteFile(m_hPort, &cmdDetect, sizeof(cmdDetect), nullptr, nullptr);
	ReadFile(m_hPort, &answerBuf, sizeof(answerBuf), nullptr, nullptr);
	PRINT_MSG_DBG("answerBuf = {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", answerBuf[0], answerBuf[1], answerBuf[2], answerBuf[3], answerBuf[4]);
	if (memcmp(answerBuf, answerCorrect, 5) != 0)
	{
		DestroyHandle();
		return E_CONNECTFAIL;
	}

	return E_SUCCESS;
}

CUnivCon::ErrorCode CUnivCon3_7::WriteConfiguration(const byte* configData, int size)
{
	PRINT_FUNCSIG;

	if (configData == nullptr)
		return E_NOCONFIGPTR;

	// Отправляем конфигурацию на контроллеры
	WriteFile(m_hPort, configData, size, nullptr, nullptr);

	COMSTAT comstat;
	DWORD errors;
	comstat.cbInQue = 0;
	ClearCommError(m_hPort, &errors, &comstat);

	DWORD bytesToRead = comstat.cbInQue;
	PRINT_MSG_DBG("\tbytesToRead = %d\n", bytesToRead);
	if (bytesToRead == 5)
	{
		byte ackBuf[5]{};
		static const byte ackCorrect[]{ 0x73, 0x07, 0x01, 0x02, 0x65 };
		ReadFile(m_hPort, &ackBuf, bytesToRead, nullptr, nullptr);

		PRINT_MSG_DBG("ackBuf = {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", ackBuf[0], ackBuf[1], ackBuf[2], ackBuf[3], ackBuf[4]);
		if (memcmp(ackBuf, ackCorrect, 5) == 0)
		{
			PurgeComm(m_hPort, PURGE_RXCLEAR | PURGE_TXCLEAR);
			Sleep(500);
			return E_SUCCESS;
		}
	}

	return E_SETUPFAIL;
}
