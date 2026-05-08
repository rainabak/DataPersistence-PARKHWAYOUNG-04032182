#include "MonitoringView.h"
#include <iostream>
#include <limits>

void MonitoringView::showMenu() const
{
    std::cout << "============================\n";
    std::cout << "         모니터링           \n";
    std::cout << "============================\n";
    std::cout << "  1. 현황 조회\n";
    std::cout << "  2. 알림 목록\n";
    std::cout << "  3. 리포트 출력\n";
    std::cout << "  0. 돌아가기\n";
    std::cout << "============================\n";
    std::cout << "선택: ";
}

int MonitoringView::getMenuChoice() const
{
    int choice;
    if (!(std::cin >> choice))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void MonitoringView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}
