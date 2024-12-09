#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <sstream>

static auto FilePath = "/Users/vincentmayer/repos/AdventOfCode/05/input.txt";

// Yeah this one is ugly, but I needed to finish it.

using DepMap         = std::unordered_map<int, std::unordered_set<int>>;
using IntListOfLists = std::deque<std::deque<int>>;

std::tuple<DepMap, IntListOfLists> parse(std::ifstream& File)
{
    DepMap         Rules;
    IntListOfLists PrintOrders;
    std::regex     Pattern(R"(\d{2})");

    for (std::string Line; std::getline(File, Line);)
    {
        if (Line.find("|") != std::string::npos)
        {
            std::sregex_iterator MatchesBegin { Line.begin(), Line.end(), Pattern };

            int First  = std::stoi(MatchesBegin->str());
            int Second = std::stoi((++MatchesBegin)->str());

            Rules[First].insert(Second);
        }
        else
        {
            std::istringstream LineStream { Line };
            std::string        Number;
            PrintOrders.emplace_back();
            while (std::getline(LineStream, Number, ','))
                PrintOrders.back().push_back(std::stoi(Number));
        }
    }
    return { Rules, PrintOrders };
}

int main(int Argc, char* ArgV[])
{
    std::ifstream File { FilePath };
    auto [Rules, PrintOrders] = parse(File);
    IntListOfLists ValidOrders;
    for (auto& Order : PrintOrders)
    {
        if (!Order.size())
            continue;
        bool Valid = true;
        for (int i = 0; i < Order.size() - 1; i++)
        {
            bool RuleExists = Rules.find(Order[i]) != Rules.end();
            if (!RuleExists || (Rules[Order[i]].find(Order[i + 1]) == Rules[Order[i]].end()))
            {
                Valid = false;
                break;
            }
        }
        if (Valid)
            ValidOrders.push_back(Order);
    }

    int Sum = 0;
    for (auto& ValidOrder : ValidOrders)
        Sum += ValidOrder[(ValidOrder.size() - 1) / 2];
    std::cout << Sum << std::endl;
}
