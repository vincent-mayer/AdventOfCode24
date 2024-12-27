#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using OperandList = std::deque<long long>;
using ResultList  = std::deque<long long>;
using Data        = std::deque<std::pair<long long, OperandList>>;

Data parse()
{
    std::ifstream File { "/Users/vincentmayer/repos/AdventOfCode/07/input_dbg.txt" };
    Data          Instructions;
    for (std::string Line; std::getline(File, Line);)
    {
        long long         Res = std::stol(Line.substr(0, Line.find(':')));
        std::stringstream Str { Line.substr(Line.find(':') + 2, Line.size()) };
        OperandList       Operands;
        for (std::string Number; Str >> Number;)
            Operands.push_back(std::stoi(Number));
        Instructions.push_back({ Res, Operands });
    }
    return Instructions;
}

int main(int Argc, char* ArgV[])
{
    Data      Instructions = parse();
    long long Total        = 0;
    for (auto const& [Result, Operands] : Instructions)
    {
        int IterCount = std::pow(2, Operands.size() - 1);
        for (uint32_t Num = 0; Num < IterCount; Num++)
        {
            long long       Candidate = Operands[0];
            std::bitset<20> Bits { Num };
            for (int i = 1; i < Operands.size(); i++)
            {
                long long Operand = Operands[i];
                if (Bits[i - 1])
                    Candidate += Operand;
                else
                    Candidate *= Operand;
            }
            if (Candidate == Result)
            {
                Total += Result;
                break;
            }
        }
    }
    std::cout << Total << std::endl;
    return 0;
}
