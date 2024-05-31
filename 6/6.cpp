#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

namespace indexes
{
static constexpr std::uint8_t TIME_DATA_INDEX{0U};
static constexpr std::uint8_t DISTANCE_DATA_INDEX{1U};
}  // namespace indexes

typedef struct race
{
    std::uint64_t total_time;
    std::uint64_t record_distance;
} race;

std::vector<std::uint64_t> parse_numbers(std::string& str)
{
    std::vector<std::uint64_t> v{};

    for (auto c = str.begin(); c != str.end(); c++)
    {
        while (*c == ' ')
        {
            str.erase(c);
        }
    }

    std::regex regex{"[0-9]{1,20}"};
    for (auto i = std::sregex_iterator(str.begin(), str.end(), regex); i != std::sregex_iterator(); ++i)
    {
        v.push_back(std::atoll((*i).str().c_str()));
    }

    return v;
}

std::vector<race> parse_race_data(std::ifstream&& input_file)
{
    std::vector<race> races{};
    std::vector<std::vector<std::uint64_t>> race_data{};
    std::string str{};
    while (std::getline(input_file, str))
    {
        race_data.push_back(parse_numbers(str));
    }

    for (std::size_t i = 0U; i < race_data.at(0U).size(); i++)
    {
        races.push_back({.total_time{race_data.at(indexes::TIME_DATA_INDEX).at(i)},
                         .record_distance{race_data.at(indexes::DISTANCE_DATA_INDEX).at(i)}});
    }

    return races;
}

std::uint64_t solve_problem(std::vector<race>&& races)
{
    std::uint64_t result{1U};

    for (auto& race : races)
    {
        auto race_time{race.total_time};
        auto distance_to_beat{race.record_distance};

        std::list<std::uint64_t> press_times(race_time);
        std::iota(press_times.begin(), press_times.end(), 0);

        std::vector<bool> is_not_new_record{};
        is_not_new_record.reserve(press_times.size());

        std::transform(press_times.begin(),
                       press_times.end(),
                       std::back_inserter(is_not_new_record),
                       [=](const auto& time_press) -> bool {
                           return ((time_press * (race_time - time_press)) <= distance_to_beat);
                       });

        auto number_of_record_races = std::count_if(is_not_new_record.cbegin(),
                                                    is_not_new_record.cend(),
                                                    [](bool const& is_not_record) { return !is_not_record; });

        result *= static_cast<std::uint64_t>(number_of_record_races);
    }

    return result;
}

int main()
{
    std::cout << "Result: " << solve_problem(parse_race_data(std::ifstream("input"))) << "\n";

    return 0;
}