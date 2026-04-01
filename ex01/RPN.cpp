#include "RPN.hpp"
#include <sstream>
#include <stdexcept>

RPN::RPN() {}

RPN::RPN(RPN const &src) : stack(src.stack) {}

RPN &RPN::operator=(RPN const &rhs)
{
	if (this != &rhs)
		stack = rhs.stack;
	return *this;
}

RPN::~RPN() {}

bool	RPN::isOperator(char c) const
{
	return c == '+' || c == '-' || c == '*' || c == '/';
}

int	RPN::applyOp(int a, int b, char op) const
{
	if (op == '+') return a + b;
	if (op == '-') return a - b;
	if (op == '*') return a * b;
	if (op == '/')
	{
		if (b == 0)
			throw std::runtime_error("Error: division by zero");
		return a / b;
	}
	throw std::runtime_error("Error: unknown operator");
}

int	RPN::evaluate(std::string const &expr)
{
	std::stringstream ss(expr);
	std::string token;

	while (ss >> token)
	{
		if (token.length() == 1 && isOperator(token[0]))
		{
			if (stack.size() < 2)
				throw std::runtime_error("Error");
			int b = stack.top(); stack.pop();
			int a = stack.top(); stack.pop();
			stack.push(applyOp(a, b, token[0]));
		}
		else if (token.length() == 1 && token[0] >= '0' && token[0] <= '9')
		{
			stack.push(token[0] - '0');
		}
		else
		{
			throw std::runtime_error("Error");
		}
	}
	if (stack.size() != 1)
		throw std::runtime_error("Error");
	int result = stack.top();
	stack.pop();
	return result;
}
