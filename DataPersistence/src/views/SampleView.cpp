#include "SampleView.h"
#include <iostream>
#include <limits>

void SampleView::showMenu() const
{
    std::cout << "============================\n";
    std::cout << "        샘플 관리           \n";
    std::cout << "============================\n";
    std::cout << "  1. 샘플 등록\n";
    std::cout << "  2. 샘플 목록 조회\n";
    std::cout << "  3. 샘플 수정\n";
    std::cout << "  4. 샘플 삭제\n";
    std::cout << "  0. 돌아가기\n";
    std::cout << "============================\n";
    std::cout << "선택: ";
}

int SampleView::getMenuChoice() const
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

void SampleView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}
