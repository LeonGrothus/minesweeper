#pragma once

#include <string>

class FileManager {
public:
    explicit FileManager(const std::string &filePath);

    bool file_exists() const;

    bool read_string_content(std::string &outContent) const;

    bool write_string_content(const std::string &content) const;

    bool write_string_content(const std::wstring &content) const;

private:
    std::string m_filePath;
};
