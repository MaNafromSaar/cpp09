#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <vector>
# include <deque>
# include <string>

class PmergeMe
{
public:
	PmergeMe();
	PmergeMe(PmergeMe const &src);
	PmergeMe &operator=(PmergeMe const &rhs);
	~PmergeMe();

	void run(int argc, char **argv);

private:
	std::vector<int> vec;
	std::deque<int> deq;

	void parseArgs(int argc, char **argv);
	static void sortVector(std::vector<int> &arr);
	static void sortDeque(std::deque<int> &arr);
	static std::vector<int> getJacobsthalOrder(int n);
};

#endif
