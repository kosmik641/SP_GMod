if CLIENT then return end
if not Metro81717Signals then
    Metro81717Signals = {}
    Metro81717Signals.circuitBrakersOn = {
        A1 = 0,
        A2 = 0,
        A3 = 0,
        A4 = 0,
        A5 = 0,
        A6 = 0,
        A7 = 0,
        A8 = 0,
        A9 = 0,
        A10 = 0,
        A11 = 0,
        A12 = 0,
        A13 = 0,
        A14 = 0,
        A15 = 0,
        A16 = 0,
        A17 = 0,
        A18 = 0,
        A19 = 0,
        A20 = 0,
        A21 = 0,
        A22 = 0,
        A24 = 0,
        A25 = 0,
        A26 = 0,
        A27 = 0,
        A28 = 0,
        A29 = 0,
        A30 = 0,
        A31 = 0,
        A32 = 0,
        A33 = 0,
        A37 = 0,
        A38 = 0,
        A39 = 0,
        A40 = 0,
        A41 = 0,
        A42 = 0,
        A43 = 0,
        A44 = 0,
        A45 = 0,
        A46 = 0,
        A47 = 0,
        A48 = 0,
        A49 = 0,
        A50 = 0,
        A51 = 0,
        A52 = 0,
        A53 = 0,
        A54 = 0,
        A56 = 0,
        A57 = 0,
        A65 = 0,
        A66 = 0,
        A68 = 0,
        A70 = 0,
        A71 = 0,
        A72 = 0,
        A73 = 0,
        A74 = 0,
        A75 = 0,
        A76 = 0,
        A77 = 0,
        A78 = 0,
        A79 = 0,
        A80 = 0,
        A81 = 0,
        A82 = 0,
        A83 = 0,
        AB1 = 0,
        AB3 = 0,
        AB6 = 0,
        AC1 = 0,
        AP63 = 0,
        AIS = 0,
        VU = 0,
    }

    Metro81717Signals.circuitBrakersOff = {
        A1 = 0,
        A2 = 0,
        A3 = 0,
        A4 = 0,
        A5 = 0,
        A6 = 0,
        A7 = 0,
        A8 = 0,
        A9 = 0,
        A10 = 0,
        A11 = 0,
        A12 = 0,
        A13 = 0,
        A14 = 0,
        A15 = 0,
        A16 = 0,
        A17 = 0,
        A18 = 0,
        A19 = 0,
        A20 = 0,
        A21 = 0,
        A22 = 0,
        A24 = 0,
        A25 = 0,
        A26 = 0,
        A27 = 0,
        A28 = 0,
        A29 = 0,
        A30 = 0,
        A31 = 0,
        A32 = 0,
        A33 = 0,
        A37 = 0,
        A38 = 0,
        A39 = 0,
        A40 = 0,
        A41 = 0,
        A42 = 0,
        A43 = 0,
        A44 = 0,
        A45 = 0,
        A46 = 0,
        A47 = 0,
        A48 = 0,
        A49 = 0,
        A50 = 0,
        A51 = 0,
        A52 = 0,
        A53 = 0,
        A54 = 0,
        A56 = 0,
        A57 = 0,
        A65 = 0,
        A66 = 0,
        A68 = 0,
        A70 = 0,
        A71 = 0,
        A72 = 0,
        A73 = 0,
        A74 = 0,
        A75 = 0,
        A76 = 0,
        A77 = 0,
        A78 = 0,
        A79 = 0,
        A80 = 0,
        A81 = 0,
        A82 = 0,
        A83 = 0,
        AB1 = 0,
        AB3 = 0,
        AB6 = 0,
        AC1 = 0,
        AP63 = 0,
        AIS = 0,
        VU = 0,

    }

    Metro81717Signals.inHardwareSignals = {
        asotpButton1 = 0,
        asotpButton2 = 0,
        asotpButton3 = 0,
        asotpButton4 = 0,
        automaticBrakeCutOffValve = 0,
        automaticBrakeDisablerSwitch = 0,
        BVoff = 0,
        BVreturn = 0,
        cabinVentillationSwitch = 0,
        circuitSplitterSwitch = 0,
        controlES = 0,
        controlSpeaker = 0,
        decoderSwitch = 0,
        doorCloseSwitch2 = 0,
        doorsChooseSwitch = 0,
        emergencyDoorSwitch = 0,
        emergencyTractionButton = 0,
        emergencyTractionSwitch = 0,
        emergencyVentillationSwitch = 0,
        endDoorsSwitch = 0,
        epvArsCutOffValve = 0,
        faultsSignallingButton = 0,
        first1Program = 0,
        first2Program = 0,
        headlights = 0,
        informerButtonDown = 0,
        informerButtonMenu = 0,
        informerButtonUp = 0,
        ARS13V = 0,
        inverterProtectionSwitch = 0,
        K3 = 0,
        KALS = 0,
        KARS = 0,
        KARSR = 0,
        KOLD1 = 0,
        KOLD2 = 0,
        KORD = 0,
        KRMK = 0,
        KRP = 0,
        KRU = 0,
        KRZD = 0,
        leftDoorOpenSwitch2 = 0,
        lightingCabin = 0,
        lightingPanel = 0,
        lightingSalon = 0,
        mainController = 0,
        manometerCutOffValve = 0,
        pantographSwitch = 0,
        parkingBrakeControlSwitch = 0,
        parkingBrakeCutOffValve = 0,
        pneumSignalCutOffValve = 0,
        pneumSignalPedal = 0,
        powerSupply = 0,
        protectionBattery = 0,
        protectionControlCircuits = 0,
        protectionInverter = 0,
        protectionLighting1 = 0,
        protectionLighting2 = 0,
        protectionMasterSwitch = 0,
        radioInformer = 0,
        RBmain = 0,
        RBres = 0,
        reverseController = 0,
        safetyPedal = 0,
        safetyStair = 0,
        second1Program = 0,
        second2Program = 0,
        stopCrane = 0,
        tap313 = 0,
        UAVAButton = 0,
        UNCHSwitch = 0,
        UOS = 0,
        valveOne = 0,
        VAVU = 0,
        VB = 0,
        VBD = 0,
        VMK = 0,
        VPSwitch = 0,
        VUSSwitch = 0,
        VZD = 0,
        -- ADC3Value = 0,
    }

    Metro81717Signals.outHardwareSignals = {
        arrowAmmeter = 0,
        arrowBatteryVoltage = 0,
        arrowBrakeCylinderPressure = 0,
        arrowBrakePipePressure = 0,
        arrowContactVoltage = 0,
        arrowFeedPipePressure = 0,
        LAB = 0,
        lampAsotp4 = 0,
        lampAsotp3 = 0,
        lampAsotp2 = 0,
        lampAsotp1 = 0,
        lampAsotpFault = 0,
        lampAsotpFire = 0,
        lampAvu = 0,
        lampBv = 0,
        lampHeaterControl = 0,
        lampInformerLeft = 0,
        lampInformerMiddle = 0,
        lampInformerRight = 0,
        lampInverterProtection = 0,
        lampIST = 0,
        lampLeftDoors1 = 0,
        lampLeftDoors2 = 0,
        lampPneumBrakes = 0,
        lampRightDoors = 0,
        LAV = 0,
        LAV1 = 0,
        LKVP = 0,
        UAVAEmergencyBrakeValve = 0,
        UAVAFeedValve = 0,
        UAVAServiceBrakeValve = 0,
        textAsotp = "",
        textInformer = "",
        vfact = 0,
        L0CH = 0,
        L00 = 0,
        L40 = 0,
        L60 = 0,
        L70 = 0,
        L80 = 0,
        LSD = 0,
        LHRK = 0,
        LRP = 0,
        LSN = 0,
        LEKK = 0,
        LKVC = 0,
        LN = 0,
        LRS = 0,
        LKVD = 0,
        LVD = 0,
        LKT = 0,
        LST = 0,
    }
end
include( "UARTFrontViewClient.lua" )

local function new_IndicatorSignals(n)
    local tmp = {}
    for i=0,n-1 do
        tmp[i] = {
            map = {}
        }
        for j=0,2 do
            tmp[i].map[j] = {0,0}
        end
    end
    return tmp
end

function Metro81717Signals:InitConfArrays(self)
    -- Количество контроллеров
	self.Number_Of_Controllers = 14
	-- Общее количество АЦП
	self.Nmb_Of_ADC_Signals = 3
	-- Размер массива сигналов стрелок
	self.Nmb_Of_Arrow_Signals = 6
	-- Размер массива сигналов индикаторам
	self.Nmb_Of_Indicator_Signals = 0
	-- Размер массива сигналов САУТ
	self.Nmb_Of_SAUT_Signals = 0
	-- Размер массива сигналов 7SegDec
	self.Nmb_Of_7SegDec_Signals = 1
	-- Количество подключенных текстовых дисплеев
	self.Nmb_Of_Text_Displays = 2
    -- Количество стрелочных приборов на контроллер
	self.Config_Arrow_Signals = {}
	self.Config_Arrow_Signals[0] = 0
	self.Config_Arrow_Signals[1] = 0
	self.Config_Arrow_Signals[2] = 3
	self.Config_Arrow_Signals[3] = 3
	self.Config_Arrow_Signals[4] = 0
	self.Config_Arrow_Signals[5] = 0
	self.Config_Arrow_Signals[6] = 0
	self.Config_Arrow_Signals[7] = 0
	self.Config_Arrow_Signals[8] = 0
	self.Config_Arrow_Signals[9] = 0
	self.Config_Arrow_Signals[10] = 0
	self.Config_Arrow_Signals[11] = 0
	self.Config_Arrow_Signals[12] = 0
	self.Config_Arrow_Signals[13] = 0
    -- Массив конфигурации приборов индикации
	self.Config_Indicator_Signals = new_IndicatorSignals(self.Number_Of_Controllers)
	self.Config_Indicator_Signals[0].map[0][0] = 0
	self.Config_Indicator_Signals[0].map[0][1] = 0
	self.Config_Indicator_Signals[0].map[1][0] = 0
	self.Config_Indicator_Signals[0].map[1][1] = 0
	self.Config_Indicator_Signals[0].map[2][0] = 0
	self.Config_Indicator_Signals[0].map[2][1] = 0
	self.Config_Indicator_Signals[1].map[0][0] = 0
	self.Config_Indicator_Signals[1].map[0][1] = 0
	self.Config_Indicator_Signals[1].map[1][0] = 0
	self.Config_Indicator_Signals[1].map[1][1] = 0
	self.Config_Indicator_Signals[1].map[2][0] = 0
	self.Config_Indicator_Signals[1].map[2][1] = 0
	self.Config_Indicator_Signals[2].map[0][0] = 0
	self.Config_Indicator_Signals[2].map[0][1] = 0
	self.Config_Indicator_Signals[2].map[1][0] = 0
	self.Config_Indicator_Signals[2].map[1][1] = 0
	self.Config_Indicator_Signals[2].map[2][0] = 0
	self.Config_Indicator_Signals[2].map[2][1] = 0
	self.Config_Indicator_Signals[3].map[0][0] = 0
	self.Config_Indicator_Signals[3].map[0][1] = 0
	self.Config_Indicator_Signals[3].map[1][0] = 0
	self.Config_Indicator_Signals[3].map[1][1] = 0
	self.Config_Indicator_Signals[3].map[2][0] = 0
	self.Config_Indicator_Signals[3].map[2][1] = 0
	self.Config_Indicator_Signals[4].map[0][0] = 0
	self.Config_Indicator_Signals[4].map[0][1] = 0
	self.Config_Indicator_Signals[4].map[1][0] = 0
	self.Config_Indicator_Signals[4].map[1][1] = 0
	self.Config_Indicator_Signals[4].map[2][0] = 0
	self.Config_Indicator_Signals[4].map[2][1] = 0
	self.Config_Indicator_Signals[5].map[0][0] = 0
	self.Config_Indicator_Signals[5].map[0][1] = 0
	self.Config_Indicator_Signals[5].map[1][0] = 0
	self.Config_Indicator_Signals[5].map[1][1] = 0
	self.Config_Indicator_Signals[5].map[2][0] = 0
	self.Config_Indicator_Signals[5].map[2][1] = 0
	self.Config_Indicator_Signals[6].map[0][0] = 0
	self.Config_Indicator_Signals[6].map[0][1] = 0
	self.Config_Indicator_Signals[6].map[1][0] = 0
	self.Config_Indicator_Signals[6].map[1][1] = 0
	self.Config_Indicator_Signals[6].map[2][0] = 0
	self.Config_Indicator_Signals[6].map[2][1] = 0
	self.Config_Indicator_Signals[7].map[0][0] = 0
	self.Config_Indicator_Signals[7].map[0][1] = 0
	self.Config_Indicator_Signals[7].map[1][0] = 0
	self.Config_Indicator_Signals[7].map[1][1] = 0
	self.Config_Indicator_Signals[7].map[2][0] = 0
	self.Config_Indicator_Signals[7].map[2][1] = 0
	self.Config_Indicator_Signals[8].map[0][0] = 0
	self.Config_Indicator_Signals[8].map[0][1] = 0
	self.Config_Indicator_Signals[8].map[1][0] = 0
	self.Config_Indicator_Signals[8].map[1][1] = 0
	self.Config_Indicator_Signals[8].map[2][0] = 0
	self.Config_Indicator_Signals[8].map[2][1] = 0
	self.Config_Indicator_Signals[9].map[0][0] = 0
	self.Config_Indicator_Signals[9].map[0][1] = 0
	self.Config_Indicator_Signals[9].map[1][0] = 0
	self.Config_Indicator_Signals[9].map[1][1] = 0
	self.Config_Indicator_Signals[9].map[2][0] = 0
	self.Config_Indicator_Signals[9].map[2][1] = 0
	self.Config_Indicator_Signals[10].map[0][0] = 0
	self.Config_Indicator_Signals[10].map[0][1] = 0
	self.Config_Indicator_Signals[10].map[1][0] = 0
	self.Config_Indicator_Signals[10].map[1][1] = 0
	self.Config_Indicator_Signals[10].map[2][0] = 0
	self.Config_Indicator_Signals[10].map[2][1] = 0
	self.Config_Indicator_Signals[11].map[0][0] = 0
	self.Config_Indicator_Signals[11].map[0][1] = 0
	self.Config_Indicator_Signals[11].map[1][0] = 0
	self.Config_Indicator_Signals[11].map[1][1] = 0
	self.Config_Indicator_Signals[11].map[2][0] = 0
	self.Config_Indicator_Signals[11].map[2][1] = 0
	self.Config_Indicator_Signals[12].map[0][0] = 0
	self.Config_Indicator_Signals[12].map[0][1] = 0
	self.Config_Indicator_Signals[12].map[1][0] = 0
	self.Config_Indicator_Signals[12].map[1][1] = 0
	self.Config_Indicator_Signals[12].map[2][0] = 0
	self.Config_Indicator_Signals[12].map[2][1] = 0
	self.Config_Indicator_Signals[13].map[0][0] = 0
	self.Config_Indicator_Signals[13].map[0][1] = 0
	self.Config_Indicator_Signals[13].map[1][0] = 0
	self.Config_Indicator_Signals[13].map[1][1] = 0
	self.Config_Indicator_Signals[13].map[2][0] = 0
	self.Config_Indicator_Signals[13].map[2][1] = 0
    -- Массив конфигурации приборов САУТ
    self.Config_SAUT_Signals = {}
	self.Config_SAUT_Signals[0] = 0
	self.Config_SAUT_Signals[1] = 0
	self.Config_SAUT_Signals[2] = 0
	self.Config_SAUT_Signals[3] = 0
	self.Config_SAUT_Signals[4] = 0
	self.Config_SAUT_Signals[5] = 0
	self.Config_SAUT_Signals[6] = 0
	self.Config_SAUT_Signals[7] = 0
	self.Config_SAUT_Signals[8] = 0
	self.Config_SAUT_Signals[9] = 0
	self.Config_SAUT_Signals[10] = 0
	self.Config_SAUT_Signals[11] = 0
	self.Config_SAUT_Signals[12] = 0
	self.Config_SAUT_Signals[13] = 0
    -- Массив конфигурации 7-ми сегментного индикатора скорости для метро (через дешифратор К514ИД2)
    for i=0,13 do
        self.Config_7SegDec_Signals[i] = {}
        for j=0,2 do
            self.Config_7SegDec_Signals[i][j] = 0
        end
    end
	self.Config_7SegDec_Signals[8][0] = 1
    -- Массив конфигурации текстовых дисплеев. Число, отличное от нуля, обозначает кол-во символов в одной строке дисплея.
    self.Config_Text_Signals[0]=0
	self.Config_Text_Signals[1]=0
	self.Config_Text_Signals[2]=0
	self.Config_Text_Signals[3]=0
	self.Config_Text_Signals[4]=0
	self.Config_Text_Signals[5]=0
	self.Config_Text_Signals[6]=0
	self.Config_Text_Signals[7]=0
	self.Config_Text_Signals[8]=0
	self.Config_Text_Signals[9]=0
	self.Config_Text_Signals[10]=0
	self.Config_Text_Signals[11]=0
	self.Config_Text_Signals[12]=20
	self.Config_Text_Signals[13]=24
    -- Главный конфигурационный массив
    self.Config_Signals_Length = 24 * self.Number_Of_Controllers
    self.Config_Signals = {}
    self.Config_Signals[0] = 0 -- 
	self.Config_Signals[1] = 0 -- 
	self.Config_Signals[2] = 0 -- 
	self.Config_Signals[3] = 0 -- 
	self.Config_Signals[4] = 0 -- 
	self.Config_Signals[5] = 0 -- 
	self.Config_Signals[6] = 0 -- 
	self.Config_Signals[7] = 3 -- Стоп-кран
	self.Config_Signals[8] = 1 -- УНЧ
	self.Config_Signals[9] = 1 -- Контроль ЭС
	self.Config_Signals[10] = 1 -- Контр громког
	self.Config_Signals[11] = 1 -- Радио
	self.Config_Signals[12] = 1 -- 1-2 программы(прав)
	self.Config_Signals[13] = 1 -- 1-2 программы(лев)
	self.Config_Signals[14] = 1 -- Закр дверей
	self.Config_Signals[15] = 1 -- Возвр РП?(тумблер без фиксации)
	self.Config_Signals[16] = 1 -- Вкл мотор компр
	self.Config_Signals[17] = 1 -- Блок питания
	self.Config_Signals[18] = 1 -- Резерв
	self.Config_Signals[19] = 1 -- Компрессор резерв
	self.Config_Signals[20] = 1 -- АРС 13V
	self.Config_Signals[21] = 1 -- Вверх(дисплей)
	self.Config_Signals[22] = 1 -- Вниз(дисплей)
	self.Config_Signals[23] = 1 -- Меню
	self.Config_Signals[24] = 3 -- Кран маш
	self.Config_Signals[25] = 3 -- Контр маш
	self.Config_Signals[26] = 0 -- 
	self.Config_Signals[27] = 0 -- 
	self.Config_Signals[28] = 0 -- 
	self.Config_Signals[29] = 0 -- 
	self.Config_Signals[30] = 0 -- 
	self.Config_Signals[31] = 0 -- 
	self.Config_Signals[32] = 1 -- АРС
	self.Config_Signals[33] = 1 -- АЛС
	self.Config_Signals[34] = 1 -- АРС-Р
	self.Config_Signals[35] = 1 -- Дешифратор
	self.Config_Signals[36] = 1 -- Осв салона
	self.Config_Signals[37] = 1 -- Осв кабины
	self.Config_Signals[38] = 1 -- Осв пульта
	self.Config_Signals[39] = 1 -- ВП
	self.Config_Signals[40] = 1 -- Рез.закр.дв.
	self.Config_Signals[41] = 1 -- Вкл БВ
	self.Config_Signals[42] = 1 -- Дв лев справа
	self.Config_Signals[43] = 1 -- Дв лев слева
	self.Config_Signals[44] = 1 -- Пр-1, лев-0
	self.Config_Signals[45] = 1 -- Бдительность лев
	self.Config_Signals[46] = 1 -- Бдительность прав
	self.Config_Signals[47] = 1 -- АВУ(инв)
	self.Config_Signals[48] = 1 -- Двери торцевые
	self.Config_Signals[49] = 1 -- Вентиляция кабины
	self.Config_Signals[50] = 1 -- Вкл авар сигн
	self.Config_Signals[51] = 1 -- Защита преобр
	self.Config_Signals[52] = 1 -- Сигнализация
	self.Config_Signals[53] = 1 -- Звонок
	self.Config_Signals[54] = 1 -- Откл БВ
	self.Config_Signals[55] = 1 -- Вентиль №1
	self.Config_Signals[56] = 1 -- Пуск резерв
	self.Config_Signals[57] = 1 -- Авар ход
	self.Config_Signals[58] = 1 -- Фары
	self.Config_Signals[59] = 1 -- ВУС
	self.Config_Signals[60] = 1 -- Авар двери(тумблер)
	self.Config_Signals[61] = 1 -- Авар ход(тумблер)
	self.Config_Signals[62] = 1 -- ВКСТ
	self.Config_Signals[63] = 1 -- Двери прав
	self.Config_Signals[64] = 1 -- КРУ1
	self.Config_Signals[65] = 1 -- КРУ2(горят оба)
	self.Config_Signals[66] = 1 -- 
	self.Config_Signals[67] = 1 -- АСОТП1
	self.Config_Signals[68] = 1 -- АСОТП2
	self.Config_Signals[69] = 1 -- АСОТП3
	self.Config_Signals[70] = 1 -- АСОТП4
	self.Config_Signals[71] = 1 -- 
	self.Config_Signals[72] = 1 -- закр дверей
	self.Config_Signals[73] = 1 -- левые двери
	self.Config_Signals[74] = 1 -- 1 программа
	self.Config_Signals[75] = 1 -- 2 программа
	self.Config_Signals[76] = 1 -- Педаль бдительности
	self.Config_Signals[77] = 1 -- Пневмосигнал
	self.Config_Signals[78] = 1 -- Реверс наз
	self.Config_Signals[79] = 1 -- Реверс вп
	self.Config_Signals[80] = 1 -- 
	self.Config_Signals[81] = 1 -- 
	self.Config_Signals[82] = 1 -- 
	self.Config_Signals[83] = 1 -- 
	self.Config_Signals[84] = 1 -- Управление(предохранитель)
	self.Config_Signals[85] = 1 -- Батарея(предохранитель)
	self.Config_Signals[86] = 1 -- Без подписи(предохранитель)
	self.Config_Signals[87] = 1 -- Преобразователь(предохранитель)
	self.Config_Signals[88] = 1 -- Освещение1(предохранитель)
	self.Config_Signals[89] = 1 -- Освещение2(предохранитель)
	self.Config_Signals[90] = 1 -- уава
	self.Config_Signals[91] = 1 -- 
	self.Config_Signals[92] = 1 -- 
	self.Config_Signals[93] = 1 -- уава(черкизово)
	self.Config_Signals[94] = 1 -- 
	self.Config_Signals[95] = 1 -- 
	self.Config_Signals[96] = 1 -- РЦ1 вкл
	self.Config_Signals[97] = 1 -- Батареи вкл
	self.Config_Signals[98] = 1 -- УОС выкл
	self.Config_Signals[99] = 1 -- Втр прижат
	self.Config_Signals[100] = 1 -- Весь состав
	self.Config_Signals[101] = 1 -- 1 пол
	self.Config_Signals[102] = 1 -- 2 пол
	self.Config_Signals[103] = 1 -- 
	self.Config_Signals[104] = 1 -- Разобщ кран кран машиниста
	self.Config_Signals[105] = 1 -- Стоян. тормоз
	self.Config_Signals[106] = 1 -- ЭПВ-АРС
	self.Config_Signals[107] = 1 -- Разобщ кран манометров
	self.Config_Signals[108] = 1 -- Разобщ кран пневмосигнала
	self.Config_Signals[109] = 1 -- 
	self.Config_Signals[110] = 1 -- 
	self.Config_Signals[111] = 1 -- 
	self.Config_Signals[112] = 1 -- A54-in
	self.Config_Signals[113] = 1 -- ВУ-in
	self.Config_Signals[114] = 1 -- A10-in
	self.Config_Signals[115] = 1 -- A53-in
	self.Config_Signals[116] = 1 -- A49-in
	self.Config_Signals[117] = 1 -- A27-in
	self.Config_Signals[118] = 1 -- AC1-in
	self.Config_Signals[119] = 1 -- A21-in
	self.Config_Signals[120] = 1 -- A26-in
	self.Config_Signals[121] = 1 -- AP63-in
	self.Config_Signals[122] = 1 -- A17-in
	self.Config_Signals[123] = 1 -- A44-in
	self.Config_Signals[124] = 1 -- A45-in
	self.Config_Signals[125] = 1 -- A11-in
	self.Config_Signals[126] = 1 -- A71-in
	self.Config_Signals[127] = 1 -- A41-in
	self.Config_Signals[128] = 1 -- A74-in
	self.Config_Signals[129] = 1 -- A73-in
	self.Config_Signals[130] = 1 -- A79-in
	self.Config_Signals[131] = 1 -- A42-in
	self.Config_Signals[132] = 1 -- A46-in
	self.Config_Signals[133] = 1 -- A47-in
	self.Config_Signals[134] = 1 -- AB1-in
	self.Config_Signals[135] = 1 -- A29-in
	self.Config_Signals[136] = 1 -- A76-in
	self.Config_Signals[137] = 1 -- A48-in
	self.Config_Signals[138] = 1 -- A56-in
	self.Config_Signals[139] = 1 -- A65-in
	self.Config_Signals[140] = 1 -- A25-in
	self.Config_Signals[141] = 1 -- A30-in
	self.Config_Signals[142] = 1 -- A1-in
	self.Config_Signals[143] = 1 -- A20-in
	self.Config_Signals[144] = 1 -- A32-in
	self.Config_Signals[145] = 1 -- A13-in
	self.Config_Signals[146] = 1 -- A43-in
	self.Config_Signals[147] = 1 -- A31-in
	self.Config_Signals[148] = 1 -- A77-in
	self.Config_Signals[149] = 1 -- A78-in
	self.Config_Signals[150] = 1 -- 
	self.Config_Signals[151] = 1 -- A75-in
	self.Config_Signals[152] = 1 -- A22-in
	self.Config_Signals[153] = 1 -- A8-in
	self.Config_Signals[154] = 1 -- A28-in
	self.Config_Signals[155] = 1 -- A38-in
	self.Config_Signals[156] = 1 -- A14-in
	self.Config_Signals[157] = 1 -- A39-in
	self.Config_Signals[158] = 1 -- A6-in
	self.Config_Signals[159] = 1 -- A70-in
	self.Config_Signals[160] = 1 -- A4-in
	self.Config_Signals[161] = 1 -- A5-in
	self.Config_Signals[162] = 1 -- A2-in
	self.Config_Signals[163] = 1 -- A3-in
	self.Config_Signals[164] = 1 -- A50-in
	self.Config_Signals[165] = 1 -- A52-in
	self.Config_Signals[166] = 1 -- A40-in
	self.Config_Signals[167] = 1 -- A80-in
	self.Config_Signals[168] = 1 -- A66-in
	self.Config_Signals[169] = 1 -- A18-in
	self.Config_Signals[170] = 1 -- A24-in
	self.Config_Signals[171] = 1 -- A19-in
	self.Config_Signals[172] = 1 -- A37-in
	self.Config_Signals[173] = 1 -- A51-in
	self.Config_Signals[174] = 1 -- A12-in
	self.Config_Signals[175] = 1 -- A16-in
	self.Config_Signals[176] = 1 -- A68-in
	self.Config_Signals[177] = 1 -- A72-in
	self.Config_Signals[178] = 1 -- A7-in
	self.Config_Signals[179] = 1 -- A9-in
	self.Config_Signals[180] = 1 -- A57-in
	self.Config_Signals[181] = 1 -- A81-in
	self.Config_Signals[182] = 1 -- A82-in
	self.Config_Signals[183] = 1 -- A15-in
	self.Config_Signals[184] = 1 -- AB6-in
	self.Config_Signals[185] = 1 -- A83-in
	self.Config_Signals[186] = 1 -- A33-in
	self.Config_Signals[187] = 1 -- AB3-in
	self.Config_Signals[188] = 1 -- 
	self.Config_Signals[189] = 1 -- 
	self.Config_Signals[190] = 1 -- 
	self.Config_Signals[191] = 1 -- 
	self.Config_Signals[192] = 2 -- 
	self.Config_Signals[193] = 2 -- 
	self.Config_Signals[194] = 2 -- 
	self.Config_Signals[195] = 2 -- 
	self.Config_Signals[196] = 2 -- 
	self.Config_Signals[197] = 2 -- 
	self.Config_Signals[198] = 2 -- 
	self.Config_Signals[199] = 2 -- 
	self.Config_Signals[200] = 2 -- 0Ч
	self.Config_Signals[201] = 2 -- 0
	self.Config_Signals[202] = 2 -- 40
	self.Config_Signals[203] = 2 -- 60
	self.Config_Signals[204] = 2 -- 70
	self.Config_Signals[205] = 2 -- 80
	self.Config_Signals[206] = 2 -- ЛСД
	self.Config_Signals[207] = 2 -- 
	self.Config_Signals[208] = 2 -- ЛХ"РК"
	self.Config_Signals[209] = 2 -- РП
	self.Config_Signals[210] = 2 -- ЛСН
	self.Config_Signals[211] = 2 -- ЛЭКК
	self.Config_Signals[212] = 2 -- ЛКВЦ
	self.Config_Signals[213] = 2 -- ЛН
	self.Config_Signals[214] = 2 -- РС
	self.Config_Signals[215] = 2 -- ЛКВД
	self.Config_Signals[216] = 2 -- 
	self.Config_Signals[217] = 2 -- 
	self.Config_Signals[218] = 2 -- 
	self.Config_Signals[219] = 2 -- ЛВД
	self.Config_Signals[220] = 2 -- ЛКТ
	self.Config_Signals[221] = 2 -- ЛСТ
	self.Config_Signals[222] = 2 -- 
	self.Config_Signals[223] = 2 -- 
	self.Config_Signals[224] = 2 -- 
	self.Config_Signals[225] = 2 -- 
	self.Config_Signals[226] = 2 -- 
	self.Config_Signals[227] = 2 -- Двери левые слева светодиод
	self.Config_Signals[228] = 2 -- Двери левые справа светодиод
	self.Config_Signals[229] = 2 -- РП светодиод 
	self.Config_Signals[230] = 2 -- ЗЗащита преобраз
	self.Config_Signals[231] = 2 -- Резервный белый
	self.Config_Signals[232] = 2 -- ЛСП
	self.Config_Signals[233] = 2 -- АВУ
	self.Config_Signals[234] = 2 -- ЛКВП
	self.Config_Signals[235] = 2 -- Пневмотормоз
	self.Config_Signals[236] = 2 -- ИСТ
	self.Config_Signals[237] = 2 -- Двери правые
	self.Config_Signals[238] = 2 -- 
	self.Config_Signals[239] = 2 -- 
	self.Config_Signals[240] = 2 -- ВУ-o
	self.Config_Signals[241] = 2 -- A54-o
	self.Config_Signals[242] = 2 -- A53-o
	self.Config_Signals[243] = 2 -- A10-o
	self.Config_Signals[244] = 2 -- A27-o
	self.Config_Signals[245] = 2 -- A49-o
	self.Config_Signals[246] = 2 -- A21-o
	self.Config_Signals[247] = 2 -- AC-1-o
	self.Config_Signals[248] = 2 -- AP63-o
	self.Config_Signals[249] = 2 -- A26-o
	self.Config_Signals[250] = 2 -- A44-o
	self.Config_Signals[251] = 2 -- A17-o
	self.Config_Signals[252] = 2 -- A11-o
	self.Config_Signals[253] = 2 -- A45-o
	self.Config_Signals[254] = 2 -- A41-o
	self.Config_Signals[255] = 2 -- A71-o
	self.Config_Signals[256] = 2 -- A73-o
	self.Config_Signals[257] = 2 -- A74-o
	self.Config_Signals[258] = 2 -- A42-o
	self.Config_Signals[259] = 2 -- A79-o
	self.Config_Signals[260] = 2 -- A47-o
	self.Config_Signals[261] = 2 -- A46-o
	self.Config_Signals[262] = 2 -- A29-o
	self.Config_Signals[263] = 2 -- AB1-o
	self.Config_Signals[264] = 2 -- A48-o
	self.Config_Signals[265] = 2 -- A76-o
	self.Config_Signals[266] = 2 -- A65-o
	self.Config_Signals[267] = 2 -- A56-o
	self.Config_Signals[268] = 2 -- A30-o
	self.Config_Signals[269] = 2 -- A25-o
	self.Config_Signals[270] = 2 -- A20-o
	self.Config_Signals[271] = 2 -- A1-o
	self.Config_Signals[272] = 2 -- A13-o
	self.Config_Signals[273] = 2 -- A32-o
	self.Config_Signals[274] = 2 -- A31-o
	self.Config_Signals[275] = 2 -- A43-o
	self.Config_Signals[276] = 2 -- A78-o
	self.Config_Signals[277] = 2 -- A77-o
	self.Config_Signals[278] = 2 -- A75-o
	self.Config_Signals[279] = 2 -- 
	self.Config_Signals[280] = 2 -- 
	self.Config_Signals[281] = 2 -- 
	self.Config_Signals[282] = 2 -- асотп 4
	self.Config_Signals[283] = 2 -- асотп 3
	self.Config_Signals[284] = 2 -- асотп 2
	self.Config_Signals[285] = 2 -- асотп 1
	self.Config_Signals[286] = 2 -- асотп неиспр
	self.Config_Signals[287] = 2 -- асотп пожар 1
	self.Config_Signals[288] = 2 -- A8-o
	self.Config_Signals[289] = 2 -- A22-o
	self.Config_Signals[290] = 2 -- A38-o
	self.Config_Signals[291] = 2 -- A28-o
	self.Config_Signals[292] = 2 -- A39-o
	self.Config_Signals[293] = 2 -- A14-o
	self.Config_Signals[294] = 2 -- A70-o
	self.Config_Signals[295] = 2 -- A6-o
	self.Config_Signals[296] = 2 -- A5-o
	self.Config_Signals[297] = 2 -- A4-o
	self.Config_Signals[298] = 2 -- A3-o
	self.Config_Signals[299] = 2 -- A2-o
	self.Config_Signals[300] = 2 -- A52-o
	self.Config_Signals[301] = 2 -- A50-o
	self.Config_Signals[302] = 2 -- A80-o
	self.Config_Signals[303] = 2 -- A40-o
	self.Config_Signals[304] = 2 -- A18-o
	self.Config_Signals[305] = 2 -- A66-o
	self.Config_Signals[306] = 2 -- A19-o
	self.Config_Signals[307] = 2 -- A24-o
	self.Config_Signals[308] = 2 -- A51-o
	self.Config_Signals[309] = 2 -- A37-o
	self.Config_Signals[310] = 2 -- A16-o
	self.Config_Signals[311] = 2 -- A12-o
	self.Config_Signals[312] = 2 -- A72-o
	self.Config_Signals[313] = 2 -- A68-o
	self.Config_Signals[314] = 2 -- A9-o
	self.Config_Signals[315] = 2 -- A7-o
	self.Config_Signals[316] = 2 -- A81-o
	self.Config_Signals[317] = 2 -- A57-o
	self.Config_Signals[318] = 2 -- A15-o
	self.Config_Signals[319] = 2 -- A82-o
	self.Config_Signals[320] = 2 -- A83-o
	self.Config_Signals[321] = 2 -- AB6-o
	self.Config_Signals[322] = 2 -- AB3-o
	self.Config_Signals[323] = 2 -- A33-o
	self.Config_Signals[324] = 2 -- 
	self.Config_Signals[325] = 2 -- 
	self.Config_Signals[326] = 2 -- 
	self.Config_Signals[327] = 2 -- 
	self.Config_Signals[328] = 2 -- Клапан 1
	self.Config_Signals[329] = 2 -- Клапан 2
	self.Config_Signals[330] = 2 -- 
	self.Config_Signals[331] = 2 -- Клапан 3
	self.Config_Signals[332] = 2 -- 
	self.Config_Signals[333] = 2 -- 
	self.Config_Signals[334] = 2 -- 
	self.Config_Signals[335] = 2 -- 
    -- PrintTable(self.Config_Indicator_Signals)
end

function Metro81717Signals:stopSignals()
    UARTFrontViewClient:shutdown()
end

Metro81717Signals.adcArrays = {}
-- Стопкран
function Metro81717Signals.Adc1(adc) 
    return adc < 500 and 1 or 0
end

-- Кран машиниста
local pos_0 = 780
local pos_1 = 1353
local pos_2 = 1896
local pos_3 = 2120
local pos_4 = 2325
local pos_5 = 2860
local pos_6 = 3265
function Metro81717Signals.Adc2(adc)
    local res = 1

    if (adc <= (pos_0 + pos_1) / 2) then res = 1
	elseif (adc > (pos_0 + pos_1) / 2 and adc <= (pos_1 + pos_2) / 2) then res = 2
	elseif (adc > (pos_1 + pos_2) / 2 and adc <= (pos_2 + pos_3) / 2) then res = 3
	elseif (adc > (pos_2 + pos_3) / 2 and adc <= (pos_3 + pos_4) / 2) then res = 4
	elseif (adc > (pos_3 + pos_4) / 2 and adc <= (pos_4 + pos_5) / 2) then res = 5
	elseif (adc > (pos_4 + pos_5) / 2 and adc <= (pos_5 + pos_6) / 2) then res = 6
	elseif (adc > (pos_5 + pos_6) / 2) then res = 7 end

	return res
end

-- Контроллер машиниста
local pos_III = 300
local pos_II = 465
local pos_I = 635
local pos_0 = 812
local pos_T1a = 975
local pos_T1b = 1145
local pos_T2 = 1315 --его вообще нет
function Metro81717Signals.Adc3(adc)
    local res = 0
    
    if (adc <= (pos_III + pos_II) / 2) then res = 3
	elseif (adc > (pos_III + pos_II) / 2 and adc <= (pos_II + pos_I) / 2) then res = 2
	elseif (adc > (pos_II + pos_I) / 2 and adc <= (pos_I + pos_0) / 2) then res = 1
	elseif (adc > (pos_I + pos_0) / 2 and adc <= (pos_0 + pos_T1a) / 2) then res = 0
	elseif (adc > (pos_0 + pos_T1a) / 2 and adc <= (pos_T1a + pos_T1b) / 2) then res = -1
	elseif (adc > (pos_T1a + pos_T1b) / 2 and adc <= (pos_T1b + pos_T2) / 2) then res = -2
	elseif (adc > (pos_T1b + pos_T2) / 2) then res = -3 end
    
	return res
end

-- Тормозной цилиндр
function Metro81717Signals.Step1(step)
    local zero_level = 636
    local max_level = 24865
    
    local delta = (max_level - zero_level) / 6.0
    
    return math.floor(math.Clamp(zero_level + (step * delta),0,27999))
end

-- Напорная магистраль
function Metro81717Signals.Step2(step)
    local zero_level = 27750
    local max_level = 3275
    
    local delta = (max_level - zero_level) / 16.0
    
    return math.floor(math.Clamp(zero_level + (step * delta),0,27999))
end

-- Тормозная магистраль
function Metro81717Signals.Step3(step)
    local zero_level = 24968
    local max_level = 478
    
    local delta = (max_level - zero_level) / 16.0
    
    return math.floor(math.Clamp(zero_level + (step * delta),0,27999))
end

-- Амперметр
function Metro81717Signals.Step4(step)
    local min_level = 1422
	local max_level = 9563
	local max_step = 500.0
	local min_step = -500.0
    
    local res = math.Clamp(((max_level - min_level) * step + max_step * min_level - min_step * max_level) / (max_step - min_step),0,27999)
    
    return math.floor(res)
end

-- Киловольтметр
function Metro81717Signals.Step5(step)
    local zero_level = 0
    local max_level = 7802
    
    local delta = (max_level - zero_level) / 1.0

    return math.floor(math.Clamp(zero_level + (step * delta),0,27999))
end

-- Акк.батарея
function Metro81717Signals.Step6(step)
    local zero_level = 150
    local max_level = 8228
    
    local delta = (max_level - zero_level) / 150.0

    return math.floor(math.Clamp(zero_level + (step * delta),0,27999))
end

local lastUpdate = CurTime()
function Metro81717Signals:update(self,Train)
    if CurTime() - lastUpdate < 0.06 then return end
    lastUpdate = CurTime()
    
    Metro81717Signals:outSignals()
    local outHardwareSignals = Metro81717Signals.outHardwareSignals
    local inHardwareSignals = Metro81717Signals.inHardwareSignals
    local circuitBrakersOff = Metro81717Signals.circuitBrakersOff
    
    self.Arrow_Signals[0] = outHardwareSignals.arrowBrakeCylinderPressure
    self.Arrow_Signals[1] = outHardwareSignals.arrowFeedPipePressure
    self.Arrow_Signals[2] = outHardwareSignals.arrowBrakePipePressure
    self.Arrow_Signals[3] = outHardwareSignals.arrowAmmeter
    self.Arrow_Signals[4] = outHardwareSignals.arrowContactVoltage
    self.Arrow_Signals[5] = outHardwareSignals.arrowBatteryVoltage
    
    self._7SegDec_Signals[0] = outHardwareSignals.vfact
    
    self.TextDisplaysSignalsStruct[1].DisplayOn = inHardwareSignals.radioInformer
    self.TextDisplaysSignalsStruct[1].DisplayLedOn = inHardwareSignals.radioInformer
    local textOut,textSize = xContainer.string2byte("АСНП")
    
    for i=0,39 do
        if i == 39 then
            self.TextDisplaysSignalsStruct[1].textBytes[i] = 0
        else
            self.TextDisplaysSignalsStruct[1].textBytes[i] = textOut[i] or 0
        end
    end
    
    self.TextDisplaysSignalsStruct[0].DisplayOn = 1
    self.TextDisplaysSignalsStruct[0].DisplayLedOn = 1
    textOut,textSize = xContainer.string2byte("АСОТП")
    
    for i=0,47 do
        if i == 47 then
            self.TextDisplaysSignalsStruct[0].textBytes[i] = 0
        else
            self.TextDisplaysSignalsStruct[0].textBytes[i] = textOut[i] or 0
        end
    end
    
    self.Output_Signals[200] = outHardwareSignals.L0CH -- 0Ч
	self.Output_Signals[201] = outHardwareSignals.L00 -- 0
	self.Output_Signals[202] = outHardwareSignals.L40 -- 40
	self.Output_Signals[203] = outHardwareSignals.L60 -- 60
	self.Output_Signals[204] = outHardwareSignals.L70 -- 70
	self.Output_Signals[205] = outHardwareSignals.L80 -- 80
	self.Output_Signals[206] = outHardwareSignals.LSD -- ЛСД
	self.Output_Signals[208] = outHardwareSignals.LHRK -- ЛХ"РК"
	self.Output_Signals[209] = outHardwareSignals.LRP -- РП
	self.Output_Signals[210] = outHardwareSignals.LSN -- ЛСН
	self.Output_Signals[211] = outHardwareSignals.LEKK -- ЛЭКК
	self.Output_Signals[212] = outHardwareSignals.LKVC -- ЛКВЦ
	self.Output_Signals[213] = outHardwareSignals.LN -- ЛН
	self.Output_Signals[214] = outHardwareSignals.LRS -- РС
	self.Output_Signals[215] = outHardwareSignals.LKVD -- ЛКВД
	self.Output_Signals[219] = outHardwareSignals.LVD -- ЛВД
	self.Output_Signals[220] = outHardwareSignals.LKT -- ЛКТ
	self.Output_Signals[221] = outHardwareSignals.LST -- ЛСТ
	self.Output_Signals[227] = outHardwareSignals.lampLeftDoors1 -- Двери левые слева светодиод
	self.Output_Signals[228] = outHardwareSignals.lampLeftDoors2 -- Двери левые справа светодиод
	self.Output_Signals[229] = outHardwareSignals.lampBv -- РП светодиод
	self.Output_Signals[230] = outHardwareSignals.lampInverterProtection -- ЗЗащита преобраз
	--self.Output_Signals[231] = outHardwareSignals. -- Резервный белый
	--self.Output_Signals[232] = outHardwareSignals.LSP -- ЛСП
	self.Output_Signals[233] = outHardwareSignals.lampAvu -- АВУ
	self.Output_Signals[234] = outHardwareSignals.LKVP -- ЛКВП
	self.Output_Signals[235] = outHardwareSignals.lampPneumBrakes -- Пневмотормоз
	self.Output_Signals[236] = outHardwareSignals.lampIST -- ИСТ
	self.Output_Signals[237] = outHardwareSignals.lampRightDoors -- Двери правые
	self.Output_Signals[240] = circuitBrakersOff.VU -- ВУ-o
	self.Output_Signals[241] = circuitBrakersOff.A54 -- A54-o
	self.Output_Signals[242] = circuitBrakersOff.A53 -- A53-o
	self.Output_Signals[243] = circuitBrakersOff.A10 -- A10-o
	self.Output_Signals[244] = circuitBrakersOff.A27 -- A27-o
	self.Output_Signals[245] = circuitBrakersOff.A49 -- A49-o
	self.Output_Signals[246] = circuitBrakersOff.A21 -- A21-o
	self.Output_Signals[247] = circuitBrakersOff.AC1 -- AC-1-o
	self.Output_Signals[248] = circuitBrakersOff.AP63 -- AP63-o
	self.Output_Signals[249] = circuitBrakersOff.A26 -- A26-o
	self.Output_Signals[250] = circuitBrakersOff.A44 -- A44-o
	self.Output_Signals[251] = circuitBrakersOff.A17 -- A17-o
	self.Output_Signals[252] = circuitBrakersOff.A11 -- A11-o
	self.Output_Signals[253] = circuitBrakersOff.A45 -- A45-o
	self.Output_Signals[254] = circuitBrakersOff.A41 -- A41-o
	self.Output_Signals[255] = circuitBrakersOff.A71 -- A71-o
	self.Output_Signals[256] = circuitBrakersOff.A73 -- A73-o
	self.Output_Signals[257] = circuitBrakersOff.A74 -- A74-o
	self.Output_Signals[258] = circuitBrakersOff.A42 -- A42-o
	self.Output_Signals[259] = circuitBrakersOff.A79 -- A79-o
	self.Output_Signals[260] = circuitBrakersOff.A47 -- A47-o
	self.Output_Signals[261] = circuitBrakersOff.A46 -- A46-o
	self.Output_Signals[262] = circuitBrakersOff.A29 -- A29-o
	self.Output_Signals[263] = circuitBrakersOff.AB1 -- AB1-o
	self.Output_Signals[264] = circuitBrakersOff.A48 -- A48-o
	self.Output_Signals[265] = circuitBrakersOff.A76 -- A76-o
	self.Output_Signals[266] = circuitBrakersOff.A65 -- A65-o
	self.Output_Signals[267] = circuitBrakersOff.A56 -- A56-o
	self.Output_Signals[268] = circuitBrakersOff.A30 -- A30-o
	self.Output_Signals[269] = circuitBrakersOff.A25 -- A25-o
	self.Output_Signals[270] = circuitBrakersOff.A20 -- A20-o
	self.Output_Signals[271] = circuitBrakersOff.A1 -- A1-o
	self.Output_Signals[272] = circuitBrakersOff.A13 -- A13-o
	self.Output_Signals[273] = circuitBrakersOff.A32 -- A32-o
	self.Output_Signals[274] = circuitBrakersOff.A31 -- A31-o
	self.Output_Signals[275] = circuitBrakersOff.A43 -- A43-o
	self.Output_Signals[276] = circuitBrakersOff.A78 -- A78-o
	self.Output_Signals[277] = circuitBrakersOff.A77 -- A77-o
	self.Output_Signals[278] = circuitBrakersOff.A75 -- A75-o
	self.Output_Signals[282] = outHardwareSignals.lampAsotp4 -- асотп 4
	self.Output_Signals[283] = outHardwareSignals.lampAsotp3 -- асотп 3
	self.Output_Signals[284] = outHardwareSignals.lampAsotp2 -- асотп 2
	self.Output_Signals[285] = outHardwareSignals.lampAsotp1 -- асотп 1
	self.Output_Signals[286] = outHardwareSignals.lampAsotpFault -- асотп неиспр
	self.Output_Signals[287] = outHardwareSignals.lampAsotpFire -- асотп пожар 1
	self.Output_Signals[288] = circuitBrakersOff.A8 -- A8-o
	self.Output_Signals[289] = circuitBrakersOff.A22 -- A22-o
	self.Output_Signals[290] = circuitBrakersOff.A38 -- A38-o
	self.Output_Signals[291] = circuitBrakersOff.A28 -- A28-o
	self.Output_Signals[292] = circuitBrakersOff.A39 -- A39-o
	self.Output_Signals[293] = circuitBrakersOff.A14 -- A14-o
	self.Output_Signals[294] = circuitBrakersOff.A70 -- A70-o
	self.Output_Signals[295] = circuitBrakersOff.A6 -- A6-o
	self.Output_Signals[296] = circuitBrakersOff.A5 -- A5-o
	self.Output_Signals[297] = circuitBrakersOff.A4 -- A4-o
	self.Output_Signals[298] = circuitBrakersOff.A3 -- A3-o
	self.Output_Signals[299] = circuitBrakersOff.A2 -- A2-o
	self.Output_Signals[300] = circuitBrakersOff.A52 -- A52-o
	self.Output_Signals[301] = circuitBrakersOff.A50 -- A50-o
	self.Output_Signals[302] = circuitBrakersOff.A80 -- A80-o
	self.Output_Signals[303] = circuitBrakersOff.A40 -- A40-o
	self.Output_Signals[304] = circuitBrakersOff.A18 -- A18-o
	self.Output_Signals[305] = circuitBrakersOff.A66 -- A66-o
	self.Output_Signals[306] = circuitBrakersOff.A19 -- A19-o
	self.Output_Signals[307] = circuitBrakersOff.A24 -- A24-o
	self.Output_Signals[308] = circuitBrakersOff.A51 -- A51-o
	self.Output_Signals[309] = circuitBrakersOff.A37 -- A37-o
	self.Output_Signals[310] = circuitBrakersOff.A16 -- A16-o
	self.Output_Signals[311] = circuitBrakersOff.A12 -- A12-o
	self.Output_Signals[312] = circuitBrakersOff.A72 -- A72-o
	self.Output_Signals[313] = circuitBrakersOff.A68 -- A68-o
	self.Output_Signals[314] = circuitBrakersOff.A9 -- A9-o
	self.Output_Signals[315] = circuitBrakersOff.A7 -- A7-o
	self.Output_Signals[316] = circuitBrakersOff.A81 -- A81-o
	self.Output_Signals[317] = circuitBrakersOff.A57 -- A57-o
	self.Output_Signals[318] = circuitBrakersOff.A15 -- A15-o
	self.Output_Signals[319] = circuitBrakersOff.A82 -- A82-o
	self.Output_Signals[320] = circuitBrakersOff.A83 -- A83-o
	self.Output_Signals[321] = circuitBrakersOff.AB6 -- AB6-o
	self.Output_Signals[322] = circuitBrakersOff.AB3 -- AB3-o
	self.Output_Signals[323] = circuitBrakersOff.A33 -- A33-o
	self.Output_Signals[328] = outHardwareSignals.UAVAServiceBrakeValve -- Клапан 1
	self.Output_Signals[329] = outHardwareSignals.UAVAFeedValve -- Клапан 2
	self.Output_Signals[331] = outHardwareSignals.UAVAEmergencyBrakeValve -- Клапан 3
    
    self:update()
    
    local this = self
    self = Metro81717Signals
    self.inHardwareSignals.stopCrane = self.Adc1(this.ADC_Signals[0])
    self.inHardwareSignals.tap313 = self.Adc2(this.ADC_Signals[1])
    self.inHardwareSignals.mainController = self.Adc3(this.ADC_Signals[2])
    -- self.inHardwareSignals.ADC3Value = this.ADC_Signals[2]
	self.inHardwareSignals.reverseController = this.Input_Signals[79] - this.Input_Signals[78]
    
    self.inHardwareSignals.UNCHSwitch = this.Input_Signals[8] -- УНЧ
	self.inHardwareSignals.controlES = this.Input_Signals[9] -- Контроль ЭС
	self.inHardwareSignals.controlSpeaker = this.Input_Signals[10] -- Контр громког
	self.inHardwareSignals.radioInformer = this.Input_Signals[11] -- Радио
	self.inHardwareSignals.second1Program = this.Input_Signals[12] -- 1-2 программы(прав)
	self.inHardwareSignals.first1Program = this.Input_Signals[13] -- 1-2 программы(лев)
	self.inHardwareSignals.VZD = this.Input_Signals[14] -- Закр дверей
	--self.Config_Signals[15] = 1 -- Возвр РП?(тумблер без фиксации)
	self.inHardwareSignals.VMK = this.Input_Signals[16] -- Вкл мотор компр
	self.inHardwareSignals.powerSupply = this.Input_Signals[17] -- Блок питания
	--self.Config_Signals[18] = 1 -- Резерв
	self.inHardwareSignals.KRMK = this.Input_Signals[19] -- Компрессор резерв
	self.inHardwareSignals.ARS13V = this.Input_Signals[20] -- АРС 13V
	self.inHardwareSignals.informerButtonUp = this.Input_Signals[21] -- Вверх(дисплей)
	self.inHardwareSignals.informerButtonDown = this.Input_Signals[22] -- Вниз(дисплей)
	self.inHardwareSignals.informerButtonMenu = this.Input_Signals[23] -- Меню
	self.inHardwareSignals.KARS = this.Input_Signals[32] -- АРС
	self.inHardwareSignals.KALS = this.Input_Signals[33] -- АЛС
	self.inHardwareSignals.KARSR = this.Input_Signals[34] -- АРС-Р
	self.inHardwareSignals.decoderSwitch = this.Input_Signals[35] -- Дешифратор
    
    self.inHardwareSignals.lightingSalon = this.Input_Signals[36] -- Осв салона
	self.inHardwareSignals.lightingCabin = this.Input_Signals[37] -- Осв кабины
	self.inHardwareSignals.lightingPanel = this.Input_Signals[38] -- Осв пульта
	self.inHardwareSignals.VPSwitch = this.Input_Signals[39] -- ВП
	self.inHardwareSignals.KRZD = this.Input_Signals[40] -- Рез.закр.дв.
	self.inHardwareSignals.BVreturn = this.Input_Signals[41] -- Вкл БВ
	self.inHardwareSignals.KOLD1 = this.Input_Signals[42] -- Дв лев справа
	self.inHardwareSignals.KOLD2 = this.Input_Signals[43] -- Дв лев слева
	self.inHardwareSignals.doorsChooseSwitch = this.Input_Signals[44] -- Пр-1, лев-0
	self.inHardwareSignals.RBmain = this.Input_Signals[45] -- Бдительность лев
	self.inHardwareSignals.RBres = this.Input_Signals[46] -- Бдительность прав
	self.inHardwareSignals.VAVU = this.Input_Signals[47] -- АВУ
    --this.Input_Signals[48] -- Двери торцевые
	self.inHardwareSignals.emergencyVentillationSwitch = this.Input_Signals[49] -- Вентиляция кабины
	--this.Input_Signals[50] -- Вкл авар сигн
	self.inHardwareSignals.inverterProtectionSwitch = this.Input_Signals[51] -- Защита преобр
    self.inHardwareSignals.faultsSignallingButton = this.Input_Signals[52] -- Сигнализация неисправностей
	self.inHardwareSignals.K3 = this.Input_Signals[53] -- Звонок
	self.inHardwareSignals.BVoff = this.Input_Signals[54] -- Откл БВ
	self.inHardwareSignals.valveOne = this.Input_Signals[55] -- Вентиль №1
	self.inHardwareSignals.KRP = this.Input_Signals[56] -- Пуск резерв
	self.inHardwareSignals.emergencyTractionButton = this.Input_Signals[57] -- Авар ход
	self.inHardwareSignals.headlights = this.Input_Signals[58] -- Фары
	self.inHardwareSignals.VUSSwitch = this.Input_Signals[59] -- ВУС
	self.inHardwareSignals.emergencyDoorSwitch = this.Input_Signals[60] -- Авар двери(тумблер)
	self.inHardwareSignals.emergencyTractionSwitch = this.Input_Signals[61] -- Авар ход(тумблер)
	self.inHardwareSignals.parkingBrakeControlSwitch = this.Input_Signals[62] -- ВКСТ
	self.inHardwareSignals.KORD = this.Input_Signals[63] -- Двери прав
    self.inHardwareSignals.KRU = this.Input_Signals[64] + this.Input_Signals[65] -- КРУ2(горят оба)
    self.inHardwareSignals.asotpButton1 = this.Input_Signals[67] -- АСОТП1
	self.inHardwareSignals.asotpButton2 = this.Input_Signals[68] -- АСОТП2
	self.inHardwareSignals.asotpButton3 = this.Input_Signals[69] -- АСОТП3
	self.inHardwareSignals.asotpButton4 = this.Input_Signals[70] -- АСОТП4
    self.inHardwareSignals.doorCloseSwitch2 = this.Input_Signals[72] -- закрытие дверей (у помощника машиниста на стене)
	self.inHardwareSignals.leftDoorOpenSwitch2 = this.Input_Signals[73] -- левые двери (у помощника машиниста на стене)
	self.inHardwareSignals.first2Program = this.Input_Signals[74] -- 1 программа
	self.inHardwareSignals.second2Program = this.Input_Signals[75] -- 2 программа
	self.inHardwareSignals.safetyPedal = this.Input_Signals[76] -- Педаль безопасности
	self.inHardwareSignals.pneumSignalPedal = this.Input_Signals[77] -- Пневмосигнал
	self.inHardwareSignals.protectionControlCircuits = this.Input_Signals[84] -- Управление(предохранитель)
	self.inHardwareSignals.protectionBattery = this.Input_Signals[85] -- Батарея(предохранитель)
	self.inHardwareSignals.protectionMasterSwitch = this.Input_Signals[86] -- Без подписи(предохранитель)
	self.inHardwareSignals.protectionInverter = this.Input_Signals[87] -- Преобразователь(предохранитель)
	self.inHardwareSignals.protectionLighting1 = this.Input_Signals[88] -- Освещение1(предохранитель)
	self.inHardwareSignals.protectionLighting2 = this.Input_Signals[89] -- Освещение2(предохранитель)
	self.inHardwareSignals.UAVAButton = this.Input_Signals[90] -- уава кнопка
	--Config_Signals[93] = 1 -- уава(черкизово)
	self.inHardwareSignals.circuitSplitterSwitch = this.Input_Signals[96] -- РЦ1 вкл
	self.inHardwareSignals.VB = this.Input_Signals[97] -- Батареи вкл
	self.inHardwareSignals.UOS = this.Input_Signals[98] -- УОС выкл
    self.inHardwareSignals.pantographSwitch = this.Input_Signals[99] + --[[ВТР прижат]] 2 * this.Input_Signals[100] + --[[Весь состав]] 3 * this.Input_Signals[101] + --[[1 пол]] 4 * this.Input_Signals[102] --[[2 пол]]
    self.inHardwareSignals.automaticBrakeCutOffValve = 1-this.Input_Signals[104] -- Разобщ кран кран машиниста
	self.inHardwareSignals.parkingBrakeCutOffValve = 1-this.Input_Signals[105] -- Стоян. тормоз
	self.inHardwareSignals.epvArsCutOffValve = 1-this.Input_Signals[106] -- ЭПВ-АРС
	self.inHardwareSignals.manometerCutOffValve = 1-this.Input_Signals[107] -- Разобщ кран манометров
	self.inHardwareSignals.pneumSignalCutOffValve = 1-this.Input_Signals[108] -- Разобщ кран пневмосигнала
    
    self.circuitBrakersOn.A54 = this.Input_Signals[112] -- A54-in
	self.circuitBrakersOn.VU = this.Input_Signals[113] -- ВУ-in
	self.circuitBrakersOn.A10 = this.Input_Signals[114] -- A10-in
	self.circuitBrakersOn.A53 = this.Input_Signals[115] -- A53-in
	self.circuitBrakersOn.A49 = this.Input_Signals[116] -- A49-in
	self.circuitBrakersOn.A27 = this.Input_Signals[117] -- A27-in
	self.circuitBrakersOn.AC1 = this.Input_Signals[118] -- AC1-in
	self.circuitBrakersOn.A21 = this.Input_Signals[119] -- A21-in
	self.circuitBrakersOn.A26 = this.Input_Signals[120] -- A26-in
	self.circuitBrakersOn.AP63 = this.Input_Signals[121] -- AP63-in
	self.circuitBrakersOn.A17 = this.Input_Signals[122] -- A17-in
	self.circuitBrakersOn.A44 = this.Input_Signals[123] -- A44-in
	self.circuitBrakersOn.A45 = this.Input_Signals[124] -- A45-in
	self.circuitBrakersOn.A11 = this.Input_Signals[125] -- A11-in
	self.circuitBrakersOn.A71 = this.Input_Signals[126] -- A71-in
	self.circuitBrakersOn.A41 = this.Input_Signals[127] -- A41-in
	self.circuitBrakersOn.A74 = this.Input_Signals[128] -- A74-in
	self.circuitBrakersOn.A73 = this.Input_Signals[129] -- A73-in
	self.circuitBrakersOn.A79 = this.Input_Signals[130] -- A79-in
	self.circuitBrakersOn.A42 = this.Input_Signals[131] -- A42-in
	self.circuitBrakersOn.A46 = this.Input_Signals[132] -- A46-in
	self.circuitBrakersOn.A47 = this.Input_Signals[133] -- A47-in
	self.circuitBrakersOn.AB1 = this.Input_Signals[134] -- AB1-in
	self.circuitBrakersOn.A29 = this.Input_Signals[135] -- A29-in
	self.circuitBrakersOn.A76 = this.Input_Signals[136] -- A76-in
	self.circuitBrakersOn.A48 = this.Input_Signals[137] -- A48-in
	self.circuitBrakersOn.A56 = this.Input_Signals[138] -- A56-in
	self.circuitBrakersOn.A65 = this.Input_Signals[139] -- A65-in
	self.circuitBrakersOn.A25 = this.Input_Signals[140] -- A25-in
	self.circuitBrakersOn.A30 = this.Input_Signals[141] -- A30-in
	self.circuitBrakersOn.A1 = this.Input_Signals[142] -- A1-in
	self.circuitBrakersOn.A20 = this.Input_Signals[143] -- A20-in
	self.circuitBrakersOn.A32 = this.Input_Signals[144] -- A32-in
	self.circuitBrakersOn.A13 = this.Input_Signals[145] -- A13-in
	self.circuitBrakersOn.A43 = this.Input_Signals[146] -- A43-in
	self.circuitBrakersOn.A31 = this.Input_Signals[147] -- A31-in
	self.circuitBrakersOn.A77 = this.Input_Signals[148] -- A77-in
	self.circuitBrakersOn.A78 = this.Input_Signals[149] -- A78-in
	self.circuitBrakersOn.A75 = this.Input_Signals[151] -- A75-in
	self.circuitBrakersOn.A22 = this.Input_Signals[152] -- A22-in
	self.circuitBrakersOn.A8 = this.Input_Signals[153] -- A8-in
	self.circuitBrakersOn.A28 = this.Input_Signals[154] -- A28-in
	self.circuitBrakersOn.A38 = this.Input_Signals[155] -- A38-in
	self.circuitBrakersOn.A14 = this.Input_Signals[156] -- A14-in
	self.circuitBrakersOn.A39 = this.Input_Signals[157] -- A39-in
	self.circuitBrakersOn.A6 = this.Input_Signals[158] -- A6-in
	self.circuitBrakersOn.A70 = this.Input_Signals[159] -- A70-in
	self.circuitBrakersOn.A4 = this.Input_Signals[160] -- A4-in
	self.circuitBrakersOn.A5 = this.Input_Signals[161] -- A5-in
	self.circuitBrakersOn.A2 = this.Input_Signals[162] -- A2-in
	self.circuitBrakersOn.A3 = this.Input_Signals[163] -- A3-in
	self.circuitBrakersOn.A50 = this.Input_Signals[164] -- A50-in
	self.circuitBrakersOn.A52 = this.Input_Signals[165] -- A52-in
	self.circuitBrakersOn.A40 = this.Input_Signals[166] -- A40-in
	self.circuitBrakersOn.A80 = this.Input_Signals[167] -- A80-in
	self.circuitBrakersOn.A66 = this.Input_Signals[168]-- A66-in
	self.circuitBrakersOn.A18 = this.Input_Signals[169] -- A18-in
	self.circuitBrakersOn.A24 = this.Input_Signals[170] -- A24-in
	self.circuitBrakersOn.A19 = this.Input_Signals[171] -- A19-in
	self.circuitBrakersOn.A37 = this.Input_Signals[172] -- A37-in
	self.circuitBrakersOn.A51 = this.Input_Signals[173] -- A51-in
	self.circuitBrakersOn.A12 = this.Input_Signals[174] -- A12-in
	self.circuitBrakersOn.A16 = this.Input_Signals[175] -- A16-in
	self.circuitBrakersOn.A68 = this.Input_Signals[176] -- A68-in
	self.circuitBrakersOn.A72 = this.Input_Signals[177] -- A72-in
	self.circuitBrakersOn.A7 = this.Input_Signals[178] -- A7-in
	self.circuitBrakersOn.A9 = this.Input_Signals[179] -- A9-in
	self.circuitBrakersOn.A57 = this.Input_Signals[180] -- A57-in
	self.circuitBrakersOn.A81 = this.Input_Signals[181] -- A81-in
	self.circuitBrakersOn.A82 = this.Input_Signals[182] -- A82-in
	self.circuitBrakersOn.A15 = this.Input_Signals[183] -- A15-in
	self.circuitBrakersOn.AB6 = this.Input_Signals[184] -- AB6-in
	self.circuitBrakersOn.A83 = this.Input_Signals[185] -- A83-in
	self.circuitBrakersOn.A33 = this.Input_Signals[186] -- A33-in
	self.circuitBrakersOn.AB3 = this.Input_Signals[187] -- AB3-in
    
    Metro81717Signals:inSignals()
end

local function xCont(key)
    return xContainer[key] or 0
end

function Metro81717Signals:inSignals()
    local inHardwareSignals = self.inHardwareSignals
    local circuitBrakersOn = self.circuitBrakersOn
    xContainer["AsotpButton1"] = inHardwareSignals.asotpButton1
	xContainer["AsotpButton2"] = inHardwareSignals.asotpButton2
	xContainer["AsotpButton3"] = inHardwareSignals.asotpButton3
	xContainer["AsotpButton4"] = inHardwareSignals.asotpButton4
	xContainer["AutomaticBrakeCutOffValve"] = inHardwareSignals.automaticBrakeCutOffValve
	xContainer["VOVT"] = 1--inHardwareSignals.automaticBrakeDisablerSwitch
	xContainer["BVoff"] = inHardwareSignals.BVoff--откл. БВ
	xContainer["BVreturn"] = inHardwareSignals.BVreturn--возврат БВ
	xContainer["CabinVentillationSwitch"] = inHardwareSignals.cabinVentillationSwitch
	xContainer["CircuitSplitterSwitch"] = inHardwareSignals.circuitSplitterSwitch
	xContainer["ControlES"] = inHardwareSignals.controlES
	xContainer["ControlSpeaker"] = inHardwareSignals.controlSpeaker
	xContainer["TrackDecoder"] = inHardwareSignals.decoderSwitch
	xContainer["DoorCloseSwitch2"] = inHardwareSignals.doorCloseSwitch2
	xContainer["DoorsChooseSwitch"] = inHardwareSignals.doorsChooseSwitch--выбор дверей
	xContainer["EmergencyDoorSwitch"] = inHardwareSignals.emergencyDoorSwitch
	xContainer["EmergencyTractionButton"] = inHardwareSignals.emergencyTractionButton
	xContainer["EmergencyTractionSwitch"] = inHardwareSignals.emergencyTractionSwitch
	xContainer["EmergencyVentillationSwitch"] = inHardwareSignals.emergencyVentillationSwitch
	xContainer["EndDoorsSwitch"] = inHardwareSignals.endDoorsSwitch
	xContainer["EpvArsCutOffValve"] = inHardwareSignals.epvArsCutOffValve -- кран ЭПВ-АРС
	xContainer["KSN"] = inHardwareSignals.faultsSignallingButton--сигнализация неисправностей
	xContainer["FirstProg717"] = (inHardwareSignals.first1Program == 1 or inHardwareSignals.first2Program == 1) and 1 or 0
	xContainer["SecondProg717"] = (inHardwareSignals.second1Program == 1 or inHardwareSignals.second2Program == 1) and 1 or 0
	xContainer["Headlights"] = inHardwareSignals.headlights--Фары
	xContainer["InformerButtonDown"] = inHardwareSignals.informerButtonDown
	xContainer["InformerButtonMenu"] = inHardwareSignals.informerButtonMenu
	xContainer["InformerButtonUp"] = inHardwareSignals.informerButtonUp
	xContainer["InverterProtectionSwitch"] = inHardwareSignals.inverterProtectionSwitch
	xContainer["K3"] = inHardwareSignals.K3--звонок
	xContainer["KALS"] = inHardwareSignals.KALS--АЛС
	xContainer["KARS"] = inHardwareSignals.KARS--АРС
	xContainer["KARSR"] = inHardwareSignals.KARSR--АРС-Р
	xContainer["KOLD1"] = inHardwareSignals.KOLD1--двери левые
	xContainer["KOLD2"] = inHardwareSignals.KOLD2
	xContainer["KORD"] = inHardwareSignals.KORD--двери правые
	xContainer["KRMK"] = inHardwareSignals.KRMK--резервный МК
	xContainer["KRP"] = inHardwareSignals.KRP--пуск резервный
	xContainer["KRU"] = inHardwareSignals.KRU--контроллер резервного управления
	xContainer["KRZD"] = inHardwareSignals.KRZD--резервн закрытие дверей
	xContainer["LeftDoorOpenSwitch2"] = inHardwareSignals.leftDoorOpenSwitch2
	xContainer["LightingCabin"] = inHardwareSignals.lightingCabin
	xContainer["LightingPanel"] = inHardwareSignals.lightingPanel
	xContainer["LightingSalon"] = inHardwareSignals.lightingSalon--освещение салона
	xContainer["MainController"] = inHardwareSignals.mainController
	-- xContainer["ADC3Value"] = inHardwareSignals.ADC3Value
	xContainer["ManometerCutOffValve"] = inHardwareSignals.manometerCutOffValve
	xContainer["PantographSwitch"] = inHardwareSignals.pantographSwitch
	xContainer["VKST"] = inHardwareSignals.parkingBrakeControlSwitch
	xContainer["ParkingBrakeCutOffValve"] = inHardwareSignals.parkingBrakeCutOffValve
	xContainer["PneumSignalCutOffValve"] = inHardwareSignals.pneumSignalCutOffValve
	xContainer["Button_Signal"] = inHardwareSignals.pneumSignalPedal
	xContainer["PowerSupply"] = inHardwareSignals.powerSupply
	xContainer["ProtectionBattery"] = inHardwareSignals.protectionBattery
	xContainer["ProtectionControlCircuits"] = inHardwareSignals.protectionControlCircuits
	xContainer["ProtectionInverter"] = inHardwareSignals.protectionInverter
	xContainer["ProtectionLighting1"] = inHardwareSignals.protectionLighting1
	xContainer["ProtectionLighting2"] = inHardwareSignals.protectionLighting2
	xContainer["ProtectionMasterSwitch"] = inHardwareSignals.protectionMasterSwitch
	xContainer["RadioInformer"] = inHardwareSignals.radioInformer
	xContainer["RBmain"] = inHardwareSignals.RBmain-- бдителльность1
	xContainer["RBres"] = inHardwareSignals.RBres--бдительность2
	xContainer["ReverseController"] = inHardwareSignals.reverseController
	xContainer["SafetyPedal"] = inHardwareSignals.safetyPedal
	xContainer["SafetyStair"] = 1
	xContainer["StopCrane"] = inHardwareSignals.stopCrane--стоп кран
	xContainer["driversTapPos"] = inHardwareSignals.tap313
	xContainer["UAVAButton"] = inHardwareSignals.UAVAButton--уава кран
	xContainer["UNCHSwitch"] = inHardwareSignals.UNCHSwitch
	xContainer["UOS"] = inHardwareSignals.UOS--УОС
	xContainer["ValveOne"] = inHardwareSignals.valveOne
	xContainer["VAVU"] = inHardwareSignals.VAVU--откл. АВУ
	xContainer["VB"] = inHardwareSignals.VB -- Переключатель батареи
	xContainer["VBD"] = inHardwareSignals.VBD--откл. бл. дверей
	xContainer["VMK"] = inHardwareSignals.VMK--МК
	xContainer["VPSwitch"] = inHardwareSignals.VPSwitch
	xContainer["VUSSwitch"] = inHardwareSignals.VUSSwitch
	xContainer["VZD"] = inHardwareSignals.VZD--закрытие дверей

	xContainer["A1"] = circuitBrakersOn.A1 -- Ход
	xContainer["A2"] = circuitBrakersOn.A2 -- Управление РК
	xContainer["A3"] = circuitBrakersOn.A3 -- Ход 3
	xContainer["A4"] = circuitBrakersOn.A4 -- Ход назад
	xContainer["A5"] = circuitBrakersOn.A5 -- Ход вперед
	xContainer["A6"] = circuitBrakersOn.A6 -- Тормоз
	xContainer["A7"] = circuitBrakersOn.A7
	xContainer["A8"] = circuitBrakersOn.A8 -- Вентиль замещения 2
	xContainer["A9"] = circuitBrakersOn.A9
	xContainer["A10"] = circuitBrakersOn.A10 -- МК
	xContainer["A11"] = circuitBrakersOn.A11 -- освещение кабины и отсеков
	xContainer["A12"] = circuitBrakersOn.A12 -- Резервное закрытие дверей
	xContainer["A13"] = circuitBrakersOn.A13
	xContainer["A14"] = circuitBrakersOn.A14
	xContainer["A15"] = circuitBrakersOn.A15
	xContainer["A16"] = circuitBrakersOn.A16 -- Закрытие дверей
	xContainer["A17"] = circuitBrakersOn.A17
	xContainer["A18"] = circuitBrakersOn.A18 -- Восстановление защиты
	xContainer["A19"] = circuitBrakersOn.A19
	xContainer["A20"] = circuitBrakersOn.A20 -- ЛК2, дифф. реле
	xContainer["A21"] = circuitBrakersOn.A21 -- управление дверями
	xContainer["A22"] = circuitBrakersOn.A22 -- МК
	xContainer["A24"] = circuitBrakersOn.A24 -- контроль заряда
	xContainer["A25"] = circuitBrakersOn.A25
	xContainer["A26"] = circuitBrakersOn.A26
	xContainer["A27"] = circuitBrakersOn.A27 -- освещение салона, кабины, стояночный тормоз, лампа РП
	xContainer["A28"] = circuitBrakersOn.A28
	xContainer["A29"] = circuitBrakersOn.A29
	xContainer["A30"] = circuitBrakersOn.A30 -- СДРК
	xContainer["A31"] = circuitBrakersOn.A31 -- Открытие вери левые
	xContainer["A32"] = circuitBrakersOn.A32 -- Открытие двери правые
	xContainer["A33"] = circuitBrakersOn.A33
	xContainer["A37"] = circuitBrakersOn.A37
	xContainer["A38"] = circuitBrakersOn.A38 -- Сигнальная лампа
	xContainer["A39"] = circuitBrakersOn.A39 -- Пуск вагона
	xContainer["A40"] = circuitBrakersOn.A40 -- Тормоз АРС
	xContainer["A41"] = circuitBrakersOn.A41
	xContainer["A42"] = circuitBrakersOn.A42 -- АРС
	xContainer["A43"] = circuitBrakersOn.A43
	xContainer["A44"] = circuitBrakersOn.A44 -- КРУ, МК РУ
	xContainer["A45"] = circuitBrakersOn.A45
	xContainer["A46"] = circuitBrakersOn.A46
	xContainer["A47"] = circuitBrakersOn.A47
	xContainer["A48"] = circuitBrakersOn.A48 -- АРС-МП РПБ
	xContainer["A49"] = circuitBrakersOn.A49
	xContainer["A50"] = circuitBrakersOn.A50 -- контактор освещения салона
	xContainer["A51"] = circuitBrakersOn.A51
	xContainer["A52"] = circuitBrakersOn.A52
	xContainer["A53"] = circuitBrakersOn.A53 -- КВЦ, аварийное освещение
	xContainer["A54"] = circuitBrakersOn.A54 -- Управление поездом
	xContainer["A56"] = circuitBrakersOn.A56 -- АКБ
	xContainer["A57"] = circuitBrakersOn.A57 -- Ход РК
	xContainer["A65"] = circuitBrakersOn.A65 -- основное освещение
	xContainer["A66"] = circuitBrakersOn.A66 -- Отключение БВ
	xContainer["A68"] = circuitBrakersOn.A68
	xContainer["A70"] = circuitBrakersOn.A70
	xContainer["A71"] = circuitBrakersOn.A71
	xContainer["A72"] = circuitBrakersOn.A72 -- Вентиль синхронизации замещения 1
	xContainer["A73"] = circuitBrakersOn.A73 -- Сигнализация неисправности
	xContainer["A74"] = circuitBrakersOn.A74 -- Возврат защиты
	xContainer["A75"] = circuitBrakersOn.A75 -- основное освещение
	xContainer["A76"] = circuitBrakersOn.A76
	xContainer["A77"] = circuitBrakersOn.A77
	xContainer["A78"] = circuitBrakersOn.A78
	xContainer["A79"] = circuitBrakersOn.A79 -- АРС
	xContainer["A80"] = circuitBrakersOn.A80
	xContainer["A81"] = circuitBrakersOn.A81
	xContainer["A82"] = circuitBrakersOn.A82
	xContainer["A83"] = circuitBrakersOn.A83
	xContainer["AB1"] = circuitBrakersOn.AB1
	xContainer["AB3"] = circuitBrakersOn.AB3
	xContainer["AB6"] = circuitBrakersOn.AB6
	xContainer["AC1"] = circuitBrakersOn.AC1
	xContainer["AP63"] = circuitBrakersOn.AP63
	xContainer["AIS"] = circuitBrakersOn.AIS
	xContainer["VU"] = circuitBrakersOn.VU -- Выключатель управления
end

function Metro81717Signals:outSignals()
    self.outHardwareSignals.arrowAmmeter = self.Step4(xCont("Ammeter"))
	self.outHardwareSignals.arrowBatteryVoltage = self.Step6(xCont("BatteryVoltage"))
	self.outHardwareSignals.arrowBrakeCylinderPressure = self.Step1(xCont("tcPressure"))
	self.outHardwareSignals.arrowBrakePipePressure = self.Step3(xCont("tmPressure"))
	self.outHardwareSignals.arrowContactVoltage = self.Step5(xCont("KVolt") / 1000.0)
	self.outHardwareSignals.arrowFeedPipePressure = self.Step2(xCont("nmPressure"))
	self.outHardwareSignals.LAB = xCont("LAB")
	self.outHardwareSignals.lampAsotp1 = xCont("LampAsotp1")
	self.outHardwareSignals.lampAsotp2 = xCont("LampAsotp2")
	self.outHardwareSignals.lampAsotp3 = xCont("LampAsotp3")
	self.outHardwareSignals.lampAsotp4 = xCont("LampAsotp4")
	self.outHardwareSignals.lampAvu = xCont("LAVU")
	self.outHardwareSignals.lampBv = xCont("LBV")
	self.outHardwareSignals.lampHeaterControl = xCont("LSP")
	self.outHardwareSignals.lampInformerLeft = xCont("InformerLampLeft")
	self.outHardwareSignals.lampInformerMiddle = xCont("InformerLampMiddle")
	self.outHardwareSignals.lampInformerRight = xCont("InformerLampRight")
	self.outHardwareSignals.lampInverterProtection = xCont("LampInverterProtection")
	self.outHardwareSignals.lampIST = xCont("LampParkingBrake")
	self.outHardwareSignals.lampLeftDoors1 = xCont("LDoorLeft")
	self.outHardwareSignals.lampLeftDoors2 = xCont("LDoorLeft")
	self.outHardwareSignals.lampPneumBrakes = xCont("LampPneumBrakes")
	self.outHardwareSignals.lampRightDoors = xCont("LDoorRight")
	self.outHardwareSignals.LAV = xCont("LAV")
	self.outHardwareSignals.LAV1 = xCont("LAV1")
	self.outHardwareSignals.LKVP = xCont("LKVP")
	self.outHardwareSignals.UAVAEmergencyBrakeValve = xCont("UAVAEmergency")
	self.outHardwareSignals.UAVAFeedValve = xCont("UAVAFeed")
	self.outHardwareSignals.UAVAServiceBrakeValve = xCont("UAVAService")
	self.outHardwareSignals.textInformer = xCont("InformerText")
	self.outHardwareSignals.textAsotp = xCont("AsotpText")

	self.outHardwareSignals.L00 = xCont("L00")
	self.outHardwareSignals.LKVC = xCont("LKVC")
	self.outHardwareSignals.LRP = xCont("LRP")
	self.outHardwareSignals.LSN = xCont("LSN")
	self.outHardwareSignals.LVD = xCont("LVD")
	self.outHardwareSignals.LST = xCont("LST")
	self.outHardwareSignals.LHRK = xCont("LHRK")
	self.outHardwareSignals.lampLeftDoors1 = xCont("LDoorLeft")
	self.outHardwareSignals.lampRightDoors = xCont("LDoorRight")
	self.outHardwareSignals.lampBv = xCont("LBV")
	self.outHardwareSignals.L0CH = xCont("LOCH")
	self.outHardwareSignals.L40 = xCont("L40")
	self.outHardwareSignals.L60 = xCont("L60")
	self.outHardwareSignals.L70 = xCont("L70")
	self.outHardwareSignals.L80 = xCont("L80")
	self.outHardwareSignals.LRS = xCont("LRV")
	self.outHardwareSignals.LAB = xCont("LAB")
	self.outHardwareSignals.LKVD = xCont("LKVD")
	self.outHardwareSignals.LAV = xCont("LAV")
	self.outHardwareSignals.LAV1 = xCont("LAV1")
	
	self.outHardwareSignals.LSD = xCont("LSD")
	self.outHardwareSignals.lampPneumBrakes = xCont("LampPneumBrakes")
	self.outHardwareSignals.LKVP = xCont("LKVP")
	self.outHardwareSignals.LN = xCont("LN")
	self.outHardwareSignals.LEKK = xCont("LEKK")
	self.outHardwareSignals.LKT = xCont("LKT")
	self.outHardwareSignals.textInformer = xCont("InformerText")
	self.outHardwareSignals.textAsotp = xCont("AsotpText")
	self.outHardwareSignals.vfact = xCont("vfact")
    
    --[[
	self.circuitBrakersOff.A1 = xCont("A1off")
	self.circuitBrakersOff.A2 = xCont("A2off")
	self.circuitBrakersOff.A3 = xCont("A3off")
	self.circuitBrakersOff.A4 = xCont("A4off")
	self.circuitBrakersOff.A5 = xCont("A5off")
	self.circuitBrakersOff.A6 = xCont("A6off")
	self.circuitBrakersOff.A7 = xCont("A7off")
	self.circuitBrakersOff.A8 = xCont("A8off")
	self.circuitBrakersOff.A9 = xCont("A9off")
	self.circuitBrakersOff.A10 = xCont("A10off")
	self.circuitBrakersOff.A11 = xCont("A11off")
	self.circuitBrakersOff.A12 = xCont("A12off")
	self.circuitBrakersOff.A13 = xCont("A13off")
	self.circuitBrakersOff.A14 = xCont("A14off")
	self.circuitBrakersOff.A15 = xCont("A15off")
	self.circuitBrakersOff.A16 = xCont("A16off")
	self.circuitBrakersOff.A17 = xCont("A17off")
	self.circuitBrakersOff.A18 = xCont("A18off")
	self.circuitBrakersOff.A19 = xCont("A19off")
	self.circuitBrakersOff.A20 = xCont("A20off")
	self.circuitBrakersOff.A21 = xCont("A21off")
	self.circuitBrakersOff.A22 = xCont("A22off")
	self.circuitBrakersOff.A24 = xCont("A24off")
	self.circuitBrakersOff.A25 = xCont("A25off")
	self.circuitBrakersOff.A26 = xCont("A26off")
	self.circuitBrakersOff.A27 = xCont("A27off")
	self.circuitBrakersOff.A28 = xCont("A28off")
	self.circuitBrakersOff.A29 = xCont("A29off")
	self.circuitBrakersOff.A30 = xCont("A30off")
	self.circuitBrakersOff.A31 = xCont("A31off")
	self.circuitBrakersOff.A32 = xCont("A32off")
	self.circuitBrakersOff.A33 = xCont("A33off")
	self.circuitBrakersOff.A37 = xCont("A37off")
	self.circuitBrakersOff.A38 = xCont("A38off")
	self.circuitBrakersOff.A39 = xCont("A39off")
	self.circuitBrakersOff.A40 = xCont("A40off")
	self.circuitBrakersOff.A41 = xCont("A41off")
	self.circuitBrakersOff.A42 = xCont("A42off")
	self.circuitBrakersOff.A43 = xCont("A43off")
	self.circuitBrakersOff.A44 = xCont("A44off")
	self.circuitBrakersOff.A45 = xCont("A45off")
	self.circuitBrakersOff.A46 = xCont("A46off")
	self.circuitBrakersOff.A47 = xCont("A47off")
	self.circuitBrakersOff.A48 = xCont("A48off")
	self.circuitBrakersOff.A49 = xCont("A49off")
	self.circuitBrakersOff.A50 = xCont("A50off")
	self.circuitBrakersOff.A51 = xCont("A51off")
	self.circuitBrakersOff.A52 = xCont("A52off")
	self.circuitBrakersOff.A53 = xCont("A53off")
	self.circuitBrakersOff.A54 = xCont("A54off")
	self.circuitBrakersOff.A56 = xCont("A56off")
	self.circuitBrakersOff.A57 = xCont("A57off")
	self.circuitBrakersOff.A65 = xCont("A65off")
	self.circuitBrakersOff.A66 = xCont("A66off")
	self.circuitBrakersOff.A68 = xCont("A68off")
	self.circuitBrakersOff.A70 = xCont("A70off")
	self.circuitBrakersOff.A71 = xCont("A71off")
	self.circuitBrakersOff.A72 = xCont("A72off")
	self.circuitBrakersOff.A73 = xCont("A73off")
	self.circuitBrakersOff.A74 = xCont("A74off")
	self.circuitBrakersOff.A75 = xCont("A75off")
	self.circuitBrakersOff.A76 = xCont("A76off")
	self.circuitBrakersOff.A77 = xCont("A77off")
	self.circuitBrakersOff.A78 = xCont("A78off")
	self.circuitBrakersOff.A79 = xCont("A79off")
	self.circuitBrakersOff.A80 = xCont("A80off")
	self.circuitBrakersOff.A81 = xCont("A81off")
	self.circuitBrakersOff.A82 = xCont("A82off")
	self.circuitBrakersOff.A83 = xCont("A83off")
	self.circuitBrakersOff.AB1 = xCont("AB1off")
	self.circuitBrakersOff.AB3 = xCont("AB3off")
	self.circuitBrakersOff.AB6 = xCont("AB6off")
	self.circuitBrakersOff.AC1 = xCont("AC1off")
	self.circuitBrakersOff.AP63 = xCont("AP63off")
	self.circuitBrakersOff.AIS = xCont("AISoff")
	self.circuitBrakersOff.VU = xCont("VUoff")
    ]]
end

local plombstbl = {"VAH","VP","OtklAVU","OVT","KAH","OtklBV","RC1","UOS","VBD","UAVA","UPPS_On"}

function Metro81717Signals:startSignals(a_port)
    local res = 0
    
    if UART.GetCOMState() ~= 0 then
        res = UARTFrontViewClient:COM_Open(a_port,115200)
        if (res ~= 0) then
            ULib.tsayError(nil,Format("Metro81717Signals.lua: Не удалось открыть COM%d",a_port))
            UART.StopCOM()
            print("Metro81717Signals.lua:",res)
        end
    end
    
    ULib.tsayColor(nil,false,Color(0,255,0),"Metro81717Signals.lua: Подключение установлено")
    Metro81717Signals:InitConfArrays(UARTFrontViewClient)
    UARTFrontViewClient:ConfigSignals()
    
    timer.Simple(1, function()
        lastUpdate = CurTime()
        local Train = Autospawn.HeadWagon
        for i=1,#plombstbl do Train:BrokePlomb(plombstbl[i]) end
    end)
    
    return res
end