#pragma once

#include <imgui/imgui.h>


#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>

	class Log
	{
	public:
		Log();
		~Log();

		static void Init();

		/*static bool m_useTerminal;
		static bool m_MakeTable; // todo

		static bool showMessages;
		static bool showWarnings;
		static bool showErrors;
		
		static std::vector<std::string> m_filter;


		static std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> messages;
		static std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> warnigns;
		static std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> errors;
		*/
		static void Message(std::string msg);
		static void Warning(std::string msg);
		static void Error(std::string msg);

		static void Message(std::string category, std::string msg);
		static void Warning(std::string category, std::string msg);
		static void Error(std::string category, std::string msg);

		static void Clear();
		static void SetFilter(std::vector<std::string> filter);

		static std::string GetTimeStamp();

		static void Show();
	};
