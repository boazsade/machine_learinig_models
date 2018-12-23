#include <boost/test/auto_unit_test.hpp>
#include "logging.h"
#include "apps/detect_duplicates/command_processing.h"
#include "apps/detect_duplicates/exceptions.h"
#include <algorithm>


BOOST_AUTO_TEST_CASE( test_detect_no_conf )
{
    app_args args;
    try {
        auto b = parse_command(std::string{}, args);
        BOOST_TEST(b == false, "this must fail! - no input for configuration");
        BOOST_TEST(args.input_path.empty(), "no data to fill the cofiguration");
        BOOST_TEST(args.duplications.empty(), "no duplications are passed");
        BOOST_TEST(args.outliers.empty(), "no outliers needs to be found");
    } catch (const configuration_error& ) {
        BOOST_TEST(false, "we are expecting to get other error exception");
    } catch (const std::exception&) {
    }
}

BOOST_AUTO_TEST_CASE( test_invalid_conf_value ) 
{
    app_args args;
    try {
        parse_command(std::string{"invalid entry that we would try to lookup as file path"}, args);
        BOOST_TEST(false, "we are expecting to get configuration error exception");
    } catch (const std::runtime_error&) {
    } catch (const configuration_error&) {
    } catch (const std::exception&) {
        BOOST_TEST(false, "we are expecting to get configuration error exception");
    }
}

BOOST_AUTO_TEST_CASE( test_invalid_conf_msg ) 
{
    const std::string invalid_json {
        "{\"dataset\":\"5979969cfa2c17397a049e6e\",\"datasource\":{\"files\":[\"/opt/foo/datasource/59795473319a330b2362ac1c/59795487319a330b2362ac1d.csv\"],\"settings\":{\"firstRow\":0,\"separator\":\",\",\"schema\":{\"cols\":[{\"colid\":\"e01efbd8-3894-4e27-89f8-a8c141e9556c\"},{\"colid\":\"e1dc7922-9aa9-4914-8325-1140406f0e59\"},{\"colid\":\"66c67606-8688-4299-a66e-c133331c3feb\"},{\"colid\":\"7abb250e-ba20-4171-b987-f1879696b575\"},{\"colid\":\"61c6cf8d-45fa-4e58-804f-3fefc7c3557b\"}]},\"actions\":[{\"attr\":{\"header\":\"date\",\"attr\":{\"format\":\"%Y-%m-%d\",\"resolution\":\"seconds\"},\"dataClass\":\"interval\",\"dataType\":\"date\",\"colid\":\"e01efbd8-3894-4e27-89f8-a8c141e9556c\"},\"action\":\"col.type\"},{\"attr\":{\"header\":\"curenct\",\"attr\":{\"unique_limit\":100},\"dataClass\":\"categorial\",\"dataType\":\"string\",\"colid\":\"e1dc7922-9aa9-4914-8325-1140406f0e59\"},\"action\":\"col.type\"},{\"attr\":{\"header\":\"code\",\"attr\":{\"unique_limit\":100},\"dataClass\":\"categorial\",\"dataType\":\"string\",\"colid\":\"66c67606-8688-4299-a66e-c133331c3feb\"},\"action\":\"col.type\"},{\"attr\":{\"header\":\"rate\",\"attr\":{\"missing_list\":[\"N/A\",\"NULL\",\"-1\",\"null\",\"n/a\"],\"bin_count\":10},\"dataClass\":\"numeric\",\"dataType\":\"number\",\"colid\":\"7abb250e-ba20-4171-b987-f1879696b575\"},\"action\":\"col.type\"},{\"attr\":{\"header\":\"number\",\"attr\":{\"missing_list\":[\"N/A\",\"NULL\",\"-1\",\"null\",\"n/a\"],\"bin_count\":10},\"dataClass\":\"numeric\",\"dataType\":\"number\",\"colid\":\"61c6cf8d-45fa-4e58-804f-3fefc7c3557b\"},\"action\":\"col.type\"}]}"
    };
    app_args args;

    try {
        auto b = parse_command(invalid_json, args);
        BOOST_TEST(b == false, "this is an invalid message that should fail to parse it should fail");
    } catch (const std::runtime_error&) {
    } catch (const configuration_error&) {
    } catch (const std::exception&) {
        BOOST_TEST(false, "we expecting to have configuration_error");
    }

}

BOOST_AUTO_TEST_CASE( test_invalid_conf_msg_2 ) 
{
    const std::string invalid_json {
        "{\"dataset\":\"5979969cfa2c17397a049e6e\",\"datasource\":{\"files\":[\"/opt/foo/datasource/59795473319a330b2362ac1c/59795487319a330b2362ac1d.csv\"],\"settings\":{\"firstRow\":0,\"separator\":\"}\",\"schema\":{\"cols\":[{\"colid\":\"e01efbd8-3894-4e27-89f8-a8c141e9556c\"},{\"colid\":\"e1dc7922-9aa9-4914-8325-1140406f0e59\"},{\"colid\":\"66c67606-8688-4299-a66e-c133331c3feb\"},{\"colid\":\"7abb250e-ba20-4171-b987-f1879696b575\"},{\"colid\":\"61c6cf8d-45fa-4e58-804f-3fefc7c3557b\"}]},\"actions\":[{\"attr\":{\"header\":\"date\",\"attr\":{\"format\":\"%Y-%m-%d\",\"resolution\":\"seconds\"},\"dataClass\":\"interval\",\"dataType\":\"date\",\"colid\":\"e01efbd8-3894-4e27-89f8-a8c141e9556c\"},\"action\":\"col.type\"},{\"attr\":{\"header\":\"curenct\",\"attr\":{\"unique_limit\":100},\"dataClass\":\"categorial\",\"dataType\":\"string\",\"colid\":\"e1dc7922-9aa9-4914-8325-1140406f0e59\"},\"action\":\"col.type\"},{\"attr\":{\"header\":\"code\",\"attr\":{\"unique_limit\":100},\"dataClass\":\"categorial\",\"dataType\":\"string\",\"colid\":\"66c67606-8688-4299-a66e-c133331c3feb\"},\"action\":\"col.type\"},{\"attr\":{\"header\":\"rate\",\"attr\":{\"missing_list\":[\"N/A\",\"NULL\",\"-1\",\"null\",\"n/a\"],\"bin_count\":10},\"dataClass\":\"numeric\",\"dataType\":\"number\",\"colid\":\"7abb250e-ba20-4171-b987-f1879696b575\"},\"action\":\"col.type\"},{\"attr\":{\"header\":\"number\",\"attr\":{\"missing_list\":[\"N/A\",\"NULL\",\"-1\",\"null\",\"n/a\"],\"bin_count\":10},\"dataClass\":\"numeric\",\"dataType\":\"number\",\"colid\":\"61c6cf8d-45fa-4e58-804f-3fefc7c3557b\"},\"action\":\"col.type\"}]}"
    };
    app_args args;

    try {
        auto b = parse_command(invalid_json, args);
        BOOST_TEST(b == false, "this is an invalid message that should fail to parse it should fail");
    } catch (const std::runtime_error&) {
    } catch (const configuration_error&) {
    } catch (const std::exception&) {
        BOOST_TEST(false, "we expecting to have configuration_error");
    }
}

BOOST_AUTO_TEST_CASE ( test_missing_actions )
{
    const std::string json_str {
        "{\"dataset\": \"59705aa8kjhkjsdhfksjdhfksjhkjhsdkjflkjsdlkjlkjdfaa39d855e5678b50\",\"datasource\": {\"path\": \"/opt/foo/some/kjhskjdhfksjhdfkjsfkjshkfjhskjhfkjskjdskjfhpath/to/where/we/exporkjhsdjkfhsdkft/akjshdkfksdhjfkssssssssssssssssssssssssss/dir\" },\"actions\": [] }"
    };

    app_args args;
    try {
        auto b = parse_command(json_str, args);
        BOOST_TEST(b == false, "should fail - this message has no actions in it");
    } catch (const configuration_error&) {
    } catch (const std::exception&) {
        BOOST_TEST(false, "we are expecting to have configuration_error exception");
    }
}

BOOST_AUTO_TEST_CASE( valid_message_test )
{
    const std::string json_str {
        "{    \"dataset\": \"59705rrr23423423423423234234rrrraa8aa39d855e5678b50\",    \"datasource\": {\"path\": \"/opt/foo/some/path/to/where/we/export/as/dir\"},  \"actions\": [  { \"action\": \"filter.dup\", \"attr\": {    \"colid\":  \"somecolid\",\"resolution\":    3,\"keep\":    \"first\" }}]}"
    };

    app_args args;
    try {
        auto b = parse_command(json_str, args);
        BOOST_TEST_REQUIRE(b, "this is a good message, we should successfully parse it");
        BOOST_TEST(!args.input_path.empty(), "we should have a valid path here");
        BOOST_TEST(args.input_path == "/opt/foo/some/path/to/where/we/export/as/dir", 
                "the path is '"<<args.input_path<<"', but it should be /opt/foo/some/path/to/where/we/export/as/dir");
        BOOST_TEST(args.duplications.size() == 1, "we should have 1 duplication to handle, but we have "<<args.duplications.size());
        BOOST_TEST(args.outliers.empty(), "we don't have any outliers in this configuration");
    } catch (const configuration_error&) {
        BOOST_TEST(false, "this is a good message, it should not generate an exception");
    } catch (const std::exception&) {
        BOOST_TEST(false, "this is a good message, it should not generate an exception");
    }
}

BOOST_AUTO_TEST_CASE( test_multi_entries )
{
    const std::string json_str {
        "{ \"dataset\": \"59705aa8aa39d855e5678b50\",\"datasource\": { \"path\": \"/home/foo/inputs/files\" }, \"actions\": [ {\"action\": \"filter.dup\", \"attr\": { \"colid\": \"StartTime\", \"resolution\": 3, \"keep\": \"first\" } },{\"action\": \"filter.dup\", \"attr\": { \"colid\": \"marital\",\"resolution\": 5, \"keep\": \"last\" }  }, { \"action\": \"outlier.find\",\"attr\": { \"colid\": \"StartTime\" } } ]}"
    };

    const duplication_conf expected_dup_conf[] = {
        duplication_conf{"StartTime", 3, duplication_conf::KEEP_FIRST},
        duplication_conf{"marital", 5, duplication_conf::KEEP_LAST}
    };
    app_args args;
    try {
        auto b =  parse_command(json_str, args);
        BOOST_TEST_REQUIRE(b, "this is a good message, we should successfully parse it");
        BOOST_TEST(!args.input_path.empty(), "we should have a valid path here");
        BOOST_TEST(args.input_path == "/home/foo/inputs/files", 
                "expected path /home/foo/inputs/files, do not matched '"<<args.input_path<<"'");
        BOOST_TEST(args.duplications.size() == 2, "we were expecting two duplication actions, but got "<<args.duplications.size());
        BOOST_TEST(args.outliers.size() == 1, "we expecting for one outlier action, but got "<<args.outliers.size());

        for (const auto& dup : args.duplications) {
            BOOST_TEST((dup.act != duplication_conf::NO_DEFINED), "we are expecing that we would have a valid action type");
            BOOST_TEST(!dup.col_id.empty(), "column id empty which is invalid!!");
            auto i = std::find(std::begin(expected_dup_conf), std::end(expected_dup_conf), dup);
            BOOST_TEST_REQUIRE((i != std::end(expected_dup_conf)), "we are expecting to find conf entry '"<<dup.col_id<<"' but failed to find it");
            BOOST_TEST_REQUIRE(i->col_id == dup.col_id);
            BOOST_TEST_REQUIRE((i->act == dup.act), "we are expecting to have a valid action, but its not");
        }
        BOOST_TEST_REQUIRE(args.outliers[0].col_id == "StartTime");
    } catch (...) {
        BOOST_TEST(false, "there should be no excaption here");
    }
}

