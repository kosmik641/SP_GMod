local scriptPath = "metrostroi/trainsignals/"

if SERVER then
    local files = file.Find(scriptPath.."cl_*.lua", "LUA")
    for _,filename in pairs(files) do AddCSLuaFile(scriptPath..filename) end
elseif CLIENT then
    if TrainSignals then return end -- check if TrainSignals already included from loader

    local files = file.Find(scriptPath.."cl_*.lua", "LUA")
    for _,filename in pairs(files) do include(scriptPath..filename) end
end



