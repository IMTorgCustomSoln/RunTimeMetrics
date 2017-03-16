#ifndef PRIORITYCLASS_H
#define PRIORITYCLASS_H

#include "windows.h"

enum Priority 
{
	Realtime = REALTIME_PRIORITY_CLASS,
	Normal = NORMAL_PRIORITY_CLASS,
	High = HIGH_PRIORITY_CLASS,
	BelowNormal = BELOW_NORMAL_PRIORITY_CLASS,
	AboveNormal = ABOVE_NORMAL_PRIORITY_CLASS
};

struct ParamsOfProcs
{
	std::string prameters;
	Priority priority;
};

#endif