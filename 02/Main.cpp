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

int countSafe(IntListOfLists Input)
{
    int     SumSafe = 0;
    IntList Diff;
    for (IntList& Data : Input)
    {
        Diff.clear();
        for (int i = 1; i < Data.size(); i++)
            Diff.push_back(Data[i] - Data[i - 1]);
        bool AllNegative = std::all_of(Diff.begin(), Diff.end(), [](int x) { return x < 0 && x >= -3; });
        bool AllPositive = std::all_of(Diff.begin(), Diff.end(), [](int x) { return x > 0 && x <= 3; });
        SumSafe += (AllNegative || AllPositive);
    }
    return SumSafe;
}

int main(int Argc, char* ArgV[])
{
    std::ifstream File { FilePath };
    std::cout << countSafe(parse(File)) << std::endl;
    return 0;
}
