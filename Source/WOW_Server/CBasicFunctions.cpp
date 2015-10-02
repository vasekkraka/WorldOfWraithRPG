/*
|--------------------------------------------------------------------|
|                                                                    |
|    Václav Macura   (c) 2015      World of Wraith - Story Begin     |
|                                                                    |
|    ===> CWOWServer.cpp - hlavièka zakladnich metod pro konverzi    |
|                          a zakladni operace s daty                 |
|--------------------------------------------------------------------|
*/

#include "HWOWServer.h"

namespace WowServer
{
	s32 convertTimeToSec(s32 hour, s32 minutes, s32 sec)
	{
		return (hour * 60) + (minutes * 60) + sec;
	};
}