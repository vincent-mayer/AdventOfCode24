#include <fstream>
#include <iostream>
#include <unordered_set>

using CharGrid      = std::deque<std::deque<char>>;
using Coord         = std::pair<int, int>;
using CoordList     = std::deque<Coord>;
using AntennaCoords = std::unordered_map<char, CoordList>;

struct CoordHasher;
using CoordSet = std::unordered_set<Coord, CoordHasher>;

struct CoordHasher
{
    std::size_t operator()(Coord const& C) const
    {
        return std::hash<int> {}(C.first) ^ std::hash<int> {}(C.second);
    }
};

CharGrid parse(std::string_view FilePath)
{
    std::ifstream File { FilePath };
    CharGrid      Grid;
    for (std::string Line; std::getline(File, Line);)
        Grid.push_back({ Line.begin(), Line.end() });
    return Grid;
}

AntennaCoords findAntennas(CharGrid const& Grid)
{
    AntennaCoords Antennas;
    for (int i = 0; i < Grid.size(); i++)
    {
        for (int j = 0; j < Grid[i].size(); j++)
        {
            if (Grid[i][j] != '.')
                Antennas[Grid[i][j]].push_back({ j, i });
        }
    }
    return Antennas;
}

CoordSet findAntiNodes(AntennaCoords const& Antennas, CharGrid& Grid)
{
    CoordSet AntiNodes;
    for (auto const& [_, AntennaPositions] : Antennas)
    {
        int CandidateX, CandidateY;
        for (auto const& Outer : AntennaPositions)
        {
            for (auto const& Inner : AntennaPositions)
            {
                if (Outer == Inner)
                    continue;
                int CandidateX = Inner.first + 2 * (Outer.first - Inner.first);
                int CandidateY = Inner.second + 2 * (Outer.second - Inner.second);
                if (CandidateX >= 0 && CandidateX < Grid[0].size() && CandidateY >= 0 && CandidateY < Grid.size())
                {
                    Grid[CandidateY][CandidateX] = '#';
                    AntiNodes.insert({ CandidateX, CandidateY });
                }
            }
        }
    }
    return AntiNodes;
}

CoordSet findAntiNodes2(AntennaCoords const& Antennas, CharGrid& Grid)
{
    CoordSet AntiNodes;
    for (auto const& [_, AntennaPositions] : Antennas)
    {
        int CandidateX, CandidateY;
        for (auto const& Outer : AntennaPositions)
        {
            for (auto const& Inner : AntennaPositions)
            {
                if (Outer == Inner)
                    continue;
                for (int Dist = 0; Dist < 300; Dist++)
                {
                    int CandidateX = Inner.first + Dist * (Outer.first - Inner.first);
                    int CandidateY = Inner.second + Dist * (Outer.second - Inner.second);
                    if (CandidateX >= 0 && CandidateX < Grid[0].size() && CandidateY >= 0 && CandidateY < Grid.size())
                    {
                        Grid[CandidateY][CandidateX] = '#';
                        AntiNodes.insert({ CandidateX, CandidateY });
                    }
                }
            }
        }
    }
    return AntiNodes;
}

int main(int Argc, char* ArgV[])
{
    CharGrid      Grid       = parse("/Users/vincentmayer/repos/AdventOfCode/08/input.txt");
    AntennaCoords Antennas   = findAntennas(Grid);
    auto          AntiNodes  = findAntiNodes(Antennas, Grid);
    auto          AntiNodes2 = findAntiNodes2(Antennas, Grid);
    for (auto const& Row : Grid)
    {
        for (auto const& Cell : Row)
            std::cout << Cell;
        std::cout << std::endl;
    }
    std::cout << "Part 1: " << AntiNodes.size() << std::endl;
    std::cout << "Part 2: " << AntiNodes2.size() << std::endl;
    return 0;
}
