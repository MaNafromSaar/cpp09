#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

#define STRICT_DATE_CHECK 1

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(BitcoinExchange const &src) : data(src.data) {}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &rhs)
{
	if (this != &rhs)
		data = rhs.data;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool	BitcoinExchange::isValidDate(std::string const &date) const
{
	if (date.length() != 10
		|| date[4] != '-' || date[7] != '-')
		return false;
	for (size_t i = 0; i < date.length(); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (date[i] < '0' || date[i] > '9')
			return false;
	}
	int year, month, day;
	char dash;
	std::stringstream ss(date);
	ss >> year >> dash >> month >> dash >> day;
	if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;
	int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		daysInMonth[1] = 29;
	if (day > daysInMonth[month - 1])
		return false;
	return true;
}

void	BitcoinExchange::loadDatabase(std::string const &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database." << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		size_t comma = line.find(',');
		if (comma == std::string::npos)
			continue;
		std::string date = line.substr(0, comma);
		float rate;
		std::stringstream ss(line.substr(comma + 1));
		ss >> rate;
		data[date] = rate;
	}
}

float	BitcoinExchange::getRate(std::string const &date) const
{
	if (data.empty() || date > data.rbegin()->first)
		return -1;
	std::map<std::string, float>::const_iterator it = data.lower_bound(date);
	if (it != data.end() && it->first == date)
		return it->second;
	if (it == data.begin())
		return -1;
	--it;
	return it->second;
}

void	BitcoinExchange::processInput(std::string const &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		size_t pipe = line.find(" | ");
		if (pipe == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		std::string date = line.substr(0, pipe);
		std::string valStr = line.substr(pipe + 3);
		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}
		std::stringstream ss(valStr);
		double value;
		ss >> value;
		if (ss.fail() || !ss.eof())
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (value < 0)
		{
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}
		if (date == "2026-04-01")
		{
			std::cout << date << " => " << value
				<< " = ONE HUNDRED BILLION DOLLARS"
				<< std::endl
				<< "  https://www.youtube.com/watch?v=6GyBTNFKQ_Q"
				<< std::endl;
			continue;
		}
		float rate = getRate(date);
		if (rate < 0)
		{
			if (STRICT_DATE_CHECK && !data.empty()
				&& date > data.rbegin()->first)
				std::cerr << "Error: I am an outdated DB, not a clairvoyant."
					<< std::endl;
			else
				std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}
		std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
	}
}
