timer.Simple(1,function()
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
    render.DrawTextureToScreenRect(Train.Mirror2,ScrW() - 240,70,240,512)
end)

-- Установка фикс камеры
net.Receive("ZarnSetCam",function(_,ply)
    local train = LocalPlayer().InMetrostroiTrain
    if not train then return end
    train.CurrentCamera = 14
end)