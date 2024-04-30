local function serverInject()
    local Train = scripted_ents.GetStored("gmod_subway_81-717_mvm").t

    Train.TS_Initialize_Old = Train.TS_Initialize_Old or Train.Initialize
    table.insert(Train.SyncTable,"IGLA1")
    table.insert(Train.SyncTable,"IGLA2")
    table.insert(Train.SyncTable,"IGLA3")
    table.insert(Train.SyncTable,"IGLA4")

    function Train:Initialize()
        self:TS_Initialize_Old()
        self.Plombs.Init = false

        for k,v in pairs(self.SyncTable) do
            local syncBtn = self[v]
            if (syncBtn) then
                self:SetPackedBool(v,syncBtn.Value)
            end
        end
    end
end

timer.Simple(1,serverInject)