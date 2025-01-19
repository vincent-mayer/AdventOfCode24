#include <fstream>
#include <iostream>
#include <deque>
#include <print>

using Coord = std::pair<long, long>;
struct Machine;
using MachineList = std::deque<Machine>;
using OptSolution = std::optional<long>;

static constexpr long OffsetPart2 = 10000000000000;

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
            A           = { std::stol(DeltaX), std::stol(DeltaY) };
        }
        else if (Line.find("B") != std::string::npos)
        {
            auto DeltaX = Line.substr(Line.find("X+"), 4).substr(2, 4);
            auto DeltaY = Line.substr(Line.find("Y+"), 4).substr(2, 4);
            B           = { std::stol(DeltaX), std::stol(DeltaY) };
        }
        else if (Line.find("Prize") != std::string::npos)
        {
            auto X = Line.substr(Line.find("X=") + 2, Line.find(","));
            auto Y = Line.substr(Line.find("Y=") + 2);
            Prize  = { std::stol(X), std::stol(Y) };
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

OptSolution solve1(Machine const& M)
{
    for (int m = 0; m < 100; ++m)
        for (int n = 0; n < 100; ++n)
            if (m * M.A.first + n * M.B.first == M.Prize.first && m * M.A.second + n * M.B.second == M.Prize.second)
                return 3 * m + n;

    return std::nullopt;
}

OptSolution solve2(Machine const M)
{
    // Now the prize is very far out and we might need way more
    // than 100 iterations to find the solution.
    // m = (PrizeY - n * By) / Ay
    // n = (PrizeX * Ay - PrizeY * Ax) / (Bx * Ay - By * Ax)

    long Ax     = M.A.first;
    long Ay     = M.A.second;
    long Bx     = M.B.first;
    long By     = M.B.second;
    long PrizeX = M.Prize.first;
    long PrizeY = M.Prize.second;

    long n = (PrizeX * Ay - PrizeY * Ax) / (Bx * Ay - By * Ax);
    long m = (PrizeY - n * By) / Ay;
    if (m * Ax + n * Bx == PrizeX && m * Ay + n * By == PrizeY)
        return 3 * m + n;
    return std::nullopt;
};

int main(int Argc, char* ArgV[])
{
    auto Machines = parse("/Users/vincentmayer/repos/AdventOfCode/13/input.txt");
    // Push A: 3 tokens
    // Push B: 1 token
    // Min(m*3 + n), s.t. m*A + n*B = Prize
    // m < 100, n < 100
    long Solution1 = 0;
    for (auto const& M : Machines)
    {
        auto Opt = solve1(M);
        if (Opt.has_value())
            Solution1 += Opt.value();
    }
    std::println("Part 1: {}", Solution1);

    // Part 2 ------------------------------------
    MachineList Machines2 = Machines;
    for (auto& M : Machines2)
    {
        M.Prize.first += OffsetPart2;
        M.Prize.second += OffsetPart2;
    }
    // Push A: 3 tokens
    // Push B: 1 token
    // Min(m*3 + n), s.t. m*A + n*B = Prize
    //
    // m * Ax + n * Bx = PrizeX
    // m * Ay + n * By = PrizeY
    //
    // m * Ax + n * Bx = PrizeX
    // m = (PrizeY - n * By) / Ay
    //
    // (PrizeY - n * By) / Ay * Ax + n * Bx = PrizeX
    // (PrizeY - n * By) * Ax + n * Bx * Ay = PrizeX * Ay
    // PrizeY * Ax - n * By * Ax + n * Bx * Ay = PrizeX * Ay
    // n(Bx * Ay - By * Ax) = PrizeX * Ay - PrizeY * Ax
    // n = (PrizeX * Ay - PrizeY * Ax) / (Bx * Ay - By * Ax)

    // Matrix form:
    // | Ax  Bx | | m | = | PrizeX |
    // | Ay  By | | n | = | PrizeY |
    long Solution2 = 0;
    for (auto const& M : Machines2)
    {
        auto Opt = solve2(M);
        if (Opt.has_value())
            Solution2 += Opt.value();
    }
    std::println("Part 2: {}", Solution2);
    return 0;
}
