#pragma once
#include <string>

class JsonFileStorage
{
public:
    explicit JsonFileStorage(const std::string& filePath);

    std::string load()                          const;
    void        save(const std::string& json)   const;
    bool        exists()                        const;

private:
    std::string m_filePath;
};
