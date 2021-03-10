local LampLEKK = math.random() > 0.9 and 1 or 0

if SERVER then
    local Train = scripted_ents.GetStored("gmod_subway_81-717_mvm").t
    timer.Simple(0,function()
        
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
            xContainer["InformerText"] = ""
            xContainer["AsotpText"] = ""
            
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
            
            -- xContainer["A1off"] = 0
            -- xContainer["A2off"] = 0
            -- xContainer["A3off"] = 0
            -- xContainer["A4off"] = 0
            -- xContainer["A5off"] = 0
            -- xContainer["A6off"] = 0
            -- xContainer["A7off"] = 0
            -- xContainer["A8off"] = 0
            -- xContainer["A9off"] = 0
            -- xContainer["A10off"] = 0
            -- xContainer["A11off"] = 0
            -- xContainer["A12off"] = 0
            -- xContainer["A13off"] = 0
            -- xContainer["A14off"] = 0
            -- xContainer["A15off"] = 0
            -- xContainer["A16off"] = 0
            -- xContainer["A17off"] = 0
            -- xContainer["A18off"] = 0
            -- xContainer["A19off"] = 0
            -- xContainer["A20off"] = 0
            -- xContainer["A21off"] = 0
            -- xContainer["A22off"] = 0
            -- xContainer["A24off"] = 0
            -- xContainer["A25off"] = 0
            -- xContainer["A26off"] = 0
            -- xContainer["A27off"] = 0
            -- xContainer["A28off"] = 0
            -- xContainer["A29off"] = 0
            -- xContainer["A30off"] = 0
            -- xContainer["A31off"] = 0
            -- xContainer["A32off"] = 0
            -- xContainer["A33off"] = 0
            -- xContainer["A37off"] = 0
            -- xContainer["A38off"] = 0
            -- xContainer["A39off"] = 0
            -- xContainer["A40off"] = 0
            -- xContainer["A41off"] = 0
            -- xContainer["A42off"] = 0
            -- xContainer["A43off"] = 0
            -- xContainer["A44off"] = 0
            -- xContainer["A45off"] = 0
            -- xContainer["A46off"] = 0
            -- xContainer["A47off"] = 0
            -- xContainer["A48off"] = 0
            -- xContainer["A49off"] = 0
            -- xContainer["A50off"] = 0
            -- xContainer["A51off"] = 0
            -- xContainer["A52off"] = 0
            -- xContainer["A53off"] = 0
            -- xContainer["A54off"] = 0
            -- xContainer["A56off"] = 0
            -- xContainer["A57off"] = 0
            -- xContainer["A65off"] = 0
            -- xContainer["A66off"] = 0
            -- xContainer["A68off"] = 0
            -- xContainer["A70off"] = 0
            -- xContainer["A71off"] = 0
            -- xContainer["A72off"] = 0
            -- xContainer["A73off"] = 0
            -- xContainer["A74off"] = 0
            -- xContainer["A75off"] = 0
            -- xContainer["A76off"] = 0
            -- xContainer["A77off"] = 0
            -- xContainer["A78off"] = 0
            -- xContainer["A79off"] = 0
            -- xContainer["A80off"] = 0
            -- xContainer["A81off"] = 0
            -- xContainer["A82off"] = 0
            -- xContainer["A83off"] = 0
            -- xContainer["AB1off"] = 0
            -- xContainer["AB3off"] = 0
            -- xContainer["AB6off"] = 0
            -- xContainer["AC1off"] = 0
            -- xContainer["AP63off"] = 0
            -- xContainer["AISoff"] = 0
            -- xContainer["VUoff"] = 0  
        end
        
        Train.Old_Think = Train.Old_Think or Train.Think
        function Train:Think()
            self.RetVal = self:Old_Think()
            if Metro81717Signals.Initialized and Autospawn.HeadWagon == self then
                self:getOutSignals()
                Metro81717Signals:update(UARTFrontViewClient,self)
                xContainer:Thread(self,self.DriverSeat:GetDriver())
            end
            return self.RetVal
        end
        
        -- Remove entity
        function Train:OnRemove()

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