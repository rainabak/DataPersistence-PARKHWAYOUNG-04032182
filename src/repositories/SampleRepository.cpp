#include "SampleRepository.h"

SampleRepository::SampleRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
}

void SampleRepository::add(const Sample& sample)
{
    // TODO: CRUD 구현
}

std::vector<Sample> SampleRepository::findAll() const
{
    // TODO: CRUD 구현
    return m_samples;
}

Sample* SampleRepository::findById(int id)
{
    // TODO: CRUD 구현
    return nullptr;
}

bool SampleRepository::update(const Sample& sample)
{
    // TODO: CRUD 구현
    return false;
}

bool SampleRepository::remove(int id)
{
    // TODO: CRUD 구현
    return false;
}
