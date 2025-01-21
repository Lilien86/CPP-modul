#include "RPN.hpp"

RPN::RPN() {}
RPN::~RPN() {}

RPN::RPN(const RPN &other)
{
	*this = other;
}

RPN		&RPN::operator=(const RPN &rhs)
{
	if (this != &rhs) {
		this->_stack = rhs._stack;
	}
	return *this;
}

int		RPN::getResult() const
{
	return this->_stack.top();
}

RPN::RPN(const std::string &str)
{
	std::istringstream	tokens(str);
	std::string			token;

	while (tokens >>token) {
		if (isdigit(token[0])) {
			this->_stack.push(token[0] - '0');
		} else if (isOperator(token)) {
			if (this->_stack.size() < 2) {
				throw std::invalid_argument("Error: Invalid expression");
			}
			int b = this->_stack.top(); this->_stack.pop();
			int a = this->_stack.top(); this->_stack.pop();
			this->_stack.push(applyOperator(a, b, token[0]));
		} else {
			throw std::invalid_argument("Error: Invalid token");
		}
	}
	if (this->_stack.size() != 1) {
		throw std::invalid_argument("Error: Invalid expression");
	}

	this->result = this->_stack.top();
}

bool RPN::isOperator(const std::string& token)
{
	if (token.size() != 1) {
		return false;
	}

	switch (token[0]) {
		case '+':
		case '-':
		case '*':
		case '/':
			return true;
		default:
			return false;
	}
}


int		RPN::applyOperator(int a, int b, const char &op) {
	switch (op) {
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': 
			if (b == 0) throw std::invalid_argument("Error: Division by zero");
			return a / b;
		default: throw std::invalid_argument("Error: Unknown operator");
	}
}