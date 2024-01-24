// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ProfilesView.h"

#include "RTB/ProfileManager.h"
#include "RTB/UI/Context.h"

#include <imgui.h>

namespace RTB {
namespace UI {

void ProfilesView::Update(Context* ctx, bool* show, bool focus)
{
	if (*show)
	{
		auto viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Appearing);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		const bool render = ImGui::Begin("Profiles", show, ImGuiWindowFlags_NoCollapse);
		ImGui::PopStyleVar(2);

		if (render)
		{
			if (focus)
				ImGui::SetWindowFocus();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));

			if (ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)1), ImVec2(0, 100), ImGuiChildFlags_Border))
			{
				ImGui::Text("Steam");

				if (auto steamProfile = ctx->ProfileManager->GetSteamProfile())
				{
					ImGui::Text("Location: %s", steamProfile->GetDirectory().generic_string().c_str());
				}
				else
				{
					auto status = ctx->ProfileManager->GetSteamProfileStatus();

					static constexpr const char* StatusMessages[] {
						"Status unknown.",
						"",
						"Blockland is not installed.",
						"Could not find Steam library.",
						"Steam profile search is not implemented for this platform.",
					};

					ImGui::Text(StatusMessages[status]);
				}
			}

			ImGui::EndChild();

			ImGui::PopStyleVar(1);
		}

		ImGui::End();
	}
}

} // namespace UI
} // namespace RTB
