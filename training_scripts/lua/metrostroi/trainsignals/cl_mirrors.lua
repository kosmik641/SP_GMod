local FPS = 25 -- требуемый FPS
local mirrorLeft  = Vector(460, 76,23)
local mirrorRight = Vector(460,-76,23)

-- Добавляем зеркала
local function addMirrors()
    local Train = scripted_ents.GetStored("gmod_subway_81-717_mvm").t -- "gmod_subway_base", если нужны на все поезда (вместо "gmod_subway_81-717_mvm", с кавычками) 
    
    Train.Mirrors_Old_Initialize = Train.Mirrors_Old_Initialize or Train.Initialize
    function Train:Initialize()
        self:Mirrors_Old_Initialize()
        self.Mirror1 = self:CreateRT("717Mirror1",256,512)
        self.Mirror2 = self:CreateRT("717Mirror2",256,512)
    end
end
timer.Simple(1,addMirrors)

-- Отрисовка зеркал
local C_DrawCams = GetConVar("metrostroi_drawcams")
local scrW = ScrW()
FPS = 1/FPS

hook.Add("HUDPaint","Metrostroi_HUD_Mirrors",function()
    if not C_DrawCams:GetBool() then return end
    local Train = LocalPlayer().InMetrostroiTrain
    if not Train then return end
    if not Train.Mirror1 then return end
    
    -- Рамки
    surface.SetDrawColor(190,150,0) -- Цвет рамки
    surface.DrawRect(0,68,242,516) -- Рамка левая
    surface.DrawRect(scrW - 242,68,242,516) -- Рамка правая

    Metrostroi.RenderCamOnRT(Train,Vector(451,0,14),"MirrorL",FPS,Train.Mirror1,Train,mirrorLeft,Angle(0,164,0),256,512,1.4,25,85)
    Metrostroi.RenderCamOnRT(Train,Vector(451,0,14),"MirrorR",FPS,Train.Mirror2,Train,mirrorRight,Angle(0,196,0),256,512,1.4,50,85)
    
    render.DrawTextureToScreenRect(Train.Mirror1,240,70,-240,512)
    render.DrawTextureToScreenRect(Train.Mirror2,scrW,70,-240,512)
end)