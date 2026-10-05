#include <neumann_class.h>
#include <vector>


void Neumann_class::fill_f_col(){
    f_col.assign(32, 0);
    int var_copy = variant;

    int i = 31;
    while(var_copy){
        f_col[i] = var_copy % 2;
        var_copy /= 2;
        i--;
    }
}

void Neumann_class::fill_neumann(){
    for(int i = 0; i < 32; i++){
        std::string str_i = "";
        for(int bias = 0; bias < 5; bias++){
            str_i += std::to_string( (i >> (4 - bias)) & 1 );
        }
        neumann[str_i] = f_col[i];
    }
}

Neumann_class::Neumann_class(){
    fill_f_col();
    fill_neumann();
}