#pragma once
#include <ge_engine/Core.h> 
#include <ge_engine/Components.h>
#include <imgui.h>
#include <unordered_map>
#include <string>

namespace geProject {
	class HierarchyWindow {
	public:
		HierarchyWindow();
		void imguiUpdate();
	private:
		std::unordered_map<int, std::string> entityList;
		void updateEntityList(HierarchyEvent* e);
	};
}