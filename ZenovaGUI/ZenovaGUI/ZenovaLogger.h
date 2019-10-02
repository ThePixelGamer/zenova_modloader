#pragma once
#include <imgui.h>
#include <imgui_internal.h>

struct ZenovaLogger {
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
    bool                AutoScroll;     // Keep scrolling if already at the bottom

	ZenovaLogger();

	void Clear();
	void AddLog(const char*, ...) IM_FMTARGS(2);
	void Draw(const char*, bool* = NULL);
};