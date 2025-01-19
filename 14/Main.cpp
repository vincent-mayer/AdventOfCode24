#include <string>
#include <fstream>
#include <iostream>
#include <print>
#include <unistd.h>

using Coord = std::pair<int, int>;
struct Robot
{
    Coord Pos;
    Coord Vel;
};
using RobotList = std::deque<Robot>;

static constexpr int XSize = 101;
static constexpr int YSize = 103;

RobotList parse(std::string const& FilePath)
{
    std::ifstream File(FilePath);
    RobotList     Robots;
    for (std::string Line; std::getline(File, Line);)
    {
        auto First  = Line.substr(0, Line.find("v=") - 1);
        auto Second = Line.substr(Line.find("v=") + 2);
        int  Px     = std::stoi(First.substr(First.find("=") + 1, First.find(",") - 1));
        int  Py     = std::stoi(First.substr(First.find(",") + 1));
        int  Vx     = std::stoi(Second.substr(0, Second.find(",")));
        int  Vy     = std::stoi(Second.substr(Second.find(",") + 1));
        Robots.push_back({ .Pos = { Px, Py }, .Vel = { Vx, Vy } });
    }
    return Robots;
}

void print(RobotList const& Robots)
{
    for (auto const& R : Robots)
        std::println("p={},{} v={},{}", R.Pos.first, R.Pos.second, R.Vel.first, R.Vel.second);
}

void debugRobots(RobotList const& Robots)
{
    std::deque<std::deque<char>> Grid(YSize, std::deque<char>(XSize, 0));

    for (auto const& R : Robots)
        Grid[R.Pos.second][R.Pos.first] += 1;

    for (int y = 0; y < YSize; y++)
    {
        for (int x = 0; x < XSize; x++)
        {
            if (Grid[y][x] > 0)
                std::cout << "#";
            else
                std::cout << ".";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int updatePos(int Pos, int Vel, int Bound)
{
    int NewPos = Pos + Vel;
    if (NewPos < 0)
        NewPos += Bound;
    else if (NewPos >= Bound)
        NewPos -= Bound;
    return NewPos;
}

void simulate(RobotList& Robots)
{
    for (auto& R : Robots)
    {
        int NewX = updatePos(R.Pos.first, R.Vel.first, XSize);
        int NewY = updatePos(R.Pos.second, R.Vel.second, YSize);
        R.Pos    = { NewX, NewY };
    }
}

int calcSafetyScore(RobotList Robots)
{
    int Quadrants[4] = {};
    for (auto const& Robot : Robots)
    {
        int x = Robot.Pos.first;
        int y = Robot.Pos.second;

        int XHalf = std::floor(XSize / 2);
        int YHalf = std::floor(YSize / 2);

        if (x == XHalf || y == YHalf)
            continue;

        if (x < XHalf && y < YHalf)
            Quadrants[0] += 1;
        else if (x >= XHalf && y < YHalf)
            Quadrants[1] += 1;
        else if (x < XHalf && y >= YHalf)
            Quadrants[2] += 1;
        else
            Quadrants[3] += 1;
    }
    return Quadrants[0] * Quadrants[1] * Quadrants[2] * Quadrants[3];
}

int main()
{
    auto Robots = parse("/Users/vincentmayer/repos/AdventOfCode/14/input.txt");
    for (int i = 0; i < 10000; i++)
    {
        simulate(Robots);
        std::println("Time {}", i + 1);
        debugRobots(Robots);
    }
    int Score = calcSafetyScore(Robots);
    std::println("Part 1: {}", Score);
    return 0;
}
