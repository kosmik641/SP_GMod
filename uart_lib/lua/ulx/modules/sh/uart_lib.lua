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
        RunConsoleCommand("uart",8)
        net.Start("ZarnSetCam")
        net.Send(calling_ply)
        hook.Call("MTHelperEnter",nil,calling_ply,Autospawn.HeadWagon)
    end)
end

local autospawntrain = ulx.command( "Metrostroi", "ulx autospawntrain", ulx.autospawntrain, "!start" )
autospawntrain:defaultAccess( ULib.ACCESS_ALL )
autospawntrain:help( "Spawn a train" )

function ulx.stopsignals( calling_ply )
    if not IsValid(calling_ply) then return end
    RunConsoleCommand("uart",-1)
end

local stopsignals = ulx.command( "Metrostroi", "ulx stopsignals", ulx.stopsignals, "!stop" )
stopsignals:defaultAccess( ULib.ACCESS_ALL )
stopsignals:help( "Disconnect from UART" )

function ulx.chCab( calling_ply )
    if not IsValid(calling_ply) then return end
    RunConsoleCommand("ch_cab")
end

local chcab2 = ulx.command( "Metrostroi", "ulx chcab2", ulx.chCab, "!ch" )
chcab2:defaultAccess( ULib.ACCESS_ALL )
chcab2:help( "Change cab" )