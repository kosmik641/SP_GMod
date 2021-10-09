local libFounded = UART
if not UART then
    if pcall(require,"uart") then
        print("UART Lib: Loaded!")
		print(Format("Version: %s",UART.Version()))
        libFounded = true
    else
        print("UART Lib not found")
    end
end
if not libFounded then return end

util.AddNetworkString("ZarnSetCam")
local plombstbl = {"VAH","VP","OtklAVU","OVT","KAH","OtklBV","RC1","UOS","VBD","UAVA","UPPS_On"}
timer.Simple(1,function()
    local Train = scripted_ents.GetStored("gmod_subway_81-717_mvm").t
    Train.Old_Init = Train.Old_Init or Train.Initialize
    function Train:Initialize()
        self:Old_Init()
        timer.Simple(1, function()
            if not IsValid(self) then return end
            for i=1,#plombstbl do self:BrokePlomb(plombstbl[i]) end
        end)
    end
    Train.Old_Think = Train.Old_Think or Train.Think
    function Train:Think()
        if IsValid(self.DriverSeat:GetDriver()) then
            if UART.Status() then
                self:SetNW2Bool("UARTWorking",true)
                local inTable = UART.Get()
                for k,v in pairs(inTable) do
                    if k == "KV70Controller" then -- Контроллер машиниста
                        if self.KV.ControllerPosition ~= v then
                            self.KV:TriggerInput("ControllerSet",v)
                        end
                    elseif k == "KM013" then -- Кран машиниста
                        if self.Pneumatic.DriverValvePosition ~= v then
                            self.Pneumatic:TriggerInput("BrakeSet",v)
                        end
                    elseif k == "KV70Reverser" then -- Реверс
                        if self.KV.ReverserPosition ~= v then
                            self.KVWrenchMode = v ~= 0 and 1 or 0
                            self.KV:TriggerInput("Enabled",1)
                            self.KV:TriggerInput("ReverserSet",v)
                        end
                    elseif k == "KRUReverser" then -- КРУ
                        if self.KRU.Position ~= v then
                            self.KVWrenchMode = v>0 and 2 or 0
                            self.KRU:TriggerInput("Enabled",self.KVWrenchMode)
                            self.KRU:TriggerInput("Set",v)
                        end
                    elseif k == "HornSignal" then -- Сигнал
                        if self.Horn.Value ~= v then
                            self.Horn:TriggerInput("Engage",v)
                        end
                    else
                        if self[k].Value ~= v then self[k]:TriggerInput("Set",v) end
                    end
                end
                local sendTable = {}
                local Panel = self.Panel
                sendTable["Ammeter"] = self.Electric.I13
                sendTable["BatteryVoltage"] = Panel["V1"]*self.Battery.Voltage
                sendTable["tcPressure"] = self.Pneumatic.BrakeCylinderPressure
                sendTable["tmPressure"] = self.Pneumatic.BrakeLinePressure
                sendTable["KVolt"] = self.Electric.Aux750V
                sendTable["nmPressure"] = self.Pneumatic.TrainLinePressure
                -- sendTable["LAB"] = 0
                sendTable["LampAsotp1"] = self.IGLA_CBKI.ButtonL1 and 1 or 0
                sendTable["LampAsotp2"] = self.IGLA_CBKI.ButtonL2 and 1 or 0
                sendTable["LampAsotp3"] = self.IGLA_CBKI.ButtonL3 and 1 or 0
                sendTable["LampAsotp4"] = self.IGLA_CBKI.ButtonL4 and 1 or 0
                sendTable["LampAsotpFire"] = self.IGLA_CBKI.Fire and 1 or 0
                sendTable["LampAsotpFault"] = self.IGLA_CBKI.Error and 1 or 0
                sendTable["LAVU"] = math.min(1,Panel.AVU)
                sendTable["LSP"] = self:GetNW2Int("LSP")
                -- sendTable["InformerLampLeft"] = 0
                -- sendTable["InformerLampMiddle"] = 0
                -- sendTable["InformerLampRight"] = 0
                sendTable["LampInverterProtection"] = math.min(1,Panel.RZP)
                sendTable["LampParkingBrake"] = math.min(1,Panel.BrT)  -- Или Panel.BrW
                -- sendTable["LAV"] = 0
                -- sendTable["LAV1"] = 0
                -- sendTable["LKVP"] = 0
                -- sendTable["UAVAEmergency"] = 0
                -- sendTable["UAVAFeed"] = 0
                -- sendTable["UAVAService"] = 0
                --sendTable["InformerText"] = self.ASNP.OutText
                --sendTable["AsotpText"] = self.IGLA_CBKI.OutText
                sendTable["InformerDisplay"] = self.ASNP_VV.Power--math.min(1,self.ASNP.DispalyOn or 0)
                sendTable["AsotpDisplay"] = (self:GetNW2Bool("IGLA:ShowTime") or not self:GetNW2Bool("IGLA:Standby")) and 1 or 0
                
                sendTable["LKVC"] = math.min(1,Panel.KVC)
                sendTable["LRP"] = math.min(1,self:GetNW2Int("RP"))
                sendTable["LSN"] = math.min(1,self:GetNW2Int("SN"))
                sendTable["LVD"] = math.min(1,Panel.LVD)
                sendTable["LST"] = math.min(1,Panel.LST)
                sendTable["LHRK"] = math.min(1,Panel.LhRK)
                sendTable["LDoorLeft"] = math.min(1,Panel.DoorsLeft)
                sendTable["LDoorRight"] = math.min(1,Panel.DoorsRight)
                sendTable["LBV"] = math.min(1,Panel.GreenRP)
                sendTable["LOCH"] = math.min(1,Panel.AR04)
                sendTable["L00"] = math.min(1,Panel.AR0)
                sendTable["L40"] = math.min(1,Panel.AR40)
                sendTable["L60"] = math.min(1,Panel.AR60)
                sendTable["L70"] = math.min(1,Panel.AR70)
                sendTable["L80"] = math.min(1,Panel.AR80)
                sendTable["LRV"] = math.min(1,Panel.RS)
                -- sendTable["LAB"] = 0
                sendTable["LKVD"] = math.min(1,Panel.LKVD)
                -- sendTable["LAV"] = 0
                -- sendTable["LAV1"] = 0
                
                sendTable["LSD"] = math.min(1,Panel.SD)
                sendTable["LampPneumBrakes"] = math.min(1,Panel.BrW) -- Или Panel.BrT
                sendTable["LKVP"] = math.min(1,Panel.LKVP)
                sendTable["LN"] = math.min(1,Panel.LN)
                sendTable["LEKK"] = math.min(1,Panel.LEKK)
                sendTable["LKT"] = math.min(1,Panel.KT)
                sendTable["vfact"] = math.floor(self:GetPackedRatio("Speed")*100)
                UART.Send(sendTable,self.ASNP.OutText or "",self.IGLA_CBKI.OutText or "")
            else
                self:SetNW2Bool("UARTWorking",false)
            end
        else
            self:SetNW2Bool("UARTWorking",false)
        end
        self.RetVal = self:Old_Think()
        return self.RetVal
    end
end)       

concommand.Add("uart",function(ply,cmd,args)
    if not args[1] then print("Thread state = ",UART.Get()) print("Enter COM port number, type -1 for disconnect") return end
    local port = tonumber(args[1]) or -1
    if port < 0 then
        UART.Shutdown()
        print("COM closed")
        ULib.tsayColor(nil,false,Color(0,255,0),"UART.DLL: Подключение закрыто")
    else
        local res = UART.Status() or UART.Initialize(args[1])
        if res == 0 then
            ULib.tsayColor(nil,false,Color(0,255,0),"UART.DLL: Подключение установлено")
        elseif res == true then
            ULib.tsayColor(nil,false,Color(255,255,0),Format("UART.DLL: Контроллер уже подключен!",res or 0))
        else
            ULib.tsayColor(nil,false,Color(255,0,0),Format("UART.DLL: Ошибка подключения (%d)",res or 0))
        end
        print("COM result:",res)
    end
end,nil,"Connect to COM controller, type -1 for disconnect")
concommand.Add("ch_cab",function()
    if not IsValid(Autospawn.HeadWagon) then MsgC(Color(255,0,0),"\nСостав не найден\n") return end
    MsgC(Color(255,255,0),"\nСмена кабины...\n")
    local ply = Autospawn.HeadWagon.DriverSeat:GetDriver()
    ulx.changecab( ply )
    Autospawn.HeadWagon = Autospawn.HeadWagon.WagonList[#Autospawn.HeadWagon.WagonList]
    timer.Simple(1.1,function() --[[RunConsoleCommand("uart",Autospawn.COMPort)]] net.Start("ZarnSetCam") net.Send(ply) end)
end)