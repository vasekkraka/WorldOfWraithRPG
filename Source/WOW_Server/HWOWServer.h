/*
|--------------------------------------------------------------------|
|                                                                    |
|    Václav Macura   (c) 2015      World of Wraith - Story Begin     |
|                                                                    |
|    ===> CWOWServer.cpp - hlavní include soubor                     |
|                                                                    |
|--------------------------------------------------------------------|
*/

#pragma warning(disable:4251)
#define _WIN32_WINNT 0x0501

#ifndef HWOWKlient
#define HWOWKlient

#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <boost\thread\locks.hpp> 
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>

#include <cstdio>
#include <Windows.h> 

#include <irrlicht.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <mysql_error.h>
#include <cppconn\prepared_statement.h>


#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "HServerInit.h"
#include "HDataTypesDB.h"
#include "HDataTypesInstance.h"
#include "HBasicFunctions.h"
#include "HServerInstance.h"

#include "HNetworkConnector.h"

#endif