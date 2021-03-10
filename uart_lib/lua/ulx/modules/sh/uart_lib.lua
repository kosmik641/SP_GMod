function ulx.autospawntrain( calling_ply )
    if not IsValid(calling_ply) then return end
    if not Autospawn then return end
    
    if calling_ply:InVehicle() then
        local veh = calling_ply:GetVehicle():GetParent()
        if Autospawn.HeadWagon ~= veh then Autospawn.HeadWagon = veh end
    else
        local tbl = Autospawn.trace[game.GetMap()] -- Получаем таблицу для спавна
        if not tbl then ULib.tsayColor(calling_ply,false,Color(255,0,0),"Нету данных для спавна! Необходимо заспавнить состав вручную") return end -- Проверка карты в таблице
        if not IsValid(Autospawn.HeadWagon) then
            Autospawn.TrainSpawn(calling_ply,tbl[1],tbl.WagonCount)
        end
        Autospawn.TeleportToTrain(calling_ply,Autospawn.HeadWagon)
    end
    
    timer.Simple(1,function()
        Metro81717Signals:startSignals(Autospawn.COMPort)
        net.Start("ZarnSetCam")
        net.Send(calling_ply)
    end)
end

local autospawntrain = ulx.command( "Metrostroi", "ulx autospawntrain", ulx.autospawntrain, "!start" )
autospawntrain:defaultAccess( ULib.ACCESS_ALL )

autospawntrain:help( "Spawn a train" )