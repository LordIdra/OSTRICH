#include <main/Control.h>



auto main() -> int {
    Control::Init(false, "OSTRICH");
    Control::PromptScenarioLoad();
    Control::Mainloop();
}