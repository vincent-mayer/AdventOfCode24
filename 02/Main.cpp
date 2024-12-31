#include <fstream>
#include <istream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <deque>

static auto FilePath = "/Users/vincentmayer/repos/AdventOfCode/02/input.txt";

using IntList        = std::deque<int>;
using IntListOfLists = std::deque<IntList>;

IntListOfLists parse(std::istream& InputStream)
{
    IntListOfLists Data;
    for (std::string Line; std::getline(InputStream, Line);)
    {
        std::istringstream LineStream { Line };
        Data.emplace_back(std::istream_iterator<int> { LineStream }, std::istream_iterator<int> {});
    }
    return Data;
}

bool isSafe(IntList const& Data)
{
    int     SumSafe = 0;
    IntList Diff;
    for (int i = 1; i < Data.size(); i++)
        Diff.push_back(Data[i] - Data[i - 1]);
    bool AllNegative = std::all_of(Diff.begin(), Diff.end(), [](int x) { return x < 0 && x >= -3; });
    bool AllPositive = std::all_of(Diff.begin(), Diff.end(), [](int x) { return x > 0 && x <= 3; });
    return (AllNegative || AllPositive);
}

int countSafe(IntListOfLists const& Input)
{
    int     SumSafe = 0;
    IntList Diff;
    for (IntList const& Data : Input)
        SumSafe += isSafe(Data);
    return SumSafe;
}

int countSafe2(IntListOfLists const& Input)
{
    int SumSafe = 0;
    for (IntList const& Data : Input)
    {
        std::vector<bool> SafeByRemoval;
        SafeByRemoval.push_back(isSafe(Data));
        for (int i = 0; i < Data.size(); i++)
        {
            IntList Slice { Data.begin(), Data.end() };
            Slice.erase(Slice.begin() + i);
            SafeByRemoval.push_back(isSafe(Slice));
        }
        SumSafe += std::any_of(SafeByRemoval.begin(), SafeByRemoval.end(), [](bool x) { return x; });
    }
    return SumSafe;
}

int main(int Argc, char* ArgV[])
{
    std::ifstream File { FilePath };
    auto          Data = parse(File);
    std::cout << "Part 1: " << countSafe(Data) << std::endl;
    std::cout << "Part 2: " << countSafe2(Data) << std::endl;
    return 0;
}
