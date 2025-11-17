#include "test_scene.hpp"

#include "api/helper/file_reader.hpp"

#include <iostream>

TestScene::TestScene()
	: m_banner([]() {
		FileReader fileReader("assets/banner.txt");
		std::string content;
		fileReader.read_string_content(content);
		return CanvasElement(content);
	}(), MIDDLE_CENTER) {
}

bool TestScene::is_dirty() {
	return m_banner.is_dirty();
}

void TestScene::keyboard_press(int key) {
}

void TestScene::update(const double delta_time) {
	m_banner.update(delta_time);
}

void TestScene::draw() {
	int width;
	int height;
	get_terminal_size(width, height);

	ElementSize size{width, height};
	const CanvasElement canvas = m_banner.build_widget(size);

	std::cout << canvas << std::flush;
}