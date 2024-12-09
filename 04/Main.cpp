#include <fstream>
#include <iostream>
#include <regex>

static auto FilePath = "/Users/vincentmayer/repos/AdventOfCode/04/input.txt";

using StringList = std::deque<std::string>;

StringList parse(std::istream& InputStream)
{
    StringList Data;
    for (std::string Line; std::getline(InputStream, Line);)
        Data.push_back(Line);
    return Data;
}

int countMatches(const StringList& Strings, const std::regex& Pattern)
{
    int NumMatches = 0;
    for (const std::string& Line : Strings)
        NumMatches += std::distance({ Line.begin(), Line.end(), Pattern }, std::sregex_iterator {});
    return NumMatches;
}

StringList reverseStrings(StringList const& Input)
{
    StringList ReversedStrings;
    for (auto const& Str : Input)
        ReversedStrings.emplace_back(Str.rbegin(), Str.rend());
    return ReversedStrings;
}

StringList rotateStrings(StringList const& Input)
{
    StringList RotatedStrings;
    for (int i = 0; i < Input.size(); i++)
    {
        std::string RotatedString;
        for (int j = 0; j < Input.size(); j++)
            RotatedString.push_back(Input[j][i]);
        RotatedStrings.push_back(RotatedString);
    }
    return RotatedStrings;
}

StringList diagonalStrings(StringList const& Input)
{
    StringList DiagonalStrings;
    // Subdiagonal
    for (int i = 0; i < Input.size(); i++)
    {
        std::string SubDiagonalString;
        std::string SuperDiagonalString;
        for (int j = 0; j < Input.size() - i; j++)
        {
            SubDiagonalString.push_back(Input[j + i][j]);
            if (j != (j + i))
                SuperDiagonalString.push_back(Input[j][j + i]);
        }
        DiagonalStrings.push_back(SubDiagonalString);
        DiagonalStrings.push_back(SuperDiagonalString);
    }
    return DiagonalStrings;
}

int main(int Argc, char* ArgV[])
{
    std::ifstream File { FilePath };
    std::regex    Pattern(R"(XMAS)");

    // TODO(vincent): There must be a smarter way of doing this
    StringList Strings = parse(File);

    StringList BackwardsStrings             = reverseStrings(Strings);
    StringList RotatedStrings               = rotateStrings(Strings);
    StringList RotatedStringsBackwards      = reverseStrings(RotatedStrings);
    StringList DiagonalStrings              = diagonalStrings(Strings);
    StringList ReverseDiagonalStrings       = reverseStrings(DiagonalStrings);
    StringList MirrorDiagonalStrings        = diagonalStrings(BackwardsStrings);
    StringList ReverseMirrorDiagonalStrings = reverseStrings(MirrorDiagonalStrings);

    int HorizontalMatches            = countMatches(Strings, Pattern);
    int HorizontalBackwardsMatches   = countMatches(BackwardsStrings, Pattern);
    int VerticalMatches              = countMatches(RotatedStrings, Pattern);
    int VerticalBackwardsMatches     = countMatches(RotatedStringsBackwards, Pattern);
    int DiagonalMatches              = countMatches(DiagonalStrings, Pattern);
    int MirrorDiagonalMatches        = countMatches(MirrorDiagonalStrings, Pattern);
    int ReverseDiagonalMatches       = countMatches(ReverseDiagonalStrings, Pattern);
    int ReverseMirrorDiagonalMatches = countMatches(ReverseMirrorDiagonalStrings, Pattern);

    int Sum = (HorizontalMatches + HorizontalBackwardsMatches + VerticalMatches        //
               + VerticalBackwardsMatches + DiagonalMatches + MirrorDiagonalMatches    //
               + ReverseDiagonalMatches + ReverseMirrorDiagonalMatches);

    std::cout << Sum << std::endl;

    return 0;
}
