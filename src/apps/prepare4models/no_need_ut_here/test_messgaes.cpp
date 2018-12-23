#include <boost/test/unit_test.hpp>
#include "libs/cleanup_operatiosn/command_processing.h"
#include "libs/rows_drops/filterout_conf.h"
#include "libs/cleanup_operatiosn/constfill_conf.h"
#include "libs/cleanup_operatiosn/schema_data.h"
#include "libs/cleanup_operatiosn/normalized_conf.h"
#include "libs/rows_drops/rowcleanup_task.h"
#include "libs/system/exceptions.h"
#include "utils/algorithms.hpp"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sstream>

namespace
{
    struct raii_file
    {
        raii_file() : 
            temp(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path())
        {
        }

        const std::string& name() const
        {
            return temp.string();
        }

        ~raii_file()
        {
            boost::filesystem::remove(temp); 
        }

        boost::filesystem::path temp;
    };

    template<typename Act>
    std::string stringify(const Act& action)
    {
        std::ostringstream parser;
        parser<<action;
        return parser.str();
    }

    struct generate_actions
    {
        using result_type = std::string;
        generate_actions(const preprocessed_action::drop_action& base_act,
                const schema_type& cols_list) :
                    base_action(base_act), columns_list(cols_list),
                    index(0)
        {
        }

        result_type operator () () const
        {
            preprocessed_action::drop_action ta{columns_list[index++],
                                                std::begin(base_action.to_remove),
                                                std::end(base_action.to_remove)
            };

            return stringify(column_actions::drop_action{ta});
        }

        const preprocessed_action::drop_action& base_action;
        const schema_type& columns_list;
        mutable unsigned int index = 0;

    };
}

BOOST_AUTO_TEST_CASE( test_invalid_message )
{
    try {
        read_configuration(std::string{});   // should throw!!
        BOOST_TEST(false, "we expect to fail with configuration error as this did not pass anything");
    } catch (const invalid_path&) {
    } catch (...) {
        BOOST_TEST(false, "we expecting to get invalid path excaption, but got something else!!");
    }
}

BOOST_AUTO_TEST_CASE( test_no_such_input_file_message )
{
    try {
        read_configuration(std::string{"no such file"});
        BOOST_TEST(false, "we expect to fail with configuration error as this pass invalid path");
    } catch (const invalid_path&) {
    } catch (...) {
        BOOST_TEST(false, "we expecting to get invalid path exception but we got something else");
    }
}

BOOST_AUTO_TEST_CASE( test_invalid_input_file )
{
    try {
        raii_file tmp;
        {
            std::ofstream f(tmp.name());
            BOOST_TEST_REQUIRE(f.good());
        }
        try {
            read_configuration(tmp.name());
            BOOST_TEST(false, "we expect to fail with configuration error as this invalid input file");
        } catch (const json_error&) {
        } catch (const std::exception& e) {
            BOOST_TEST(false, "test invalid input file: "<<e.what());
        } catch (...) {
            BOOST_TEST(false, "we expecting to get standard  exception but we got something else");
        }
    } catch (const std::exception& e) {
        BOOST_TEST(false, "failed to create tmp file: "<<e.what());
    } catch (...) {
        BOOST_TEST(false, "failed to create tmp file for testing");
    }
}

BOOST_AUTO_TEST_CASE( test_invalid_input_string )
{
    try {
        read_configuration("this string cannot be parsed as a valid json message so we should fail with this string as this string not not valid input and we are expecting to get some sort of exception in this test for it---------------------------------------------------------------------------------------");
        BOOST_TEST(false, "we expecting to get an exception in this case");
    } catch (const json_error&) {
    } catch (...) {
        BOOST_TEST(false, "we expecting to get json error in this case");
    }
}

BOOST_AUTO_TEST_CASE( test_invalid_input_json )
{
    const std::string json_str{
    "{\"dataset\": \"59705lkjsdfsldkjflsdkjlkdfaa8aa39d855ettttttttttttttttttttt5678b50\",\"datasource\": {\"path\": \"/howerme/booiuwerouaz/inpudddddddddddddts/file\"},\"actions\": [{\"action\": \"filter.dup\",\"attr\": {\"colid\": \"StartiertertTime\",\"resolution\": 3,\"kettttep\": \"fitttrst\"}}"
    };
    try {
        read_configuration(json_str);
        BOOST_TEST(false, "this is an invalid input, we do not expecting this to succeed");
    } catch (const json_error&) {
    } catch (...) {
        BOOST_TEST(false, "we expecting to get json exception but got something else");
    }
}

BOOST_AUTO_TEST_CASE (test_invalid_input_json_data )
{
    const std::string json_str{
        "{\"dataset\":\"hkjshdkfjshdkjfshdjkjhsdfksjdkfkjskdjfhskjdfskjdfksdhfkjsogwarts\",\"actions\":[],\"datasource\":{\"files\":[\"/stars/tower\",\"/big/hall\"],\"settings\":{\"firstRow\":0,\"separator\":\",\"}},\"schema\":{\"cols\":[{\"colid\":\"Gryffindor\"},{\"colid\":\"Hufflepuff\"},{\"colid\":\"Ravenclaw\"},{\"colid\":\"Slytherin\"}]},\"export\":{\"format\":\"csv\",\"path\":\"/opt/foo/dataset/hogwakjshdfkjshkdjfhsdkfhkjsrts\"}}"
    };
    try {
        read_configuration(json_str);
        BOOST_TEST(false, "this is an invalid input, we do not expecting this to succeed");
    } catch (const json_error&) {
    } catch (...) {
        BOOST_TEST(false, "we expecting to get json exception but got something else");
    }
}

BOOST_AUTO_TEST_CASE( test_invalid_message_message4_model_generation ) 
{
    static const std::vector<preprocessed_action::id_type> expected_id {
        std::string{"StartTime"}, std::string{"marital"}, std::string{"data"},
        std::string{"age"}, std::string{"status"}, std::string{"matrial"}
    };
    const std::string json_str{
        "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/files\"},\"export\": {\"type\": \"csv\",\"path\": \"/path/to/existing/dirctory/to/which/to/export/file_name.csv\"},\"actions\": [{\"action\": \"filter.rows\",\"attr\": {\"colid\": \"age\",\"rows\": [1, 2, 3]}},{\"action\": \"fix.missing\",\"attr\": {\"colid\": \"marital\",\"algo\": \"cubical\",\"rows\": [7, 89, 65]}},{\"action\": \"normalized\",\"attr\": {\"colid\": \"StartTime\",\"algo\": \"min-max\",\"min\": 23.1,\"max\": 33.4}},{\"action\": \"normalized\",\"attr\": {\"colid\": \"age\",\"algo\": \"znorm\"}},{\"action\": \"normalized\",\"attr\": {\"colid\": \"matrial\",\"algo\": \"tnorm\"}},{\"action\": \"fix.missing\",\"attr\": {\"colid\": \"status\",\"algo\": \"kneighborhood\",\"count\": 3,\"rows\": [11, 43, 675]}},{\"action\": \"fix.missing\",\"attr\": {\"colid\": \"data\",\"algo\": \"cubical\",\"rows\": [1893, 2343, 99975]}}]}"
    };
    try {
        auto conf = read_configuration(json_str);
        BOOST_TEST_REQUIRE(false, "this should not pass, this message is formated for data clean and not for this application");
        BOOST_TEST(!conf.source_location.empty(), "we should have a valid path for file location");
        BOOST_TEST(!conf.export_file.empty(), "we should have a valid location for export");
        BOOST_TEST(!conf.actions.empty(), "we should have a valid list of actions");
        BOOST_TEST(conf.source_location == "/home/foo/inputs/files", 
                "we are expecting to have path to sources at '/home/foo/inputs/files'"<<
                "but we got '"<<conf.source_location<<"'");
        BOOST_TEST(conf.export_file == "/path/to/existing/dirctory/to/which/to/export/file_name.csv",
                "we expected to have path to '/path/to/existing/dirctory/to/which/to/export/file_name.csv' but got "<<conf.export_file);
        BOOST_TEST(conf.actions.size() == utils::size(expected_id), 
                "we got "<<conf.actions.size()<<" and not what we expected to be which is "<<utils::size(expected_id));
        for(auto& action : conf.actions) {
                    auto i = std::find(std::begin(expected_id), std::end(expected_id), id(action));
                    bool found = (i != std::end(expected_id));
                    BOOST_TEST(found, "action '"<<id(action)<<"' expected to be listed");
        }
    } catch (const invalid_path& e) {
        BOOST_TEST(false, "error '"<<e.what()<<"'");
    } catch (const std::exception& e) {
        BOOST_TEST(false, "error '"<<e.what()<<"'");
    } catch (const json_error& e) {
    } catch (...) {
        BOOST_TEST(false, "this is valid message we do not expect to get any exception here");
    }
}


BOOST_AUTO_TEST_CASE( test_validate_message_data ) 
{

    const std::string json_str{
         "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/files\"},\"export\": {\"type\": \"csv\",\"path\": \"/path/to/existing/dirctory/to/which/to/export/file_name.csv\"},\"actions\": [{\"action\": \"filter.row\",\"attr\": {\"colid\": \"age\",  \"rows\": [1, 2, 3]}},{\"action\": \"fix.missing\",\"attr\": {\"colid\": \"marital\",\"algo\": \"cubical\",\"rows\": [7, 89, 65]}}]}"
   };
    try {
        read_configuration(json_str);
        BOOST_TEST(false, "we should not succeed in reading this json - we have invalid action name");
    } catch (const json_error&) {
    } catch (...) {
        BOOST_TEST(false, "we are expecting to get json error exception");
    }

}

BOOST_AUTO_TEST_CASE( test_actions_entries ) 
{
    const std::string json_str{
         "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/files\"},\"export\": {\"type\": \"csv\",\"path\": \"/path/to/existing/dirctory/to/which/to/export/file_name.csv\"},\"actions\": [{\"action\": \"filter.rows\",\"attr\": {\"colid\": \"age\",  \"rows\": [1, 2, 3]}},{\"action\": \"fix.missing\",\"attr\": {\"colid\": \"marital\",\"algo\": \"const\",\"value\":20,\"rows\": [7, 89, 65]}}]}"
   };
    const filterout_conf::row_numbers expected_droped_rows{
        1, 2, 3
    };
    const const_value_fix::row_numbers expected_invalid_rows{
        7, 89, 65
    };
    const std::string valid_action_id = "marital"; // this would have a valid action to preform
    const std::string action_filter_rows = "age";    // this would have an invalid action that we don't support
    try {
        auto pre_conf = read_configuration(json_str);
         BOOST_TEST(!pre_conf.source_location.empty(), "we should have a valid path for file location");
         BOOST_TEST(!pre_conf.export_file.empty(), "we should have a valid location for export");
         BOOST_TEST(!pre_conf.actions.empty(), "we should have a valid list of actions");
         BOOST_TEST(pre_conf.actions.size() == 2, "there are two action in the json message");
         auto it = std::find_if(std::begin(pre_conf.actions), std::end(pre_conf.actions), [&valid_action_id](auto& act) {
                                return id(act) == valid_action_id;
                            }
                    );
        bool found = (it != std::end(pre_conf.actions));
        BOOST_TEST_REQUIRE(found, "action '"<<valid_action_id<<"' must be listed in the actions list");
        auto fr_it = std::find_if(std::begin(pre_conf.actions), std::end(pre_conf.actions), [&action_filter_rows](auto& act) {
                                                return id(act) == action_filter_rows;
                              }
                );
        bool found_fw = (fr_it != std::end(pre_conf.actions));
        BOOST_TEST_REQUIRE(found_fw, "action '"<<action_filter_rows<<"' must be listed in the actions list");

        filterout_conf ref_fo_action{action_filter_rows, std::begin(expected_droped_rows), std::end(expected_droped_rows)};
        const std::string as_string_expected_filterout_action = stringify(ref_fo_action);
        const std::string as_string_filterout_action = stringify(fr_it->drop());
        BOOST_TEST(as_string_expected_filterout_action == as_string_filterout_action, 
                "we are expecting that the action we created '"<<as_string_filterout_action<<"' would be the same as '"<<as_string_expected_filterout_action);
        const const_value_fix expected_missing_action{valid_action_id, std::begin(expected_invalid_rows), std::end(expected_invalid_rows), 20};
        const std::string as_string_expected_missing_action = stringify(expected_missing_action);
        const std::string as_string_missing_action = stringify(it->fixes());
        BOOST_TEST(as_string_missing_action == as_string_expected_missing_action,
                "we are expecting that the missing fix action '"<<as_string_missing_action<<"' would be '"<<as_string_expected_missing_action<<"'");
    } catch (const json_error& e) {
        BOOST_TEST(false, "json error: "<<e.what());
    } catch (const invalid_path& e) {
        BOOST_TEST(false, "invalid path error '"<<e.what()<<"'");
    } catch (const std::exception& e) {
        BOOST_TEST(false, "error '"<<e.what()<<"'");
    } catch (...) {
        BOOST_TEST(false, "this is a valid operation input, we should not have gotten an exception");
    }
}

BOOST_AUTO_TEST_CASE( test_conf_actions_to_running_actions ) 
{
    const std::string json_str{
         "{\"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": {\"path\": \"/home/foo/inputs/files\"},\"export\": {\"type\": \"csv\",\"path\": \"/path/to/existing/dirctory/to/which/to/export/file_name.csv\"},\"actions\": [{\"action\": \"filter.rows\",\"attr\": {\"colid\": \"filterout\",  \"rows\": [1, 2, 3]}},{\"action\": \"fix.missing\",\"attr\": {\"colid\": \"missing\",\"algo\": \"const\",\"value\": 44,\"rows\": [7, 89, 65]}},{\"action\": \"normalized\",\"attr\": {\"colid\": \"norm\",\"algo\": \"znorm\", \"mean\": 22, \"STD\": 1}}]}"
   };
    const filterout_conf::row_numbers expected_droped_rows{
        1, 2, 3
    };
    const const_value_fix::row_numbers expected_invalid_rows{
        7, 89, 65
    };

    const std::string missing_id = "missing"; // this would have a valid action to preform
    const std::string drop_id = "filterout";    // this would have an invalid action that we don't support
    const std::string normalized_id = "norm";
    const schema_type drop_col_action_id {
        "no_action_colid_1", drop_id, "no_action_colid_2", 
        "no_action_colid_3", "no_action_colid_4"
    };
    const schema_type test_schema{
        drop_col_action_id[0], missing_id, 
        drop_col_action_id[2], drop_col_action_id[3],
        normalized_id, drop_col_action_id[4],
        drop_id
    };
    try {
        auto pre_conf = read_configuration(json_str);
        BOOST_TEST(!pre_conf.source_location.empty(), "we should have a valid path for file location");
        BOOST_TEST(!pre_conf.export_file.empty(), "we should have a valid location for export");
        BOOST_TEST(!pre_conf.actions.empty(), "we should have a valid list of actions");
        BOOST_TEST(pre_conf.actions.size() == 3, "there are two action in the json message");
        auto it = std::find_if(std::begin(pre_conf.actions), std::end(pre_conf.actions), [&missing_id](auto& act) {
                                return id(act) == missing_id;
                            }
                    );
        bool found = (it != std::end(pre_conf.actions));
        BOOST_TEST_REQUIRE(found, "action '"<<missing_id<<"' must be listed in the actions list");
        auto fr_it = std::find_if(std::begin(pre_conf.actions), std::end(pre_conf.actions), [&drop_id](auto& act) {
                                                return id(act) == drop_id;
                              }
                );
        bool found_fw = (fr_it != std::end(pre_conf.actions));
        BOOST_TEST_REQUIRE(found_fw, "action '"<<drop_id<<"' must be listed in the actions list");

        auto norm_it = std::find_if(std::begin(pre_conf.actions), std::end(pre_conf.actions), [&normalized_id](auto& act) {
                                return id(act) == normalized_id;
                            }
        );
        bool found_norm = (norm_it != std::end(pre_conf.actions));
        BOOST_TEST_REQUIRE(found_norm, "we are expecting that action '"<<normalized_id<<"' in our actions list");

        const z_normaliztion_conf_stats norm_action{normalized_id, 22, 1};
        const auto expected_norm_action_as_str = stringify(norm_action);
        const auto norm_action_as_str = stringify(norm_it->normalization());
        BOOST_TEST(expected_norm_action_as_str == norm_action_as_str, "we are expecting that the norm action '"<<norm_action_as_str<<"' would be '"<<expected_norm_action_as_str<<"'");
        // other actions are the same as the above test so we would not run these again..
        // now we would like to generate the actions that we are going to run on the actual data, note that
        // this would create new actions for each of the columns that are not even listed in the actions
        // from the conf becasue of drop actions
        auto tmp_conf(pre_conf);
        auto run_actions = finilized_configuration(std::move(tmp_conf), test_schema);
        BOOST_TEST_REQUIRE(run_actions.source_location == pre_conf.source_location);
        BOOST_TEST_REQUIRE(run_actions.export_file == pre_conf.export_file);
        BOOST_TEST_REQUIRE(!empty(run_actions), "the list of actions cannot be empty after running finilized action!!");
        BOOST_TEST_REQUIRE(size(run_actions) == test_schema.size(), 
                "the number of actions "<<size(run_actions)<<" must be same as number of columns "<<test_schema.size()<<" in schema");
        BOOST_TEST_REQUIRE(size(run_actions) > pre_conf.actions.size(), 
                "we are adding new actions from schema so we should have more actions after finilized_configuration is called");
        for (const auto& col : test_schema) {
            auto i = std::find_if(begin(run_actions), end(run_actions), [&col] (auto& act) {
                                        return id(act) == col;
                                    }
                    );
            bool f = (i != end(run_actions));
            BOOST_TEST_REQUIRE(f, "column '"<<col<<"' must be in the actions list");

        }

        std::vector<std::string> expected_drop_actions{utils::size(drop_col_action_id), std::string{}};
        std::generate(std::begin(expected_drop_actions), std::end(expected_drop_actions), generate_actions{fr_it->drop(), drop_col_action_id});
        for (const auto& drop_col : drop_col_action_id) {
            auto i = std::find_if(begin(run_actions), end(run_actions), [&drop_col] (auto& action) {
                                return id(action) == drop_col;
                            }
            );
            bool fc = (i != end(run_actions));
            BOOST_TEST_REQUIRE(fc, "column '"<<drop_col<<"' must be in actions list");
            const auto action_as_string = stringify(i->drop());
            auto ai = std::find(std::begin(expected_drop_actions), std::end(expected_drop_actions), action_as_string);
            bool af = (ai != std::end(expected_drop_actions));
            BOOST_TEST_REQUIRE(af, "we are expecting that drop action '"<<action_as_string<<"' would be listed as action");
        }

    } catch (const json_error& e) {
        BOOST_TEST(false, "json error: "<<e.what());
    } catch (const invalid_path& e) {
        BOOST_TEST(false, "invalid path error '"<<e.what()<<"'");
    } catch (const std::exception& e) {
        BOOST_TEST(false, "error '"<<e.what()<<"'");
    } catch (...) {
        BOOST_TEST(false, "this is a valid operation input, we should not have gotten an exception");
    }
}


