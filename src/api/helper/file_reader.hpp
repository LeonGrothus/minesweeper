#pragma once

#include <string>

class FileReader {

  public:
	FileReader(const std::string &filePath);

	bool file_exists();
	bool read_string_content(std::string &outContent);
	bool write_string_content(const std::string &content);

  private:
	std::string m_filePath;
};
