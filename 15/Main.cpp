#include <cstdint>
#include <deque>
#include <unordered_set>
#include <string_view>
#include <fstream>
#include <ranges>
#include <iostream>

using Map        = std::deque<std::deque<char>>;
using Coord      = std::pair<int, int>;
using CoordList  = std::deque<Coord>;
using CoordLists = std::deque<CoordList>;

struct CoordHasher
{
    std::size_t operator()(Coord const& C) const
    {
        return std::hash<int> {}(C.first) ^ std::hash<int> {}(C.second);
    }
};
using CoordSet = std::unordered_set<Coord, CoordHasher>;

enum class Direction : uint8_t
{
    Up,
    Right,
    Down,
    Left,
};

using MoveSequence = std::deque<Direction>;

struct InputData
{
    Coord        Robot;
    CoordSet     Obstacles;
    CoordSet     Boxes;
    MoveSequence Moves;
    Coord        GridSize = { 0, 0 };
};

InputData parse(std::string_view FilePath)
{
    std::ifstream File { FilePath };

    int       Row = 0;
    InputData Out;
    int       GridX = 0;
    int       GridY = 0;
    for (std::string Line; std::getline(File, Line); Row++)
    {
        if (Line == "\n")
            continue;

        if (Line.contains("<") || Line.contains(">"))
        {
            for (auto [Pos, Char] : std::ranges::zip_view(std::views::iota(0), Line))
            {
                if (Char == '<')
                    Out.Moves.push_back(Direction::Left);
                else if (Char == '>')
                    Out.Moves.push_back(Direction::Right);
                else if (Char == '^')
                    Out.Moves.push_back(Direction::Up);
                else if (Char == 'v')
                    Out.Moves.push_back(Direction::Down);
            }
            continue;
        }
        for (auto [Col, Char] : std::ranges::zip_view(std::views::iota(0), Line))
        {
            if (Char == '#')
                Out.Obstacles.insert({ Col, Row });
            else if (Char == 'O')
                Out.Boxes.insert({ Col, Row });
            else if (Char == '@')
                Out.Robot = { Col, Row };
        }
        if (GridX == 0)
            GridX = Line.size();
        GridY++;
    }
    Out.GridSize = { GridX, GridY - 1 };
    return Out;
}

void print(InputData const& Data)
{
    Coord C;
    for (int i = 0; i < Data.GridSize.second; i++)
    {
        for (int j = 0; j < Data.GridSize.first; j++)
        {
            C = { j, i };
            if (Data.Robot == C)
                std::cout << "@";
            else if (Data.Obstacles.count(C))
                std::cout << "#";
            else if (Data.Boxes.count(C))
                std::cout << "O";
            else
                std::cout << ".";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Coord calcNewPos(Coord& Pos, Direction Move)
{
    Coord Out = Pos;
    switch (Move)
    {
    case Direction::Up: Out.second--; break;
    case Direction::Right: Out.first++; break;
    case Direction::Down: Out.second++; break;
    case Direction::Left: Out.first--; break;
    }
    return Out;
}

void makeMove(InputData& Input, Direction Move)
{
    Coord NewPos = calcNewPos(Input.Robot, Move);

    // If we have an obstacle, do nothing.
    if (Input.Obstacles.count(NewPos))
        return;

    // If we have a box, check if we can move it.
    if (Input.Boxes.count(NewPos))
    {
        Coord NewBoxPos = calcNewPos(NewPos, Move);
        while (Input.Boxes.count(NewBoxPos))
            NewBoxPos = calcNewPos(NewBoxPos, Move);

        if (Input.Obstacles.count(NewBoxPos))
            return;
        Input.Boxes.erase(NewPos);
        Input.Boxes.insert(NewBoxPos);
    }
    Input.Robot = NewPos;
    // print(Input);
    return;
}

int computeScore(InputData const& Input)
{
    long Score = 0;
    for (auto const& Box : Input.Boxes)
        Score += (Box.first + 100 * Box.second);
    return Score;
}

int main()
{
    auto Input = parse("/Users/vincentmayer/repos/AdventOfCode/15/input.txt");
    std::cout << "X: " << Input.GridSize.first << " Y: " << Input.GridSize.second << std::endl;

    Coord PrevPos;
    int   Ctrl;
    for (auto Move : Input.Moves)
        makeMove(Input, Move);

    std::cout << "Part 1: " << computeScore(Input) << std::endl;
    return 0;
}
