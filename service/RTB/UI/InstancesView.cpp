// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InstancesView.h"

#include "RTB/InstanceManager.h"
#include "RTB/Profile.h"
#include "RTB/UI/Context.h"

#include <imgui.h>

namespace RTB {
namespace UI {

void InstancesView::Update(Context* ctx, bool* show, bool focus)
{
	if (*show)
	{
		auto viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Appearing);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		const bool render = ImGui::Begin("Instances", show, ImGuiWindowFlags_NoCollapse);
		ImGui::PopStyleVar(2);

		if (render)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));

			auto instances = ctx->InstanceManager->GetInstances();
			size_t count = instances.size();

			for (size_t i = 0; i < count; ++i)
			{
				const auto& instance { instances[i] };

				if (ImGui::BeginChild(ImGui::GetID((void*)(uintptr_t)i), ImVec2(0, 100), ImGuiChildFlags_Border))
				{
					ImGui::Text("Name: %s", instance->GetName().c_str());

					if (auto profile = instance->GetProfile())
						ImGui::Text("Profile: %s", profile->GetDirectory().generic_string().c_str());
					else
						ImGui::Text("Profile: Not bound (%s).", instance->GetProfileDirectory().generic_string().c_str());

					ImGui::Button("Start");
					ImGui::SameLine();
					ImGui::Button("Delete");
				}

				ImGui::EndChild();
			}

			if (ImGui::BeginChild(ImGui::GetID((void*)(uintptr_t)count), ImVec2(0, 100), ImGuiChildFlags_Border))
			{
				m_focusCreateInstance = ImGui::Button("Create");
				m_showCreateInstance |= m_focusCreateInstance;
			}

			ImGui::EndChild();

			ImGui::PopStyleVar(1);
		}

		ImGui::End();
	}

	m_createInstance.Update(ctx, &m_showCreateInstance, m_focusCreateInstance);
	m_focusCreateInstance = false;
}

} // namespace UI
} // namespace RTB
