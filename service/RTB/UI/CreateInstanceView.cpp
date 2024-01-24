// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CreateInstanceView.h"

#include "RTB/InstanceManager.h"
#include "RTB/ProfileManager.h"
#include "RTB/UI/Context.h"

#include <imgui.h>

#include <cstring>

namespace RTB {
namespace UI {

static bool ValidateName(const char* name, size_t maxSize);

void CreateInstanceView::Update(Context* ctx, bool* show, bool focus)
{
	if (!*show)
		return;

	auto viewport = ImGui::GetMainViewport();

	bool busy = ctx->InstanceManager->IsBusy();
	bool nameInUse = false;
	bool nameValid = true;

	ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(320, 240), ImGuiCond_Appearing);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	const bool render = ImGui::Begin("Create Instance", busy ? nullptr : show, ImGuiWindowFlags_NoCollapse);
	ImGui::PopStyleVar(2);

	if (focus)
		ImGui::SetWindowFocus();

	if (render)
	{
		ImGui::InputText("Name", m_nameInput, NameInputSize);

		size_t nameLength = strnlen_s(m_nameInput, NameInputSize);

		nameValid = ValidateName(m_nameInput, NameInputSize);
		nameInUse = nameValid && static_cast<bool>(ctx->InstanceManager->FindByName({ m_nameInput, nameLength }));

		ImGui::BeginDisabled(busy || !nameValid || nameInUse);

		if (ImGui::Button("Confirm"))
		{
			ctx->InstanceManager->Create(ctx->Scheduler, ctx->ProfileManager->GetSteamProfile(), { m_nameInput, nameLength });
			*show = false;
		}

		ImGui::EndDisabled();

		ImGui::SameLine();
		*show &= !ImGui::Button("Cancel");

		if (!nameValid)
			ImGui::Text("Name is invalid.");
		if (nameInUse)
			ImGui::Text("Name is in use.");
		if (busy)
			ImGui::Text("Instance manager is busy.");
	}

	ImGui::End();

	if (!*show)
	{
		m_nameInput[0] = 0;
	}
}

static bool ValidateName(const char* name, size_t maxSize)
{
	size_t length = strnlen_s(name, maxSize);
	if (length == 0)
		return false;

	char c;

	for (size_t i = 0; i < length; ++i)
	{
		c = name[i];

		if (c == '\\' || c == '/')
			return false;
	}

	// TODO: Invalidate if contains illegal path characters.

	return true;
}

} // namespace UI
} // namespace RTB
