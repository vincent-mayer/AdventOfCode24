#include <fstream>
#include <iostream>
#include <string>

using Memory = std::deque<int>;

std::string parse(std::string_view FilePath)
{
    std::ifstream File { FilePath };
    std::string   Line;
    std::getline(File, Line);
    return Line;
}

bool isEven(int Number)
{
    return Number % 2 == 0;
}

Memory transform(std::string_view Line)
{
    Memory Result;
    int    ID = -1;
    for (int i = 0; i < Line.size(); i++)
    {
        int Repeat = Line[i] - '0';
        int Val    = isEven(i) ? ++ID : -1;
        for (int j = 0; j < Repeat; j++)
            Result.push_back(Val);
    }
    return Result;
}

Memory compact(Memory const& Fragmented)
{
    Memory Result;
    auto   StartIter = Fragmented.begin();
    auto   EndIter   = Fragmented.end() - 1;
    while (StartIter < (EndIter + 1))
    {
        if (*StartIter != -1)
        {
            Result.push_back(*StartIter);
            StartIter++;
        }
        else if (*EndIter != -1)
        {
            Result.push_back(*EndIter);
            StartIter++;
            EndIter--;
        }
        else
        {
            EndIter--;
        }
    }

    return Result;
}

long long computeCheckSum(Memory const& Compacted)
{
    long long CheckSum = 0;
    for (int i = 0; i < Compacted.size(); i++)
        CheckSum += (Compacted[i] * i);
    return CheckSum;
}

void print(Memory const& Mem)
{
    for (auto const& Val : Mem)
        std::cout << ((Val == -1) ? "." : std::to_string(Val));
    std::cout << std::endl;
}

int main(int Argc, char* ArgV[])
{
    auto      Line       = parse("/Users/vincentmayer/repos/AdventOfCode/09-01/input.txt");
    auto      Fragmented = transform(Line);
    auto      Compacted  = compact(Fragmented);
    long long CheckSum   = computeCheckSum(Compacted);
    std::cout << CheckSum << std::endl;
    return 0;
}
