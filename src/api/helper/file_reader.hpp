#pragma once

#include <fstream>
#include <sstream>
#include <string>

class FileReader {

  public:
	FileReader(const std::string &filePath) { this->m_filePath = filePath; }

	bool fileExists() {
		std::ifstream fileStream(m_filePath);
		return fileStream.good();
	}

	bool readStringContent(std::string &outContent) {
		std::ifstream fileStream(m_filePath);
		if (!fileStream.good()) {
			return false;
		}

		std::ostringstream outStringStream;

		outStringStream << fileStream.rdbuf();
		outContent = outStringStream.str();
		return true;
	}

	bool writeStringContent(const std::string &content) {
		std::ofstream outFileStream(m_filePath);

		if (!outFileStream.good()) {
			return false;
		}

		outFileStream << content;
		return true;
	}

  private:
	std::string m_filePath;
};