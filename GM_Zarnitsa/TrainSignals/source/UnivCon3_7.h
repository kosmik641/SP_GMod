// Class for Universal CAN controller v3.7
// 81-765, 81-717 (Tashkent)
#pragma once
#include "params.h"
#include "UnivCon.h"

class CUnivCon3_7 : public CUnivCon
{
public:
    virtual ErrorCode Setup() override;
    virtual void Stop() override;
    
    virtual void ReadSignals(Signals& signals) override;
    virtual void WriteSignals(Signals& signals) override;
    virtual void WriteUARTData(Signals& signals) override;

private:
    virtual ErrorCode OpenCOMPort(int port) override;
    virtual ErrorCode WriteConfiguration(const byte* configData, int size) override;
};

