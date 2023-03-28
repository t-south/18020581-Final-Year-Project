#include "HierarchyWindow.h"

geProject::HierarchyWindow::HierarchyWindow()
{
	eventSystem.subscribe(this, &HierarchyWindow::updateEntityList);
}

void geProject::HierarchyWindow::imguiUpdate(){		
	ImGui::Begin("Scene Hierarchy");
	int index = 0;
	for (auto const& entity : entityList) {
		ImGui::PushID(index);	
		bool tree = ImGui::TreeNodeEx(entity.second.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding |
													ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);

		ImGui::PopID();
		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("sceneObject", &entity.first, sizeof(entity.first));
			ImGui::Text(entity.second.c_str());			
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget()) {
			auto sceneObject = ImGui::AcceptDragDropPayload("sceneObject");
			if (sceneObject != nullptr) {
				std::cout << "Payload Accepted" << std::endl;
			}
			ImGui::EndDragDropTarget();
		}
		if (tree) {
			ImGui::TreePop();
		}
		index++;
	}


	ImGui::End();
}

void geProject::HierarchyWindow::updateEntityList(HierarchyEvent* e){	
	if (e->availability) {
		entityList[e->entityId] = e->entityName;
	}
	else {
		entityList.erase(e->entityId);
	}
}
