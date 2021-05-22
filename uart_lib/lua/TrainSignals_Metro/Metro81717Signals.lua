if CLIENT then return end
Metro81717Signals = Metro81717Signals or {}
include( "UARTFrontViewClient.lua" )

local function new_IndicatorSignals(n)
    local tmp = {}
    for i=0,n-1 do
        tmp[i] = {}
        for j=0,2 do
            tmp[i][j] = {[0] = 0,[1] = 0}
        end
    end
    return tmp
end

function Metro81717Signals:InitConfArrays(this)
    -- Количество контроллеров
	this.Number_Of_Controllers = 14
	-- Общее количество АЦП
	this.Nmb_Of_ADC_Signals = 3
	-- Размер массива сигналов стрелок
	this.Nmb_Of_Arrow_Signals = 6
	-- Размер массива сигналов индикаторам
	this.Nmb_Of_Indicator_Signals = 0
	-- Размер массива сигналов САУТ
	this.Nmb_Of_SAUT_Signals = 0
	-- Размер массива сигналов 7SegDec
	this.Nmb_Of_7SegDec_Signals = 1
	-- Количество подключенных текстовых дисплеев
	this.Nmb_Of_Text_Displays = 2
    -- Количество стрелочных приборов на контроллер
	this.Config_Arrow_Signals = {}
	this.Config_Arrow_Signals[0] = 0
	this.Config_Arrow_Signals[1] = 0
	this.Config_Arrow_Signals[2] = 3
	this.Config_Arrow_Signals[3] = 3
	this.Config_Arrow_Signals[4] = 0
	this.Config_Arrow_Signals[5] = 0
	this.Config_Arrow_Signals[6] = 0
	this.Config_Arrow_Signals[7] = 0
	this.Config_Arrow_Signals[8] = 0
	this.Config_Arrow_Signals[9] = 0
	this.Config_Arrow_Signals[10] = 0
	this.Config_Arrow_Signals[11] = 0
	this.Config_Arrow_Signals[12] = 0
	this.Config_Arrow_Signals[13] = 0
    -- Массив конфигурации приборов индикации
	this.Config_Indicator_Signals = new_IndicatorSignals(this.Number_Of_Controllers)
	this.Config_Indicator_Signals[0][0][0] = 0
	this.Config_Indicator_Signals[0][0][1] = 0
	this.Config_Indicator_Signals[0][1][0] = 0
	this.Config_Indicator_Signals[0][1][1] = 0
	this.Config_Indicator_Signals[0][2][0] = 0
	this.Config_Indicator_Signals[0][2][1] = 0
	this.Config_Indicator_Signals[1][0][0] = 0
	this.Config_Indicator_Signals[1][0][1] = 0
	this.Config_Indicator_Signals[1][1][0] = 0
	this.Config_Indicator_Signals[1][1][1] = 0
	this.Config_Indicator_Signals[1][2][0] = 0
	this.Config_Indicator_Signals[1][2][1] = 0
	this.Config_Indicator_Signals[2][0][0] = 0
	this.Config_Indicator_Signals[2][0][1] = 0
	this.Config_Indicator_Signals[2][1][0] = 0
	this.Config_Indicator_Signals[2][1][1] = 0
	this.Config_Indicator_Signals[2][2][0] = 0
	this.Config_Indicator_Signals[2][2][1] = 0
	this.Config_Indicator_Signals[3][0][0] = 0
	this.Config_Indicator_Signals[3][0][1] = 0
	this.Config_Indicator_Signals[3][1][0] = 0
	this.Config_Indicator_Signals[3][1][1] = 0
	this.Config_Indicator_Signals[3][2][0] = 0
	this.Config_Indicator_Signals[3][2][1] = 0
	this.Config_Indicator_Signals[4][0][0] = 0
	this.Config_Indicator_Signals[4][0][1] = 0
	this.Config_Indicator_Signals[4][1][0] = 0
	this.Config_Indicator_Signals[4][1][1] = 0
	this.Config_Indicator_Signals[4][2][0] = 0
	this.Config_Indicator_Signals[4][2][1] = 0
	this.Config_Indicator_Signals[5][0][0] = 0
	this.Config_Indicator_Signals[5][0][1] = 0
	this.Config_Indicator_Signals[5][1][0] = 0
	this.Config_Indicator_Signals[5][1][1] = 0
	this.Config_Indicator_Signals[5][2][0] = 0
	this.Config_Indicator_Signals[5][2][1] = 0
	this.Config_Indicator_Signals[6][0][0] = 0
	this.Config_Indicator_Signals[6][0][1] = 0
	this.Config_Indicator_Signals[6][1][0] = 0
	this.Config_Indicator_Signals[6][1][1] = 0
	this.Config_Indicator_Signals[6][2][0] = 0
	this.Config_Indicator_Signals[6][2][1] = 0
	this.Config_Indicator_Signals[7][0][0] = 0
	this.Config_Indicator_Signals[7][0][1] = 0
	this.Config_Indicator_Signals[7][1][0] = 0
	this.Config_Indicator_Signals[7][1][1] = 0
	this.Config_Indicator_Signals[7][2][0] = 0
	this.Config_Indicator_Signals[7][2][1] = 0
	this.Config_Indicator_Signals[8][0][0] = 0
	this.Config_Indicator_Signals[8][0][1] = 0
	this.Config_Indicator_Signals[8][1][0] = 0
	this.Config_Indicator_Signals[8][1][1] = 0
	this.Config_Indicator_Signals[8][2][0] = 0
	this.Config_Indicator_Signals[8][2][1] = 0
	this.Config_Indicator_Signals[9][0][0] = 0
	this.Config_Indicator_Signals[9][0][1] = 0
	this.Config_Indicator_Signals[9][1][0] = 0
	this.Config_Indicator_Signals[9][1][1] = 0
	this.Config_Indicator_Signals[9][2][0] = 0
	this.Config_Indicator_Signals[9][2][1] = 0
	this.Config_Indicator_Signals[10][0][0] = 0
	this.Config_Indicator_Signals[10][0][1] = 0
	this.Config_Indicator_Signals[10][1][0] = 0
	this.Config_Indicator_Signals[10][1][1] = 0
	this.Config_Indicator_Signals[10][2][0] = 0
	this.Config_Indicator_Signals[10][2][1] = 0
	this.Config_Indicator_Signals[11][0][0] = 0
	this.Config_Indicator_Signals[11][0][1] = 0
	this.Config_Indicator_Signals[11][1][0] = 0
	this.Config_Indicator_Signals[11][1][1] = 0
	this.Config_Indicator_Signals[11][2][0] = 0
	this.Config_Indicator_Signals[11][2][1] = 0
	this.Config_Indicator_Signals[12][0][0] = 0
	this.Config_Indicator_Signals[12][0][1] = 0
	this.Config_Indicator_Signals[12][1][0] = 0
	this.Config_Indicator_Signals[12][1][1] = 0
	this.Config_Indicator_Signals[12][2][0] = 0
	this.Config_Indicator_Signals[12][2][1] = 0
	this.Config_Indicator_Signals[13][0][0] = 0
	this.Config_Indicator_Signals[13][0][1] = 0
	this.Config_Indicator_Signals[13][1][0] = 0
	this.Config_Indicator_Signals[13][1][1] = 0
	this.Config_Indicator_Signals[13][2][0] = 0
	this.Config_Indicator_Signals[13][2][1] = 0
    -- Массив конфигурации приборов САУТ
    this.Config_SAUT_Signals = {}
	this.Config_SAUT_Signals[0] = 0
	this.Config_SAUT_Signals[1] = 0
	this.Config_SAUT_Signals[2] = 0
	this.Config_SAUT_Signals[3] = 0
	this.Config_SAUT_Signals[4] = 0
	this.Config_SAUT_Signals[5] = 0
	this.Config_SAUT_Signals[6] = 0
	this.Config_SAUT_Signals[7] = 0
	this.Config_SAUT_Signals[8] = 0
	this.Config_SAUT_Signals[9] = 0
	this.Config_SAUT_Signals[10] = 0
	this.Config_SAUT_Signals[11] = 0
	this.Config_SAUT_Signals[12] = 0
	this.Config_SAUT_Signals[13] = 0
    -- Массив конфигурации 7-ми сегментного индикатора скорости для метро (через дешифратор К514ИД2)
    for i=0,13 do
        this.Config_7SegDec_Signals[i] = {}
        for j=0,2 do
            this.Config_7SegDec_Signals[i][j] = 0
        end
    end
	this.Config_7SegDec_Signals[8][0] = 1
    -- Массив конфигурации текстовых дисплеев. Число, отличное от нуля, обозначает кол-во символов в одной строке дисплея.
    this.Config_Text_Signals[0]=0
	this.Config_Text_Signals[1]=0
	this.Config_Text_Signals[2]=0
	this.Config_Text_Signals[3]=0
	this.Config_Text_Signals[4]=0
	this.Config_Text_Signals[5]=0
	this.Config_Text_Signals[6]=0
	this.Config_Text_Signals[7]=0
	this.Config_Text_Signals[8]=0
	this.Config_Text_Signals[9]=0
	this.Config_Text_Signals[10]=0
	this.Config_Text_Signals[11]=0
	this.Config_Text_Signals[12]=20
	this.Config_Text_Signals[13]=24
    -- Главный конфигурационный массив
    this.Config_Signals_Length = 24 * this.Number_Of_Controllers
    this.Config_Signals = {}
    this.Config_Signals[0] = 0 -- 
	this.Config_Signals[1] = 0 -- 
	this.Config_Signals[2] = 0 -- 
	this.Config_Signals[3] = 0 -- 
	this.Config_Signals[4] = 0 -- 
	this.Config_Signals[5] = 0 -- 
	this.Config_Signals[6] = 0 -- 
	this.Config_Signals[7] = 3 -- Стоп-кран
	this.Config_Signals[8] = 1 -- УНЧ
	this.Config_Signals[9] = 1 -- Контроль ЭС
	this.Config_Signals[10] = 1 -- Контр громког
	this.Config_Signals[11] = 1 -- Радио
	this.Config_Signals[12] = 1 -- 1-2 программы(прав)
	this.Config_Signals[13] = 1 -- 1-2 программы(лев)
	this.Config_Signals[14] = 1 -- Закр дверей
	this.Config_Signals[15] = 1 -- Возвр РП?(тумблер без фиксации)
	this.Config_Signals[16] = 1 -- Вкл мотор компр
	this.Config_Signals[17] = 1 -- Блок питания
	this.Config_Signals[18] = 1 -- Резерв
	this.Config_Signals[19] = 1 -- Компрессор резерв
	this.Config_Signals[20] = 1 -- АРС 13V
	this.Config_Signals[21] = 1 -- Вверх(дисплей)
	this.Config_Signals[22] = 1 -- Вниз(дисплей)
	this.Config_Signals[23] = 1 -- Меню
	this.Config_Signals[24] = 3 -- Кран маш
	this.Config_Signals[25] = 3 -- Контр маш
	this.Config_Signals[26] = 0 -- 
	this.Config_Signals[27] = 0 -- 
	this.Config_Signals[28] = 0 -- 
	this.Config_Signals[29] = 0 -- 
	this.Config_Signals[30] = 0 -- 
	this.Config_Signals[31] = 0 -- 
	this.Config_Signals[32] = 1 -- АРС
	this.Config_Signals[33] = 1 -- АЛС
	this.Config_Signals[34] = 1 -- АРС-Р
	this.Config_Signals[35] = 1 -- Дешифратор
	this.Config_Signals[36] = 1 -- Осв салона
	this.Config_Signals[37] = 1 -- Осв кабины
	this.Config_Signals[38] = 1 -- Осв пульта
	this.Config_Signals[39] = 1 -- ВП
	this.Config_Signals[40] = 1 -- Рез.закр.дв.
	this.Config_Signals[41] = 1 -- Вкл БВ
	this.Config_Signals[42] = 1 -- Дв лев справа
	this.Config_Signals[43] = 1 -- Дв лев слева
	this.Config_Signals[44] = 1 -- Пр-1, лев-0
	this.Config_Signals[45] = 1 -- Бдительность лев
	this.Config_Signals[46] = 1 -- Бдительность прав
	this.Config_Signals[47] = 1 -- АВУ(инв)
	this.Config_Signals[48] = 1 -- Двери торцевые
	this.Config_Signals[49] = 1 -- Вентиляция кабины
	this.Config_Signals[50] = 1 -- Вкл авар сигн
	this.Config_Signals[51] = 1 -- Защита преобр
	this.Config_Signals[52] = 1 -- Сигнализация
	this.Config_Signals[53] = 1 -- Звонок
	this.Config_Signals[54] = 1 -- Откл БВ
	this.Config_Signals[55] = 1 -- Вентиль №1
	this.Config_Signals[56] = 1 -- Пуск резерв
	this.Config_Signals[57] = 1 -- Авар ход
	this.Config_Signals[58] = 1 -- Фары
	this.Config_Signals[59] = 1 -- ВУС
	this.Config_Signals[60] = 1 -- Авар двери(тумблер)
	this.Config_Signals[61] = 1 -- Авар ход(тумблер)
	this.Config_Signals[62] = 1 -- ВКСТ
	this.Config_Signals[63] = 1 -- Двери прав
	this.Config_Signals[64] = 1 -- КРУ1
	this.Config_Signals[65] = 1 -- КРУ2(горят оба)
	this.Config_Signals[66] = 1 -- 
	this.Config_Signals[67] = 1 -- АСОТП1
	this.Config_Signals[68] = 1 -- АСОТП2
	this.Config_Signals[69] = 1 -- АСОТП3
	this.Config_Signals[70] = 1 -- АСОТП4
	this.Config_Signals[71] = 1 -- 
	this.Config_Signals[72] = 1 -- закр дверей
	this.Config_Signals[73] = 1 -- левые двери
	this.Config_Signals[74] = 1 -- 1 программа
	this.Config_Signals[75] = 1 -- 2 программа
	this.Config_Signals[76] = 1 -- Педаль бдительности
	this.Config_Signals[77] = 1 -- Пневмосигнал
	this.Config_Signals[78] = 1 -- Реверс наз
	this.Config_Signals[79] = 1 -- Реверс вп
	this.Config_Signals[80] = 1 -- 
	this.Config_Signals[81] = 1 -- 
	this.Config_Signals[82] = 1 -- 
	this.Config_Signals[83] = 1 -- 
	this.Config_Signals[84] = 1 -- Управление(предохранитель)
	this.Config_Signals[85] = 1 -- Батарея(предохранитель)
	this.Config_Signals[86] = 1 -- Без подписи(предохранитель)
	this.Config_Signals[87] = 1 -- Преобразователь(предохранитель)
	this.Config_Signals[88] = 1 -- Освещение1(предохранитель)
	this.Config_Signals[89] = 1 -- Освещение2(предохранитель)
	this.Config_Signals[90] = 1 -- уава
	this.Config_Signals[91] = 1 -- 
	this.Config_Signals[92] = 1 -- 
	this.Config_Signals[93] = 1 -- уава(черкизово)
	this.Config_Signals[94] = 1 -- 
	this.Config_Signals[95] = 1 -- 
	this.Config_Signals[96] = 1 -- РЦ1 вкл
	this.Config_Signals[97] = 1 -- Батареи вкл
	this.Config_Signals[98] = 1 -- УОС выкл
	this.Config_Signals[99] = 1 -- Втр прижат
	this.Config_Signals[100] = 1 -- Весь состав
	this.Config_Signals[101] = 1 -- 1 пол
	this.Config_Signals[102] = 1 -- 2 пол
	this.Config_Signals[103] = 1 -- 
	this.Config_Signals[104] = 1 -- Разобщ кран кран машиниста
	this.Config_Signals[105] = 1 -- Стоян. тормоз
	this.Config_Signals[106] = 1 -- ЭПВ-АРС
	this.Config_Signals[107] = 1 -- Разобщ кран манометров
	this.Config_Signals[108] = 1 -- Разобщ кран пневмосигнала
	this.Config_Signals[109] = 1 -- 
	this.Config_Signals[110] = 1 -- 
	this.Config_Signals[111] = 1 -- 
	this.Config_Signals[112] = 1 -- A54-in
	this.Config_Signals[113] = 1 -- ВУ-in
	this.Config_Signals[114] = 1 -- A10-in
	this.Config_Signals[115] = 1 -- A53-in
	this.Config_Signals[116] = 1 -- A49-in
	this.Config_Signals[117] = 1 -- A27-in
	this.Config_Signals[118] = 1 -- AC1-in
	this.Config_Signals[119] = 1 -- A21-in
	this.Config_Signals[120] = 1 -- A26-in
	this.Config_Signals[121] = 1 -- AP63-in
	this.Config_Signals[122] = 1 -- A17-in
	this.Config_Signals[123] = 1 -- A44-in
	this.Config_Signals[124] = 1 -- A45-in
	this.Config_Signals[125] = 1 -- A11-in
	this.Config_Signals[126] = 1 -- A71-in
	this.Config_Signals[127] = 1 -- A41-in
	this.Config_Signals[128] = 1 -- A74-in
	this.Config_Signals[129] = 1 -- A73-in
	this.Config_Signals[130] = 1 -- A79-in
	this.Config_Signals[131] = 1 -- A42-in
	this.Config_Signals[132] = 1 -- A46-in
	this.Config_Signals[133] = 1 -- A47-in
	this.Config_Signals[134] = 1 -- AB1-in
	this.Config_Signals[135] = 1 -- A29-in
	this.Config_Signals[136] = 1 -- A76-in
	this.Config_Signals[137] = 1 -- A48-in
	this.Config_Signals[138] = 1 -- A56-in
	this.Config_Signals[139] = 1 -- A65-in
	this.Config_Signals[140] = 1 -- A25-in
	this.Config_Signals[141] = 1 -- A30-in
	this.Config_Signals[142] = 1 -- A1-in
	this.Config_Signals[143] = 1 -- A20-in
	this.Config_Signals[144] = 1 -- A32-in
	this.Config_Signals[145] = 1 -- A13-in
	this.Config_Signals[146] = 1 -- A43-in
	this.Config_Signals[147] = 1 -- A31-in
	this.Config_Signals[148] = 1 -- A77-in
	this.Config_Signals[149] = 1 -- A78-in
	this.Config_Signals[150] = 1 -- 
	this.Config_Signals[151] = 1 -- A75-in
	this.Config_Signals[152] = 1 -- A22-in
	this.Config_Signals[153] = 1 -- A8-in
	this.Config_Signals[154] = 1 -- A28-in
	this.Config_Signals[155] = 1 -- A38-in
	this.Config_Signals[156] = 1 -- A14-in
	this.Config_Signals[157] = 1 -- A39-in
	this.Config_Signals[158] = 1 -- A6-in
	this.Config_Signals[159] = 1 -- A70-in
	this.Config_Signals[160] = 1 -- A4-in
	this.Config_Signals[161] = 1 -- A5-in
	this.Config_Signals[162] = 1 -- A2-in
	this.Config_Signals[163] = 1 -- A3-in
	this.Config_Signals[164] = 1 -- A50-in
	this.Config_Signals[165] = 1 -- A52-in
	this.Config_Signals[166] = 1 -- A40-in
	this.Config_Signals[167] = 1 -- A80-in
	this.Config_Signals[168] = 1 -- A66-in
	this.Config_Signals[169] = 1 -- A18-in
	this.Config_Signals[170] = 1 -- A24-in
	this.Config_Signals[171] = 1 -- A19-in
	this.Config_Signals[172] = 1 -- A37-in
	this.Config_Signals[173] = 1 -- A51-in
	this.Config_Signals[174] = 1 -- A12-in
	this.Config_Signals[175] = 1 -- A16-in
	this.Config_Signals[176] = 1 -- A68-in
	this.Config_Signals[177] = 1 -- A72-in
	this.Config_Signals[178] = 1 -- A7-in
	this.Config_Signals[179] = 1 -- A9-in
	this.Config_Signals[180] = 1 -- A57-in
	this.Config_Signals[181] = 1 -- A81-in
	this.Config_Signals[182] = 1 -- A82-in
	this.Config_Signals[183] = 1 -- A15-in
	this.Config_Signals[184] = 1 -- AB6-in
	this.Config_Signals[185] = 1 -- A83-in
	this.Config_Signals[186] = 1 -- A33-in
	this.Config_Signals[187] = 1 -- AB3-in
	this.Config_Signals[188] = 1 -- 
	this.Config_Signals[189] = 1 -- 
	this.Config_Signals[190] = 1 -- 
	this.Config_Signals[191] = 1 -- 
	this.Config_Signals[192] = 2 -- 
	this.Config_Signals[193] = 2 -- 
	this.Config_Signals[194] = 2 -- 
	this.Config_Signals[195] = 2 -- 
	this.Config_Signals[196] = 2 -- 
	this.Config_Signals[197] = 2 -- 
	this.Config_Signals[198] = 2 -- 
	this.Config_Signals[199] = 2 -- 
	this.Config_Signals[200] = 2 -- 0Ч
	this.Config_Signals[201] = 2 -- 0
	this.Config_Signals[202] = 2 -- 40
	this.Config_Signals[203] = 2 -- 60
	this.Config_Signals[204] = 2 -- 70
	this.Config_Signals[205] = 2 -- 80
	this.Config_Signals[206] = 2 -- ЛСД
	this.Config_Signals[207] = 2 -- 
	this.Config_Signals[208] = 2 -- ЛХ"РК"
	this.Config_Signals[209] = 2 -- РП
	this.Config_Signals[210] = 2 -- ЛСН
	this.Config_Signals[211] = 2 -- ЛЭКК
	this.Config_Signals[212] = 2 -- ЛКВЦ
	this.Config_Signals[213] = 2 -- ЛН
	this.Config_Signals[214] = 2 -- РС
	this.Config_Signals[215] = 2 -- ЛКВД
	this.Config_Signals[216] = 2 -- 
	this.Config_Signals[217] = 2 -- 
	this.Config_Signals[218] = 2 -- 
	this.Config_Signals[219] = 2 -- ЛВД
	this.Config_Signals[220] = 2 -- ЛКТ
	this.Config_Signals[221] = 2 -- ЛСТ
	this.Config_Signals[222] = 2 -- 
	this.Config_Signals[223] = 2 -- 
	this.Config_Signals[224] = 2 -- 
	this.Config_Signals[225] = 2 -- 
	this.Config_Signals[226] = 2 -- 
	this.Config_Signals[227] = 2 -- Двери левые слева светодиод
	this.Config_Signals[228] = 2 -- Двери левые справа светодиод
	this.Config_Signals[229] = 2 -- РП светодиод 
	this.Config_Signals[230] = 2 -- ЗЗащита преобраз
	this.Config_Signals[231] = 2 -- Резервный белый
	this.Config_Signals[232] = 2 -- ЛСП
	this.Config_Signals[233] = 2 -- АВУ
	this.Config_Signals[234] = 2 -- ЛКВП
	this.Config_Signals[235] = 2 -- Пневмотормоз
	this.Config_Signals[236] = 2 -- ИСТ
	this.Config_Signals[237] = 2 -- Двери правые
	this.Config_Signals[238] = 2 -- 
	this.Config_Signals[239] = 2 -- 
	this.Config_Signals[240] = 2 -- ВУ-o
	this.Config_Signals[241] = 2 -- A54-o
	this.Config_Signals[242] = 2 -- A53-o
	this.Config_Signals[243] = 2 -- A10-o
	this.Config_Signals[244] = 2 -- A27-o
	this.Config_Signals[245] = 2 -- A49-o
	this.Config_Signals[246] = 2 -- A21-o
	this.Config_Signals[247] = 2 -- AC-1-o
	this.Config_Signals[248] = 2 -- AP63-o
	this.Config_Signals[249] = 2 -- A26-o
	this.Config_Signals[250] = 2 -- A44-o
	this.Config_Signals[251] = 2 -- A17-o
	this.Config_Signals[252] = 2 -- A11-o
	this.Config_Signals[253] = 2 -- A45-o
	this.Config_Signals[254] = 2 -- A41-o
	this.Config_Signals[255] = 2 -- A71-o
	this.Config_Signals[256] = 2 -- A73-o
	this.Config_Signals[257] = 2 -- A74-o
	this.Config_Signals[258] = 2 -- A42-o
	this.Config_Signals[259] = 2 -- A79-o
	this.Config_Signals[260] = 2 -- A47-o
	this.Config_Signals[261] = 2 -- A46-o
	this.Config_Signals[262] = 2 -- A29-o
	this.Config_Signals[263] = 2 -- AB1-o
	this.Config_Signals[264] = 2 -- A48-o
	this.Config_Signals[265] = 2 -- A76-o
	this.Config_Signals[266] = 2 -- A65-o
	this.Config_Signals[267] = 2 -- A56-o
	this.Config_Signals[268] = 2 -- A30-o
	this.Config_Signals[269] = 2 -- A25-o
	this.Config_Signals[270] = 2 -- A20-o
	this.Config_Signals[271] = 2 -- A1-o
	this.Config_Signals[272] = 2 -- A13-o
	this.Config_Signals[273] = 2 -- A32-o
	this.Config_Signals[274] = 2 -- A31-o
	this.Config_Signals[275] = 2 -- A43-o
	this.Config_Signals[276] = 2 -- A78-o
	this.Config_Signals[277] = 2 -- A77-o
	this.Config_Signals[278] = 2 -- A75-o
	this.Config_Signals[279] = 2 -- 
	this.Config_Signals[280] = 2 -- 
	this.Config_Signals[281] = 2 -- 
	this.Config_Signals[282] = 2 -- асотп 4
	this.Config_Signals[283] = 2 -- асотп 3
	this.Config_Signals[284] = 2 -- асотп 2
	this.Config_Signals[285] = 2 -- асотп 1
	this.Config_Signals[286] = 2 -- асотп неиспр
	this.Config_Signals[287] = 2 -- асотп пожар 1
	this.Config_Signals[288] = 2 -- A8-o
	this.Config_Signals[289] = 2 -- A22-o
	this.Config_Signals[290] = 2 -- A38-o
	this.Config_Signals[291] = 2 -- A28-o
	this.Config_Signals[292] = 2 -- A39-o
	this.Config_Signals[293] = 2 -- A14-o
	this.Config_Signals[294] = 2 -- A70-o
	this.Config_Signals[295] = 2 -- A6-o
	this.Config_Signals[296] = 2 -- A5-o
	this.Config_Signals[297] = 2 -- A4-o
	this.Config_Signals[298] = 2 -- A3-o
	this.Config_Signals[299] = 2 -- A2-o
	this.Config_Signals[300] = 2 -- A52-o
	this.Config_Signals[301] = 2 -- A50-o
	this.Config_Signals[302] = 2 -- A80-o
	this.Config_Signals[303] = 2 -- A40-o
	this.Config_Signals[304] = 2 -- A18-o
	this.Config_Signals[305] = 2 -- A66-o
	this.Config_Signals[306] = 2 -- A19-o
	this.Config_Signals[307] = 2 -- A24-o
	this.Config_Signals[308] = 2 -- A51-o
	this.Config_Signals[309] = 2 -- A37-o
	this.Config_Signals[310] = 2 -- A16-o
	this.Config_Signals[311] = 2 -- A12-o
	this.Config_Signals[312] = 2 -- A72-o
	this.Config_Signals[313] = 2 -- A68-o
	this.Config_Signals[314] = 2 -- A9-o
	this.Config_Signals[315] = 2 -- A7-o
	this.Config_Signals[316] = 2 -- A81-o
	this.Config_Signals[317] = 2 -- A57-o
	this.Config_Signals[318] = 2 -- A15-o
	this.Config_Signals[319] = 2 -- A82-o
	this.Config_Signals[320] = 2 -- A83-o
	this.Config_Signals[321] = 2 -- AB6-o
	this.Config_Signals[322] = 2 -- AB3-o
	this.Config_Signals[323] = 2 -- A33-o
	this.Config_Signals[324] = 2 -- 
	this.Config_Signals[325] = 2 -- 
	this.Config_Signals[326] = 2 -- 
	this.Config_Signals[327] = 2 -- 
	this.Config_Signals[328] = 2 -- Клапан 1
	this.Config_Signals[329] = 2 -- Клапан 2
	this.Config_Signals[330] = 2 -- 
	this.Config_Signals[331] = 2 -- Клапан 3
	this.Config_Signals[332] = 2 -- 
	this.Config_Signals[333] = 2 -- 
	this.Config_Signals[334] = 2 -- 
	this.Config_Signals[335] = 2 -- 
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

function Metro81717Signals:update(this)
    
    this.Arrow_Signals[0] = self.Step1(xCont("tcPressure"))
    this.Arrow_Signals[1] = self.Step2(xCont("nmPressure"))
    this.Arrow_Signals[2] = self.Step3(xCont("tmPressure"))
    this.Arrow_Signals[3] = self.Step4(xCont("Ammeter"))
    this.Arrow_Signals[4] = self.Step5(xCont("KVolt") / 1000.0)
    this.Arrow_Signals[5] = self.Step6(xCont("BatteryVoltage"))
    
    this._7SegDec_Signals[0] = xCont("vfact") -- Скорость
    
    this.TextDisplaysSignalsStruct[0].DisplayOn = xContainer["AsotpDisplay"]
    this.TextDisplaysSignalsStruct[0].DisplayLedOn = xContainer["AsotpDisplay"]
    this.TextDisplaysSignalsStruct[1].DisplayOn = xContainer["InformerDisplay"]
    this.TextDisplaysSignalsStruct[1].DisplayLedOn = xContainer["InformerDisplay"]
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
    
    xContainer["EmergencyBrakeValve"] = self.Adc1(this.ADC_Signals[0]) --стоп кран
    xContainer["driversTapPos"] = self.Adc2(this.ADC_Signals[1]) --кран машиниста
    xContainer["MainController"] = self.Adc3(this.ADC_Signals[2])
	xContainer["ReverseController"] = this.Input_Signals[79] - this.Input_Signals[78]
    
    xContainer["R_UNch"] = this.Input_Signals[8] -- УНЧ
	xContainer["R_ZS"] = this.Input_Signals[9] -- Контроль ЭС
	xContainer["R_G"] = this.Input_Signals[10] -- Контр громког
	xContainer["R_Radio"] = this.Input_Signals[11] -- Радио
	xContainer["R_Program2"] = this.Input_Signals[12] -- 1-2 программы(прав)
	xContainer["R_Program1"] = this.Input_Signals[13] -- 1-2 программы(лев)
	xContainer["VUD1"] = this.Input_Signals[14] -- Закр дверей
	xContainer["R_VPR"] = this.Input_Signals[15] -- ВПР
	xContainer["VMK"] = this.Input_Signals[16] -- Вкл мотор компр
	xContainer["BPSNon"] = this.Input_Signals[17] -- Блок питания
	--self.Config_Signals[18] = 1 -- Резерв
	xContainer["RezMK"] = this.Input_Signals[19] -- Компрессор резерв
	xContainer["ARS13"] = this.Input_Signals[20] -- АРС 13V
	xContainer["R_ASNPUp"] = this.Input_Signals[21] -- Вверх(дисплей)
	xContainer["R_ASNPDown"] = this.Input_Signals[22] -- Вниз(дисплей)
	xContainer["R_ASNPMenu"] = this.Input_Signals[23] -- Меню
	xContainer["ARS"] = this.Input_Signals[32] -- АРС
	xContainer["ALS"] = this.Input_Signals[33] -- АЛС
	xContainer["ARSR"] = this.Input_Signals[34] -- АРС-Р
	xContainer["ALSFreq"] = this.Input_Signals[35] -- Дешифратор
    
    xContainer["L_1"] = this.Input_Signals[36] -- Осв салона
	xContainer["L_2"] = this.Input_Signals[37] -- Осв кабины
	xContainer["L_3"] = this.Input_Signals[38] -- Осв пульта
	xContainer["VP"] = this.Input_Signals[39] -- ВП
	xContainer["KRZD"] = this.Input_Signals[40] -- Рез.закр.дв.
	xContainer["VozvratRP"] = this.Input_Signals[41] -- Вкл БВ
	xContainer["KDLR"] = this.Input_Signals[42] -- Дв лев справа
	xContainer["KDL"] = this.Input_Signals[43] -- Дв лев слева
	xContainer["DoorSelect"] = this.Input_Signals[44] -- Пр-1, лев-0
	xContainer["KVT"] = this.Input_Signals[45] -- Бдительность лев
	xContainer["KVTR"] = this.Input_Signals[46] -- Бдительность прав
	xContainer["OtklAVU"] = this.Input_Signals[47] -- АВУ
    --xContainer["EndDoorsSwitch"] = this.Input_Signals[48] -- Двери торцевые
	xContainer["V13"] = this.Input_Signals[49] -- Вентиляция кабины
	--this.Input_Signals[50] -- Вкл авар сигн
	xContainer["ConverterProtection"] = this.Input_Signals[51] -- Защита преобр
    xContainer["KSN"] = this.Input_Signals[52] -- Сигнализация неисправностей
	xContainer["Ring"] = this.Input_Signals[53] -- Звонок
	xContainer["OtklBV"] = this.Input_Signals[54] -- Откл БВ
	xContainer["VZ1"] = this.Input_Signals[55] -- Вентиль №1
	xContainer["KRP"] = this.Input_Signals[56] -- Пуск резерв
	xContainer["KAH"] = this.Input_Signals[57] -- Авар ход
	xContainer["L_4"] = this.Input_Signals[58] -- Фары
	xContainer["VUS"] = this.Input_Signals[59] -- ВУС
	xContainer["VAD"] = this.Input_Signals[60] -- Авар двери(тумблер)
	xContainer["VAH"] = this.Input_Signals[61] -- Авар ход(тумблер)
	-- xContainer["VKST"] = this.Input_Signals[62] -- ВКСТ
	xContainer["KDP"] = this.Input_Signals[63] -- Двери прав
    xContainer["KRU"] = this.Input_Signals[64] + this.Input_Signals[65] -- КРУ2(горят оба)
    xContainer["IGLA1"] = this.Input_Signals[67] -- АСОТП1
	xContainer["IGLA2"] = this.Input_Signals[68] -- АСОТП2
	xContainer["IGLA3"] = this.Input_Signals[69] -- АСОТП3
	xContainer["IGLA4"] = this.Input_Signals[70] -- АСОТП4
    xContainer["VUD2"] = this.Input_Signals[72] -- закрытие дверей (у помощника машиниста на стене)
	xContainer["VDL"] = this.Input_Signals[73] -- левые двери (у помощника машиниста на стене)
	xContainer["R_Program1H"] = this.Input_Signals[74] -- 1 программа
	xContainer["R_Program2H"] = this.Input_Signals[75] -- 2 программа
	xContainer["PB"] = this.Input_Signals[76] -- Педаль безопасности
	xContainer["Button_Signal"] = this.Input_Signals[77] -- Пневмосигнал
	-- xContainer["ProtectionControlCircuits"] = this.Input_Signals[84] -- Управление(предохранитель)
	-- xContainer["ProtectionBattery"] = this.Input_Signals[85] -- Батарея(предохранитель)
	-- xContainer["ProtectionMasterSwitch"] = this.Input_Signals[86] -- Без подписи(предохранитель)
	-- xContainer["ProtectionInverter"] = this.Input_Signals[87] -- Преобразователь(предохранитель)
	-- xContainer["ProtectionLighting1"] = this.Input_Signals[88] -- Освещение1(предохранитель)
	-- xContainer["ProtectionLighting2"] = this.Input_Signals[89] -- Освещение2(предохранитель)
	xContainer["UAVAC"] = this.Input_Signals[90] -- уава кнопка
	--Config_Signals[93] = 1 -- уава(черкизово)
	xContainer["RC1"] = this.Input_Signals[96] -- РЦ1 вкл
	xContainer["VB"] = this.Input_Signals[97] -- Батареи вкл
	xContainer["UOS"] = this.Input_Signals[98] -- УОС выкл
    -- xContainer["PantographSwitch"] = this.Input_Signals[99] + --[[ВТР прижат]] 2 * this.Input_Signals[100] + --[[Весь состав]] 3 * this.Input_Signals[101] + --[[1 пол]] 4 * this.Input_Signals[102] --[[2 пол]]
    xContainer["DriverValveDisconnect"] = 1-this.Input_Signals[104] -- Разобщ кран кран машиниста
	xContainer["ParkingBrake"] = 1-this.Input_Signals[105] -- Стоян. тормоз
	xContainer["EPK"] = 1-this.Input_Signals[106] -- ЭПВ-АРС
	-- xContainer["ManometerCutOffValve"] = 1-this.Input_Signals[107] -- Разобщ кран манометров
	-- xContainer["PneumSignalCutOffValve"] = 1-this.Input_Signals[108] -- Разобщ кран пневмосигнала
    
    xContainer["A54"] = this.Input_Signals[112] -- A54-in
	xContainer["A84"] = this.Input_Signals[113] -- ВУ-in
	xContainer["A10"] = this.Input_Signals[114] -- A10-in
	xContainer["A53"] = this.Input_Signals[115] -- A53-in
	xContainer["A49"] = this.Input_Signals[116] -- A49-in
	xContainer["A27"] = this.Input_Signals[117] -- A27-in
	xContainer["AC1"] = this.Input_Signals[118] -- AC1-in
	xContainer["A21"] = this.Input_Signals[119] -- A21-in
	xContainer["A26"] = this.Input_Signals[120] -- A26-in
	xContainer["AR63"] = this.Input_Signals[121] -- AP63-in
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
	xContainer["AV1"] = this.Input_Signals[134] -- AB1-in
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
	xContainer["AV6"] = this.Input_Signals[184] -- AB6-in
	xContainer["A83"] = this.Input_Signals[185] -- A83-in
	xContainer["A33"] = this.Input_Signals[186] -- A33-in
	xContainer["AV3"] = this.Input_Signals[187] -- AB3-in
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
    UARTFrontViewClient:Initialize()
    Metro81717Signals:InitConfArrays(UARTFrontViewClient)
    UARTFrontViewClient:ConfigSignals(true)
    
    timer.Simple(1, function()
        local Train = Autospawn.HeadWagon
        for i=1,#plombstbl do Train:BrokePlomb(plombstbl[i]) end
    end)
    
    return res
end