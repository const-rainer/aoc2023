#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::map<std::string, int> limits = {{"red", 12}, {"green", 13}, {"blue", 14}};

std::vector<std::string> SplitStringByDelimiter(std::string& str, const char* delimiter)
{
#ifdef VERBOSE
    std::cout << "Splitting string: " << std::quoted(str) << " by delimiter " << std::quoted(delimiter) << "\n";
#endif

    std::vector<std::string> substrings;

    auto pos = str.find(delimiter);

#ifdef VERBOSE
    std::cout << "Delimiter at position: " << pos << "\n";
#endif

    while (pos != str.npos)
    {
        const auto substring = str.substr(0, pos);

#ifdef VERBOSE
        std::cout << "Substring: " << substring << "\n";
#endif

        substrings.push_back(substring);
        str = str.substr(pos + 1, str.size() - pos);

#ifdef VERBOSE
        std::cout << "New str: " << str << "\n";
#endif

        pos = str.find(delimiter);
    }

    substrings.push_back(str);  // the remaining part of it

    return substrings;
}

void PreprocessGameString(std::string& game_str)
{
    for (auto i = 0; i < game_str.size(); i++)
    {
        if (game_str.at(i) == ' ')
        {
            game_str.erase(i, 1);
        }
    }
}

bool IsPossible(std::string& set)
{
    auto extraction = SplitStringByDelimiter(set, ",");

    for (auto& ext : extraction)
    {
        std::size_t i = 0;
        for (; i < ext.size(); i++)
        {
            if (!std::isdigit(ext.at(i)))
                break;
        }

        auto number = atoi(ext.substr(0, i).c_str());
        auto color = ext.substr(i);

        if (number > limits[color])
        {
#ifdef VERBOSE
            std::cout << "Extraction not possible because " << number << " of " << color
                      << " cubes is higher than the limit (" << limits[color] << ")\n";
#endif
            return false;
        }
    }

    return true;
}

std::vector<std::string> ExtractSets(std::string const& game)
{
    std::vector<std::string> sets;
#ifdef VERBOSE
    std::cout << "Processing game: " << game << "\n";
#endif
    auto pos = game.find(":");
    if (pos != game.size())
    {
        auto sets_str = game.substr(pos + 1, game.size() - pos);
        sets = SplitStringByDelimiter(sets_str, ";");
    }

    return sets;
}

bool IsGamePossible(std::string const& game)
{
    auto sets = ExtractSets(game);
    for (auto& set : sets)
    {
        if (!IsPossible(set))
            return false;
    }

    return true;
}

int GetTotal(std::vector<std::string> const& impossible_games)
{
    int total = 0;

    for (auto& game : impossible_games)
    {
        total += std::atoi(game.substr(4, game.find(":")).c_str());
    }

    return total;
}

int GetPower(std::string const& game)
{
    auto sets = ExtractSets(game);
    std::map<std::string, int> max_values{{"red", 1}, {"green", 1}, {"blue", 1}};

    for (auto& set : sets)
    {
        auto extraction = SplitStringByDelimiter(set, ",");

        for (auto& ext : extraction)
        {
            std::size_t i = 0;
            for (; i < ext.size(); i++)
            {
                if (!std::isdigit(ext.at(i)))
                    break;
            }

            auto amount = atoi(ext.substr(0, i).c_str());
            auto color = ext.substr(i);

            if (amount > max_values.at(color))
            {
                max_values.at(color) = amount;
            }
        }
    }

    return max_values.at("red") * max_values.at("green") * max_values.at("blue");
}

int main()
{
    std::string input_file_name{"input"};
    std::ifstream input_file{input_file_name};

    int count{0};
    int total_power{0};
    std::vector<std::string> possible_games{};

    std::string game;
    while (std::getline(input_file, game))
    {
        PreprocessGameString(game);

        total_power += GetPower(game);

        if (IsGamePossible(game))
            possible_games.push_back(game);

        count++;
    }

#ifdef VERBOSE
    std::cout << "\nNumber of lines processed: " << count << "\n";
    std::cout << "Possible games: \n";
#endif

    std::cout << "Total is: " << GetTotal(possible_games) << "\n\n";
    std::cout << "Total power is: " << total_power << "\n\n";
}
