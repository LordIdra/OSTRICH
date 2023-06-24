#include "Fonts.h"



namespace Fonts {
    namespace {
        const string MAIN_FONT_PATH = "../resources/fonts/inter-light.ttf";
        const string DATA_FONT_PATH = "../resources/fonts/source-code-pro-medium.otf";
        const string ICON_FONT_PATH = "../resources/fonts/material-design-icons.ttf";

        const float FONT_SIZE = 20.0F;
        const float BIG_FONT_SIZE = 40.0F;

        const ImWchar ICON_RANGE[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0}; // NOLINT (interfacing with C library)

        ImFont* mainFont;
        ImFont* mainFontBig;
        ImFont* dataFont;
        ImFont* iconFont;
    }

    auto Init() -> void {
        // Load font config and set config options
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig config;
        config.MergeMode = true;
        config.GlyphMinAdvanceX = FONT_SIZE;

        // Load 'main' font and merge icons
        mainFont = io.Fonts->AddFontFromFileTTF(MAIN_FONT_PATH.c_str(), FONT_SIZE);
        io.Fonts->AddFontFromFileTTF(ICON_FONT_PATH.c_str(), FONT_SIZE, &config, (const ImWchar*)ICON_RANGE);

        // Load 'main' font (but bigger) and merge icons
        mainFontBig = io.Fonts->AddFontFromFileTTF(MAIN_FONT_PATH.c_str(), BIG_FONT_SIZE);
        io.Fonts->AddFontFromFileTTF(ICON_FONT_PATH.c_str(), BIG_FONT_SIZE, &config, (const ImWchar*)ICON_RANGE);

        // Load 'data' font and merge icons
        dataFont = io.Fonts->AddFontFromFileTTF(DATA_FONT_PATH.c_str(), FONT_SIZE);
        io.Fonts->AddFontFromFileTTF(ICON_FONT_PATH.c_str(), FONT_SIZE, &config, (const ImWchar*)ICON_RANGE);
    }

    auto NormalizeString(ImFont* font, const string &str, const float maxWidth) -> string {
        string newString = str;

        ImGui::PushFont(font);

        bool atLeastOneCharacterRemoved = false;

        // While the text width is greater than the max width, remove the last character
        while (ImGui::CalcTextSize(newString.c_str()).x > maxWidth) {
            newString = newString.substr(0, newString.length() - 2);
            atLeastOneCharacterRemoved = true;
        }

        if (atLeastOneCharacterRemoved) {
            newString += "...";
        }

        ImGui::PopFont();

        return newString;
    }

    auto Main() -> ImFont* {
        return mainFont;
    }
    auto MainBig() -> ImFont* {
        return mainFontBig;
    }

    auto Data() -> ImFont* {
        return dataFont;
    }
}