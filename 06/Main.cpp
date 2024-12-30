#include <fstream>
#include <iostream>
#include <unordered_set>

struct CoordHasher;

using CharGrid = std::deque<std::deque<char>>;
using Coord    = std::pair<int, int>;
using CoordSet = std::unordered_set<Coord, CoordHasher>;

struct CoordHasher
{
    std::size_t operator()(Coord const& C) const
    {
        return std::hash<int> {}(C.first) ^ std::hash<int> {}(C.second);
    }
};

static constexpr char StartChar = '^';

enum class Direction : uint8_t
{
    Up,
    Right,
    Down,
    Left,
};

Direction nextDirection(Direction Current)
{
    return static_cast<Direction>((static_cast<uint8_t>(Current) + 1) % 4);
}

Coord move(Coord Current, Direction Dir)
{
    switch (Dir)
    {
    case Direction::Up: return { Current.first - 1, Current.second };
    case Direction::Right: return { Current.first, Current.second + 1 };
    case Direction::Down: return { Current.first + 1, Current.second };
    case Direction::Left: return { Current.first, Current.second - 1 };
    }
}

CharGrid parse()
{
    std::ifstream File { "/Users/vincentmayer/repos/AdventOfCode/06/input.txt" };
    CharGrid      Grid;
    std::string   Line;
    while (std::getline(File, Line))
        Grid.push_back({ Line.begin(), Line.end() });
    return Grid;
}

Coord findStart(CharGrid const& Grid)
{
    Coord Start;
    for (int i = 0; i < Grid.size(); i++)
    {
        for (int j = 0; j < Grid[i].size(); j++)
            if (Grid[i][j] == StartChar)
                Start = { i, j };
    }
    return Start;
}

int main(int Argc, char* ArgV[])
{
    CharGrid Grid  = parse();
    Coord    Start = findStart(Grid);

    CoordSet  UniquePath;
    Coord     Current          = Start;
    Direction CurrentDirection = Direction::Up;
    while (Current.first > 0 && Current.first < Grid.size() && Current.second > 0 && Current.second < Grid[0].size())
    {
        if (auto LookAhead = move(Current, CurrentDirection); Grid[LookAhead.first][LookAhead.second] != '#')
        {
            Current = LookAhead;
            UniquePath.insert(Current);
        }
        else
        {
            CurrentDirection = nextDirection(CurrentDirection);
        }
    }

    std::cout << "Unique visits: " << UniquePath.size() << std::endl;
    return 0;
}
