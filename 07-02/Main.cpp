#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using ll                  = long long;
using OperandList         = std::deque<ll>;
using ResultList          = std::deque<ll>;
using Data                = std::deque<std::pair<ll, OperandList>>;
using OperatorList        = std::deque<std::deque<char>>;
static constexpr char Add = '+';
static constexpr char Mul = '*';
static constexpr char Cat = '|';

Data parse()
{
    std::ifstream File { "/Users/vincentmayer/repos/AdventOfCode/07/input.txt" };
    Data          Instructions;
    for (std::string Line; std::getline(File, Line);)
    {
        ll                Res = std::stol(Line.substr(0, Line.find(':')));
        std::stringstream Str { Line.substr(Line.find(':') + 2, Line.size()) };
        OperandList       Operands;
        for (std::string Number; Str >> Number;)
            Operands.push_back(std::stoi(Number));
        Instructions.push_back({ Res, Operands });
    }
    return Instructions;
}

OperatorList cartesianProduct(int N, std::deque<char> const& Operators)
{
    OperatorList Result;
    for (int i = 0; i < std::pow(Operators.size(), N); i++)
    {
        std::deque<char> Product;
        for (int j = 0; j < N; j++)
            Product.push_back(Operators[(i / static_cast<int>(std::pow(Operators.size(), j))) % Operators.size()]);
        Result.push_back(Product);
    }
    return Result;
}

int main(int Argc, char* ArgV[])
{
    Data Instructions = parse();
    ll   Total        = 0;
    for (auto const& [Result, Operands] : Instructions)
    {
        std::cout << Result << " computing" << std::endl;
        auto OperatorList = cartesianProduct(Operands.size() - 1, { Add, Mul, Cat });
        for (auto const& Operators : OperatorList)
        {
            ll Candidate = Operands[0];
            for (int i = 1; i < Operands.size(); i++)
            {
                if (Operators[i - 1] == Add)
                    Candidate += Operands[i];
                else if (Operators[i - 1] == Mul)
                    Candidate *= Operands[i];
                else if (Operators[i - 1] == Cat)
                    Candidate = std::stoll(std::to_string(Candidate) + std::to_string(Operands[i]));
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
