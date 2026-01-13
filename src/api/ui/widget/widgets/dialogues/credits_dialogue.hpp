#pragma once
#include <memory>

#include "api/ui/scene/dialogue.hpp"
#include "api/ui/widget/widgets/dialogues/simple_dialogue.hpp"

class CreditsDialogue : public SimpleDialogue {
public:
    explicit CreditsDialogue();

    static DialogueOptions get_dialogue_options();

    static StackInfo get_stack_info();

    static std::shared_ptr<Dialogue> get_dialogue();
};
