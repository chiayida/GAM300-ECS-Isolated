/******************************************************************************/
/*!
\file       Logging.hpp
\author     Ow Jian Wen
			Chia Yi Da
\param      jianwen.o
			c.yida
\param      GAM 300
\date       31 August 2022
\brief
  This file contains the declaration and implementation of LogState and Logging class.
  
  Logging allows different colors of text to be written to console to indicate the 
  different types of message. This would enable user to keep track during debugging.
  
  How to Use:
  #include Logging.hpp before any other files to prevent redefinition
  
  LOG_INFO(...)
  - Printing of strings for checks whether something works (Blue text)
  
  LOG_EVENT(...)
  - Printing of event information (Green text)
  
  LOG_WARNING(...)
  - Printing of warnings (Purple text)
  
  LOG_ERROR(...)
  - Printing of errors (Red text)
  
  LOG_INSTANCE(...)
  - Printing of instance (Eg: application, manager) creation and destruction (Yellow text)

Copyright (C) 2022 DigiPen Institure of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/
#pragma once

// Logs will not be shown other than in DEBUG mode
#ifdef _DEBUG

#define NOMINMAX
#include <Windows.h> // Console text color
#include <iostream>  // Output stream
#include <assert.h>

enum class LogState 
{
	Info = 0,
	Event,
	Warning,
	Error,
	Instance
};

#define LOG_INFO(...)	Engine::Logging::AddLog(LogState::Info, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_EVENT(...)	Engine::Logging::AddLog(LogState::Event, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) Engine::Logging::AddLog(LogState::Warning, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)	Engine::Logging::AddLog(LogState::Error, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INSTANCE(...)	Engine::Logging::AddLog(LogState::Instance, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ASSERT(t) assert(t)

namespace Engine 
{
	class Logging 
	{
	public:
		template <typename T, typename... Types>
		static void print(T var1, Types... var2)
		{
			std::cout << var1 << " ";

			print(var2...);
		}

		template <typename T>
		static void print(T var1)
		{
			std::cout << var1 << "\n";
		}

		template <typename... Types>
		static void AddLog(LogState ls, std::string _fileName, int _location, Types... listOfVar) 
		{
			WORD col = 7;
			switch (ls) 
			{
			case LogState::Info:
			{
				col = 9;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
				std::cout << "[Info]";
				break;
			}
			case LogState::Event:
			{
				col = 10;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
				std::cout << "[Event]";
				break;
			}
			case LogState::Warning:
			{
				col = 13;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
				std::cout << "[Warning]";
				break;
			}
			case LogState::Error:
			{
				col = 4;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
				std::cout << "[Error]";
				break;
			}
			case LogState::Instance:
			{
				col = 14;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
				std::cout << "[Instance]";
				break;
			}
			}

			// Change back default color
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

			std::cout << "[" << _fileName.substr(_fileName.find_last_of('\\') + 1) << "(Line: " << _location << ")] ";

			print(listOfVar...);
		}
	};
}

#else
// do {} while (0) to remove warnings
#define LOG_INFO(...)		do {} while (0)
#define LOG_EVENT(...)		do {} while (0)
#define LOG_WARNING(...)	do {} while (0)
#define LOG_ERROR(...)		do {} while (0)
#define LOG_INSTANCE(...)   do {} while (0)
#define LOG_ASSERT(t)		do {} while (0)

#endif