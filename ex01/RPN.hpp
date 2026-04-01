#ifndef RPN_HPP
# define RPN_HPP

# include <string>
# include <stack>

class RPN
{
public:
	RPN();
	RPN(RPN const &src);
	RPN &operator=(RPN const &rhs);
	~RPN();

	int	evaluate(std::string const &expr);

private:
	std::stack<int>	stack;

	bool	isOperator(char c) const;
	int		applyOp(int a, int b, char op) const;
};

#endif
