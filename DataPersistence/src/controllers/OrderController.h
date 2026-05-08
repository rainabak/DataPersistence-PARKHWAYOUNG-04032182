#pragma once
#include "IController.h"
#include "../views/OrderView.h"

class OrderController : public IController
{
public:
    explicit OrderController(OrderView& view);
    void run() override;

private:
    OrderView& m_view;

    void handleChoice(int choice);
};
