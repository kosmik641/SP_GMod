if CLIENT then return end
Autospawn = Autospawn or {}
gameevent.Listen( "player_spawn" )

-- Номер COM порта вводить сюда:
Autospawn.COMPort = 8

Autospawn.Param = {
    ["Type"] = 2, -- 81-717.5
    ["BodyType"] = 1, -- МВМ
    ["MaskType"] = 6, -- 2-2-2 (M)
    ["Cran"] = 2, -- 013
    ["LampType"] = 3, -- ЛЛВ-01
    ["SeatType"] = 2, -- Старые
    ["ARSType"] = 2, -- Тип 1
    ["RingType"] = 1, -- Рандом
    ["BPSNType"] = 13, -- Тип 12
    ["SpawnMode"] = 1, -- Полностью запущен
    ["Texture"] = "Def_717MSKClassic1",
    ["CabTexture"] = "Def_ClassicG",
    ["PassTexture"] = "Def_717MSKWood3",
}
Autospawn.trace = {
    ["gm_metro_nekrasovskaya_line_v5"] = {
        WagonCount = 6,
        [1] = {
            Name = "Ст. Некрасовка, 3 ст. п.",
            AllSolid = false,
            Contents = 1,
            DispFlags = 0,
            Entity = player.GetByID(0), --
            Fraction = 0.019450338557363,
            FractionLeftSolid = 0,
            Hit = true,
            HitBox = 1218,
            HitGroup = 0,
            HitNoDraw = false,
            HitNonWorld = false,
            HitNormal = Vector(-0.000000,0.000000,1.000000),
            HitPos  = Vector(9080.407227,-14622.814453,-14425.968750),
            HitSky  = false,
            --HitTexture      =       **studio**,
            HitWorld = true,
            MatType = 77,
            Normal  = Vector(0.936578,0.016004,-0.350092),
            PhysicsBone = 0,
            StartPos = Vector(8483.480469,-14612.614258,-14202.837891),
            StartSolid = false,
            SurfaceFlags = 0,
            SurfaceProps = 3,
        }
    }
}

local function SetValue(ent,id,val)
    if type(val) == "number" then
        ent:SetNW2Int(id,val)
    elseif type(val) == "string" then
        ent:SetNW2String(id,val)
    elseif type(val) == "boolean" then
        ent:SetNW2Bool(id,val)
    end
end

function Autospawn.TrainSpawn(ply,trace,WagNum)
    local Train = scripted_ents.Get("gmod_subway_81-717_mvm")
    if not Train or not trace then return end
    if not IsValid(ply) then return end
    
    local FIXFIXFIX = {}
    for i=1,math.random(12) do
        FIXFIXFIX[i] = ents.Create("env_sprite")
        FIXFIXFIX[i]:Spawn()
    end
    
    local LastRot,LastEnt = false
    local trains = {}
    for i=1,WagNum do
        local ent
        if i == 1 then
            ent = Train:SpawnFunction(ply,trace,Train.Spawner.head or Train.ClassName,true)
            if ent then
                undo.Create(Train.Spawner.head or Train.ClassName)
            else
                ply:LimitHit("spawner_wrong_pos")
                return false
            end
            Autospawn.HeadWagon = ent
        end
        if i > 1 then
            local rot = (i==WagNum or math.random() > 0.5) -- Rotate last wagon or rotate it randomly
            ent = ents.Create(i~=WagNum and Train.Spawner.interim or Train.Spawner.head or Train.ClassName)
            ent.Owner = ply
            ent:Spawn()
            -- Invert bogeys by rotation
            local bogeyL1,bogeyE1,bogeyE2
            local couplL1,couplE1,couplE2
            if LastRot then
                bogeyL1 = LastEnt.FrontBogey
                couplL1 = LastEnt.FrontCouple
            else
                bogeyL1 = LastEnt.RearBogey
                couplL1 = LastEnt.RearCouple
            end
            if rot then
                bogeyE1,bogeyE2 = ent.RearBogey,ent.FrontBogey
                couplE1,couplE2 = ent.FrontCouple,ent.RearCouple
            else
                bogeyE1,bogeyE2 = ent.FrontBogey,ent.RearBogey
                couplE1,couplE2 = ent.RearCouple,ent.FrontCouple
            end
            local haveCoupler = couplL1 ~= nil
            if haveCoupler then
                bogeyE1:SetAngles(ent:LocalToWorldAngles(bogeyE1.SpawnAng))
                bogeyE2:SetAngles(ent:LocalToWorldAngles(bogeyE1.SpawnAng))
                -- Set bogey position by our bogey couple offset and lastent bogey couple offset
                couplE1:SetPos(
                    couplL1:LocalToWorld(
                        Vector(
                            couplL1.CouplingPointOffset.x*1.1+couplE1.CouplingPointOffset.x*1.1,
                            couplL1.CouplingPointOffset.y-couplE1.CouplingPointOffset.y,
                            couplL1.CouplingPointOffset.z-couplE1.CouplingPointOffset.z
                        )
                    )
                )
                -- Set bogey angles
                couplE1:SetAngles(couplL1:LocalToWorldAngles(Angle(0,180,0)))
                -- Set entity position by bogey pos and bogey offset
                couplE2:SetAngles(couplE1:LocalToWorldAngles(Angle(0,180,0)))
                ent:SetPos(couplE1:LocalToWorld(couplE1.SpawnPos*Vector(rot and -1 or 1,-1,-1)))
                -- Set entity angles by last ent and rotation
                ent:SetAngles(LastEnt:LocalToWorldAngles(Angle(0,rot ~= LastRot and 180 or 0,0)))

                -- Set bogey pos
                bogeyE1:SetPos(ent:LocalToWorld(bogeyE1.SpawnPos))
                bogeyE2:SetPos(ent:LocalToWorld(bogeyE2.SpawnPos))
                -- Set bogey angles
                bogeyE1:SetAngles(ent:LocalToWorldAngles(bogeyE1.SpawnAng))
                bogeyE2:SetAngles(ent:LocalToWorldAngles(bogeyE1.SpawnAng))
            else
                -- Set bogey position by our bogey couple offset and lastent bogey couple offset
                bogeyE1:SetPos(
                            bogeyL1:LocalToWorld(
                                Vector(bogeyL1.CouplingPointOffset.x*1.1+bogeyE1.CouplingPointOffset.x*1.05,bogeyL1.CouplingPointOffset.y-bogeyE1.CouplingPointOffset.y,bogeyL1.CouplingPointOffset.z-bogeyE1.CouplingPointOffset.z)
                            )
                )
                -- Set bogey angles
                bogeyE1:SetAngles(bogeyL1:LocalToWorldAngles(Angle(0,180,0)))
                -- Set entity position by bogey pos and bogey offset
                bogeyE2:SetAngles(bogeyE1:LocalToWorldAngles(Angle(0,180,0)))
                ent:SetPos(bogeyE1:LocalToWorld(bogeyE1.SpawnPos*Vector(rot and -1 or 1,-1,-1)))
                -- Set entity angles by last ent and rotation
                ent:SetAngles(LastEnt:LocalToWorldAngles(Angle(0,rot ~= LastRot and 180 or 0,0)))
                -- Set second bogey pos
                bogeyE2:SetPos(ent:LocalToWorld(bogeyE2.SpawnPos))
            end
            
            Metrostroi.RerailTrain(ent) --Rerail train
            
            LastRot = rot
        end
        table.insert(trains,ent)
        undo.AddEntity(ent)
        for _, set in ipairs(Train.Spawner) do
            local val = 1 --self.Settings[set[1]]
            if set[3] == "List" then
                if set[6] and type(set[6]) == "function" then   set[6](ent,val,LastRot,i,WagNum) else SetValue(ent,set[1],val) end
            elseif set[3] == "Boolean" then
                if set[5] and type(set[5]) == "function" then   set[5](ent,val,LastRot,i,WagNum) else ent:SetNW2Bool(set[1],val) end
            elseif set[3] == "Slider" then
                if set[8] and type(set[8]) == "function" then   set[8](ent,val,LastRot,i,WagNum) else ent:SetNW2Int(set[1],val) end
            end
        end
        for k,v in pairs(Autospawn.Param) do
            ent.CustomSettings = true
            if isnumber(v) then
                ent:SetNW2Int(k,v)
            else
                ent:SetNW2String(k,v)
            end
        end
        if Train.Spawner.func then Train.Spawner.func(ent,i,WagNum,LastRot) end
        if Train.Spawner.wagfunc then ent:GenerateWagonNumber(function(_,number) return Train.Spawner.wagfunc(ent,i,number) end) end
        if ent.TrainSpawnerUpdate then ent:TrainSpawnerUpdate() end
        for k,v in pairs(ent.CustomSpawnerUpdates) do if k ~= "BaseClass" then v(ent) end end
        ent:UpdateTextures()
        ent.FrontAutoCouple = i > 1 and i < WagNum
        ent.RearAutoCouple = true
        LastEnt = ent
    end
    undo.SetPlayer(ply)
    undo.SetCustomUndoText("Undone a train")
    undo.Finish()
    if Train.Spawner.postfunc then Train.Spawner.postfunc(trains,WagNum) end
    local CoupledTrains = 0
    local function StopCoupling()
        if not IsValid(trains[1]) or not trains[1].IgnoreEngine then return end
        for _,train in ipairs(trains) do
            train.FrontBogey.BrakeCylinderPressure = 3
            train.RearBogey.BrakeCylinderPressure = 3
            train.FrontBogey.MotorPower = 0
            train.RearBogey.MotorPower = 0
            train.OnCoupled = nil
        end
        timer.Simple(1,function() for i,train in ipairs(trains) do train.IgnoreEngine = false end end)
    end
    for i,train in ipairs(trains) do
        train.IgnoreEngine = true
        train.RearBogey.MotorForce = 40000
        train.FrontBogey.MotorForce = 40000
        train.RearBogey.PneumaticBrakeForce = 50000
        train.FrontBogey.PneumaticBrakeForce = 50000
        if i==#trains then
            train.RearBogey.MotorPower = 1
            train.FrontBogey.MotorPower = 0
        else
            train.RearBogey.MotorPower = 0
            train.FrontBogey.MotorPower = 0
        end
        if i==1 then
            train.FrontBogey.BrakeCylinderPressure = 3
            train.RearBogey.BrakeCylinderPressure = 3
        else
            train.FrontBogey.BrakeCylinderPressure = 0
            train.RearBogey.BrakeCylinderPressure = 0
        end
        train.OnCoupled = function(ent)
            CoupledTrains = CoupledTrains + 0.5
            if CoupledTrains==WagNum-1 then StopCoupling() end
        end
    end
    timer.Simple(3+1*#trains,StopCoupling)
    for k,v in pairs(FIXFIXFIX) do SafeRemoveEntity(v) end
end

function Autospawn.TeleportToTrain(ply,Train)
    if not ply then return 1 end
    if not IsValid(ply) then return 2 end
    if not IsValid(Train) then return 3 end
    local DriverSeat = Train.DriverSeat
    if not IsValid(DriverSeat) then RunConsoleCommand("say","Can't seat in train!") return 4 end
    
    if ply:InVehicle() then ply:ExitVehicle() end
    ply:EnterVehicle(DriverSeat)
    ply:SetEyeAngles(DriverSeat:GetParent():LocalToWorldAngles(-DriverSeat:GetAngles()))
end

hook.Add("player_spawn", "SpawnTrainForPlayer", function(data)
    local ply = Player(data.userid) -- Получаем входящего игрока (в консоли вывод в HEXе!)
    local tbl = Autospawn.trace[game.GetMap()] -- Получаем таблицу для спавна
    
    if not tbl then ULib.tsayColor(calling_ply,false,Color(255,0,0),"Нету данных для спавна!") ULib.tsayColor(calling_ply,false,Color(255,0,0),"Необходимо заспавнить состав вручную и написать !start") return end -- Проверка карты в таблице 
    
    GetConVar("metrostroi_maxwagons"):SetInt(tbl.WagonCount) -- Выставляем количество вагонов в соответсвии с картой
    if not IsValid(Autospawn.HeadWagon) then
        Autospawn.TrainSpawn(ply,tbl[1],tbl.WagonCount)
    end
    Autospawn.TeleportToTrain(ply,Autospawn.HeadWagon)
    timer.Simple(10,function()
        Metro81717Signals:startSignals(Autospawn.COMPort)
        net.Start("ZarnSetCam")
        net.Send(ply)
    end)
end)