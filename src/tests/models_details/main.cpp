
#include "apps/run_experiments/pipeline_flow.h"
#include "apps/run_experiments/process_input.h"
#include "apps/run_experiments/row_data_input.h"
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <iterator>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr<<"usage: "<<argv[0]<<" <input config json file>\n";
        return -1;
    }
    /*
    const std::string msg{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/boaz/work/binah_be/src/apps/run_experiments/ut/input.csv\"},\"model\": {\"class\": \"tar\",\"repeations\": 43},\"actions\": [{\"action\": \"model.svm\",\"attr\": {\"arg1\": 111,\"arg2\": 2343}}],\"evaluation\": [\"auc-roc\", \"logisic-los\", \"ks-test\"],\"validation\": {\"action\": \"precent\",\"attr\": {\"arg\": 37}}}"
    };
    */
    std::ifstream input{argv[1]};
    if (!input) {
        std::cerr<<"failed to open "<<argv[1]<<" for reading\n";
        return -1;
    }
    std::string msg{std::istreambuf_iterator<char>(input),
                    std::istreambuf_iterator<char>()
                    };
    if (msg.empty()) {
        std::cerr<<"failed to read from "<<argv[1]<<std::endl;
        return -1;
    }
    try {
        auto conf = process_input(msg);
        pipeline::read_step rs{conf};
        auto data = pipeline::read(rs);
        auto ds = pipeline::read_step::cast(rs);   
        auto it = ds.begin();
        auto ts = *it;
        auto t = pipeline::train(ts, data);
     // now test and show
        auto test_result = pipeline::test(t, data);
        const auto& test_out = std::get<0>(test_result);
        const auto& test_expect = std::get<1>(test_result);
        if (true) {
            std::cout<<"expected\tactual\tgood\n--------------------------------------------------------------\n";
            auto be = std::begin(test_expect);
            std::for_each(std::begin(test_out), std::end(test_out), [&be](auto tr) {
                    std::cout<<*be<<"\t\t"<<tr<<"\t"<<std::boolalpha<<(*be == tr)<<"\n";
                    ++be;
                }
            );
        }
    } catch (const std::exception& e) {
        std::cerr<<"error while running the test  - "<<e.what()<<std::endl;
        return -1;
    } catch (...) {
        std::cerr<< "unkown exception while running test"<<std::endl;
        return -1;
    }
}
