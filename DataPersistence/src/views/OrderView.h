#pragma once
#include <string>

class OrderView
{
public:
    void showMenu()                            const;
    int  getMenuChoice()                       const;
    void showMessage(const std::string& msg)   const;
};
