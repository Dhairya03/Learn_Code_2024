#include <iostream>
#include <unordered_map>
#include <string>
#include <limits>

bool isValidUserChoice();

void handleCountryCodeInput();

std::string getCountryCodeInput();

bool isValidCountryCodeInput(std::string enteredCountryCode);

void findCountryNameByCode(std::string enteredCountryCode, std::unordered_map<std::string, std::string> countryCodeNames);

int main()
{
    std::unordered_map<std::string, std::string> countryCodeAndNames = {
        {"IN", "India"},
        {"US", "United States of America"},
        {"NZ", "New Zealand"},
        {"CA", "Canada"},
        {"CN", "China"},
        {"AU", "Australia"},
        {"RU", "Russia"},
        {"BR", "Brazil"},
        {"FR", "France"},
        {"DE", "Germany"},
        {"GB", "United Kingdom"},
        {"IT", "Italy"},
        {"JP", "Japan"},
        {"KR", "South Korea"},
        {"MX", "Mexico"},
        {"AR", "Argentina"},
        {"ZA", "South Africa"},
        {"ES", "Spain"},
        {"SE", "Sweden"},
        {"NO", "Norway"},
        {"FI", "Finland"},
        {"IN", "India"},
        {"GR", "Greece"},
        {"PL", "Poland"}};

    int userChoice;

    do
    {
        std::cout << "1. Find Country by Country Code\n";
        std::cout << "2. Exit\n";
        std::cout << "Enter your choice (1 or 2): ";
        std::cin >> userChoice;

        if (isValidUserChoice())
        {
            switch (userChoice)
            {
            case 1:
                handleCountryCodeInput();
                break;
            case 2:
                std::cout << "Exiting program." << std::endl;
                break;
            default:
                std::cout << "Invalid choice! Please enter 1 or 2." << std::endl;
                break;
            }
        }
        else
            std::cout << "Invalid Input!!! Please Enter Valid Integer" << std::endl;
    } while (userChoice != 2);
    return 0;
}

bool isValidUserChoice()
{
    bool isValidInput = false;
    if (std::cin.fail() || std::cin.peek() != '\n')
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
        isValidInput = true;
    return isValidInput;
}

void handleCountryCodeInput(){
    std::string countryCode = getCountryCodeInput();
    if (!isValidCountryCodeInput(countryCode))
    {
        std::cout << "Invalid input!!! Enter valid country code like(IN, NZ etc.)" << std::endl;
    }
    else
    {
        findCountryNameByCode(countryCode, countryCodeAndNames);
    }
}

std::string getCountryCodeInput()
{
    std::string enteredCountryCode = "";
    std::cout << "Enter the country code" << std::endl;
    std::cin >> enteredCountryCode;
    std::cout << "Entered country code is: " << enteredCountryCode << std::endl;
    return enteredCountryCode;
}

bool isValidCountryCodeInput(std::string enteredCountryCode)
{
    for (char &character : enteredCountryCode)
    {
        if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void findCountryNameByCode(std::string enteredCountryCode, std::unordered_map<std::string, std::string> countryCodeNames)
{
    for (char &character : enteredCountryCode)
    {
        character = toupper(character);
    }

    if (countryCodeNames.find(enteredCountryCode) == countryCodeNames.end())
    {
        std::cout << "Country Code not found" << std::endl;
    }
    else
    {
        std::cout << "Country Name is: " << countryCodeNames.at(enteredCountryCode) << std::endl;
    }
}
