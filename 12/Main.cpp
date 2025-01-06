#include <deque>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <ranges>
#include <algorithm>

using Map        = std::deque<std::deque<char>>;
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

enum class Orientation
{
    Horizontal,
    Vertical
};
using OrientationList = std::deque<Orientation>;

Map parse(std::string_view FilePath)
{
    std::ifstream File { FilePath };
    Map           Output;
    for (std::string Line; std::getline(File, Line);)
    {
        std::deque<char> Row;
        for (char Value : Line)
            Row.push_back(Value);
        Output.push_back(Row);
    }
    return Output;
}

std::optional<Coord> findStartingPoint(Map const& Map, CoordSet const& Known)
{
    for (int i = 0; i < Map.size(); i++)
        for (int j = 0; j < Map[i].size(); j++)
        {
            if (Known.count({ i, j }) == 0)
                return Coord { i, j };
        }
    return std::nullopt;
}

CoordList nextMoves(Map const& Map, Coord const& Current)
{
    CoordList Moves;
    char      Value = Map[Current.first][Current.second];
    // Left
    if (Current.second > 0 && Map[Current.first][Current.second - 1] == Value)
        Moves.push_back({ Current.first, Current.second - 1 });
    // Right
    if (Current.second < Map[Current.first].size() - 1 && Map[Current.first][Current.second + 1] == Value)
        Moves.push_back({ Current.first, Current.second + 1 });
    // Up
    if (Current.first > 0 && Map[Current.first - 1][Current.second] == Value)
        Moves.push_back({ Current.first - 1, Current.second });
    // Down
    if (Current.first < Map.size() - 1 && Map[Current.first + 1][Current.second] == Value)
        Moves.push_back({ Current.first + 1, Current.second });
    return Moves;
}

CoordSet findRegion(Map const& Map, Coord const& StartingPoint)
{
    CoordList Moves = nextMoves(Map, StartingPoint);
    Coord     Current;
    CoordSet  Region { StartingPoint };
    while (!Moves.empty())
    {
        Current = Moves.back();
        Moves.pop_back();
        if (Region.count(Current))
            continue;
        Region.insert(Current);
        auto NextMoves = nextMoves(Map, Current);
        Moves.insert(Moves.end(), NextMoves.begin(), NextMoves.end());
    }
    return Region;
}

std::pair<CoordList, OrientationList> findPerimeter(CoordSet const& Region)
{
    CoordList       Perimeter;
    OrientationList Orientations;
    for (auto const& Coord : Region)
    {
        if (Region.count({ Coord.first - 1, Coord.second }) == 0)
        {
            Perimeter.push_back({ Coord.first - 1, Coord.second });
            Orientations.push_back(Orientation::Horizontal);
        }
        if (Region.count({ Coord.first + 1, Coord.second }) == 0)
        {
            Perimeter.push_back({ Coord.first + 1, Coord.second });
            Orientations.push_back(Orientation::Horizontal);
        }
        if (Region.count({ Coord.first, Coord.second - 1 }) == 0)
        {
            Perimeter.push_back({ Coord.first, Coord.second - 1 });
            Orientations.push_back(Orientation::Vertical);
        }
        if (Region.count({ Coord.first, Coord.second + 1 }) == 0)
        {
            Perimeter.push_back({ Coord.first, Coord.second + 1 });
            Orientations.push_back(Orientation::Vertical);
        }
    }
    return { Perimeter, Orientations };
}

int findSidesCount(CoordSet const& Region)
{
    auto [Perimeter, Orientations] = findPerimeter(Region);
    int SidesCount                 = 0;
    for (int i = 0; i < Perimeter.size(); i++)
    {
        auto        C         = Perimeter[i];
        auto        Orient    = Orientations[i];
        auto        Right     = std::find(Perimeter.begin(), Perimeter.end(), Coord { C.first, C.second + 1 });
        auto        Down      = std::find(Perimeter.begin(), Perimeter.end(), Coord { C.first - 1, C.second });
        bool        HasRight  = Right != Perimeter.end();
        bool        HasDown   = Down != Perimeter.end();
        std::string OrientStr = Orient == Orientation::Horizontal ? "Horizontal" : "Vertical";
        // std::cout << "Coord: " << C.first << "," << C.second << " Orient: " << OrientStr << " Right: " << HasRight << " Down: " << HasDown
        //           << std::endl;
        if (Orient == Orientation::Horizontal
            && (!HasRight || Orientations[std::distance(Perimeter.begin(), Right)] == Orientation::Vertical))
        {
            SidesCount++;
            continue;
        }
        if (Orient == Orientation::Vertical
            && (!HasDown || Orientations[std::distance(Perimeter.begin(), Down)] == Orientation::Horizontal))
        {
            SidesCount++;
            continue;
        }
        // if (Orient == Orientation::Horizontal && !HasRight)
        // {
        //     SidesCount++;
        //     continue;
        // }
        // if (Orient == Orientation::Vertical && !HasDown)
        // {
        //     SidesCount++;
        //     continue;
        // }
    }
    return SidesCount;
}

int calculatePrice1(Map const& Map)
{
    int      Sum = 0;
    CoordSet Known;
    while (true)
    {
        auto StartingPoint = findStartingPoint(Map, Known);
        if (!StartingPoint.has_value())
            break;
        char Val    = Map[(*StartingPoint).first][(*StartingPoint).second];
        auto Region = findRegion(Map, *StartingPoint);
        Known.insert(Region.begin(), Region.end());
        int Perimeter = findPerimeter(Region).first.size();
        Sum += (Region.size() * Perimeter);
        // std::cout << "Val: " << Val << " Region: " << Region.size() << " Perim: " << Perimeter << std::endl;
    }
    return Sum;
}

int calculatePrice2(Map const& Map)
{
    int      Sum = 0;
    CoordSet Known;
    while (true)
    {
        auto StartingPoint = findStartingPoint(Map, Known);
        if (!StartingPoint.has_value())
            break;
        char Val = Map[(*StartingPoint).first][(*StartingPoint).second];

        auto Region = findRegion(Map, *StartingPoint);
        int  Sides  = findSidesCount(Region);

        Known.insert(Region.begin(), Region.end());

        Sum += (Region.size() * Sides);
        std::cout << "Val: " << Val << " Region: " << Region.size() << " Sides: " << Sides << std::endl;
    }
    return Sum;
}

int main(int Argc, char* ArgV[])
{
    auto Map    = parse("/Users/vincentmayer/repos/AdventOfCode/12/input_dbg.txt");
    int  Price1 = calculatePrice1(Map);
    std::cout << "Sum Part 1: " << Price1 << std::endl;
    int Price2 = calculatePrice2(Map);
    std::cout << "Sum Part 2: " << Price2 << std::endl;
    return 0;
}
