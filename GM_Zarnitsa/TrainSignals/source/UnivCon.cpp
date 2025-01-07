// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "UnivCon.h"

CUnivCon::ErrorCode CUnivCon::Start(int port)
{
	PRINT_FUNCSIG;

	ErrorCode openError = OpenCOMPort(port);
	if (openError)
		return openError;

	m_PortNumber = port;
	return E_SUCCESS;
}

bool CUnivCon::IsConnected()
{
    return m_Connected;
}

int CUnivCon::GetPortNumber()
{
	PRINT_FUNCSIG;

    return m_PortNumber;
}

void CUnivCon::DestroyHandle()
{
	PRINT_FUNCSIG;

	PRINT_MSG_DBG("Destroy handle\n");

	if (m_hPort != INVALID_HANDLE_VALUE)
		PurgeComm(m_hPort, PURGE_TXCLEAR | PURGE_RXCLEAR);

	CloseHandle(m_hPort);
	m_PortNumber = -1;
	m_Connected = false;
	m_hPort = INVALID_HANDLE_VALUE;
}

static byte byte7DecSeg[] = { 0x00,0x08,0x01,0x09,0x02,0x0A,0x03,0x0B,0x04,0x0C };
byte CUnivCon::ConvertIntTo7DecSegByte(int number)
{
	if (number == -1)
		return 0xFF;

	number = number % 100;

	int number1 = number % 10;
	int number10 = (number - number1) / 10;

	return (byte7DecSeg[number10] | (byte7DecSeg[number1] << 4));
}
