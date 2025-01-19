#include <fstream>
#include <iostream>
#include <deque>
#include <print>

using Coord = std::pair<int, int>;
struct Machine;
using MachineList = std::deque<Machine>;

struct Machine
{
    Coord A;
    Coord B;
    Coord Prize;
};

MachineList parse(std::string_view FileName)
{
    std::ifstream File(FileName);

    Coord       A;
    Coord       B;
    Coord       Prize;
    MachineList Machines;
    for (std::string Line; std::getline(File, Line);)
    {
        if (Line.find("A") != std::string::npos)
        {
            auto DeltaX = Line.substr(Line.find("X+"), 4).substr(2, 4);
            auto DeltaY = Line.substr(Line.find("Y+"), 4).substr(2, 4);
            A           = { std::stoi(DeltaX), std::stoi(DeltaY) };
        }
        else if (Line.find("B") != std::string::npos)
        {
            auto DeltaX = Line.substr(Line.find("X+"), 4).substr(2, 4);
            auto DeltaY = Line.substr(Line.find("Y+"), 4).substr(2, 4);
            B           = { std::stoi(DeltaX), std::stoi(DeltaY) };
        }
        else if (Line.find("Prize") != std::string::npos)
        {
            auto X = Line.substr(Line.find("X=") + 2, Line.find(","));
            auto Y = Line.substr(Line.find("Y=") + 2);
            Prize  = { std::stoi(X), std::stoi(Y) };
            Machines.push_back({ A, B, Prize });
        }
        // Parse Line
    }
    return Machines;
}

void print(MachineList const& Machines)
{
    for (auto const& M : Machines)
    {
        std::cout << "A: " << M.A.first << ", " << M.A.second << std::endl;
        std::cout << "B: " << M.B.first << ", " << M.B.second << std::endl;
        std::cout << "Prize: " << M.Prize.first << ", " << M.Prize.second << std::endl;
    }
}

using OptInt = std::optional<int>;

OptInt solve(Machine const& M)
{
    for (int m = 0; m < 100; ++m)
        for (int n = 0; n < 100; ++n)
            if (m * M.A.first + n * M.B.first == M.Prize.first && m * M.A.second + n * M.B.second == M.Prize.second)
                return 3 * m + n;

    return std::nullopt;
}

int main(int Argc, char* ArgV[])
{
    auto Machines = parse("/Users/vincentmayer/repos/AdventOfCode/13/input.txt");
    print(Machines);
    // Push A: 3 tokens
    // Push B: 1 token
    // Min(m*3 + n), s.t. m*A + n*B = Prize
    // m < 100, n < 100
    int Solution = 0;
    for (auto const& M : Machines)
    {
        auto Opt = solve(M);
        if (Opt.has_value())
            Solution += Opt.value();
    }
    std::println("Found solution: {}", Solution);

    return 0;
}
