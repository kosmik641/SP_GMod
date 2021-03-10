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
    return byte_char,byte_char_index
end

local btnMap = {
    ["AsotpButton1"] = "IGLA1",
    ["AsotpButton2"] = "IGLA2",
    ["AsotpButton3"] = "IGLA3",
    ["AsotpButton4"] = "IGLA4",
    ["AutomaticBrakeCutOffValve"] = "DriverValveDisconnect",
    --["VOVT"] = 0,--inHardwareSignals.automaticBrakeDisablerSwitch
    ["BVoff"] = "OtklBV",--откл. БВ
    ["BVreturn"] = "VozvratRP",--возврат БВ
    -- ["CabinVentillationSwitch"] = "V13",
    ["CircuitSplitterSwitch"] = "RC1",
    ["ControlES"] = "R_ZS",
    ["ControlSpeaker"] = "R_G",
    ["TrackDecoder"] = "ALSFreq",
    ["DoorCloseSwitch2"] = "VUD2",
    ["DoorsChooseSwitch"] = "DoorSelect",--выбор дверей
    ["EmergencyDoorSwitch"] = "VAD",
    ["EmergencyTractionButton"] = "KAH",
    ["EmergencyTractionSwitch"] = "VAH",
    ["EmergencyVentillationSwitch"] = "V13",
    -- ["EndDoorsSwitch"] = "",
    ["EpvArsCutOffValve"] = "EPK", -- кран ЭПВ-АРС
    ["KSN"] = "KSN",--сигнализация неисправностей
    ["FirstProg717"] = "R_Program1",
    ["SecondProg717"] = "R_Program2",
    ["Headlights"] = "L_4",--Фары
    ["InformerButtonDown"] = "R_ASNPDown",
    ["InformerButtonMenu"] = "R_ASNPMenu",
    ["InformerButtonUp"] = "R_ASNPUp",
    ["InverterProtectionSwitch"] = "ConverterProtection",
    ["K3"] = "Ring",--звонок
    ["KALS"] = "ALS",--АЛС
    ["KARS"] = "ARS",--АРС
    ["KARSR"] = "ARSR",--АРС-Р
    ["KOLD1"] = "KDL",--двери левые
    ["KOLD2"] = "KDLR",
    ["KORD"] = "KDP",--двери правые
    ["KRMK"] = "RezMK",--резервный МК
    ["KRP"] = "KRP",--пуск резервный
    ["KRU"] = 0,--контроллер резервного управления
    ["KRZD"] = "KRZD",--резервн закрытие дверей
    ["LeftDoorOpenSwitch2"] = "VDL",
    ["LightingCabin"] = "L_2",
    ["LightingPanel"] = "L_3",
    ["LightingSalon"] = "L_1",--освещение салона
    ["MainController"] = 0,
    -- ["ManometerCutOffValve"] = "",
    -- ["PantographSwitch"] = "",
    -- ["VKST"] = "",
    ["ParkingBrakeCutOffValve"] = "ParkingBrake",
    -- ["PneumSignalCutOffValve"] = "",
    ["Button_Signal"] = 0,
    ["PowerSupply"] = "BPSNon",
    -- ["ProtectionBattery"] = "",
    -- ["ProtectionControlCircuits"] = "",
    -- ["ProtectionInverter"] = "",
    -- ["ProtectionLighting1"] = "",
    -- ["ProtectionLighting2"] = "",
    -- ["ProtectionMasterSwitch"] = "",
    ["RadioInformer"] = "R_Radio",
    ["RBmain"] = "KVT",-- бдителльность1
    ["RBres"] = "KVTR",--бдительность2
    ["ReverseController"] = 0,
    ["SafetyPedal"] = "PB",
    --["SafetyStair"] = {1}
    ["StopCrane"] = "EmergencyBrakeValve",--стоп кран
    ["driversTapPos"] = 0,
    ["UAVAButton"] = "UAVAC",--уава кран
    ["UNCHSwitch"] = "R_UNch",
    ["UOS"] = "UOS",--УОС
    ["ValveOne"] = "VZ1",
    ["VAVU"] = "OtklAVU",--откл. АВУ
    ["VB"] = "VB", -- Переключатель батареи
    ["VBD"] = "VBD",--откл. бл. дверей
    ["VMK"] = "VMK",--МК
    ["VPSwitch"] = "VP",
    ["VUSSwitch"] = "VUS",
    ["VZD"] = "VUD1",--закрытие дверей

    ["A1"] = "A1", -- Ход
    ["A2"] = "A2", -- Управление РК
    ["A3"] = "A3", -- Ход 3
    ["A4"] = "A4", -- Ход назад
    ["A5"] = "A5", -- Ход вперед
    ["A6"] = "A6", -- Тормоз
    ["A7"] = "A7",
    ["A8"] = "A8", -- Вентиль замещения 2
    ["A9"] = "A9",
    ["A10"] = "A10", -- МК
    ["A11"] = "A11", -- освещение кабины и отсеков
    ["A12"] = "A12", -- Резервное закрытие дверей
    ["A13"] = "A13",
    ["A14"] = "A14",
    ["A15"] = "A15",
    ["A16"] = "A16", -- Закрытие дверей
    ["A17"] = "A17",
    ["A18"] = "A18", -- Восстановление защиты
    ["A19"] = "A19",
    ["A20"] = "A20", -- ЛК2, дифф. реле
    ["A21"] = "A21", -- управление дверями
    ["A22"] = "A22", -- МК
    ["A24"] = "A24", -- контроль заряда
    ["A25"] = "A25",
    ["A26"] = "A26",
    ["A27"] = "A27", -- освещение салона, кабины, стояночный тормоз, лампа РП
    ["A28"] = "A28",
    ["A29"] = "A29",
    ["A30"] = "A30", -- СДРК
    ["A31"] = "A31", -- Открытие вери левые
    ["A32"] = "A32", -- Открытие двери правые
    ["A33"] = "A33",
    ["A37"] = "A37",
    ["A38"] = "A38", -- Сигнальная лампа
    ["A39"] = "A39", -- Пуск вагона
    ["A40"] = "A40", -- Тормоз АРС
    ["A41"] = "A41",
    ["A42"] = "A42", -- АРС
    ["A43"] = "A43",
    ["A44"] = "A44", -- КРУ, МК РУ
    ["A45"] = "A45",
    ["A46"] = "A46",
    ["A47"] = "A47",
    ["A48"] = "A48", -- АРС-МП РПБ
    ["A49"] = "A49",
    ["A50"] = "A50", -- контактор освещения салона
    ["A51"] = "A51",
    ["A52"] = "A52",
    ["A53"] = "A53", -- КВЦ, аварийное освещение
    ["A54"] = "A54", -- Управление поездом
    ["A56"] = "A56", -- АКБ
    ["A57"] = "A57", -- Ход РК
    ["A65"] = "A65", -- основное освещение
    ["A66"] = "A66", -- Отключение БВ
    ["A68"] = "A68",
    ["A70"] = "A70",
    ["A71"] = "A71",
    ["A72"] = "A72", -- Вентиль синхронизации замещения 1
    ["A73"] = "A73", -- Сигнализация неисправности
    ["A74"] = "A74", -- Возврат защиты
    ["A75"] = "A75", -- основное освещение
    ["A76"] = "A76",
    ["A77"] = "A77",
    ["A78"] = "A78",
    ["A79"] = "A79", -- АРС
    ["A80"] = "A80",
    ["A81"] = "A81",
    ["A82"] = "A82",
    ["A83"] = "A83",
    ["AB1"] = "AV1",
    ["AB3"] = "AV3",
    ["AB6"] = "AV6",
    ["AC1"] = "AS1",
    ["AP63"] = "AR63",
    -- ["AIS"] = "AIS",
    ["VU"] = "A84", -- Выключатель управления
}

function xContainer:Thread(Train,driverPly)
    if IsValid(driverPly) then driverPly:SetNW2Bool("UARTWorking",true) end
    for k,v in pairs(btnMap) do
        local inVal = self[k]
        if v ~= 0 then
            if Train[v] then
                if Train[v].Value ~= inVal then
                    -- print(v)
                    Train[v]:TriggerInput("Set",inVal)
                end
            end
        else
            if k == "MainController" then -- Контроллер машиниста
                if Train.KV.ControllerPosition ~= inVal then
                    Train.KV:TriggerInput("ControllerSet",inVal)
                end
            elseif k == "driversTapPos" then -- Кран машиниста
                if Train.Pneumatic.DriverValvePosition ~= inVal then
                    Train.Pneumatic:TriggerInput("BrakeSet",inVal)
                end
            elseif k == "ReverseController" then -- Реверс
                if Train.KV.ReverserPosition ~= inVal then
                    Train.KVWrenchMode = inVal ~= 0 and 1 or 0
                    Train.KV:TriggerInput("Enabled",1)
                    Train.KV:TriggerInput("ReverserSet",inVal)
                end
            elseif k == "KRU" then -- КРУ
                if Train.KRU.Position ~= inVal then
                    Train.KVWrenchMode = inVal>0 and 2 or 0
                    Train.KRU:TriggerInput("Enabled",Train.KVWrenchMode)
                    Train.KRU:TriggerInput("Set",inVal)
                end
            elseif k == "Button_Signal" then -- Сигнал
                if Train.Horn.Value ~= inVal then
                    Train.Horn:TriggerInput("Engage",inVal)
                end
            end
        end
    end
end