if CLIENT then return end
UARTFrontViewClient = UARTFrontViewClient or {}
    
-- Инициализация таблицы (aka UARTFrontViewClient.h)
UARTFrontViewClient = {
    ADC_Signals = {},
    Input_Signals = {},
    Output_Signals = {},
    Arrow_Signals = {},
    Indicator_Signals = {},
    SAUT_Signals = {},
    
    Number_Of_Controllers = 0,Nmb_Of_ADC_Signals = 0,Nmb_Of_Arrow_Signals = 0,Nmb_Of_Indicator_Signals = 0,Nmb_Of_SAUT_Signals = 0,Nmb_Of_7SegDec_Signals = 0,Nmb_Of_Text_Displays = 0,
    
    Config_Arrow_Signals = {},Config_SAUT_Signals = {},Config_Text_Signals = {},
    Config_Indicator_Signals = {},
    Config_7SegDec_Signals = {},
    Config_Signals_Length = 0,
    Config_Signals = {},
    
    _7SegDec_Signals = {},
    TextDisplaysSignalsStruct = {},

    Nmb_Of_Input_Bytes = 0,Nmb_Of_Output_Bytes = 0,Nmb_Of_Arrow_Bytes = 0,Nmb_Of_SAUT_Bytes = 0,Nmb_Of_Indicator_Bytes = 0,Nmb_Of_Text_Bytes = 0,
    Nmb_Of_ADC_Signals_Per_Contr = {},Nmb_Of_Arrow_Bytes_Per_Contr = {},Nmb_Of_Indicator_Bytes_Per_Contr = {},Nmb_Of_SAUT_Bytes_Per_Contr = {},
    Nmb_Of_UART_Bytes_Per_Contr = {},Nmb_Of_Text_Bytes_Per_Controller = {},
    Nmb_Of_Input_Signals = 0,Nmb_Of_Output_Signals = 0,
    Nmb_Of_Config_Bytes = 0, Nmb_Of_UART_Bytes = 0,
    
    Config_Bytes = {},
    ACK_Bytes = {},
    Input_Bytes = {},
    Output_Bytes = {},
    UART_Bytes = {},
    test = 0x0B,
}
include( "xcontainer.lua" )

function UARTFrontViewClient:COM_Open(a_portNumber,a_portRate)
    if not UART then return -10 end
    return UART.StartCOM(a_portNumber)
end

function UARTFrontViewClient:COM_Close()
    UART.StopCOM()
end

local function tobyte(nmb) -- Явное преобразование (byte)
    return math.floor(nmb) % 256
end

local switch_Edinici = {[0] = 0x00,0x80,0x10,0x90,0x20,0xA0,0x30,0xB0,0x40,0xC0}
local switch_Desyatki = {[0] = 0x00,0x08,0x01,0x09,0x02,0x0A,0x03,0x0B,0x04,0x0C}

function UARTFrontViewClient.Convert_Int_Number_To_8_Output_Signals(Number)
    Number = math.floor(Number) % 100
    
    local Edinici = Number % 10
    local Desyatki = math.floor((Number - Edinici) / 10)

    return bit.bor(switch_Desyatki[Desyatki],switch_Edinici[Edinici])
end

function UARTFrontViewClient:Convert_Float_Number_To_Byte_Digits(Number, Number_Of_Digits, Number_Of_Digits_After_Point_User, DigitsTbl)
    Number_Of_Digits = math.floor(Number_Of_Digits); Number_Of_Digits_After_Point_User = math.floor(Number_Of_Digits_After_Point_User) -- Приводим к int
    
    local Nmb_Of_Dig_After_Point = Number_Of_Digits_After_Point_User
    local Max_Int_Designator = math.pow(10, Number_Of_Digits)
    
    -- Если число за пределами возможностей отображения экрана, выходим ничего не сделав
    if (math.floor(Number / Max_Int_Designator) ~= 0) then
        return DigitsTbl
    end
    
    -- Проверяем хватит ли цифр, для обозначения всей дробной части, если нет-уменьшаем ее и снова проверяем
    local success = false
    while (not success) do
        Max_Int_Designator = math.pow(10,math.floor(Number_Of_Digits - Nmb_Of_Dig_After_Point))
        
        if (math.floor(Number / Max_Int_Designator) == 0) then
            success = true
        else
            Nmb_Of_Dig_After_Point = Nmb_Of_Dig_After_Point - 1
        end
    end
    
    for i=0,Number_Of_Digits-1 do
        if (i < Nmb_Of_Dig_After_Point) then
            local var1 = math.floor(math.pow(10,(Nmb_Of_Dig_After_Point - i)))
            local var2 = math.floor(Number * var1 + 0.5)
            DigitsTbl[Number_Of_Digits - i - 1] = tobyte(var2 % 10)
        elseif ((i == Nmb_Of_Dig_After_Point) and (Nmb_Of_Dig_After_Point ~= 0)) then
            DigitsTbl[Number_Of_Digits - i - 1] = tobyte(bit.bor(tobyte(math.floor(Number) % 10),0x80));
        else
            local var1 = math.floor(math.pow(10,(i - Nmb_Of_Dig_After_Point + 1)))
            local numerator
            
            if (i == 0) then
                numerator = (Number + 0.5) % var1
            else
                numerator = math.floor(Number) % var1
            end
            
            local designator = math.floor(math.pow(10, (i - Nmb_Of_Dig_After_Point)))
            
            DigitsTbl[Number_Of_Digits - i - 1] = tobyte(math.floor(numerator / designator))
        end
    end
    
    for i=0,Number_Of_Digits-2 do
        if (DigitsTbl[i] == 0) then
            DigitsTbl[i] = 0x0A
        else
            break
        end
    end
    
    return DigitsTbl
end

local Read_Command = {}
Read_Command[0] = 0x00
Read_Command[1] = 0x85
local command = 0
function UARTFrontViewClient:update()

    if command == 0 then
        UART.WriteByte(Read_Command,2)
        
    -- Reading_Bytes
        self.Input_Bytes = UART.ReadByte(self.Nmb_Of_Input_Bytes + 3)
        if ((self.Input_Bytes[0] == 0x0B) and (self.Input_Bytes[1] == 0x0B) and (self.Input_Bytes[2] == self.Nmb_Of_Input_Bytes)) then  -- Если эти байты удовлетворяют протоколу, работаем с ними
            local Input_Bytes_Index = 3
            local Input_Signals_Index = 0
            local ADC_Signals_Index = 0
            
            for Cntr_Nmb = 0,self.Number_Of_Controllers-1 do
                for ADC_Nmb = 0,self.Nmb_Of_ADC_Signals_Per_Contr[Cntr_Nmb]-1 do
                    self.ADC_Signals[ADC_Signals_Index] = bit.lshift(self.Input_Bytes[Input_Bytes_Index],8); Input_Bytes_Index = Input_Bytes_Index + 1
                    self.ADC_Signals[ADC_Signals_Index] = bit.bor(self.ADC_Signals[ADC_Signals_Index],self.Input_Bytes[Input_Bytes_Index]); ADC_Signals_Index = ADC_Signals_Index + 1; Input_Bytes_Index = Input_Bytes_Index + 1
                end
                
                for Pin_Nmb = 0,23 do
                    self.Input_Signals[Input_Signals_Index] = bit.rshift(bit.band(bit.lshift(self.Input_Bytes[Input_Bytes_Index],(Pin_Nmb % 8)),0x80),7); Input_Signals_Index = Input_Signals_Index + 1
                    if (Pin_Nmb == 7 or Pin_Nmb == 15 or Pin_Nmb == 23) then Input_Bytes_Index = Input_Bytes_Index + 1 end
                end
                
            end
        end
        
    -- Writing_Bytes
        -- Готовим исходящие байты
        self.Output_Bytes[0] = tobyte(self.Nmb_Of_Output_Bytes) -- 42
        self.Output_Bytes[1] = 0x83
        
        for Output_Signals_Index = 0,self.Nmb_Of_Output_Signals-1 do
            local ind = math.floor(Output_Signals_Index / 8) + 2
            if (Output_Signals_Index % 8 == 0) then self.Output_Bytes[ind] = 0 end
            self.Output_Bytes[ind] = bit.bor(self.Output_Bytes[ind],bit.rshift(tobyte(bit.lshift(self.Output_Signals[Output_Signals_Index],7)),(Output_Signals_Index % 8)))
        end
        
        local _7SegDec_Signals_Index = 0
        
        for Cntr_Nmb = 0,self.Number_Of_Controllers-1 do
            for Port_Nmb=0,2 do
                if (self.Config_7SegDec_Signals[Cntr_Nmb][Port_Nmb] == 1) then
                    self.Output_Bytes[Cntr_Nmb * 3 + Port_Nmb + 2] = self.Convert_Int_Number_To_8_Output_Signals(self._7SegDec_Signals[_7SegDec_Signals_Index])
                    _7SegDec_Signals_Index = _7SegDec_Signals_Index + 1
                end
            end
        end
        
        UART.WriteByte(self.Output_Bytes,self.Nmb_Of_Output_Bytes + 2)
        command = 1
    else
    -- Writing_UART_Bytes
        if (self.Nmb_Of_UART_Bytes > 0) then
            -- Готовим байты, выводимые контроллером через UART
            self.UART_Bytes[0] = tobyte(self.Nmb_Of_UART_Bytes)
            self.UART_Bytes[1] = 0x87
            
            local Arrow_Signals_Index = 0
            local Indicator_Signals_Counter = 0
            local SAUT_Signals_Counter = 0
            local Text_Display_Counter = 0
            local UART_Bytes_Counter = 2

            for Cntr_Nmb = 0,self.Number_Of_Controllers-1 do
                if (self.Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Nmb] > 0) then
                    self.UART_Bytes[UART_Bytes_Counter] = 0x41; --[["A" aka ARROWS]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                    for i=0,self.Config_Arrow_Signals[Cntr_Nmb]-1 do
                        self.UART_Bytes[UART_Bytes_Counter] = tobyte(i+1); UART_Bytes_Counter = UART_Bytes_Counter + 1
                        self.UART_Bytes[UART_Bytes_Counter] = tobyte(bit.rshift(bit.band(self.Arrow_Signals[Arrow_Signals_Index],0xFF00),8)); UART_Bytes_Counter = UART_Bytes_Counter + 1
                        self.UART_Bytes[UART_Bytes_Counter] = tobyte(bit.band(self.Arrow_Signals[Arrow_Signals_Index],0x00FF)); UART_Bytes_Counter = UART_Bytes_Counter + 1; Arrow_Signals_Index = Arrow_Signals_Index + 1
                    end
                end
                
                if (self.Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Nmb] > 0) then
                    for Ind_Nmb=0,2 do
                        if (self.Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0] > 0) then
                            self.UART_Bytes[UART_Bytes_Counter] = 0x49; --[["I" aka INDICATORS]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                            self.UART_Bytes[UART_Bytes_Counter] = tobyte(Ind_Nmb+1); UART_Bytes_Counter = UART_Bytes_Counter + 1
                            self.UART_Bytes[UART_Bytes_Counter] = tobyte(self.Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0]); UART_Bytes_Counter = UART_Bytes_Counter + 1
                            
                            local Digits = {}
                            
                            Digits = Convert_Float_Number_To_Byte_Digits(self.Indicator_Signals[Indicator_Signals_Counter], self.Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0], self.Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][1], Digits); Indicator_Signals_Counter = Indicator_Signals_Counter + 1
                            
                            for Indicator_Bytes_Index = 0,self.Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0]-1 do
                                self.UART_Bytes[UART_Bytes_Counter] = Digits[Indicator_Bytes_Index]; UART_Bytes_Counter = UART_Bytes_Counter + 1
                            end
                            
                            self.UART_Bytes[UART_Bytes_Counter] = 0xFF; UART_Bytes_Counter = UART_Bytes_Counter + 1
                        end
                    end
                end
                
                if (self.Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Nmb] > 0) then
                    self.UART_Bytes[UART_Bytes_Counter] = 0x4C; --[["L" aka LEDS]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                    self.UART_Bytes[UART_Bytes_Counter] = tobyte(bit.bor(bit.lshift(self.SAUT_Signals[SAUT_Signals_Counter],2),bit.lshift(self.SAUT_Signals[SAUT_Signals_Counter + 1],1),self.SAUT_Signals[SAUT_Signals_Counter + 2]))
                    SAUT_Signals_Counter = SAUT_Signals_Counter + 3
                end
                
                if (self.Nmb_Of_Text_Bytes_Per_Controller[Cntr_Nmb] > 0) then
                    self.UART_Bytes[UART_Bytes_Counter] = 0x54; --[["T" aka TEXT]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                    self.UART_Bytes[UART_Bytes_Counter] = 0x42; --[["B" aka BEGIN]]UART_Bytes_Counter = UART_Bytes_Counter + 1
                    if (self.TextDisplaysSignalsStruct[Text_Display_Counter].DisplayOn == 1) then
                        if (self.TextDisplaysSignalsStruct[Text_Display_Counter].DisplayLedOn == 1) then
                            self.UART_Bytes[UART_Bytes_Counter] = 0x11; --[[Включены экран и подсветка.]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                        else
                            self.UART_Bytes[UART_Bytes_Counter] = 0x01; --[[Включен только экран.]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                        end
                    else
                        self.UART_Bytes[UART_Bytes_Counter] = 0x00; --[[Выключен экран.]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                    end
                    local temp = self.Config_Text_Signals[Cntr_Nmb] * 2
                    self.UART_Bytes[UART_Bytes_Counter] = tobyte(temp); UART_Bytes_Counter = UART_Bytes_Counter + 1
                    for Text_Byte_Index = 0,temp-1 do
                        self.UART_Bytes[UART_Bytes_Counter] = tobyte(self.TextDisplaysSignalsStruct[Text_Display_Counter].textBytes[Text_Byte_Index]); UART_Bytes_Counter = UART_Bytes_Counter + 1
                    end
                    
                    self.UART_Bytes[UART_Bytes_Counter] = 0x54; --[["T" aka TEXT]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                    self.UART_Bytes[UART_Bytes_Counter] = 0x45; --[["E" aka END ]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                    
                    Text_Display_Counter = Text_Display_Counter + 1
                end
            end
            UART.WriteByte(self.UART_Bytes,self.Nmb_Of_UART_Bytes + 2)
        end
        command = 0
    end
end

local function new(tbl,n) -- Аналог new[]() в C++
    for i=0,n-1 do
       tbl[i] = 0
    end
    return tbl
end

local function new_textDisplaySignals(tbl,n)
    for i=0,n-1 do
        tbl[i] = {
            DisplayOn = 0,
            DisplayLedOn = 0,
            textBytes = {}
        }
        for j=0,49 do
            tbl[i].textBytes[j] = 0
        end
    end
    return tbl
end


function UARTFrontViewClient:ConfigSignals() --Config_Signals
    self.Nmb_Of_Input_Bytes = 0
	self.Nmb_Of_Output_Bytes = 0

	self.Nmb_Of_Arrow_Bytes = 0
	self.Nmb_Of_SAUT_Bytes = 0
	self.Nmb_Of_Indicator_Bytes = 0
	self.Nmb_Of_Text_Bytes = 0
    
    for Cntr_Nmb = 0,self.Number_Of_Controllers-1 do
        self.Nmb_Of_ADC_Signals_Per_Contr[Cntr_Nmb] = 0
		self.Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Nmb] = 0
		self.Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Nmb] = 0
		self.Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Nmb] = 0
		self.Nmb_Of_UART_Bytes_Per_Contr[Cntr_Nmb] = 0
		self.Nmb_Of_Text_Bytes_Per_Controller[Cntr_Nmb] = 0
    end
    
    self.Nmb_Of_Input_Signals = 0
	self.Nmb_Of_Output_Signals = 0
    
    self.Nmb_Of_Config_Bytes = self.Number_Of_Controllers * 7 + 2
    
    self.Config_Bytes = new(self.Config_Bytes,self.Nmb_Of_Config_Bytes)
    
    self.Config_Bytes[0] = tobyte(self.Nmb_Of_Config_Bytes - 2)
    self.Config_Bytes[1] = 0x81
    
    local Config_Signals_Counter = 0
	local Config_Bytes_Counter = 0
	local Cntr_Counter = 0

    local i,tar_i=0,self.Config_Signals_Length + self.Number_Of_Controllers
    repeat
        if ((i + 1) % 25 ~= 0) then -- 24 бита отвечают за конфигурирование пинов контроллера, на шаге, кратном 25, записываем байт-показатель количества байт, передаваемых данным контроллером по UART
            Config_Signals_Counter = i - Cntr_Counter
			Config_Bytes_Counter = math.floor(Config_Signals_Counter / 4) + 2
            self.Config_Bytes[Config_Bytes_Counter + Cntr_Counter] = bit.bor(self.Config_Bytes[Config_Bytes_Counter + Cntr_Counter],tobyte(bit.rshift(bit.lshift(self.Config_Signals[Config_Signals_Counter],6),((Config_Signals_Counter % 4) * 2))))
        
            if (self.Config_Signals[Config_Signals_Counter] == 3) then
                self.Nmb_Of_Input_Bytes = self.Nmb_Of_Input_Bytes + 2
                self.Nmb_Of_ADC_Signals_Per_Contr[math.floor(Config_Signals_Counter / 24)] = self.Nmb_Of_ADC_Signals_Per_Contr[math.floor(Config_Signals_Counter / 24)] + 1
            end
        else
            self.Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Counter] = (self.Config_Arrow_Signals[Cntr_Counter] == 0) and 0 or self.Config_Arrow_Signals[Cntr_Counter] * 3 + 1
            
            for Ind_Nmb=0,2 do
                self.Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Counter] = self.Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Counter] + ((self.Config_Indicator_Signals[Cntr_Counter].map[Ind_Nmb][0] == 0) and 0 or (Config_Indicator_Signals[Cntr_Counter].map[Ind_Nmb][0] + 4))
            end
            self.Nmb_Of_Indicator_Bytes = self.Nmb_Of_Indicator_Bytes + self.Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Counter]
            
            self.Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Counter] = self.Config_SAUT_Signals[Cntr_Counter] * 2
            
            self.Nmb_Of_Text_Bytes_Per_Controller[Cntr_Counter] = (self.Config_Text_Signals[Cntr_Counter] == 0) and 0 or (self.Config_Text_Signals[Cntr_Counter] * 2 + 6)
            
            self.Nmb_Of_UART_Bytes_Per_Contr[Cntr_Counter] = self.Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Counter] + self.Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Counter] + self.Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Counter] + self.Nmb_Of_Text_Bytes_Per_Controller[Cntr_Counter]
            
            self.Config_Bytes[Config_Bytes_Counter + Cntr_Counter + 1] = tobyte(self.Nmb_Of_UART_Bytes_Per_Contr[Cntr_Counter])
            
            self.Nmb_Of_Arrow_Bytes = self.Nmb_Of_Arrow_Bytes + self.Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Counter]
            self.Nmb_Of_SAUT_Bytes = self.Nmb_Of_SAUT_Bytes + self.Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Counter]
			self.Nmb_Of_Text_Bytes = self.Nmb_Of_Text_Bytes + self.Nmb_Of_Text_Bytes_Per_Controller[Cntr_Counter]
            
            Cntr_Counter = Cntr_Counter + 1
        end
        i = i + 1
    until i==tar_i

    local ConfigDone = false
    timer.Simple(5,function() -- Удаляем поток ожидания ответа
        if not ConfigDone then
            ULib.tsayError(nil, "UARTFrontViewClient.lua: Не удалось сконфигурировать контроллер")
            hook.Remove("Think","UARTFrontViewClient:WaitConfResponse")
        end
    end)
    
    local lastSend = CurTime()-0.15
    local Sended = false
    hook.Add("Think","UARTFrontViewClient:WaitConfResponse",function()
        if CurTime()-lastSend<0.12 then return end
        lastSend = CurTime()
        
        if not ConfigDone then
            if not Sended then
                UART.WriteByte(self.Config_Bytes,self.Nmb_Of_Config_Bytes)
                Sended = true
            else
                self.ACK_Bytes = UART.ReadByte(2)
                Sended = false
                -- print(self.ACK_Bytes[0],self.ACK_Bytes[1])
                if (self.ACK_Bytes[0] == 0x01 and self.ACK_Bytes[1] == 0x8F) then
                    ULib.tsayColor(nil,false,Color(0,255,0),"UARTFrontViewClient.lua: Контроллер сконфигурирован успешно!")
                    ConfigDone = true
                end  
            end
        end
        if not ConfigDone then return end
        
        self.Nmb_Of_Input_Bytes = self.Nmb_Of_Input_Bytes + 3 * self.Number_Of_Controllers
        self.Nmb_Of_Output_Bytes = self.Nmb_Of_Output_Bytes + 3 * self.Number_Of_Controllers
        self.Nmb_Of_Input_Signals = 24 * self.Number_Of_Controllers
        self.Nmb_Of_Output_Signals = 24 * self.Number_Of_Controllers
        
        self.Nmb_Of_UART_Bytes = self.Nmb_Of_Arrow_Bytes + self.Nmb_Of_SAUT_Bytes + self.Nmb_Of_Indicator_Bytes + self.Nmb_Of_Text_Bytes
        
        -- Определяем размеры всех используемых массивов
        self.Input_Bytes = new(self.Input_Bytes,self.Nmb_Of_Input_Bytes + 3)
        self.Output_Bytes = new(self.Output_Bytes,self.Nmb_Of_Output_Bytes + 2)
        self.UART_Bytes = new(self.UART_Bytes,self.Nmb_Of_UART_Bytes + 2)

        self.ADC_Signals = new(self.ADC_Signals,self.Nmb_Of_ADC_Signals)
        self.Input_Signals = new(self.Input_Signals,self.Nmb_Of_Input_Signals)
        self.Output_Signals = new(self.Output_Signals,self.Nmb_Of_Output_Signals)
        self.Arrow_Signals = new(self.Arrow_Signals,self.Nmb_Of_Arrow_Signals)
        self.Indicator_Signals = new(self.Indicator_Signals,self.Nmb_Of_Indicator_Signals)
        self.SAUT_Signals = new(self.SAUT_Signals,self.Nmb_Of_SAUT_Signals)
        self._7SegDec_Signals = new(self._7SegDec_Signals,self.Nmb_Of_7SegDec_Signals)
        self.TextDisplaysSignalsStruct = new_textDisplaySignals(self.TextDisplaysSignalsStruct,self.Nmb_Of_Text_Displays)
        timer.Simple(1.5,function() Metro81717Signals.Initialized = true end)

        hook.Remove("Think","UARTFrontViewClient:WaitConfResponse")
    end)
end

function UARTFrontViewClient:shutdown()
-- Writing_UART_Bytes
    if (self.Nmb_Of_UART_Bytes > 0) then
        -- Готовим байты, выводимые контроллером через UART
        self.UART_Bytes[0] = tobyte(self.Nmb_Of_UART_Bytes)
        self.UART_Bytes[1] = 0x87
        
        local UART_Bytes_Counter = 2
        
        for Cntr_Nmb = 0,self.Number_Of_Controllers-1 do
            if (self.Nmb_Of_Arrow_Bytes_Per_Contr[Cntr_Nmb] > 0) then
                self.UART_Bytes[UART_Bytes_Counter] = 0x41; --[["A" aka ARROWS]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                for i = 0,self.Config_Arrow_Signals[Cntr_Nmb]-1 do
                    self.UART_Bytes[UART_Bytes_Counter] = tobyte(i + 1); UART_Bytes_Counter = UART_Bytes_Counter + 1
					self.UART_Bytes[UART_Bytes_Counter] = 0x00; UART_Bytes_Counter = UART_Bytes_Counter + 1
					self.UART_Bytes[UART_Bytes_Counter] = 0x00; UART_Bytes_Counter = UART_Bytes_Counter + 1
                end
            end
            
            if (self.Nmb_Of_Indicator_Bytes_Per_Contr[Cntr_Nmb] > 0) then
                for Ind_Nmb = 0,2 do
                    if (self.Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0] > 0) then
                        self.UART_Bytes[UART_Bytes_Counter] = 0x49; --[["I" aka INDICATORS]] UART_Bytes_Counter = UART_Bytes_Counter + 1
						self.UART_Bytes[UART_Bytes_Counter] = tobyte(Ind_Nmb+1); UART_Bytes_Counter = UART_Bytes_Counter + 1
						self.UART_Bytes[UART_Bytes_Counter] = tobyte(Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0]); UART_Bytes_Counter = UART_Bytes_Counter + 1
                        
                        for Indicator_Bytes_Index = 0,self.Config_Indicator_Signals[Cntr_Nmb].map[Ind_Nmb][0]-1 do
                            self.UART_Bytes[UART_Bytes_Counter] = 0x0A; UART_Bytes_Counter = UART_Bytes_Counter + 1
                        end
                        
                        self.UART_Bytes[UART_Bytes_Counter] = 0xFF; UART_Bytes_Counter = UART_Bytes_Counter + 1
                    end
                end
            end
            
            if (self.Nmb_Of_SAUT_Bytes_Per_Contr[Cntr_Nmb] > 0) then
                self.UART_Bytes[UART_Bytes_Counter] = 0x4C; --[["L" aka LEDS]] UART_Bytes_Counter = UART_Bytes_Counter + 1
				self.UART_Bytes[UART_Bytes_Counter] = 0x00; UART_Bytes_Counter = UART_Bytes_Counter + 1
            end
            
            if (self.Nmb_Of_Text_Bytes_Per_Controller[Cntr_Nmb] > 0) then
                self.UART_Bytes[UART_Bytes_Counter] = 0x54; --[["T" aka TEXT ]] UART_Bytes_Counter = UART_Bytes_Counter + 1
				self.UART_Bytes[UART_Bytes_Counter] = 0x42; --[["B" aka BEGIN]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                
                self.UART_Bytes[UART_Bytes_Counter] = 0x00; --[[ Выключен экран.]] UART_Bytes_Counter = UART_Bytes_Counter + 1
                
                self.UART_Bytes[UART_Bytes_Counter] = tobyte(self.Config_Text_Signals[Cntr_Nmb] * 2); UART_Bytes_Counter = UART_Bytes_Counter + 1
                
                for Text_Byte_Index = 0,(self.Config_Text_Signals[Cntr_Nmb] * 2) do
					self.UART_Bytes[UART_Bytes_Counter] = 0x00; UART_Bytes_Counter = UART_Bytes_Counter + 1
				end
                
                self.UART_Bytes[UART_Bytes_Counter] = 0x54; --[["T" aka TEXT]] UART_Bytes_Counter = UART_Bytes_Counter + 1
				self.UART_Bytes[UART_Bytes_Counter] = 0x45; --[["E" aka END ]] UART_Bytes_Counter = UART_Bytes_Counter + 1
            end
        end
        
        timer.Simple(0.4,function()
            timer.Create("UARTFrontViewClient:DeconfigurationStep1",0.1,5,function()
                UART.WriteByte(self.UART_Bytes,self.Nmb_Of_UART_Bytes + 2)
            end)
        end)
    end
    
-- Deconfiguration
    local Disable_Bytes = {}
    for i=2,self.Number_Of_Controllers * 7 + 1 do
        Disable_Bytes[i] = 0
    end
    
    Disable_Bytes[0] = tobyte(self.Number_Of_Controllers * 7)
    Disable_Bytes[1] = 0x81
    
    timer.Simple(1.2,function()
        timer.Create("UARTFrontViewClient:DeconfigurationStep2",0.1,5,function()
            UART.WriteByte(Disable_Bytes,self.Number_Of_Controllers * 7 + 2)
        end)
        timer.Simple(0.6,function() UART.StopCOM() end)
    end)
    
end