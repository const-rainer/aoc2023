#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <map>

int ToInt(const char c)
{
    return c - '0';
}

void PreprocessLine(std::string& line)
{
    for(int i=0; i<line.size(); i++)
    {
        switch(line.at(i))
        {
        case 'o':
            if(line.substr(i, 3) == "one")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "1", 1);
            }
            break;
        
        case 't':
            if(line.substr(i, 3) == "two")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "2", 1);
            }
            else if(line.substr(i, 5) == "three")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "3", 1);
            }
            break;
        
        case 'f':
            if(line.substr(i, 4) == "four")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "4", 1);
            }
            else if(line.substr(i, 4) == "five")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "5", 1);
            }
            break;
        
        case 's':
            if(line.substr(i, 3) == "six")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "6", 1);
            }
            else if(line.substr(i, 5) == "seven")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "7", 1);
            }

            break;
        
        case 'e':
            if(line.substr(i, 5) == "eight")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "8", 1);
            }
            break;
        
        case 'n':
            if(line.substr(i, 4) == "nine")
            {
                line.erase(i+1, 1);
                line.insert(i+1, "9", 1);
            }
            break;
        }
    }
}

int ParseNumber(std::string& line)
{
    int first_digit = -1;
    int last_digit = -1;

    for(auto it = line.cbegin(); it != line.cend(); ++it)
    {
        if(std::isdigit(*it))
        {
            if(first_digit == -1)
            {
                first_digit = ToInt(*it);
            }
            
            last_digit = ToInt(*it);
        }
    }
    
    std::cout << "first digit: " << first_digit << "\n";
    std::cout << "last digit: " << last_digit << "\n";
    
    std::cout << "return value: " << 10*first_digit + last_digit << "\n\n";
    
    return 10*first_digit + last_digit;
}

int GetNumberFromLine(std::string& line)
{
    std::cout << "Input line: " << line << "\n";
    
    PreprocessLine(line);   
    std::cout << "Preprocessed line: " << line << "\n";
    
    return ParseNumber(line);
}

int main()
{
    //std::string input_file_name = "test_input";
    std::string input_file_name = "input";

    std::ifstream input_file{input_file_name};
    
    int total = 0;
    int count = 0;
    
    std::string line;
    while(std::getline(input_file, line))
    {
        total += GetNumberFromLine(line);
        count++;
    }
    
    std::cout << "Number of lines processed: " << count << "\n";
    std::cout << "Total: " << total << "\n";
    
    return 0;
}
