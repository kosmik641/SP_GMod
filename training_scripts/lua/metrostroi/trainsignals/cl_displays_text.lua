local function printTextASOTP(self,x,y,text,col)
    local str = {utf8.codepoint(text,1,-1)}
    for i=1,#str do
        local char = utf8.char(str[i])
        local alpha = col.a
        if alpha > 1 then
            draw.SimpleText(char,"MetrostroiSubway_IGLAb",-8+(x+i)*25,30+y*43+1,ColorAlpha(col,alpha*0.08),TEXT_ALIGN_CENTER,TEXT_ALIGN_CENTER)
        end
        draw.SimpleText(char,"MetrostroiSubway_IGLA",-8+(x+i)*25,30+y*43,col,TEXT_ALIGN_CENTER,TEXT_ALIGN_CENTER)
        self.OutText[x+y*20+i] = str[i] == 9608 and " " or char
    end
end

local function iglaScreen(self,Train)
    self:IGLA_Old(Train)
    
    if Metrostroi.GetText then
        local strOut = ""
        for i=1,40 do
            strOut = strOut..(self.OutText[i] or " ")
        end
        self.Train.ASOTPText = strOut
        self.OutText = {}
    end
end

local function printTextASNP(self,x,y,text,inverse)
    local str = {utf8.codepoint(text,1,-1)}
    for i=1,#str do
        local char = utf8.char(str[i])
        if inverse then
            draw.SimpleText(string.char(0x7f),"Metrostroi_ASNP",(x+i)*20.5+5,y*40+40,Color(0,0,0),TEXT_ALIGN_CENTER,TEXT_ALIGN_CENTER)
            draw.SimpleText(char,"Metrostroi_ASNP",(x+i)*20.5+5,y*40+40,Color(140,190,0,150),TEXT_ALIGN_CENTER,TEXT_ALIGN_CENTER)
        else
            draw.SimpleText(char,"Metrostroi_ASNP",(x+i)*20.5+5,y*40+40,Color(0,0,0),TEXT_ALIGN_CENTER,TEXT_ALIGN_CENTER)
        end

        if x == -0.2 then
            self.OutText[y*24+i] = char
        elseif x == 0.2 then
            self.OutText[1+y*24+i] = char
        else
            self.OutText[x+y*24+i] = char
        end
        
    end
end

local function asnpScreen(self,Train)
    self:ASNPScreen_Old(Train)

    if Metrostroi.GetText then
        local strOut = ""
        for i=1,48 do
            strOut = strOut..(self.OutText[i] or " ")
        end
        self.Train.ASNPText = strOut
        self.OutText = {}
    end
end

local function clientInject()
    local Train = scripted_ents.GetStored("gmod_subway_81-717_mvm").t

    Train.TS_Initialize_Old = Train.TS_Initialize_Old or Train.Initialize
    function Train:Initialize()
        self:TS_Initialize_Old()
        local sys = self.Systems

        -- Init IGLA
        self.ASOTPText = ""
        sys.IGLA_CBKI.OutText = {}
        sys.IGLA_CBKI.IGLA_Old = sys.IGLA_CBKI.IGLA_Old or sys.IGLA_CBKI.IGLA
        sys.IGLA_CBKI.PrintText = printTextASOTP
        sys.IGLA_CBKI.IGLA = iglaScreen

        -- Init ASNP
        self.ASNPText = ""
        sys.ASNP.OutText = {}
        sys.ASNP.ASNPScreen_Old = sys.ASNP.ASNPScreen_Old or sys.ASNP.ASNPScreen
        sys.ASNP.PrintText = printTextASNP
        sys.ASNP.ASNPScreen = asnpScreen
    end
end

timer.Simple(1,clientInject)