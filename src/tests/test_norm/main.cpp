#include "utils/math_utils.h"
#include "utils/algorithms.hpp"
#include <boost/program_options.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>

template<typename It, typename T>
bool verify(It from, It to, T min, T max)
{
    return std::all_of(from, to, [min, max](auto val) {
                    return val >= min && val <= max;
                }
            );
}

struct program_args
{
    enum norm_type {
        MIN_MAX, MAX, Z_NORM, NONE
    };

    norm_type type = NONE;
    double min = 0;
    double max = 1;
    std::string in_file;

    program_args() = default;
    program_args(norm_type t, double mn, double mx, const std::string& p) :
            type(t), min(mn), max(mx), in_file(p)
    {
    }

    static norm_type type_convert(const std::string& from) {
            static const std::map<std::string, norm_type> convertion{
                {"minmax", MIN_MAX}, {"max", MAX}, {"znorm", Z_NORM}
            };
            auto i = convertion.find(from);
            return i == std::end(convertion) ? NONE : i->second;
    }
};

namespace po = boost::program_options;
program_args process_args(int argc, char** argv)
{
    program_args args;
    po::options_description desc("Allowed options");
    std::string algo;
    desc.add_options()
        ("help,h", "test usage:")
        ("min,m", po::value<double>(&args.min)->default_value(args.min),
         "algorithm min value (for min/max norm only)")
        ("max,x", po::value<double>(&args.max)->default_value(args.max),
         "max value (for min/max  norm only")
        ("file,f", po::value<std::string>(&args.in_file),
         "input file to read data from")
        ("algo,a", po::value<std::string>(&algo),
         "normalization algoritm, select from 'minmax' 'max' or 'znorm'");
    po::variables_map cli;
    if (argc == 1 || cli.count("help")) {
        std::cerr<<desc<<std::endl;
        throw std::runtime_error{"no args"};;
    }
    try {
        po::store(po::command_line_parser(argc, argv).options(desc).run(), cli);
        po::notify(cli);
    } catch (const std::exception& e) {
        std::cerr<<"error - invalid command line: "<<e.what()<<"\n"<<desc<<std::endl;
        throw e; 
    }
    if (algo.empty()) {
        std::cerr<<"algorithm type not set\n"<<desc<<std::endl;
        throw std::runtime_error{"missing args"};
    } else {
        auto a = program_args::type_convert(algo);
        if (a == program_args::NONE) {
            std::cerr<<"invalid algorithm: "<<algo<<"\n"<<desc<<std::endl;
            throw std::runtime_error{"invalid algortihm"};
        }
        args.type = a;
        return args;
    }
}

int process_min_max(const std::vector<double>& data, double min, double max)
{
    auto out = utils::min_max_normalization(data, min, max);
    std::cout<<"after normalization:\n";
    out_range(std::cout, out);
    std::cout<<std::endl;
    if (verify(std::begin(out), std::end(out), min, max)) {
        std::cout<<"all values after norm are valid"<<std::endl;
        return 0;
    } else {
        std::cout<<"verification failed - we have values that are out of range ["<<min<<", "<<max<<"]\n";
        return -1;
    }
}

int process_max(const std::vector<double>& data, double)
{
    auto out = utils::max_normalization(data);
    std::cout<<"after normalization:\n";
    out_range(std::cout, out);
    std::cout<<std::endl;
    return 0;
}

int process_znorm(const std::vector<double>& data)
{
    auto out = utils::z_normalization(data);
    std::cout<<"after normalization:\n";
    out_range(std::cout, out);
    std::cout<<std::endl;
    return 0;
}

int process(const program_args& args)
{
    std::ifstream input(args.in_file.c_str());
    if (!input) {
        std::cerr<<"failed to open "<<args.in_file<<" for reading\n";
        return -1;
    }
    std::cout<<"reading input from "<<args.in_file<<std::endl;
    const std::vector<double> data{std::istream_iterator<double>(input), 
                                   std::istream_iterator<double>()
    };
    if (data.empty()) {
        std::cerr<<"failed  to read from "<<args.in_file<<std::endl;
        return -1;
    }
    std::cout<<"before data:\n";
    out_range(std::cout, data); 
    std::cout<<std::endl;
    switch (args.type) {
        case program_args::MAX:
            return process_max(data, args.max);
            utils::max_normalization(data);
            break;
            case program_args::MIN_MAX:
                return process_min_max(data, args.min, args.max);
            case program_args::Z_NORM:
                return process_znorm(data); 
            default:
                throw std::runtime_error{"invalid algorithm type"};
    }
}

int main(int argc, char** argv)
{
    try {
        auto args = process_args(argc, argv);
        return process(args);
    } catch (const std::exception&) {
        return -1;
    }
#if 0
    if (argc != 4) {
        std::cerr<<"usage: "<<argv[0]<<" <input file> <min> <max>\n";
        return -1;
    }
#endif
}
