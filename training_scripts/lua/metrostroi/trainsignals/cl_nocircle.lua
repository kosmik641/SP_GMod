-- https://github.com/metrostroi-repo/MetrostroiAddon
-- Code from Metrostroi Subway Simulator
-- Commit: d17cb74

local drawCrosshair
local canDrawCrosshair
local toolTipText
local toolTipColor
local lastAimButtonChange
local lastAimButton

local C_DrawDebug = GetConVar("metrostroi_drawdebug")
local C_DisableHoverText = GetConVar("metrostroi_disablehovertext")
local C_DisableHoverTextPos = GetConVar("metrostroi_disablehovertextpos")
local C_TooltipDelay = GetConVar("metrostroi_tooltip_delay")

local function isValidTrainDriver(ply)
    if IsValid(ply.InMetrostroiTrain) then return ply.InMetrostroiTrain end

    local weapon = IsValid(LocalPlayer():GetActiveWeapon()) and LocalPlayer():GetActiveWeapon():GetClass()
    if weapon ~= "train_kv_wrench" and weapon ~= "train_kv_wrench_gold" then return end

    local train = util.TraceLine({
        start = LocalPlayer():GetPos(),
        endpos = LocalPlayer():GetPos() - LocalPlayer():GetAngles():Up() * 100,
        filter = function( ent ) if ent.ButtonMap ~= nil then return true end end
    }).Entity
    if not IsValid(train) then
        train = util.TraceLine({
            start = LocalPlayer():EyePos(),
            endpos = LocalPlayer():EyePos() + LocalPlayer():EyeAngles():Forward() * 300,
            filter = function( ent ) if ent.ButtonMap ~= nil then return true end end
        }).Entity
    end
    return IsValid(train) and train, true
end

local function WorldToScreen(vWorldPos, vPos, vScale, aRot)
    vWorldPos = vWorldPos - vPos
    vWorldPos:Rotate(Angle(0, -aRot.y, 0))
    vWorldPos:Rotate(Angle(-aRot.p, 0, 0))
    vWorldPos:Rotate(Angle(0, 0, -aRot.r))

    return vWorldPos.x / vScale, (-vWorldPos.y) / vScale
end

local function findAimButton(ply,train)
    local panel,panelDist = nil,1e9
    for kp,pan in pairs(train.ButtonMap) do
        if not train:ShouldDrawPanel(kp) then continue end
        --If player is looking at this panel
        if pan.aimedAt and (pan.buttons or pan.sensor or pan.mouse) and pan.aimedAt < panelDist then
            panel = pan
            panelDist = pan.aimedAt
        end
    end
    if not panel then return false end
    if panel.aimX and panel.aimY and (panel.sensor or panel.mouse) and math.InRangeXY(panel.aimX,panel.aimY,0,0,panel.width,panel.height) then return false,panel.aimX,panel.aimY,panel.system end
    if not panel.buttons then return false end

    local buttonTarget
    for _,button in pairs(panel.buttons) do
        if (train.Hidden[button.PropName] or train.Hidden.button[button.PropName]) and (not train.ClientProps[button.PropName] or not train.ClientProps[button.PropName].config or not train.ClientProps[button.PropName].config.staylabel) then continue end
        if (train.Hidden[button.ID] or train.Hidden.button[button.ID])  and (not train.ClientProps[button.ID] or not train.ClientProps[button.ID].config or not train.ClientProps[button.ID].config.staylabel) then  continue end
        if button.w and button.h then
            if  panel.aimX >= button.x and panel.aimX <= (button.x + button.w) and
                    panel.aimY >= button.y and panel.aimY <= (button.y + button.h) then
                buttonTarget = button
            end
        else
            --If the aim location is withing button radis
            local dist = math.Distance(button.x,button.y,panel.aimX,panel.aimY)
            if dist < (button.radius or 10) then
                buttonTarget = button
            end
        end
    end

    if not buttonTarget then return false end
    return buttonTarget
end

local function LinePlaneIntersect(PlanePos,PlaneNormal,LinePos,LineDir)
    local dot = LineDir:Dot(PlaneNormal)
    local fac = LinePos-PlanePos
    local dis = -PlaneNormal:Dot(fac) / dot
    return LineDir * dis + LinePos
end

local function metrostroiCabinPanel(arguments)
    local ply = LocalPlayer()
    if not IsValid(ply) then return end

    toolTipText = nil
    drawCrosshair = false
    canDrawCrosshair = false

    local train, outside = isValidTrainDriver(ply)
    if not IsValid(train) then return end
    if gui.IsConsoleVisible() or gui.IsGameUIVisible() or IsValid(vgui.GetHoveredPanel()) and not vgui.IsHoveringWorld() and vgui.GetHoveredPanel():GetParent() ~= vgui.GetWorldPanel() then return end
    if train.ButtonMap ~= nil then
        canDrawCrosshair = true
        local plyaimvec
        if outside then
            plyaimvec = ply:GetAimVector()
        else
            local x,y = input.GetCursorPos()
            plyaimvec = gui.ScreenToVector(x,y) -- ply:GetAimVector() is unreliable when in seats
        end

        -- Loop trough every panel
        for k2,panel in pairs(train.ButtonMap) do
            if not train:ShouldDrawPanel(kp2) then continue end
            local pang = train:LocalToWorldAngles(panel.ang)

            if plyaimvec:Dot(pang:Up()) < 0 then
                local campos = not outside and train.CamPos or ply:EyePos()
                local ppos = train:LocalToWorld(panel.pos)-- - Vector(math.Round((not outside and train.HeadAcceleration or 0),2),0,0))
                local isectPos = LinePlaneIntersect(ppos,pang:Up(),campos,plyaimvec)
                local localx,localy = WorldToScreen(isectPos,ppos,panel.scale,pang)

                panel.aimX = localx
                panel.aimY = localy
                if plyaimvec:Dot(isectPos - campos)/(isectPos-campos):Length() > 0 and localx > 0 and localx < panel.width and localy > 0 and localy < panel.height then
                    panel.aimedAt = isectPos:Distance(campos)
                    drawCrosshair = panel.aimedAt
                else
                    panel.aimedAt = false
                end
                panel.outside = outside
            else
                panel.aimedAt = false
            end
        end

        -- Tooltips
        local ttdelay = C_TooltipDelay:GetFloat()
        if C_DisableHoverText:GetInt() == 0 and ttdelay and ttdelay >= 0 then
            local button = findAimButton(ply,train)
            if button and
                ((train.Hidden[button.ID] or train.Hidden[button.PropName]) and (not train.ClientProps[button.ID].config or not train.ClientProps[button.ID].config.staylabel) or
                (train.Hidden.button[button.ID] or train.Hidden.button[button.PropName]) and (not train.ClientProps[button.PropName].config or not train.ClientProps[button.PropName].config.staylabel)) then
                return
            end

            if button ~= lastAimButton then
                lastAimButtonChange = CurTime()
                lastAimButton = button
            end

            if button then
                if ttdelay == 0 or CurTime() - lastAimButtonChange > ttdelay then
                    if C_DrawDebug:GetInt() > 0 then
                        toolTipText,toolTipColor = button.ID,Color(255,0,255)
                    elseif button.plombed then
                        toolTipText,_,toolTipColor = button.plombed(train)
                    else
                        toolTipText,toolTipColor = button.tooltip
                    end

                    if C_DisableHoverTextPos:GetInt() == 0 and button.tooltipState and button.tooltip then
                        toolTipText = toolTipText..button.tooltipState(train)
                    end
                end
            end
        end
    end
end

local scrX,scrY = ScrW(),ScrH()
local function metrostroiDrawCrosshair()
    if not drawCrosshair then return end
    if not IsValid(LocalPlayer()) then return end

    if canDrawCrosshair then
        surface.DrawCircle(scrX/2,scrY/2,4.1,drawCrosshair and Color(255,0,0) or Color(255,255,150))
    end

    if toolTipText ~= nil then
        surface.SetFont("MetrostroiLabels")
        local w,h = surface.GetTextSize("SomeText")
        local height = h*1.1
        local texts = string.Explode("\n",toolTipText)
        surface.SetDrawColor(0,0,0,125)
        for i,v in ipairs(texts) do
            local y = scrY/2+height*(i)
            if #v==0 then continue end
            local w2,h2 = surface.GetTextSize(v)
            surface.DrawRect(scrX/2-w2/2-5, scrY/2-h2/2+height*(i), w2+10, h2)
            draw.SimpleText(v,"MetrostroiLabels",scrX/2,y, toolTipColor or Color(255,255,255),TEXT_ALIGN_CENTER,TEXT_ALIGN_CENTER)
        end
    end
end

timer.Simple(1,function()
    hook.Add("Think","metrostroi-cabin-panel",metrostroiCabinPanel)
    hook.Add("HUDPaint","metrostroi-draw-crosshair-tooltip",metrostroiDrawCrosshair)
end)