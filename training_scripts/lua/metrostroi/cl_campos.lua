Metrostroi.OldCalcViewFunction = Metrostroi.OldCalcViewFunction or nil

local function injectCameraPos()
    if not Metrostroi.OldCalcViewFunction and hook.GetTable()["CalcVehicleView"] then
        Metrostroi.OldCalcViewFunction = hook.GetTable()["CalcVehicleView"]["Metrostroi_TrainView"]
        if not Metrostroi.OldCalcViewFunction then return end
    end

    local Train = scripted_ents.GetStored("gmod_subway_81-717_mvm").t
    Train.Cameras[14] = {Vector(463,0,15),Angle(0,0,0),"Train.Common.Camera0",100}

    hook.Add("CalcVehicleView", "Metrostroi_TrainView", function(seat,ply,tbl)
        local retVal = Metrostroi.OldCalcViewFunction(seat,ply,tbl)
        if not retVal then return end

        local train = ply.InMetrostroiTrain
        local camera = train.Cameras[train.CurrentCamera]
        if not seat:GetThirdPersonMode() and train.CurrentCamera > 0 and camera and camera[4] then
            train.CamPos =  train:LocalToWorld(camera[1])
            train.CamAngles =  train:LocalToWorldAngles(camera[2])
            return {
                origin = train.CamPos,
                angles = train.CamAngles,
                fov = camera[4],
            }
        end

        return retVal
    end)
end

timer.Simple(1,injectCameraPos)