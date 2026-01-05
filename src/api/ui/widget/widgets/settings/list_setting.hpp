#pragma once
#include <functional>
#include <memory>

#include "api/helper/looped_execution_wrapper.hpp"
#include "api/ui/widget/widget.hpp"

struct ListSettingOption {
    explicit ListSettingOption(std::u16string option, std::function<void()> select = []() {
                               },
                               std::function<void()> deselect = []() {
                               }) : option(std::move(option)),
                                    select(std::move(select)), deselect(std::move(deselect)) {
    }

    std::u16string option;
    std::function<void()> select;
    std::function<void()> deselect;

    bool operator==(std::vector<ListSettingOption>::const_reference other) const {
        return option == other.option;
    }
};

class ListSetting : public Widget {
public:
    explicit ListSetting(std::u16string description, bool loop_selection = true);

    void add_option(const ListSettingOption &option);

    void add_option_at_index(const ListSettingOption &option, int index);

    void remove_option(const ListSettingOption &option);

    void remove_option(int index);

    int get_options_count() const;

    int get_index_of_option(const ListSettingOption &option) const;

    const ListSettingOption &get_current_option();

    void set_description_spacing(int spacing);

    void set_min_options_spacing(int spacing);

    Vector2D get_minimum_size() const override;

    void keyboard_press(int key) override;

    void update(double delta_time) override;

    bool is_dirty() const override;

protected:
    CanvasElement build_canvas_element(const Vector2D &size) override;

private:
    int get_options_size() const;

    void move_selection(int amount);

    void move_selection_up();

    void move_selection_down();

    std::u16string m_description;

    bool m_loop_selection = true;
    bool m_blink_option = true;

    int m_description_spacing = 20;
    int m_min_options_spacing = 10;

    int m_current_option = 0;
    std::vector<ListSettingOption> m_options = std::vector<ListSettingOption>();
};
