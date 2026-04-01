# cpp09 — STL

42 C++ Module 09: Using the Standard Template Library — containers and algorithms.

## Exercises

| # | Program | Task | Container | Why |
|---|---------|------|-----------|-----|
| 00 | `btc` | Bitcoin price lookup by date | `std::map` | Sorted keys + `lower_bound()` for nearest-date lookup |
| 01 | `RPN` | Reverse Polish Notation calculator | `std::stack` | LIFO — push operands, pop for each operator |
| 02 | `PmergeMe` | Ford-Johnson merge-insertion sort | `std::vector` + `std::deque` | Random access needed; two containers required for timing comparison |

Each container used in one exercise is **banned** in all subsequent exercises.
