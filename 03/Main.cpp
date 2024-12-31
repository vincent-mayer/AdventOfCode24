#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

static auto FilePath = "/Users/vincentmayer/repos/AdventOfCode/03/input.txt";

int partOne(std::string const& Contents)
{
    std::regex           Pattern(R"(mul\(\d{1,3},\d{1,3}\))");
    std::sregex_iterator MatchesBegin { Contents.begin(), Contents.end(), Pattern };
    std::sregex_iterator MatchesEnd {};

    std::regex           IntegerPattern(R"(\d{1,3})");
    std::sregex_iterator SubMatches;
    int                  Result = 0;
    for (auto It = MatchesBegin; It != MatchesEnd; ++It)
    {
        // std::cout << It->str() << std::endl;
        SubMatches = std::sregex_iterator(It->str().begin(), It->str().end(), IntegerPattern);
        Result += std::stoi(SubMatches->str()) * std::stoi((++SubMatches)->str());
    }
    return Result;
}

int partTwo(std::string const& Contents)
{
    std::regex           Pattern(R"(mul\(\d{1,3},\d{1,3}\)|do\(\)|don't\(\))");
    std::sregex_iterator MatchesBegin { Contents.begin(), Contents.end(), Pattern };
    std::sregex_iterator MatchesEnd {};

    std::regex           IntegerPattern(R"(\d{1,3})");
    std::sregex_iterator SubMatches;
    int                  Result     = 0;
    bool                 Accumulate = true;
    for (auto It = MatchesBegin; It != MatchesEnd; ++It)
    {
        // std::cout << It->str() << std::endl;
        if (It->str() == "do()")
        {
            Accumulate = true;
            continue;
        }
        if (It->str() == "don't()")
        {
            Accumulate = false;
            continue;
        }
        if (Accumulate)
        {
            SubMatches = std::sregex_iterator(It->str().begin(), It->str().end(), IntegerPattern);
            Result += std::stoi(SubMatches->str()) * std::stoi((++SubMatches)->str());
        }
    }
    return Result;
}

int main(int Argc, char* ArgV[])
{
    std::ifstream     File { FilePath };
    std::stringstream Stream;
    Stream << File.rdbuf();
    std::string Contents { Stream.str() };

    std::cout << "Part 1: " << partOne(Contents) << std::endl;
    std::cout << "Part 2: " << partTwo(Contents) << std::endl;

    return 0;
}
