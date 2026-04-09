#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <sys/time.h>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(PmergeMe const &src) : vec(src.vec), deq(src.deq) {}

PmergeMe &PmergeMe::operator=(PmergeMe const &rhs)
{
	if (this != &rhs)
	{
		vec = rhs.vec;
		deq = rhs.deq;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::parseArgs(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
	{
		std::string s(argv[i]);
		if (s.empty())
			throw std::runtime_error("Error - empty input");
		for (size_t j = 0; j < s.length(); j++)
		{
			if (s[j] < '0' || s[j] > '9')
				throw std::runtime_error("Error - numbers exclusively expected");
		}
		long n;
		std::stringstream ss(s);
		ss >> n;
		if (ss.fail() || !ss.eof() || n > 2147483647L)
			throw std::runtime_error("Error");
		vec.push_back(static_cast<int>(n));
		deq.push_back(static_cast<int>(n));
	}
	if (vec.empty())
		throw std::runtime_error("Error");
}

static double getTimeUs()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

// Generates insertion order for pend elements using Jacobsthal boundaries.
// Pend elements correspond to b_2, b_3, ..., b_{n+1} in Ford-Johnson notation.
// Returns 0-indexed positions into the pend array.
std::vector<int> PmergeMe::getJacobsthalOrder(int n)
{
	std::vector<int> order;
	if (n <= 0)
		return order;

	// Jacobsthal boundary values: t_1=1, t_2=3, t_3=5, t_4=11, ...
	// Recurrence: t_k = t_{k-1} + 2 * t_{k-2}
	std::vector<int> jacob;
	jacob.push_back(1);
	jacob.push_back(3);
	while (jacob.back() < n + 1)
	{
		size_t s = jacob.size();
		jacob.push_back(jacob[s - 1] + 2 * jacob[s - 2]);
	}

	std::vector<bool> added(n, false);
	for (size_t k = 1; k < jacob.size(); k++)
	{
		int upper = jacob[k];
		if (upper > n + 1)
			upper = n + 1;
		int lower = jacob[k - 1] + 1;
		for (int b = upper; b >= lower; b--)
		{
			int idx = b - 2;
			if (idx >= 0 && idx < n && !added[idx])
			{
				order.push_back(idx);
				added[idx] = true;
			}
		}
		if (jacob[k] >= n + 1)
			break;
	}
	for (int i = 0; i < n; i++)
	{
		if (!added[i])
			order.push_back(i);
	}
	return order;
}

// Ford-Johnson merge-insertion sort for std::vector
void PmergeMe::sortVector(std::vector<int> &arr)
{
	if (arr.size() <= 1)
		return;

	bool hasStraggler = (arr.size() % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
		straggler = arr.back();

	// Pair adjacent elements, larger first
	std::vector<std::pair<int, int> > pairs;
	for (size_t i = 0; i + 1 < arr.size(); i += 2)
	{
		if (arr[i] > arr[i + 1])
			pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
		else
			pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
	}

	// Recursively sort winners
	std::vector<int> winners;
	for (size_t i = 0; i < pairs.size(); i++)
		winners.push_back(pairs[i].first);
	sortVector(winners);

	// Rebuild losers to match sorted winner order
	std::vector<bool> used(pairs.size(), false);
	std::vector<int> losers;
	for (size_t i = 0; i < winners.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].first == winners[i])
			{
				losers.push_back(pairs[j].second);
				used[j] = true;
				break;
			}
		}
	}

	// Main chain: first loser (< first winner) + all sorted winners
	std::vector<int> chain;
	chain.push_back(losers[0]);
	for (size_t i = 0; i < winners.size(); i++)
		chain.push_back(winners[i]);

	// Pend: remaining losers
	std::vector<int> pend;
	for (size_t i = 1; i < losers.size(); i++)
		pend.push_back(losers[i]);

	// Insert pend in Jacobsthal order
	std::vector<int> order = getJacobsthalOrder(pend.size());
	for (size_t i = 0; i < order.size(); i++)
	{
		int val = pend[order[i]];
		std::vector<int>::iterator pos =
			std::lower_bound(chain.begin(), chain.end(), val);
		chain.insert(pos, val);
	}

	// Insert straggler last
	if (hasStraggler)
	{
		std::vector<int>::iterator pos =
			std::lower_bound(chain.begin(), chain.end(), straggler);
		chain.insert(pos, straggler);
	}

	arr = chain;
}

// Ford-Johnson merge-insertion sort for std::deque
void PmergeMe::sortDeque(std::deque<int> &arr)
{
	if (arr.size() <= 1)
		return;

	bool hasStraggler = (arr.size() % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
		straggler = arr.back();

	std::deque<std::pair<int, int> > pairs;
	for (size_t i = 0; i + 1 < arr.size(); i += 2)
	{
		if (arr[i] > arr[i + 1])
			pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
		else
			pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
	}

	std::deque<int> winners;
	for (size_t i = 0; i < pairs.size(); i++)
		winners.push_back(pairs[i].first);
	sortDeque(winners);

	std::deque<bool> used(pairs.size(), false);
	std::deque<int> losers;
	for (size_t i = 0; i < winners.size(); i++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (!used[j] && pairs[j].first == winners[i])
			{
				losers.push_back(pairs[j].second);
				used[j] = true;
				break;
			}
		}
	}

	std::deque<int> chain;
	chain.push_back(losers[0]);
	for (size_t i = 0; i < winners.size(); i++)
		chain.push_back(winners[i]);

	std::deque<int> pend;
	for (size_t i = 1; i < losers.size(); i++)
		pend.push_back(losers[i]);

	std::vector<int> order = getJacobsthalOrder(pend.size());
	for (size_t i = 0; i < order.size(); i++)
	{
		int val = pend[order[i]];
		std::deque<int>::iterator pos =
			std::lower_bound(chain.begin(), chain.end(), val);
		chain.insert(pos, val);
	}

	if (hasStraggler)
	{
		std::deque<int>::iterator pos =
			std::lower_bound(chain.begin(), chain.end(), straggler);
		chain.insert(pos, straggler);
	}

	arr = chain;
}

void PmergeMe::run(int argc, char **argv)
{
	parseArgs(argc, argv);

	std::cout << "Before:";
	for (size_t i = 0; i < vec.size(); i++)
		std::cout << " " << vec[i];
	std::cout << std::endl;

	// Time includes data management (copying unsorted input) + sorting
	std::vector<int> unsortedVec(vec);
	std::deque<int> unsortedDeq(deq);

	double t1 = getTimeUs();
	vec = unsortedVec;
	sortVector(vec);
	double t2 = getTimeUs();

	double t3 = getTimeUs();
	deq = unsortedDeq;
	sortDeque(deq);
	double t4 = getTimeUs();

	std::cout << "After:";
	for (size_t i = 0; i < vec.size(); i++)
		std::cout << " " << vec[i];
	std::cout << std::endl;

	std::cout << "Time to process a range of " << vec.size()
		<< " elements with std::vector : " << (t2 - t1) << " us" << std::endl;
	std::cout << "Time to process a range of " << deq.size()
		<< " elements with std::deque  : " << (t4 - t3) << " us" << std::endl;

	// --- Comment out the block below to disable std::sort / std::stable_sort comparison ---
	// /*
	{
		std::vector<int> vecCopy(vec.begin(), vec.end());
		std::deque<int> deqCopy(deq.begin(), deq.end());

		// Re-parse unsorted input for fair comparison
		vecCopy.clear();
		deqCopy.clear();
		for (int i = 1; i < argc; i++)
		{
			std::stringstream ss(argv[i]);
			int n;
			ss >> n;
			vecCopy.push_back(n);
			deqCopy.push_back(n);
		}

		std::vector<int> vecSort(vecCopy);
		double t5 = getTimeUs();
		std::sort(vecSort.begin(), vecSort.end());
		double t6 = getTimeUs();

		std::vector<int> vecStable(vecCopy);
		double t7 = getTimeUs();
		std::stable_sort(vecStable.begin(), vecStable.end());
		double t8 = getTimeUs();

		std::deque<int> deqSort(deqCopy);
		double t9 = getTimeUs();
		std::sort(deqSort.begin(), deqSort.end());
		double t10 = getTimeUs();

		std::deque<int> deqStable(deqCopy);
		double t11 = getTimeUs();
		std::stable_sort(deqStable.begin(), deqStable.end());
		double t12 = getTimeUs();

		std::cout << "Time to process a range of " << vecSort.size()
			<< " elements with std::sort(vector)         : " << (t6 - t5) << " us" << std::endl;
		std::cout << "Time to process a range of " << deqSort.size()
			<< " elements with std::sort(deque)           : " << (t10 - t9) << " us" << std::endl;
		std::cout << "Time to process a range of " << vecStable.size()
			<< " elements with std::stable_sort(vector)   : " << (t8 - t7) << " us" << std::endl;
		std::cout << "Time to process a range of " << deqStable.size()
			<< " elements with std::stable_sort(deque)    : " << (t12 - t11) << " us" << std::endl;
	}
	// */
}
