PROJECT_GENERATOR_VERSION = 3
include("include/garrysmod_common")

CreateWorkspace({name = "GM_Zarnitsa", abi_compatible = false})
	CreateProject({name = "trainsignals", serverside = false, manual_files = false,source_path = "./TrainSignals/source"})
		IncludeLuaShared()
		IncludeSDKCommon()
		IncludeSDKTier0()
        
    CreateProject({name = "trainsignals_loader", serverside = true, manual_files = false,source_path = "./TrainSignals/loader"}) -- Lua menu state is loading gmsv_...
		IncludeLuaShared()
		IncludeSDKCommon()
		IncludeSDKTier0()