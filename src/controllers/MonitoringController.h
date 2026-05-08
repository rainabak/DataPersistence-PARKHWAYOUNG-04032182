#pragma once
#include "IController.h"
#include "../views/MonitoringView.h"

class MonitoringController : public IController
{
public:
    explicit MonitoringController(MonitoringView& view);
    void run() override;

private:
    MonitoringView& m_view;

    void handleChoice(int choice);
};
