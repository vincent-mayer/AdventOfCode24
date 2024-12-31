#include <deque>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>

using TopoMap    = std::deque<std::deque<int>>;
using Coord      = std::pair<int, int>;
using CoordList  = std::deque<Coord>;
using CoordLists = std::deque<CoordList>;
struct CoordHasher;

using CoordSet = std::unordered_set<Coord, CoordHasher>;

struct CoordHasher
{
    std::size_t operator()(Coord const& C) const
    {
        return std::hash<int> {}(C.first) ^ std::hash<int> {}(C.second);
    }
};

TopoMap parse(std::string_view FilePath)
{
    std::ifstream File { FilePath };
    TopoMap       Output;
    for (std::string Line; std::getline(File, Line);)
    {
        std::deque<int> Row;
        for (char Value : Line)
            Row.push_back(Value - '0');
        Output.push_back(Row);
    }
    return Output;
}

void print(TopoMap const& Map, Coord const& Current)
{
    for (int i = 0; i < Map.size(); i++)
    {
        for (int j = 0; j < Map[i].size(); j++)
        {
            if (i == Current.first && j == Current.second)
                std::cout << 'X' << ' ';
            else
                std::cout << Map[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

CoordList findStartingPoints(TopoMap const& Map)
{
    CoordList StartingPoints;
    for (int i = 0; i < Map.size(); i++)
        for (int j = 0; j < Map[i].size(); j++)
            if (Map[i][j] == 0)
                StartingPoints.push_back({ i, j });
    return StartingPoints;
}

CoordList nextMoves(TopoMap const& Map, Coord const& Current)
{
    CoordList Moves;
    int       Value = Map[Current.first][Current.second];
    // Left
    if (Current.second > 0 && Map[Current.first][Current.second - 1] == Value + 1)
        Moves.push_back({ Current.first, Current.second - 1 });
    // Right
    if (Current.second < Map[Current.first].size() - 1 && Map[Current.first][Current.second + 1] == Value + 1)
        Moves.push_back({ Current.first, Current.second + 1 });
    // Up
    if (Current.first > 0 && Map[Current.first - 1][Current.second] == Value + 1)
        Moves.push_back({ Current.first - 1, Current.second });
    // Down
    if (Current.first < Map.size() - 1 && Map[Current.first + 1][Current.second] == Value + 1)
        Moves.push_back({ Current.first + 1, Current.second });
    return Moves;
}

bool isSuccess(TopoMap const& Map, Coord const& Current)
{
    return Map[Current.first][Current.second] == 9;
}

int findPaths(TopoMap const& Map, Coord const& StartingPoint)
{
    int ValidPathsCount = 0;

    CoordList Moves = nextMoves(Map, StartingPoint);
    Coord     Current;
    CoordSet  Visited;
    while (!Moves.empty())
    {
        Current = Moves.back();
        //print(Map, Current);
        Moves.pop_back();
        if (isSuccess(Map, Current) && !Visited.count(Current))
        {
            // Visited.insert(Current);
            ValidPathsCount++;
            continue;
        }
        auto NextMoves = nextMoves(Map, Current);
        Moves.insert(Moves.end(), NextMoves.begin(), NextMoves.end());
    }
    return ValidPathsCount;
}

int main(int Argc, char* ArgV[])
{
    auto Map            = parse("/Users/vincentmayer/repos/AdventOfCode/10/input.txt");
    auto StartingPoints = findStartingPoints(Map);
    int  ValidSum       = 0;
    for (auto const& StartingPoint : StartingPoints)
        ValidSum += findPaths(Map, StartingPoint);

    std::cout << "Part 1: " << ValidSum << std::endl;
    return 0;
}
