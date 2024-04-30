Metrostroi.MuteSounds = Metrostroi.MuteSounds or false

local muteSounds = {
    ["av_on"] = true,
    ["av_off"] = true,
    ["switch_on"] = true,
    ["switch_off"] = true,
    ["switchbl_off"] = true,
    ["switchbl_on"] = true,
    ["button1_on"] = true,
    ["button1_off"] = true,
    ["button2_on"] = true,
    ["button2_off"] = true,
    ["button3_on"] = true,
    ["button3_off"] = true,
    ["vu224_on"] = true,
    ["vu224_off"] = true,
    ["br_013"] = true,
    ["pb_on"] = true,
    ["pb_off"] = true,
    ["pak_off"] = true,
    ["pak_on"] = true,
    ["revers_in"] = true,
    ["revers_0-f"] = true,
    ["revers_f-0"] = true,
    ["revers_0-b"] = true,
    ["revers_b-0"] = true,
    ["revers_out"] = true,
    ["kru_in"] = true,
    ["kru_out"] = true,
    ["kru_0_1"] = true,
    ["kru_1_2"] = true,
    ["kru_2_1"] = true,
    ["kru_1_0"] = true,
    ["kv70_0_x1"] = true,
    ["kv70_x1_x2"] = true,
    ["kv70_x2_x3"] = true,
    ["kv70_x3_x2"] = true,
    ["kv70_x2_x1"] = true,
    ["kv70_x1_0"] = true,
    ["kv70_0_t1"] = true,
    ["kv70_t1_t1a"] = true,
    ["kv70_t1a_t2"] = true,
    ["kv70_t2_t1a"] = true,
    ["kv70_t1a_t1"] = true,
    ["kv70_t1_0"] = true,
    ["R_ASNPOn"] = true,
    ["pnm_button1_on"] = true,
    ["pnm_button1_off"] = true,
    ["pnm_button2_on"] = true,
    ["pnm_button2_off"] = true,
    ["triple_0-down"] = true,
    ["triple_down-0"] = true,
    ["triple_0-up"] = true,
    ["triple_up-0"] = true,
    ["triple_up-0"] = true,
    ["triple_up-0"] = true,
    ["igla_on"] = true,
    ["igla_off"] = true,
    ["PVK"] = true,
}

local function injectPlayOnce()
    local TrainBase = scripted_ents.GetStored("gmod_subway_base").t
    TrainBase.PlayOnce_Old = TrainBase.PlayOnce_Old or TrainBase.PlayOnce
    
    function TrainBase:PlayOnce(soundid,location,range,pitch,randoff)
        if Metrostroi.MuteSounds then
            -- print("soundid",soundid)
            if muteSounds[soundid] then return end

            -- For interactive elements (aka buttons, switches etc.)
            local clientSounds = self.ClientSounds
            if clientSounds and clientSounds[soundid] then
                local entsound = clientSounds[soundid]
                for i,esnd in ipairs(entsound) do
                    local cl_soundid = esnd[2](self,range,location)
                    -- print("cl_soundid",cl_soundid)
                    if muteSounds[cl_soundid] then return end
                end
            end
        end
        self:PlayOnce_Old(soundid,location,range,pitch,randoff)
    end
end
timer.Simple(1,injectPlayOnce)