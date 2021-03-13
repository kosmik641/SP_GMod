if SERVER then
    if pcall(require,"uart") then
        if UART then
            include("TrainSignals_Metro/Metro81717Signals.lua")
            util.AddNetworkString("ZarnSetCam")
            concommand.Add("ch",function()
                if not IsValid(Autospawn.HeadWagon) then MsgC(Color(255,0,0),"\nСостав не найден\n") return end
                MsgC(Color(255,255,0),"\nСмена кабины...\n")
                local ply = Autospawn.HeadWagon.DriverSeat:GetDriver()
                ulx.changecab( ply )
                Autospawn.HeadWagon:SetNW2Bool("UARTWorking",false)
                Metro81717Signals.Initialized = false
                Autospawn.HeadWagon = Autospawn.HeadWagon.WagonList[#Autospawn.HeadWagon.WagonList]
                timer.Simple(1.1,function() Metro81717Signals:startSignals(Autospawn.COMPort) net.Start("ZarnSetCam") net.Send(ply) end)
            end)

            concommand.Add("com",function(ply,cmd,args)
                if not args[1] then print("COM State = ",UART.GetCOMState()) print("Enter COM port number, type -1 for disconnect") return end
                local port = tonumber(args[1]) or -1
                if port < 0 then
                    UART.StopCOM()
                    print("COM closed")
                else
                    local res = UART.StartCOM(args[1])
                    print("COM result:",res)
                end
            end,nil,"Connect to COM device, type -1 for disconnect")
        else
            print("Some error while loading DLL!")
        end
    else
        print("DLL not found!")
    end
end