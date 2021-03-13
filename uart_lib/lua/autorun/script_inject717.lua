local LampLEKK = math.random() > 0.9 and 1 or 0

if SERVER then
    
    timer.Simple(0,function()
        local Train = scripted_ents.GetStored("gmod_subway_81-717_mvm").t
        function Train:getOutSignals()
            if not xContainer then return end
            xContainer["Ammeter"] = (self:GetPackedRatio("EnginesCurrent") - 0.5) * 1000
            xContainer["BatteryVoltage"] = self:GetPackedRatio("BatteryVoltage") * 150
            xContainer["tcPressure"] = self:GetPackedRatio("BCPressure") * 6
            xContainer["tmPressure"] = self:GetPackedRatio("BLPressure") * 16
            xContainer["KVolt"] = self:GetPackedRatio("EnginesVoltage") * 1000
            xContainer["nmPressure"] = self:GetPackedRatio("TLPressure") * 16
            -- xContainer["LAB"] = 0
            xContainer["LampAsotp1"] = self:GetNW2Int("IGLA:ButtonL1")
            xContainer["LampAsotp2"] = self:GetNW2Int("IGLA:ButtonL2")
            xContainer["LampAsotp3"] = self:GetNW2Int("IGLA:ButtonL3")
            xContainer["LampAsotp4"] = self:GetNW2Int("IGLA:ButtonL4")
            xContainer["LampAsotpFire"] = self:GetNW2Int("IGLA:Fire")
            xContainer["LampAsotpFault"] = self:GetNW2Int("IGLA:Error")
            xContainer["LAVU"] = self:GetNW2Int("AVU")
            xContainer["LSP"] = self:GetNW2Int("LSP")
            -- xContainer["InformerLampLeft"] = 0
            -- xContainer["InformerLampMiddle"] = 0
            -- xContainer["InformerLampRight"] = 0
            xContainer["LampInverterProtection"] = self:GetNW2Int("RZP")
            xContainer["LampParkingBrake"] = Или self:GetNW2Int("PN")  -- Или self:GetNW2Int("BrW")
            -- xContainer["LAV"] = 0
            -- xContainer["LAV1"] = 0
            -- xContainer["LKVP"] = 0
            -- xContainer["UAVAEmergency"] = 0
            -- xContainer["UAVAFeed"] = 0
            -- xContainer["UAVAService"] = 0
            xContainer["InformerText"] = self.ASNP.OutText
            xContainer["AsotpText"] = self.IGLA_CBKI.OutText
            
            xContainer["L00"] = self:GetNW2Int("AR0")
            xContainer["LKVC"] = self:GetNW2Int("KVC")
            xContainer["LRP"] = self:GetNW2Int("RP")
            xContainer["LSN"] = self:GetNW2Int("SN")
            xContainer["LVD"] = self:GetNW2Int("VD")
            xContainer["LST"] = self:GetNW2Int("ST")
            xContainer["LHRK"] = self:GetNW2Int("HRK")
            xContainer["LDoorLeft"] = self:GetNW2Int("DoorsLeftL")
            xContainer["LDoorRight"] = self:GetNW2Int("DoorsRightL")
            xContainer["LBV"] = self:GetNW2Int("GreenRP")
            xContainer["LOCH"] = self:GetNW2Int("AR04")
            xContainer["L40"] = self:GetNW2Int("AR40")
            xContainer["L60"] = self:GetNW2Int("AR60")
            xContainer["L70"] = self:GetNW2Int("AR70")
            xContainer["L80"] = self:GetNW2Int("AR80")
            xContainer["LRV"] = self:GetNW2Int("RS")
            -- xContainer["LAB"] = 0
            xContainer["LKVD"] = self:GetNW2Int("KVD")
            -- xContainer["LAV"] = 0
            -- xContainer["LAV1"] = 0
            
            xContainer["LSD"] = self:GetNW2Int("SD")
            xContainer["LampPneumBrakes"] = self:GetNW2Int("BrW") -- Или self:GetNW2Int("PN")
            xContainer["LKVP"] = self:GetNW2Int("LKVP")
            xContainer["LN"] = self:GetNW2Int("LN")
            xContainer["LEKK"] = LampLEKK
            xContainer["LKT"] = self:GetNW2Int("KT")
            xContainer["vfact"] = math.floor(self:GetPackedRatio("Speed")*100)
        end
        
        Train.Old_Think = Train.Old_Think or Train.Think
        function Train:Think()
            self.RetVal = self:Old_Think()
            if Autospawn.HeadWagon == self then
                if IsValid(self.DriverSeat:GetDriver()) and Metro81717Signals.Initialized then
                    self:getOutSignals()
                    UARTFrontViewClient:update()
                    Metro81717Signals:update(UARTFrontViewClient,self)
                    xContainer:Thread(self)
                    self:SetNW2Bool("UARTWorking",true)
                else
                    if Metro81717Signals.Initialized then
                        Metro81717Signals.Initialized = false
                        Metro81717Signals:stopSignals()
                        self:SetNW2Bool("UARTWorking",false)
                        print("Close UART!")
                    end
                end
            end
            return self.RetVal
        end
        
        -- Remove entity
        function Train:OnRemove()
            if Metro81717Signals.Initialized then
                Metro81717Signals.Initialized = false
                Metro81717Signals:stopSignals()
            end
            -- print("Close UART!")
            -- Remove all linked objects
            constraint.RemoveAll(self)
            if self.TrainEntities then
                for k,v in pairs(self.TrainEntities) do
                    SafeRemoveEntity(v)
                end
            end

            -- Deinitialize train
            if Turbostroi then
                Turbostroi.DeinitializeTrain(self)
            end
            SetGlobalInt("metrostroi_train_count",Metrostroi.TrainCount())
            net.Start("MetrostroiTrainCount") net.Broadcast()
        end
    end)
    
end
if CLIENT then
    -- Добавляем зеркала
    timer.Simple(0,function()
        local Train = scripted_ents.GetStored("gmod_subway_81-717_mvm").t
        
        Train.Old_Initialize = Train.Old_Initialize or Train.Initialize
        function Train:Initialize()
            self:Old_Initialize()
            self.Mirror1 = self:CreateRT("717Mirror1",256,512)
            self.Mirror2 = self:CreateRT("717Mirror2",256,512)
        end
    end)
    
    -- Отрисовка зеркал
    hook.Add("HUDPaint","Autospawn:Mirrors",function()
        local Train = LocalPlayer().InMetrostroiTrain
        if not Train then return end
        if not Train.Mirror1 then return end
        
        surface.SetDrawColor(190,150,0)
        
        surface.DrawRect(0,68,242,516)
        Metrostroi.RenderCamOnRT(Train,Vector(451,0,14),"Mirror1",0.03125,Train.Mirror1,Train,Vector(460, 73,25),Angle(0,170,0),256,512,1,0,0)
        render.DrawTextureToScreenRect(Train.Mirror1,0,70,240,512)
        
        surface.DrawRect(ScrW() - 242,68,242,516)
        Metrostroi.RenderCamOnRT(Train,Vector(451,0,14),"Mirror2",0.03125,Train.Mirror2,Train,Vector(460,-73,25),Angle(0,190,0),256,512,1,0,0)
        render.DrawTextureToScreenRect(Train.Mirror2,1680,70,240,512)
    end)
    
    -- Триггеры с сервера
    -- net.Receive("UARTStarter", function(_,ply)
        -- timer.Simple(3,function()
            -- Metro81717Signals:startSignals(COMPort)
        -- end)
    -- end)
    net.Receive("ZarnSetCam",function(_,ply)
        local train = LocalPlayer().InMetrostroiTrain
        if not train then return end
        train.CurrentCamera = 14
    end)
    
    -- Подключение к COM порту принудительно
    -- concommand.Add("com",function(ply,cmd,args)
        -- if not args[1] then print("Enter COM port number, type -1 for disconnect") return end
        -- local port = tonumber(args[1]) or -1
        -- if port < 0 then
            -- UART.StopCOM()
            -- print("COM closed")
        -- else
            -- local res = UART.StartCOM(args[1])
            -- print("COM result:",res)
        -- end
    -- end,nil,"Connect to COM device, type -1 for disconnect")
end