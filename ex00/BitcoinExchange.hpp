#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <string>
# include <map>

class BitcoinExchange
{
public:
	BitcoinExchange();
	BitcoinExchange(BitcoinExchange const &src);
	BitcoinExchange &operator=(BitcoinExchange const &rhs);
	~BitcoinExchange();

	void	loadDatabase(std::string const &filename);
	void	processInput(std::string const &filename);

private:
	std::map<std::string, float>	data;

	bool	isValidDate(std::string const &date) const;
	float	getRate(std::string const &date) const;
};

#endif
