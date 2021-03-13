if CLIENT then return end
if not Metro81717Signals then
    Metro81717Signals = {}
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
end

function Metro81717Signals:stopSignals()
    UARTFrontViewClient:shutdown()
end

--[[
Симуляторы и HidType:
81-760 Новогиреево - 0 - Metro81760_01
81-760 Владыкино - 1 - Metro81760_02
81-717 Северное - 2 - Metro81717_01
81-717 Свиблово - 3 - Metro81717_02
81-717 Калужское - 4 - Metro81717_03
81-717 Планерное - 5 - Metro81717_04
81-717 Сокол - 6 - Metro81717_05
81-717 Черкизово - 7 - Metro81717_06
81-717 Печатники - 8 - Metro81717_07
81-717 Выхино - 9 - Metro81717_08
81-717 Минск - 10 - Metro81717_09
]]

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

local function xCont(key)
    return xContainer[key] or 0
end

local lastUpdate = CurTime()
function Metro81717Signals:update(this,Train)
    if CurTime() - lastUpdate < 0.05 then return end
    lastUpdate = CurTime()
    
    this.Arrow_Signals[0] = self.Step1(xCont("tcPressure"))
    this.Arrow_Signals[1] = self.Step2(xCont("nmPressure"))
    this.Arrow_Signals[2] = self.Step3(xCont("tmPressure"))
    this.Arrow_Signals[3] = self.Step4(xCont("Ammeter"))
    this.Arrow_Signals[4] = self.Step5(xCont("KVolt") / 1000.0)
    this.Arrow_Signals[5] = self.Step6(xCont("BatteryVoltage"))
    
    this._7SegDec_Signals[0] = xCont("vfact") -- Скорость
    
    this.TextDisplaysSignalsStruct[0].DisplayOn = 1
    this.TextDisplaysSignalsStruct[0].DisplayLedOn = 1
    this.TextDisplaysSignalsStruct[1].DisplayOn = this.Input_Signals[11] -- Радио
    this.TextDisplaysSignalsStruct[1].DisplayLedOn = this.Input_Signals[11] -- Радио
    local textOutInfor = xContainer.string2byte(xContainer["InformerText"] or "")
    local textOutASOTP = xContainer.string2byte(xContainer["AsotpText"] or "")
    
    for i=0,47 do
        this.TextDisplaysSignalsStruct[0].textBytes[i] = textOutASOTP[i] or 0
        this.TextDisplaysSignalsStruct[1].textBytes[i] = textOutInfor[i] or 0
    end
    
    this.Output_Signals[200] = xCont("LOCH") -- 0Ч
	this.Output_Signals[201] = xCont("L00") -- 0
	this.Output_Signals[202] = xCont("L40") -- 40
	this.Output_Signals[203] = xCont("L60") -- 60
	this.Output_Signals[204] = xCont("L70") -- 70
	this.Output_Signals[205] = xCont("L80") -- 80
	this.Output_Signals[206] = xCont("LSD") -- ЛСД
	this.Output_Signals[208] = xCont("LHRK") -- ЛХ"РК"
	this.Output_Signals[209] = xCont("LRP") -- РП
	this.Output_Signals[210] = xCont("LSN") -- ЛСН
	this.Output_Signals[211] = xCont("LEKK") -- ЛЭКК
	this.Output_Signals[212] = xCont("LKVC") -- ЛКВЦ
	this.Output_Signals[213] = xCont("LN") -- ЛН
	this.Output_Signals[214] = xCont("LRV") -- РС
	this.Output_Signals[215] = xCont("LKVD") -- ЛКВД
	this.Output_Signals[219] = xCont("LVD") -- ЛВД
	this.Output_Signals[220] = xCont("LKT") -- ЛКТ
	this.Output_Signals[221] = xCont("LST") -- ЛСТ
	this.Output_Signals[227] = xCont("LDoorLeft") -- Двери левые слева светодиод
	this.Output_Signals[228] = xCont("LDoorLeft") -- Двери левые справа светодиод
	this.Output_Signals[229] = xCont("LBV") -- РП светодиод
	this.Output_Signals[230] = xCont("LampInverterProtection") -- ЗЗащита преобраз
	--this.Output_Signals[231] = xCont( -- Резервный белый
	this.Output_Signals[232] = xCont("LSP") -- ЛСП
	this.Output_Signals[233] = xCont("LAVU") -- АВУ
	this.Output_Signals[234] = xCont("LKVP") -- ЛКВП
	this.Output_Signals[235] = xCont("LampPneumBrakes") -- Пневмотормоз
	this.Output_Signals[236] = xCont("LampParkingBrake") -- ИСТ
	this.Output_Signals[237] = xCont("LDoorRight") -- Двери правые
	-- this.Output_Signals[240] = 0 -- ВУ-o
	-- this.Output_Signals[241] = 0 -- A54-o
	-- this.Output_Signals[242] = 0 -- A53-o
	-- this.Output_Signals[243] = 0 -- A10-o
	-- this.Output_Signals[244] = 0 -- A27-o
	-- this.Output_Signals[245] = 0 -- A49-o
	-- this.Output_Signals[246] = 0 -- A21-o
	-- this.Output_Signals[247] = 0 -- AC-1-o
	-- this.Output_Signals[248] = 0 -- AP63-o
	-- this.Output_Signals[249] = 0 -- A26-o
	-- this.Output_Signals[250] = 0 -- A44-o
	-- this.Output_Signals[251] = 0 -- A17-o
	-- this.Output_Signals[252] = 0 -- A11-o
	-- this.Output_Signals[253] = 0 -- A45-o
	-- this.Output_Signals[254] = 0 -- A41-o
	-- this.Output_Signals[255] = 0 -- A71-o
	-- this.Output_Signals[256] = 0 -- A73-o
	-- this.Output_Signals[257] = 0 -- A74-o
	-- this.Output_Signals[258] = 0 -- A42-o
	-- this.Output_Signals[259] = 0 -- A79-o
	-- this.Output_Signals[260] = 0 -- A47-o
	-- this.Output_Signals[261] = 0 -- A46-o
	-- this.Output_Signals[262] = 0 -- A29-o
	-- this.Output_Signals[263] = 0 -- AB1-o
	-- this.Output_Signals[264] = 0 -- A48-o
	-- this.Output_Signals[265] = 0 -- A76-o
	-- this.Output_Signals[266] = 0 -- A65-o
	-- this.Output_Signals[267] = 0 -- A56-o
	-- this.Output_Signals[268] = 0 -- A30-o
	-- this.Output_Signals[269] = 0 -- A25-o
	-- this.Output_Signals[270] = 0 -- A20-o
	-- this.Output_Signals[271] = 0 -- A1-o
	-- this.Output_Signals[272] = 0 -- A13-o
	-- this.Output_Signals[273] = 0 -- A32-o
	-- this.Output_Signals[274] = 0 -- A31-o
	-- this.Output_Signals[275] = 0 -- A43-o
	-- this.Output_Signals[276] = 0 -- A78-o
	-- this.Output_Signals[277] = 0 -- A77-o
	-- this.Output_Signals[278] = 0 -- A75-o
	this.Output_Signals[282] = xCont("LampAsotp4") -- асотп 4
	this.Output_Signals[283] = xCont("LampAsotp3") -- асотп 3
	this.Output_Signals[284] = xCont("LampAsotp2") -- асотп 2
	this.Output_Signals[285] = xCont("LampAsotp1") -- асотп 1
	this.Output_Signals[286] = xCont("LampAsotpFault") -- асотп неиспр
	this.Output_Signals[287] = xCont("LampAsotpFire") -- асотп пожар 1
	-- this.Output_Signals[288] = 0 -- A8-o
	-- this.Output_Signals[289] = 0 -- A22-o
	-- this.Output_Signals[290] = 0 -- A38-o
	-- this.Output_Signals[291] = 0 -- A28-o
	-- this.Output_Signals[292] = 0 -- A39-o
	-- this.Output_Signals[293] = 0 -- A14-o
	-- this.Output_Signals[294] = 0 -- A70-o
	-- this.Output_Signals[295] = 0 -- A6-o
	-- this.Output_Signals[296] = 0 -- A5-o
	-- this.Output_Signals[297] = 0 -- A4-o
	-- this.Output_Signals[298] = 0 -- A3-o
	-- this.Output_Signals[299] = 0 -- A2-o
	-- this.Output_Signals[300] = 0 -- A52-o
	-- this.Output_Signals[301] = 0 -- A50-o
	-- this.Output_Signals[302] = 0 -- A80-o
	-- this.Output_Signals[303] = 0 -- A40-o
	-- this.Output_Signals[304] = 0 -- A18-o
	-- this.Output_Signals[305] = 0 -- A66-o
	-- this.Output_Signals[306] = 0 -- A19-o
	-- this.Output_Signals[307] = 0 -- A24-o
	-- this.Output_Signals[308] = 0 -- A51-o
	-- this.Output_Signals[309] = 0 -- A37-o
	-- this.Output_Signals[310] = 0 -- A16-o
	-- this.Output_Signals[311] = 0 -- A12-o
	-- this.Output_Signals[312] = 0 -- A72-o
	-- this.Output_Signals[313] = 0 -- A68-o
	-- this.Output_Signals[314] = 0 -- A9-o
	-- this.Output_Signals[315] = 0 -- A7-o
	-- this.Output_Signals[316] = 0 -- A81-o
	-- this.Output_Signals[317] = 0 -- A57-o
	-- this.Output_Signals[318] = 0 -- A15-o
	-- this.Output_Signals[319] = 0 -- A82-o
	-- this.Output_Signals[320] = 0 -- A83-o
	-- this.Output_Signals[321] = 0 -- AB6-o
	-- this.Output_Signals[322] = 0 -- AB3-o
	-- this.Output_Signals[323] = 0 -- A33-o
	this.Output_Signals[328] = xCont("UAVAService") -- Клапан 1
	this.Output_Signals[329] = xCont("UAVAFeed") -- Клапан 2
	this.Output_Signals[331] = xCont("UAVAEmergency") -- Клапан 3
    
    -- this:update()
    
    xContainer["StopCrane"] = self.Adc1(this.ADC_Signals[0]) --стоп кран
    xContainer["driversTapPos"] = self.Adc2(this.ADC_Signals[1]) --кран машиниста
    xContainer["MainController"] = self.Adc3(this.ADC_Signals[2])
	xContainer["ReverseController"] = this.Input_Signals[79] - this.Input_Signals[78]
    
    xContainer["UNCHSwitch"] = this.Input_Signals[8] -- УНЧ
	xContainer["ControlES"] = this.Input_Signals[9] -- Контроль ЭС
	xContainer["ControlSpeaker"] = this.Input_Signals[10] -- Контр громког
	xContainer["RadioInformer"] = this.Input_Signals[11] -- Радио
	xContainer["SecondProg717"] = this.Input_Signals[12] -- 1-2 программы(прав)
	xContainer["FirstProg717"] = this.Input_Signals[13] -- 1-2 программы(лев)
	xContainer["VZD"] = this.Input_Signals[14] -- Закр дверей
	xContainer["VPR"] = this.Input_Signals[15] -- ВПР
	xContainer["VMK"] = this.Input_Signals[16] -- Вкл мотор компр
	xContainer["PowerSupply"] = this.Input_Signals[17] -- Блок питания
	--self.Config_Signals[18] = 1 -- Резерв
	xContainer["KRMK"] = this.Input_Signals[19] -- Компрессор резерв
	xContainer["ARS13V"] = this.Input_Signals[20] -- АРС 13V
	xContainer["InformerButtonUp"] = this.Input_Signals[21] -- Вверх(дисплей)
	xContainer["InformerButtonDown"] = this.Input_Signals[22] -- Вниз(дисплей)
	xContainer["InformerButtonMenu"] = this.Input_Signals[23] -- Меню
	xContainer["KARS"] = this.Input_Signals[32] -- АРС
	xContainer["KALS"] = this.Input_Signals[33] -- АЛС
	xContainer["KARSR"] = this.Input_Signals[34] -- АРС-Р
	xContainer["TrackDecoder"] = this.Input_Signals[35] -- Дешифратор
    
    xContainer["LightingSalon"] = this.Input_Signals[36] -- Осв салона
	xContainer["LightingCabin"] = this.Input_Signals[37] -- Осв кабины
	xContainer["LightingPanel"] = this.Input_Signals[38] -- Осв пульта
	xContainer["VPSwitch"] = this.Input_Signals[39] -- ВП
	xContainer["KRZD"] = this.Input_Signals[40] -- Рез.закр.дв.
	xContainer["BVreturn"] = this.Input_Signals[41] -- Вкл БВ
	xContainer["KOLD2"] = this.Input_Signals[42] -- Дв лев справа
	xContainer["KOLD1"] = this.Input_Signals[43] -- Дв лев слева
	xContainer["DoorsChooseSwitch"] = this.Input_Signals[44] -- Пр-1, лев-0
	xContainer["RBmain"] = this.Input_Signals[45] -- Бдительность лев
	xContainer["RBres"] = this.Input_Signals[46] -- Бдительность прав
	xContainer["VAVU"] = this.Input_Signals[47] -- АВУ
    --xContainer["EndDoorsSwitch"] = this.Input_Signals[48] -- Двери торцевые
	xContainer["EmergencyVentillationSwitch"] = this.Input_Signals[49] -- Вентиляция кабины
	--this.Input_Signals[50] -- Вкл авар сигн
	xContainer["InverterProtectionSwitch"] = this.Input_Signals[51] -- Защита преобр
    xContainer["KSN"] = this.Input_Signals[52] -- Сигнализация неисправностей
	xContainer["K3"] = this.Input_Signals[53] -- Звонок
	xContainer["BVoff"] = this.Input_Signals[54] -- Откл БВ
	xContainer["ValveOne"] = this.Input_Signals[55] -- Вентиль №1
	xContainer["KRP"] = this.Input_Signals[56] -- Пуск резерв
	xContainer["EmergencyTractionButton"] = this.Input_Signals[57] -- Авар ход
	xContainer["Headlights"] = this.Input_Signals[58] -- Фары
	xContainer["VUSSwitch"] = this.Input_Signals[59] -- ВУС
	xContainer["EmergencyDoorSwitch"] = this.Input_Signals[60] -- Авар двери(тумблер)
	xContainer["EmergencyTractionSwitch"] = this.Input_Signals[61] -- Авар ход(тумблер)
	xContainer["VKST"] = this.Input_Signals[62] -- ВКСТ
	xContainer["KORD"] = this.Input_Signals[63] -- Двери прав
    xContainer["KRU"] = this.Input_Signals[64] + this.Input_Signals[65] -- КРУ2(горят оба)
    xContainer["AsotpButton1"] = this.Input_Signals[67] -- АСОТП1
	xContainer["AsotpButton2"] = this.Input_Signals[68] -- АСОТП2
	xContainer["AsotpButton3"] = this.Input_Signals[69] -- АСОТП3
	xContainer["AsotpButton4"] = this.Input_Signals[70] -- АСОТП4
    xContainer["DoorCloseSwitch2"] = this.Input_Signals[72] -- закрытие дверей (у помощника машиниста на стене)
	xContainer["LeftDoorOpenSwitch2"] = this.Input_Signals[73] -- левые двери (у помощника машиниста на стене)
	xContainer["First2Prog717"] = this.Input_Signals[74] -- 1 программа
	xContainer["Second2Prog717"] = this.Input_Signals[75] -- 2 программа
	xContainer["SafetyPedal"] = this.Input_Signals[76] -- Педаль безопасности
	xContainer["Button_Signal"] = this.Input_Signals[77] -- Пневмосигнал
	xContainer["ProtectionControlCircuits"] = this.Input_Signals[84] -- Управление(предохранитель)
	xContainer["ProtectionBattery"] = this.Input_Signals[85] -- Батарея(предохранитель)
	xContainer["ProtectionMasterSwitch"] = this.Input_Signals[86] -- Без подписи(предохранитель)
	xContainer["ProtectionInverter"] = this.Input_Signals[87] -- Преобразователь(предохранитель)
	xContainer["ProtectionLighting1"] = this.Input_Signals[88] -- Освещение1(предохранитель)
	xContainer["ProtectionLighting2"] = this.Input_Signals[89] -- Освещение2(предохранитель)
	xContainer["UAVAButton"] = this.Input_Signals[90] -- уава кнопка
	--Config_Signals[93] = 1 -- уава(черкизово)
	xContainer["CircuitSplitterSwitch"] = this.Input_Signals[96] -- РЦ1 вкл
	xContainer["VB"] = this.Input_Signals[97] -- Батареи вкл
	xContainer["UOS"] = this.Input_Signals[98] -- УОС выкл
    xContainer["PantographSwitch"] = this.Input_Signals[99] + --[[ВТР прижат]] 2 * this.Input_Signals[100] + --[[Весь состав]] 3 * this.Input_Signals[101] + --[[1 пол]] 4 * this.Input_Signals[102] --[[2 пол]]
    xContainer["AutomaticBrakeCutOffValve"] = 1-this.Input_Signals[104] -- Разобщ кран кран машиниста
	xContainer["ParkingBrakeCutOffValve"] = 1-this.Input_Signals[105] -- Стоян. тормоз
	xContainer["EpvArsCutOffValve"] = 1-this.Input_Signals[106] -- ЭПВ-АРС
	xContainer["ManometerCutOffValve"] = 1-this.Input_Signals[107] -- Разобщ кран манометров
	xContainer["PneumSignalCutOffValve"] = 1-this.Input_Signals[108] -- Разобщ кран пневмосигнала
    
    xContainer["A54"] = this.Input_Signals[112] -- A54-in
	xContainer["VU"] = this.Input_Signals[113] -- ВУ-in
	xContainer["A10"] = this.Input_Signals[114] -- A10-in
	xContainer["A53"] = this.Input_Signals[115] -- A53-in
	xContainer["A49"] = this.Input_Signals[116] -- A49-in
	xContainer["A27"] = this.Input_Signals[117] -- A27-in
	xContainer["AC1"] = this.Input_Signals[118] -- AC1-in
	xContainer["A21"] = this.Input_Signals[119] -- A21-in
	xContainer["A26"] = this.Input_Signals[120] -- A26-in
	xContainer["AP63"] = this.Input_Signals[121] -- AP63-in
	xContainer["A17"] = this.Input_Signals[122] -- A17-in
	xContainer["A44"] = this.Input_Signals[123] -- A44-in
	xContainer["A45"] = this.Input_Signals[124] -- A45-in
	xContainer["A11"] = this.Input_Signals[125] -- A11-in
	xContainer["A71"] = this.Input_Signals[126] -- A71-in
	xContainer["A41"] = this.Input_Signals[127] -- A41-in
	xContainer["A74"] = this.Input_Signals[128] -- A74-in
	xContainer["A73"] = this.Input_Signals[129] -- A73-in
	xContainer["A79"] = this.Input_Signals[130] -- A79-in
	xContainer["A42"] = this.Input_Signals[131] -- A42-in
	xContainer["A46"] = this.Input_Signals[132] -- A46-in
	xContainer["A47"] = this.Input_Signals[133] -- A47-in
	xContainer["AB1"] = this.Input_Signals[134] -- AB1-in
	xContainer["A29"] = this.Input_Signals[135] -- A29-in
	xContainer["A76"] = this.Input_Signals[136] -- A76-in
	xContainer["A48"] = this.Input_Signals[137] -- A48-in
	xContainer["A56"] = this.Input_Signals[138] -- A56-in
	xContainer["A65"] = this.Input_Signals[139] -- A65-in
	xContainer["A25"] = this.Input_Signals[140] -- A25-in
	xContainer["A30"] = this.Input_Signals[141] -- A30-in
	xContainer["A1"] = this.Input_Signals[142] -- A1-in
	xContainer["A20"] = this.Input_Signals[143] -- A20-in
	xContainer["A32"] = this.Input_Signals[144] -- A32-in
	xContainer["A13"] = this.Input_Signals[145] -- A13-in
	xContainer["A43"] = this.Input_Signals[146] -- A43-in
	xContainer["A31"] = this.Input_Signals[147] -- A31-in
	xContainer["A77"] = this.Input_Signals[148] -- A77-in
	xContainer["A78"] = this.Input_Signals[149] -- A78-in
	xContainer["A75"] = this.Input_Signals[151] -- A75-in
	xContainer["A22"] = this.Input_Signals[152] -- A22-in
	xContainer["A8"] = this.Input_Signals[153] -- A8-in
	xContainer["A28"] = this.Input_Signals[154] -- A28-in
	xContainer["A38"] = this.Input_Signals[155] -- A38-in
	xContainer["A14"] = this.Input_Signals[156] -- A14-in
	xContainer["A39"] = this.Input_Signals[157] -- A39-in
	xContainer["A6"] = this.Input_Signals[158] -- A6-in
	xContainer["A70"] = this.Input_Signals[159] -- A70-in
	xContainer["A4"] = this.Input_Signals[160] -- A4-in
	xContainer["A5"] = this.Input_Signals[161] -- A5-in
	xContainer["A2"] = this.Input_Signals[162] -- A2-in
	xContainer["A3"] = this.Input_Signals[163] -- A3-in
	xContainer["A50"] = this.Input_Signals[164] -- A50-in
	xContainer["A52"] = this.Input_Signals[165] -- A52-in
	xContainer["A40"] = this.Input_Signals[166] -- A40-in
	xContainer["A80"] = this.Input_Signals[167] -- A80-in
	xContainer["A66"] = this.Input_Signals[168]-- A66-in
	xContainer["A18"] = this.Input_Signals[169] -- A18-in
	xContainer["A24"] = this.Input_Signals[170] -- A24-in
	xContainer["A19"] = this.Input_Signals[171] -- A19-in
	xContainer["A37"] = this.Input_Signals[172] -- A37-in
	xContainer["A51"] = this.Input_Signals[173] -- A51-in
	xContainer["A12"] = this.Input_Signals[174] -- A12-in
	xContainer["A16"] = this.Input_Signals[175] -- A16-in
	xContainer["A68"] = this.Input_Signals[176] -- A68-in
	xContainer["A72"] = this.Input_Signals[177] -- A72-in
	xContainer["A7"] = this.Input_Signals[178] -- A7-in
	xContainer["A9"] = this.Input_Signals[179] -- A9-in
	xContainer["A57"] = this.Input_Signals[180] -- A57-in
	xContainer["A81"] = this.Input_Signals[181] -- A81-in
	xContainer["A82"] = this.Input_Signals[182] -- A82-in
	xContainer["A15"] = this.Input_Signals[183] -- A15-in
	xContainer["AB6"] = this.Input_Signals[184] -- AB6-in
	xContainer["A83"] = this.Input_Signals[185] -- A83-in
	xContainer["A33"] = this.Input_Signals[186] -- A33-in
	xContainer["AB3"] = this.Input_Signals[187] -- AB3-in
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
    if res ~= 0 then return end
    
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