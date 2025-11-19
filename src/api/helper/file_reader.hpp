#pragma once

#include <string>

class FileReader {

  public:
	FileReader(const std::string &filePath);

	bool file_exists() const;
	bool read_string_content(std::string &outContent) const;
	bool write_string_content(const std::string &content) const;

  private:
	std::string m_filePath;
};
