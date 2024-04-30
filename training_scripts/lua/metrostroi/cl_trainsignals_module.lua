if not file.Exists("lua/bin/gmcl_trainsignals_win32.dll", "GAME") then MsgC(Color(255,0,0),"gmcl_trainsignals_win32 not found.\n") return end

TrainSignals = TrainSignals or {}
if not TrainSignals.Module then require("trainsignals") end

local lastBtnSend = {}
local function sendButtonMessage(train,btnID,value,override)
	if not lastBtnSend[btnID] then lastBtnSend[btnID] = 0 end

	local btnIDnet
	if not override then
		btnIDnet = value and (btnID.."Close") or (btnID.."Open")
	else
		btnIDnet = btnID
	end
	if CurTime() > lastBtnSend[btnID] then
		net.Start("metrostroi-cabin-button")
			net.WriteEntity(train)
			net.WriteString(btnIDnet)
			net.WriteBit(true)
			net.WriteBool(false)
		net.SendToServer()

		-- Server button buffer workaround
		net.Start("metrostroi-cabin-button")
			net.WriteEntity(train)
			net.WriteString(btnIDnet)
			net.WriteBit(false)
			net.WriteBool(false)
		net.SendToServer()

		lastBtnSend[btnID] = CurTime()+0.06
	end
end

local function sendButtonMessageNamed(train,btnID,value)
	if not lastBtnSend[btnID] then lastBtnSend[btnID] = 0 end

	if CurTime() > lastBtnSend[btnID] then
		if value then
			net.Start("metrostroi-cabin-button")
				net.WriteEntity(train)
				net.WriteString(btnID)
				net.WriteBit(value)
				net.WriteBool(false)
			net.SendToServer()
		else
			net.Start("metrostroi-cabin-button")
				net.WriteEntity(train)
				net.WriteString(btnID)
				net.WriteBit(true)
				net.WriteBool(false)
			net.SendToServer()

			-- Server button buffer workaround
			net.Start("metrostroi-cabin-button")
				net.WriteEntity(train)
				net.WriteString(btnID)
				net.WriteBit(false)
				net.WriteBool(false)
			net.SendToServer()
		end

		lastBtnSend[btnID] = CurTime()+0.06
	end
end

local curTime = 0
local reverserSwitchTimer = 0

local kv717Fix = {
	[857] = "KVSetX3",
	[714] = "KVSetX2",
	[571] = "KVSetX1",
    [429] = "KVSet0",
    [286] = "KVSetT1",
    [143] = "KVSetT1A",
    [0]   = "KVSetT2",
}

local kru717Fix = {
	[0] = "KRUSet0",
	[1] = "KRUSetX1",
	[2] = "KRUSetX2"
}

local km013_717Fix = {
	[1] = "PneumaticBrakeSet0", -- workaround for PneumaticBrakeSet1
	[2] = "PneumaticBrakeSet2",
	[3] = "PneumaticBrakeSet3",
	[4] = "PneumaticBrakeSet4",
	[5] = "PneumaticBrakeSet5",
	[6] = "PneumaticBrakeSet6",
	[7] = "PneumaticBrakeSet7",
}

local specialButtons = {
	["ControllerPosition"] = 1,
	["ReverserPosition"] = 2,
	["KRUPosition"] = 3,
	["CranePosition"] = 4,
	["HornState"] = 5,
}

local function trainSignals_Main_Think()
	Metrostroi.GetText = false
	Metrostroi.MuteSounds = false
	if not TrainSignals.Module then return end
	local isConnected = TrainSignals.Module.IsConnected()
	if not isConnected then return end
	local Train = LocalPlayer().InMetrostroiTrain
	if not Train then return end
	if Train.ClassName ~= TrainSignals.Module.TargetTrain then return end
	Metrostroi.GetText = true
	Metrostroi.MuteSounds = true

	local InVar = TrainSignals.Module.DataExchange(Train)

	curTime = CurTime()
	for k,v in pairs(InVar) do
		if (specialButtons[k]) then
			local id = specialButtons[k]
			if (id == 4) then -- CranePosition
				if (v == 0) then return end
				sendButtonMessageNamed(Train,km013_717Fix[v/1000])
			elseif (id == 5) then -- HornState
				sendButtonMessageNamed(Train,"HornEngage",v,true)
			elseif (id < 4) then
				local wrenchMode = Train:GetNW2Int("WrenchMode")
				local kru = Train:GetNW2Int("KRUPosition")
				local reverser = Train:GetNW2Int("ReverserPosition")
				local controller = Train:GetNW2Int("ControllerPosition")

				if (id == 1) then -- ControllerPosition
					if (wrenchMode ~= 1 || reverser == 1 || kru > 0) then continue end
					sendButtonMessageNamed(Train,kv717Fix[v])
					continue
				elseif (id == 2) then -- ReverserPosition
					if (kru > 0) then continue end
					if (curTime < reverserSwitchTimer) then continue end
					reverserSwitchTimer = curTime + 0.15
		
					if (wrenchMode ~= 1) then
						sendButtonMessageNamed(Train,"KVWrenchNone")
						sendButtonMessageNamed(Train,"KVWrenchKV")
						continue
					end
		
					if v > reverser then sendButtonMessageNamed(Train,"KVReverserUp") 
					else sendButtonMessageNamed(Train,"KVReverserDown") end
					
					continue
				elseif (id == 3) then -- KRUPosition
					if (reverser ~= 1) then continue end
					if (curTime < reverserSwitchTimer) then continue end
					reverserSwitchTimer = curTime + 0.15
		
					if (wrenchMode ~= 2 and reverser == 1) then
						sendButtonMessageNamed(Train,"KVWrenchNone")
						sendButtonMessageNamed(Train,"KVWrenchKRU")
						continue
					end
		
					sendButtonMessageNamed(Train,kru717Fix[v])
		
					continue
				end
			end
		else
			sendButtonMessage(Train,k,v)
		end
	end
end

hook.Add("Think","TrainSignals_Main_Think",trainSignals_Main_Think)

