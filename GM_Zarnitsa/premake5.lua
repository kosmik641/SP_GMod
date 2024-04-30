PROJECT_GENERATOR_VERSION = 3

newoption({
	trigger = "gmcommon",
	description = "Sets the path to the garrysmod_common (https://github.com/danielga/garrysmod_common) directory",
	value = "../garrysmod_common"
})

local gmcommon = assert(_OPTIONS.gmcommon or os.getenv("GARRYSMOD_COMMON"),
	"you didn't provide a path to your garrysmod_common (https://github.com/danielga/garrysmod_common) directory")
include(gmcommon)

CreateWorkspace({name = "GM_Zarnitsa", abi_compatible = false, path = "./"})
	CreateProject({name = "trainsignals", serverside = false, manual_files = false,source_path = "./TrainSignals/source"})
		IncludeLuaShared()
		IncludeSDKCommon()
		IncludeSDKTier0()