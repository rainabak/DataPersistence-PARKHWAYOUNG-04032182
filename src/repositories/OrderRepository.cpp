#include "OrderRepository.h"

OrderRepository::OrderRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
}

void OrderRepository::add(const Order& order)
{
    // TODO: CRUD 구현
}

std::vector<Order> OrderRepository::findAll() const
{
    // TODO: CRUD 구현
    return m_orders;
}

Order* OrderRepository::findById(int id)
{
    // TODO: CRUD 구현
    return nullptr;
}

bool OrderRepository::update(const Order& order)
{
    // TODO: CRUD 구현
    return false;
}

bool OrderRepository::remove(int id)
{
    // TODO: CRUD 구현
    return false;
}
