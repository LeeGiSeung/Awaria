#pragma once


#include "framework.h"
#include "Client.h"

#include <crtdbg.h>
#include <Engine/global.h>
#include <Engine/CEngine.h>

#ifndef _DEBUG
#pragma comment(lib, "Engine//Engine")
#else
#pragma comment(lib, "Engine//Engine_d")
#endif


#ifndef _DEBUG
#pragma comment(lib, "Lecture//Lecture")
#else
#pragma comment(lib, "Lecture//Lecture_d")
#endif
