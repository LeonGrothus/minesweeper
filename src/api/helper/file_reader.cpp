#include "file_reader.hpp"
#include <fstream>
#include <sstream>

FileReader::FileReader(const std::string &filePath) {
	m_filePath = filePath;
}

bool FileReader::file_exists() const {
	const std::ifstream fileStream(m_filePath);
	return fileStream.good();
}

bool FileReader::read_string_content(std::string &outContent) const {
	std::ifstream fileStream(m_filePath);
	if (!fileStream.good()) {
		return false;
	}

	std::ostringstream outStringStream;

	outStringStream << fileStream.rdbuf();
	outContent = outStringStream.str();
	return true;
}

bool FileReader::write_string_content(const std::string &content) const {
	std::ofstream outFileStream(m_filePath);

	if (!outFileStream.good()) {
		return false;
	}

	outFileStream << content;
	return true;
}