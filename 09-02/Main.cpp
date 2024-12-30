#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

struct Spec
{
    int Start;
    int Size;
    int ID;
};

using Memory = std::deque<Spec>;
using IDSet  = std::unordered_set<int>;

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
    Memory Mem;
    int    ID    = 0;
    int    Start = 0;
    for (int i = 0; i < Line.size(); i++)
    {
        int Size = Line[i] - '0';
        int Val  = isEven(i) ? ID++ : -1;
        Mem.push_back({ .Start = Start, .Size = Size, .ID = Val });
        Start += Size;
    }
    return Mem;
}

Memory compactBlockwise(Memory const& Fragmented)
{
    Memory Result;
    IDSet  MovedIDs;
    auto   EndIter   = Fragmented.end() - 1;
    auto   StartIter = Fragmented.begin();

    while ((*StartIter).ID != -1)
        Result.push_back(*StartIter++);

    while (true)
    {
        auto const& CurBlock = *EndIter;
        if (CurBlock.ID == -1 || MovedIDs.count(CurBlock.ID))
        {
            EndIter--;
            continue;
        }
    }
    return Result;
}

long long computeCheckSum(Memory const& Compacted)
{
    long long CheckSum = 0;
    int       Idx      = 0;
    for (auto const& Spec : Compacted)
        for (int i = 0; i < Spec.Size; i++)
            CheckSum += (Spec.ID * Idx);
    return CheckSum;
}

void print(Memory const& Mem)
{
    for (auto const& Spec : Mem)
        for (int i = 0; i < Spec.Size; i++)
            std::cout << ((Spec.ID == -1) ? "." : std::to_string(Spec.ID));
    std::cout << std::endl;
}

int main(int Argc, char* ArgV[])
{
    auto Line       = parse("/Users/vincentmayer/repos/AdventOfCode/09-02/input_dbg.txt");
    auto Fragmented = transform(Line);
    print(Fragmented);
    // auto      Compacted = compactBlockwise(Fragmented);
    // long long CheckSum  = computeCheckSum(Compacted);
    // std::cout << CheckSum << std::endl;
    return 0;
}
