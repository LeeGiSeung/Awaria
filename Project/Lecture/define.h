#pragma once

//#define GAME_RELEASED

#ifdef GAME_RELEASED
#define ADD_FLOAT(Desc, pData)
#else
#define ADD_FLOAT(Desc, pData) AddScriptParam(SCRIPT_PARAM::FLOAT, Desc, pData);
#endif