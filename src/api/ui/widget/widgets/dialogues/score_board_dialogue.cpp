#include "score_board_dialogue.hpp"

#include "../alignment.hpp"
#include "api/ui/widget/widgets/score_board_view.hpp"

static std::shared_ptr<Widget> make_score_board_content(std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& score_board_entries) {
    const std::shared_ptr<ScoreBoardView> score_board_view = std::make_shared<ScoreBoardView>(score_board_entries);
    return std::make_shared<Alignment>(score_board_view, MIDDLE_CENTER);
}

ScoreBoardDialogue::ScoreBoardDialogue(std::array<ScoreBoardEntry, SCORE_BOARD_NUMBER>& score_board_entries)
    : SimpleDialogue(make_score_board_content(score_board_entries)) {}

DialogueOptions ScoreBoardDialogue::get_dialogue_options() {
    return {};
}

StackInfo ScoreBoardDialogue::get_stack_info() {
    StackInfo stack_info;
    stack_info.height_percentage = 0.4;
    stack_info.width_percentage = 0.3;
    return stack_info;
}

std::shared_ptr<Dialogue> ScoreBoardDialogue::get_dialogue(
    const std::shared_ptr<ScoreBoardDialogue>& score_board_dialogue) {
    return std::make_shared<Dialogue>(score_board_dialogue, get_dialogue_options());
}
