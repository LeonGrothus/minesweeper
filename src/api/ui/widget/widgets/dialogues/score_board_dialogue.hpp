#pragma once

#include <memory>

#include "api/controller/score_board.hpp"
#include "api/ui/scene/dialogue.hpp"
#include "api/ui/widget/widgets/dialogues/simple_dialogue.hpp"

class ScoreBoardDialogue : public SimpleDialogue {
public:
    explicit ScoreBoardDialogue(std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& score_board_entries);

    static DialogueOptions get_dialogue_options();

    static StackInfo get_stack_info();

    static std::shared_ptr<Dialogue> get_dialogue(const std::shared_ptr<ScoreBoardDialogue>& score_board_dialogue);
};
