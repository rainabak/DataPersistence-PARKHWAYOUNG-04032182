#pragma once
#include "IController.h"
#include "../views/SampleView.h"

class SampleController : public IController
{
public:
    explicit SampleController(SampleView& view);
    void run() override;

private:
    SampleView& m_view;

    void handleChoice(int choice);
};
