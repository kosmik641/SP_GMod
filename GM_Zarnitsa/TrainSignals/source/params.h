#pragma once
#define CALIBRATIONS_FILE ".\\uart_config\\calibrations.ini"
#define TRAIN_CLASSNAME "gmod_subway_81-717_mvm"
#define MSG_COLOR Color(200, 255, 100, 255)
#define MSG_COLOR_NORMAL Color(255, 255, 255, 255)
#define MSG_COLOR_ERROR Color(255, 0, 0, 255)

#ifdef DEBUG
//#define SHOW_CONSOLE
#endif // DEBUG

#define PRINT_MSG(_msg,...) ConColorMsg(MSG_COLOR, "TrainSignals: "); ConColorMsg(MSG_COLOR_NORMAL, _msg,__VA_ARGS__)
#define PRINT_MSG_ERROR(_msg,...) ConColorMsg(MSG_COLOR, "TrainSignals: "); ConColorMsg(MSG_COLOR_ERROR, _msg,__VA_ARGS__)

//#define PRINT_DEBUG_MESSAGES
#ifdef PRINT_DEBUG_MESSAGES
	#define PRINT_MSG_DBG(_msg,...) DevMsg("TrainSignals: "); DevMsg(_msg,__VA_ARGS__)
#else
    #define PRINT_MSG_DBG(_msg,...)
#endif