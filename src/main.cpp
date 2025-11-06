#include "api/helper/file_reader.hpp"
#include "api/helper/terminal_helper.hpp"
#include <iostream>
#include <ostream>
#include <string_view>

int count(std::string& string, char c) {
	int result = 0;
	for(char character : string) {
		if(character == c) result++;
	}
	return result;
}

void printBanner(long windowWidth, long windowHeight) {
	FileReader fileReader("assets/banner.txt");
	std::string content;
	fileReader.readStringContent(content);

	std::string::size_type currentPos = 0;
	std::string::size_type prevPos = 0;

	std::string::size_type size = content.size();

	std::string_view contentView(content);

	int heigtOfContent = count(content, '\n');
	int heightPadding = (windowHeight - heigtOfContent) / 2;

	std::cout << std::string(heightPadding, '\n');

	while (prevPos < size) {
		currentPos = content.find('\n', prevPos);
		std::string_view subString;
		if (currentPos == std::string::npos) {
			subString = contentView.substr(prevPos);
			prevPos = size;
		} else {
			subString = contentView.substr(prevPos, currentPos - prevPos);
			prevPos = currentPos + 1;
		}
		long widthPadding = (windowWidth - subString.length()) / 2;

		if(widthPadding > 0) {
			std::cout << std::string(widthPadding, ' ');
		}

		std::cout << subString << std::endl;
	}
	std::cout << std::string(heightPadding, '\n');
}

int main(int argc, char *argv[]) {
	int width, height;
	getTerminalSize(width, height);
	printBanner(width, height);
	return 0;
}
