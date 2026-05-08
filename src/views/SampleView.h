#pragma once
#include <string>

class SampleView
{
public:
    void showMenu()                            const;
    int  getMenuChoice()                       const;
    void showMessage(const std::string& msg)   const;
};
