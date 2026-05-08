#include "OrderView.h"
#include <iostream>
#include <limits>

void OrderView::showMenu() const
{
    std::cout << "============================\n";
    std::cout << "        주문 관리           \n";
    std::cout << "============================\n";
    std::cout << "  1. 주문 등록\n";
    std::cout << "  2. 주문 목록 조회\n";
    std::cout << "  3. 주문 수정\n";
    std::cout << "  4. 주문 취소\n";
    std::cout << "  0. 돌아가기\n";
    std::cout << "============================\n";
    std::cout << "선택: ";
}

int OrderView::getMenuChoice() const
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

void OrderView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}
