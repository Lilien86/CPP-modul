#include "BitcoinExchange.hpp"

bool is_digit(const std::string& str) {
	if (str.empty()) return false;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}

BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &rhs)
{
	if (this != &rhs) {
		this->_dataBase = rhs._dataBase;
	}
	return (*this);
}

double string_to_double(const std::string& str) {
	std::stringstream ss(str);
	double result;
	ss >> result;
	if (ss.fail()) {
		throw std::runtime_error("Conversion to double failed: " + str);
	}
	return result;
}

BitcoinExchange::~BitcoinExchange() {}

std::map<std::string, double> &BitcoinExchange::getDataBase() {
	return this->_dataBase;
}


std::ostream& operator<<( std::ostream &os, BitcoinExchange &bit)
{
	std::map<std::string, double>::iterator it = bit.getDataBase().begin();

	while (it != bit.getDataBase().end()) {
		os << it->first
			 << " -> " << it->second << std::endl;
		++it;
	}
	return os;
}

BitcoinExchange::BitcoinExchange(const std::string &file)
{
	std::ifstream ifs;
	std::string line;
	try {
		ifs.open(file.c_str());
		if (ifs.fail())
			throw std::runtime_error("Failed to open file");
	} catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		std::cerr << file << " was not successfully opened. Please check that the file currently exists." << std::endl;
		return ;
	}
	std::getline(ifs, line);
	while (std::getline(ifs, line)) {
		size_t pos_sep = line.find(',');
		if (pos_sep == std::string::npos) {
			std::cerr << "Error: Invalid format in database => " << line << '\n';
			continue;
		}
		std::string date = line.substr(0, pos_sep);
		std::string rate_s = line.substr(pos_sep + 1);

		double rate = string_to_double(rate_s);
		this->_dataBase[date] = rate;
	}

}

bool	BitcoinExchange::validNumber(const double &nb)
{
	if (nb < 0 || nb > 1000) {
		return false;
	}
	return true;
}

bool	BitcoinExchange::validDate(const std::string &date)
{
	size_t	sep1 = date.find('-');
	size_t	sep2 = date.rfind('-');

	if (sep1 == std::string::npos || sep1 == sep2)
		return false;

	std::string year_str = date.substr(0, sep1);
	std::string month_str = date.substr(sep1 + 1, sep2 - sep1 - 1);
	std::string day_str = date.substr(sep2 + 1);
	
	int year, month, day;
	std::istringstream year_ss(year_str), month_ss(month_str), day_ss(day_str);
	
	if (!(year_ss >> year) || !(month_ss >> month) || !(day_ss >> day))
		return false;

	if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day > 30)
			return false;
	}

	if (month == 2) {
		bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
		if ((isLeapYear && day > 29) || (!isLeapYear && day > 28)) {
			return false;
		}
	}
	return true;
}

double BitcoinExchange::getRate(const std::string &date)
{
	std::map<std::string, double>::iterator it = this->_dataBase.lower_bound(date);

	if (it->first != date && it != this->_dataBase.begin()) {
		--it;
	}
	if (it != this->_dataBase.end()) {
		return it->second;
	} else if (!this->_dataBase.empty()) {
		return it->second;
	}
	return -1.0;
}


void	BitcoinExchange::processInfile(const std::string &infile)
{
	std::ifstream ifs;
	std::string line;
	try {
		ifs.open(infile.c_str());
		if (ifs.fail())
			throw std::runtime_error("Failed to open infile");
	} catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		std::cerr << infile << " was not successfully opened. Please check that the infile currently exists." << std::endl;
		return ;
	}
	std::getline(ifs, line);
	while (std::getline(ifs, line)) {
		std::istringstream ss(line);
		std::string date;
		double value;
		char delimiter;

		if (!(ss >> date >> delimiter >> value)) {
			std::cerr << "Error: Unable to parse line => " << line << std::endl;
			continue ;
		}

		if (delimiter != '|') {
			std::cerr << "Error: Expected '|' delimiter in line => " << line << std::endl;
			continue ;
		}

		if (!validDate(date)) {
			std::cerr << "Error: Invalid date format in line => " << line << std::endl;
			continue ;
		}

		if (date < this->_dataBase.begin()->first || date > this->_dataBase.rbegin()->first) {
			std::cerr << "Error: Date " << date << " is out of range." << std::endl;
			continue ;
		}

		if (!validNumber(value)) {
			std::cerr << "Error: Value " << value << " is too large or too small number." << std::endl;
			continue ;
		}
		double exchangeRate = getRate(date);
		if (exchangeRate == -1.0) {
			std::cerr << "Error: No exchange rate available for date => " << date << std::endl;
			continue;
		}

		std::cout << date << " => "  << value << " = " << value * exchangeRate << std::endl;
	}

}
