#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_set>
#include <utility>

class scratchcard
{
  public:
    explicit scratchcard(int i, std::unordered_set<int> const& n, std::unordered_set<int> const& w)
    {
        id = i;
        numbers = n;
        winners = w;
        points = 0;
        matching_numbers = 0;
    }

    inline constexpr int get_id() const noexcept { return id; }
    inline constexpr int get_matching_numbers() const noexcept { return matching_numbers; }

    int get_points() noexcept
    {
        if (points == 0)
        {
            auto check_winner = [this](int const& number) {
                if (winners.find(number) != winners.cend())
                {
                    if (points == 0)
                        points = 1;

                    else
                        points *= 2;

                    matching_numbers++;
                    std::cout << "Found a match in card " << id << ". Matching numbers = " << matching_numbers << "\n";
                }
            };

            std::for_each(numbers.cbegin(), numbers.cend(), check_winner);
        }

        return points;
    }

  private:
    int id;
    std::unordered_set<int> numbers;
    std::unordered_set<int> winners;
    int points;
    int matching_numbers;
};

struct scratchcard_parser
{
  public:
    int parse_id(std::string const& input_string)
    {
        auto right_delimiter = input_string.find(":");
        auto id_string = input_string.substr(0, right_delimiter);
        std::regex regex("[0-9]{1,3}");
        std::smatch match;

        std::regex_search(id_string, match, regex);

        return std::atoi(match[0].str().c_str());
    }

    std::unordered_set<int> parse_numbers(std::string const& input_string)
    {
        auto numbers_string = get_numbers_string(input_string);

        std::unordered_set<int> numbers;

        std::regex regex("[0-9]{1,2}");
        for (std::sregex_iterator i = std::sregex_iterator(numbers_string.begin(), numbers_string.end(), regex);
             i != std::sregex_iterator();
             ++i)
        {
            std::smatch n = *i;
            numbers.insert(std::atoi(n.str().c_str()));
        }

        return numbers;
    }

    std::unordered_set<int> parse_winners(std::string const& input_string)
    {
        auto winners_string = get_winners_string(input_string);

        std::unordered_set<int> winners;

        std::regex regex("[0-9]{1,2}");
        for (std::sregex_iterator i = std::sregex_iterator(winners_string.begin(), winners_string.end(), regex);
             i != std::sregex_iterator();
             ++i)
        {
            std::smatch n = *i;
            winners.insert(std::atoi(n.str().c_str()));
        }

        return winners;
    }

  private:
    std::string get_numbers_string(std::string const& input_string)
    {
        auto left_delimiter = input_string.find(":");
        auto right_delimiter = input_string.find("|");
        auto n_string_length = right_delimiter - left_delimiter;
        auto numbers_string = input_string.substr(left_delimiter + 2, n_string_length - 2);

        return numbers_string;
    }

    std::string get_winners_string(std::string const& input_string)
    {
        auto left_delimiter = input_string.find("|");
        auto w_string_length = input_string.size() - left_delimiter;
        auto winners_string = input_string.substr(left_delimiter + 2, w_string_length);

        return winners_string;
    }
};

scratchcard make_scratchcard(std::string const& input_string)
{
    scratchcard_parser parser{};

    auto id = parser.parse_id(input_string);
    auto numbers = parser.parse_numbers(input_string);
    auto winners = parser.parse_winners(input_string);

    return scratchcard{id, numbers, winners};
}

int get_scratchcards_amount(std::map<int, std::pair<scratchcard, int>>& cards_record)
{
    int total_amount{0};

    for (int i = 1; i <= cards_record.size(); i++)
    {
        auto& scratchcard = cards_record.at(i).first;
        auto matching_numbers = scratchcard.get_matching_numbers();
        auto card_id = scratchcard.get_id();

        for (int i = 1; i <= matching_numbers; i++)
        {
            cards_record.at(card_id + i).second += cards_record.at(card_id).second;
        }

        total_amount += cards_record.at(card_id).second;
    }

    return total_amount;
}

int main()
{
    std::map<int, std::pair<scratchcard, int>> cards_record{};

    int total_points{0};
    int count{0};
    std::string line{};
    std::ifstream input{"input"};

    while (std::getline(input, line))
    {
        auto card = make_scratchcard(line);
        total_points += card.get_points();
        cards_record.insert({card.get_id(), {card, 1}});

        count++;
    }

    std::cout << "Scatchcards parsed: " << count << "\n";
    std::cout << "Total: " << total_points << "\n";

    auto total_cards_amount = get_scratchcards_amount(cards_record);
    std::cout << "Final amount of scratchcards: " << total_cards_amount << "\n";
}
