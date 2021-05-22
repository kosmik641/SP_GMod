if CLIENT then return end
xContainer = xContainer or {}
xContainerOld = xContainerOld or {}

local cp1251 = {
    ["Ђ"] = 128,["Ѓ"] = 129,["‚"] = 130,["ѓ"] = 131,["„"] = 132,["…"] = 133,["†"] = 134,["‡"] = 135,
    ["€"] = 136,["‰"] = 137,["Љ"] = 138,["‹"] = 139,["Њ"] = 140,["Ќ"] = 141,["Ћ"] = 142,["Џ"] = 143,
    ["ђ"] = 144,["‘"] = 145,["’"] = 146,["“"] = 147,["”"] = 148,["•"] = 149,["–"] = 150,["—"] = 151,
    [""] = 152,["™"] = 153,["љ"] = 154,["›"] = 155,["њ"] = 156,["ќ"] = 157,["ћ"] = 158,["џ"] = 159,
    [" "] = 160,["Ў"] = 161,["ў"] = 162,["Ј"] = 163,["¤"] = 164,["Ґ"] = 165,["¦"] = 166,["§"] = 167,
    ["Ё"] = 168,["©"] = 169,["Є"] = 170,["«"] = 171,["¬"] = 172,["­"] = 173,["®"] = 174,["Ї"] = 175,
    ["°"] = 176,["±"] = 177,["І"] = 178,["і"] = 179,["ґ"] = 180,["µ"] = 181,["¶"] = 182,["·"] = 183,
    ["ё"] = 184,["№"] = 185,["є"] = 186,["»"] = 187,["ј"] = 188,["Ѕ"] = 189,["ѕ"] = 190,["ї"] = 191,
    ["А"] = 192,["Б"] = 193,["В"] = 194,["Г"] = 195,["Д"] = 196,["Е"] = 197,["Ж"] = 198,["З"] = 199,
    ["И"] = 200,["Й"] = 201,["К"] = 202,["Л"] = 203,["М"] = 204,["Н"] = 205,["О"] = 206,["П"] = 207,
    ["Р"] = 208,["С"] = 209,["Т"] = 210,["У"] = 211,["Ф"] = 212,["Х"] = 213,["Ц"] = 214,["Ч"] = 215,
    ["Ш"] = 216,["Щ"] = 217,["Ъ"] = 218,["Ы"] = 219,["Ь"] = 220,["Э"] = 221,["Ю"] = 222,["Я"] = 223,
    ["а"] = 224,["б"] = 225,["в"] = 226,["г"] = 227,["д"] = 228,["е"] = 229,["ж"] = 230,["з"] = 231,
    ["и"] = 232,["й"] = 233,["к"] = 234,["л"] = 235,["м"] = 236,["н"] = 237,["о"] = 238,["п"] = 239,
    ["р"] = 240,["с"] = 241,["т"] = 242,["у"] = 243,["ф"] = 244,["х"] = 245,["ц"] = 246,["ч"] = 247,
    ["ш"] = 248,["щ"] = 249,["ъ"] = 250,["ы"] = 251,["ь"] = 252,["э"] = 253,["ю"] = 254,["я"] = 255,
}

function xContainer.string2byte(str)
    local byte_char = {}
    local byte_char_index = 0
    for p, c in utf8.codes(str) do
        if c > 127 then
            byte_char[byte_char_index] = cp1251[utf8.GetChar(str,byte_char_index+1)]
        else
            byte_char[byte_char_index] = string.byte(str,p,p)
        end
        byte_char_index = byte_char_index + 1
    end
    return byte_char
end

function xContainer:Thread(Train)
    for k,v in pairs(self) do
        if k == "MainController" then -- Контроллер машиниста
            if Train.KV.ControllerPosition ~= v then
                Train.KV:TriggerInput("ControllerSet",v)
            end
        elseif k == "driversTapPos" then -- Кран машиниста
            if Train.Pneumatic.DriverValvePosition ~= v then
                Train.Pneumatic:TriggerInput("BrakeSet",v)
            end
        elseif k == "ReverseController" then -- Реверс
            if Train.KV.ReverserPosition ~= v then
                Train.KVWrenchMode = v ~= 0 and 1 or 0
                Train.KV:TriggerInput("Enabled",1)
                Train.KV:TriggerInput("ReverserSet",v)
            end
        elseif k == "KRU" then -- КРУ
            if Train.KRU.Position ~= v then
                Train.KVWrenchMode = v>0 and 2 or 0
                Train.KRU:TriggerInput("Enabled",Train.KVWrenchMode)
                Train.KRU:TriggerInput("Set",v)
            end
        elseif k == "Button_Signal" then -- Сигнал
            if Train.Horn.Value ~= v then
                Train.Horn:TriggerInput("Engage",v)
            end
        else
            if Train[k] then
                if Train[k].Value ~= v then
                    Train[k]:TriggerInput("Set",v)
                end
            end
        end
    end
end