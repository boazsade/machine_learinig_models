#include "libs/csv_parser/csv_iterator.h"
#include "libs/csv_parser/csv_parser.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <string>
#include <algorithm>

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr<<"usage: "<<argv[0]<<" <input file> <line index>\n";
        return -1;
    }

    std::ifstream input{argv[1]};
    if (input) {
        
        csv::parser::source_type buffer{input};
        csv::parser parser;
        csv::rows_iterator<csv::parser> be{buffer, parser};
        csv::rows_iterator<csv::parser> e;
        auto ec = csv::cells_iterator<csv::parser>{};
        const int col = std::atoi(argv[2]);
        std::map<std::string, std::size_t> histogram;
        for (; be != e; ++be) {
            int off = 0;
            for (auto it = *be; it != ec; ++it, off++) {
                if (off == col) {
                    histogram[*it]++;    
                    std::cout<<"["<<*it<<"]";
                }
            }
        }
        std::cout<<"\nfinish processing"<<std::endl;
        std::for_each(std::begin(histogram), std::end(histogram), [](auto e) { std::cout<<"["<<e.first<<"] = "<<e.second<<std::endl; }); 
        std::cout<<std::endl;
    } else {
        std::cerr<<"error - cannot open "<<argv[1]<<std::endl;
        return -1;
    }
}
