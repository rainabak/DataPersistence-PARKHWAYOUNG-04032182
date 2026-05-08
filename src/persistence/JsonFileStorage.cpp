#include "JsonFileStorage.h"

JsonFileStorage::JsonFileStorage(const std::string& filePath)
    : m_filePath(filePath)
{
}

std::string JsonFileStorage::load() const
{
    // TODO: 파일 읽기 구현
    return "";
}

void JsonFileStorage::save(const std::string& json) const
{
    // TODO: 파일 쓰기 구현
}

bool JsonFileStorage::exists() const
{
    // TODO: 파일 존재 확인 구현
    return false;
}
