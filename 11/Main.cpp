#include <deque>
#include <fstream>
#include <sstream>
#include <iostream>

using LongList = std::deque<long long>;
using LongPair = std::pair<long, long>;

LongList parse(std::string_view FilePath)
{
    LongList      Output;
    std::ifstream File { FilePath };
    for (std::string Line; std::getline(File, Line);)
    {
        std::stringstream Stream { Line };
        for (std::string Number; Stream >> Number;)
            Output.push_back(std::stoll(Number));
    }
    return Output;
}

LongPair split(long Data)
{
    auto DataStr = std::to_string(Data);
    int  Middle  = DataStr.size() / 2;
    return { std::stoll(DataStr.substr(0, Middle)), std::stoll(DataStr.substr(Middle, DataStr.size())) };
}

bool hasEvenNumberOfDigits(long Data)
{
    return std::to_string(Data).size() % 2 == 0;
}

void print(LongList const& Data)
{
    for (auto Elem : Data)
        std::cout << Elem << " ";
    std::cout << std::endl;
}

LongList transform(LongList const& Data, int StepCount)
{
    LongList Transformed;
    LongList Current = Data;
    for (int i = 0; i < StepCount; i++)
    {
        std::cout << "Step " << i << std::endl;
        for (int j = 0; j < Current.size(); j++)
        {
            if (Current[j] == 0)
                Transformed.push_back(1);
            else if (hasEvenNumberOfDigits(Current[j]))
            {
                auto [First, Second] = split(Current[j]);
                Transformed.push_back(First);
                Transformed.push_back(Second);
            }
            else
                Transformed.push_back(Current[j] * 2024);
        }
        Current = Transformed;
        Transformed.clear();
        //print(Current);
    }
    return Current;
}

int main(int ArgC, char* ArgV[])
{
    auto Data        = parse("/Users/vincentmayer/repos/AdventOfCode/11/input.txt");
    auto Transformed = transform(Data, 75);
    std::cout << "Part 1: " << Transformed.size() << std::endl;
    return 0;
}
