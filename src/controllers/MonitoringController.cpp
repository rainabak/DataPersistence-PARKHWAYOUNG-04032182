#include "MonitoringController.h"
#include "../utils/ConsoleUtil.h"

MonitoringController::MonitoringController(MonitoringView& view)
    : m_view(view)
{
}

void MonitoringController::run()
{
    while (true)
    {
        ConsoleUtil::clearScreen();
        m_view.showMenu();

        int choice = m_view.getMenuChoice();

        if (choice == 0)
            break;

        handleChoice(choice);
        ConsoleUtil::pause();
    }
}

void MonitoringController::handleChoice(int choice)
{
    ConsoleUtil::printSeparator();
    switch (choice)
    {
    case 1:
    case 2:
    case 3:
        m_view.showMessage("준비 중입니다.");
        break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}
