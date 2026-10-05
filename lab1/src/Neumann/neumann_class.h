#pragma once
#include <vector>
#include <string>
#include <map>

class Neumann_class{
public:
    int variant = 9 * 11 * 2006 * 21 * 12;

    std::vector<int> f_col;
    //верх, лево, центр, право, низ
    std::map<std::string, int> neumann;

    Neumann_class();
    void fill_f_col();
    void fill_neumann();
};