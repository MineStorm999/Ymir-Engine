#include "Log.h"

#include <imgui/imgui.h>


#define EDITOR

namespace {
	bool m_useTerminal;
	bool m_MakeTable; // todo

	bool showMessages;
	bool showWarnings;
	bool showErrors;

	bool debug;

	std::vector<std::string> m_filter;


	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> messages;
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> warnigns;
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> errors;

	std::unordered_map<std::string, bool> filter;

}
	Log::Log()
	{
	}

	Log::~Log()
	{
		Clear();
	}

	void Log::Init()
	{
		m_useTerminal = true;
		m_MakeTable = false;

		showMessages = true;
		showWarnings = true;
		showErrors = true;

		debug = true;
	}

	void Log::Message(std::string msg)
	{
#ifdef EDITOR



		messages[""].push_back(std::pair<std::string, std::string>(msg, GetTimeStamp()));

		if (filter.find("") == filter.end()) {

		}
		filter[""] = true;

		if (m_useTerminal) {
			std::cout << "[message] " << msg << "\n";
		}
#endif // EDITOR
	}

	void Log::Warning(std::string msg)
	{
#ifdef EDITOR
		warnigns[""].push_back(std::pair<std::string, std::string>(msg, GetTimeStamp()));

		if (filter.find("") == filter.end()) {

		}
		filter[""] = true;

		if (m_useTerminal) {
			std::cout << "[Warning] " << msg << "\n";
		}
#endif // EDITOR
	}

	void Log::Error(std::string msg)
	{
#ifdef EDITOR
		errors[""].push_back(std::pair<std::string, std::string>(msg, GetTimeStamp()));

		if (filter.find("") == filter.end()) {

		}
		filter[""] = true;

		if (m_useTerminal) {
			std::cout << "[ERROR] " << msg << "\n";
		}
#endif // EDITOR
	}

	void Log::Message(std::string category, std::string msg)
	{
#ifdef EDITOR
		messages[category].push_back(std::pair<std::string, std::string>(msg, GetTimeStamp()));

		if (filter.find("") == filter.end()) {

		}
		filter[category] = true;

		if (m_useTerminal) {
			std::cout << "[message][" << category << "] " << msg << "\n";
		}
#endif // EDITOR
	}

	void Log::Warning(std::string category, std::string msg)
	{
#ifdef EDITOR
		warnigns[category].push_back(std::pair<std::string, std::string>(msg, GetTimeStamp()));

		if (filter.find("") == filter.end()) {

		}
		filter[category] = true;

		if (m_useTerminal) {
			std::cout << "[Warning][" << category << "] " << msg << "\n";
		}
#endif // EDITOR
	}

	void Log::Error(std::string category, std::string msg)
	{
#ifdef EDITOR
		errors[category].push_back(std::pair<std::string, std::string>(msg, GetTimeStamp()));

		if (filter.find("") == filter.end()) {

		}
		filter[category] = true;

		if (m_useTerminal) {
			std::cout << "[ERROR][" << category << "] " << msg << "\n";
		}
#endif // EDITOR
	}

	void Log::Clear()
	{
#ifdef EDITOR
		m_filter.clear();

		messages.clear();
		warnigns.clear();
		errors.clear();
#endif // EDITOR
	}

	void Log::SetFilter(std::vector<std::string> fil)
	{
#ifdef EDITOR
		m_filter = fil;
#endif // EDITOR
	}

	std::string Log::GetTimeStamp()
	{
#ifdef EDITOR
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
		return ss.str();
#endif // EDITOR
	}
	bool ShowFilter;



	void Log::Show()
	{
#ifdef EDITOR
		if (m_useTerminal) {
			return;
		}

		if (ImGui::Begin("Log"))
		{

			if (!messages.empty() || !warnigns.empty() || errors.empty()) {
				if (ImGui::Button("Clear")) {
					Clear();
				}
			}
			else {
				if (ImGui::Button("     ")) {
					//Clear();
				}
			}

			if (ImGui::Button("Filter")) {
				ShowFilter = !ShowFilter;
			}

			if (ShowFilter) {
				ImGui::Checkbox("Show Messages", &showMessages);
				ImGui::SameLine();
				ImGui::Checkbox("Show Warnings", &showWarnings);
				ImGui::SameLine();
				ImGui::Checkbox("Show Errors", &showErrors);

				ImGui::Text("Show ");
				for (auto& kv : filter)
				{
					if (kv.first == "") {
						ImGui::Checkbox("Unknown", &kv.second);
						continue;
					}
					ImGui::Checkbox(kv.first.c_str(), &kv.second);
				}
			}

			ImGui::BeginChild("Output");

			if (m_MakeTable) {
				// todo
				

				return;
			}

			std::string str = "";
			//message
			if (showMessages) {
				for (auto& kv : messages)
				{
					if (!filter[kv.first]) {
						continue;
					}
					if (kv.first == "") {
						for (std::pair<std::string, std::string> s : kv.second)
						{
							str = "[message][" + s.second + "] " + s.first;
							ImGui::Text(str.c_str());
						}
						continue;
					}
					for (std::pair<std::string, std::string> s : kv.second)
					{
						str = "[message][" + s.second + "][" + kv.first + "] " + s.first;
						ImGui::Text(str.c_str());
					}
				}
			}

			// warning
			if (showWarnings) {
				for (auto& kv : warnigns)
				{
					if (!filter[kv.first]) {
						continue;
					}
					if (kv.first == "") {
						for (std::pair<std::string, std::string> s : kv.second)
						{
							str = "[Warning][" + s.second + "] " + s.first;
							ImGui::TextColored(ImVec4(255, 220, 0, 255), str.c_str());
						}
						continue;
					}
					for (std::pair<std::string, std::string> s : kv.second)
					{
						str = "[Warning][" + s.second + "][" + kv.first + "] " + s.first;
						ImGui::TextColored(ImVec4(255, 220, 0, 255), str.c_str());
					}
				}
			}

			// error
			if (showErrors) {
				for (auto& kv : errors)
				{
					if (!filter[kv.first]) {
						continue;
					}
					if (kv.first == "") {
						for (std::pair<std::string, std::string> s : kv.second)
						{
							str = "[ERROR][" + s.second + "] " + s.first;
							ImGui::TextColored(ImVec4(255, 0, 0, 255), str.c_str());
						}
						continue;
					}
					for (std::pair<std::string, std::string> s : kv.second)
					{
						str = "[ERROR][" + s.second + "][" + kv.first + "] " + s.first;
						ImGui::TextColored(ImVec4(255, 0, 0, 255), str.c_str());
					}
				}
			}

			ImGui::EndChild();

			ImGui::End();
		}
#endif // EDITOR
	}

