// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "HubView.h"

#include <cstdlib>

#include <imgui.h>

namespace RTB {
namespace UI {

void HubView::Update(Context* ctx)
{
	static constexpr ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar;

	auto viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("#hub", nullptr, windowFlags);
	ImGui::PopStyleVar(3);

	ImGui::DockSpace(ImGui::GetID("hub"), {}, ImGuiDockNodeFlags_None);

	bool showInstances {};
	bool showProfiles {};

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Open", nullptr, nullptr, false);

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
				std::exit(0);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Undo", nullptr, nullptr, false);
			ImGui::MenuItem("Redo", nullptr, nullptr, false);

			ImGui::Separator();

			ImGui::MenuItem("Cut", nullptr, nullptr, false);
			ImGui::MenuItem("Copy", nullptr, nullptr, false);
			ImGui::MenuItem("Paste", nullptr, nullptr, false);
			ImGui::MenuItem("Delete", nullptr, nullptr, false);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			showInstances = ImGui::MenuItem("Instances");
			showProfiles = ImGui::MenuItem("Profiles");
			ImGui::MenuItem("Mods", nullptr, nullptr, false);
			ImGui::MenuItem("Saves", nullptr, nullptr, false);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Help", nullptr, nullptr, false);

			ImGui::Separator();

			ImGui::MenuItem("About", nullptr, nullptr, false);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	m_showInstances |= showInstances;
	m_showProfiles |= showProfiles;

	m_instances.Update(ctx, &m_showInstances, showInstances);
	m_profiles.Update(ctx, &m_showProfiles, showProfiles);

	ImGui::End();
}

} // namespace UI
} // namespace RTB
