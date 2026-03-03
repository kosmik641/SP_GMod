concommand.Add("zarn_start",function(ply,cmd,args)
    if not args[1] then
        MsgC(Color(255,127,0),"Enter at least one COM port number.\n")
        return
    end
    if not TrainSignals.Module then
        MsgC(Color(255,0,0),"TrainSignals module not loaded!\n")
        return
    end
    TrainSignals.Module.Start(tonumber(args[1]))
end,nil,"Start TrainSignals module. Usage: zarn_start [COM port number]")

concommand.Add("zarn_stop",function(ply,cmd,args)
    if TrainSignals.Module then TrainSignals.Module.Stop() end
end,nil,"Stop TrainSignals module. Usage: zarn_stop")

concommand.Add("zarn_reload_calibrations",function(ply,cmd,args)
    if TrainSignals.Module then TrainSignals.Module.LoadCalibrations(args[1]) end
end,nil,"Reload calibrations from file. Usage: zarn_reload_calibrations [true for print values]")

concommand.Add("zarn_reload_sleeps",function(ply,cmd,args)
    if TrainSignals.Module then TrainSignals.Module.LoadSleepTimings(args[1]) end
end,nil,"Reload sleep timings from file. Usage: zarn_reload_sleeps [true for print values]")

concommand.Add("zarn_force_stop",function(ply,cmd,args)
    if TrainSignals.Module then TrainSignals.Module.ForceStop() end
end,nil,"Force stop without deconfiguration. Usage: zarn_force_stop")