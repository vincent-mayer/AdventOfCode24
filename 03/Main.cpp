#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

static auto FilePath = "/Users/vincentmayer/repos/AdventOfCode/03/input.txt";

int main(int Argc, char* ArgV[])
{
    std::ifstream     File { FilePath };
    std::stringstream Stream;
    Stream << File.rdbuf();
    std::string Contents { Stream.str() };

    std::regex           Pattern(R"(mul\(\d{1,3},\d{1,3}\))");
    std::sregex_iterator MatchesBegin { Contents.begin(), Contents.end(), Pattern };
    std::sregex_iterator MatchesEnd {};

    std::regex           IntegerPattern(R"(\d{1,3})");
    std::sregex_iterator SubMatchesBegin;
    std::sregex_iterator SubMatchesEnd;
    int                  Result = 0;
    for (auto It = MatchesBegin; It != MatchesEnd; ++It)
    {
        SubMatchesBegin = std::sregex_iterator(It->str().begin(), It->str().end(), IntegerPattern);
        SubMatchesEnd   = std::sregex_iterator();
        Result += std::stoi(SubMatchesBegin->str()) * std::stoi((++SubMatchesBegin)->str());
    }
    std::cout << Result << std::endl;
    return 0;
}
