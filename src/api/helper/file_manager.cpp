#include "file_manager.hpp"
#include <fstream>
#include <sstream>

FileManager::FileManager(const std::string &filePath) {
    m_filePath = filePath;
}

bool FileManager::file_exists() const {
    const std::ifstream fileStream(m_filePath);
    return fileStream.good();
}

bool FileManager::read_string_content(std::string &outContent) const {
    std::ifstream fileStream(m_filePath);
    if (!fileStream.good()) {
        return false;
    }

    std::ostringstream outStringStream;

    outStringStream << fileStream.rdbuf();
    outContent = outStringStream.str();
    return true;
}

bool FileManager::write_string_content(const std::string &content) const {
    std::ofstream outFileStream(m_filePath);

    if (!outFileStream.good()) {
        return false;
    }

    outFileStream << content;
    return true;
}
