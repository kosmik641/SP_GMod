if SERVER then
    if pcall(require,"uart") then
        if UART then
            include("TrainSignals_Metro/Metro81717Signals.lua")
        else
            print("Some error while loading DLL!")
        end
    else
        print("DLL not found!")
    end
else
    timer.Simple(8,function()
        local ply = LocalPlayer()
        timer.Create("Player:ListenTrain",0.25,0,function()
            Metrostroi.UARTConnected = ply:GetNW2Bool("UARTWorking",false)
            if Metrostroi.UARTConnected and not ply.InMetrostroiTrain then
                net.Start("UARTCloser")
                net.SendToServer()
            end
        end)
    end)
end