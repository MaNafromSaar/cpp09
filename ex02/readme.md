# Exercise 02 — PmergeMe (Ford-Johnson Merge-Insertion Sort)

**Program:** `PmergeMe`
**Containers:** two different ones, e.g. `std::vector<int>` + `std::deque<int>`
**Files:** `Makefile`, `main.cpp`, `PmergeMe.cpp`, `PmergeMe.hpp`

---

## What it does

Sorts a positive integer sequence using the **Ford-Johnson merge-insertion sort**
algorithm, implemented with **two different containers**, and compares their
performance.

---

## Expected outputs from subject

```
$> ./PmergeMe 3 5 9 7 4
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : 0.00031 us
Time to process a range of 5 elements with std::deque  : 0.00014 us

$> ./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
Before: 141 79 526 321 [...]
After:  79 141 321 526 [...]
Time to process a range of 3000 elements with std::vector : 62.14389 us
Time to process a range of 3000 elements with std::deque  : 69.27212 us

$> ./PmergeMe "-1" "2"
Error
```

---

## Ford-Johnson Merge-Insertion Sort — Visual Overview

### The idea

Minimize the number of **comparisons** needed to sort n elements.
Combines **merge** (pairing) with **binary insertion** in an optimal order.

### Step-by-step with example: `[5, 3, 9, 7, 4]`

---

#### Step 1: Make pairs and find the larger of each pair

```
Input:  5  3  9  7  4
        ────  ────  ─
        pair  pair  straggler (odd element out)

Compare within pairs:
  (5, 3) → winner: 5,  loser: 3
  (9, 7) → winner: 9,  loser: 7

Straggler: 4
```

---

#### Step 2: Recursively sort the winners

```
Winners: [5, 9]

Sort winners → [5, 9]      (recursive call — base case for 2 elements)

Carry losers along with their paired winners:
  5 ←→ 3
  9 ←→ 7
```

---

#### Step 3: Build the sorted chain and the pend list

```
Sorted chain (S):   [5, 9]        ← the sorted winners

Pend list (losers): [3, 7]        ← each paired with its winner
                     ↕  ↕
                     5  9

We know: each loser < its paired winner
So 3 < 5 already → insert 3 at the front:

S = [3, 5, 9]
Remaining pend: [7]
Straggler: [4]
```

---

#### Step 4: Insert pend elements using binary search (Jacobsthal order)

The Jacobsthal sequence determines the **optimal insertion order**
to minimize comparisons:

```
Jacobsthal numbers:  1, 1, 3, 5, 11, 21, 43, ...
Insertion order:     For k pend elements, insert at indices
                     derived from Jacobsthal to exploit the
                     binary search bound.

With our small example, we just insert the remaining elements:

Insert 7 into S = [3, 5, 9]:
  Binary search range: [3, 5] (we know 7 < 9)
  7 > 5 → insert after 5
  S = [3, 5, 7, 9]

Insert straggler 4 into S = [3, 5, 7, 9]:
  Binary search the whole chain
  4 > 3, 4 < 5 → insert between 3 and 5
  S = [3, 4, 5, 7, 9]  ✓
```

---

### Why Jacobsthal order matters

```
Jacobsthal:  0, 1, 1, 3, 5, 11, 21, 43, 85, ...
             J(n) = J(n-1) + 2·J(n-2)

Insertion order for pend elements b₁, b₂, b₃, b₄, b₅, ...

Group by Jacobsthal boundaries, insert RIGHT-TO-LEFT within each group:

  group 1:  b₁
  group 2:  b₃, b₂           ← insert b₃ first, then b₂
  group 3:  b₅, b₄           ← insert b₅ first, then b₄
  group 4:  b₁₁, b₁₀, ... b₆

This ordering guarantees each insertion's binary search
operates on the smallest possible range (power of 2 - 1),
minimizing total comparisons.
```

---

### Comparison count

Ford-Johnson achieves near-optimal comparisons:

```
n elements → ⌈log₂(n!)⌉ comparisons (information-theoretic lower bound)

Ford-Johnson matches this bound for small n and comes very
close for larger n. It was the comparison-optimal algorithm
for decades.
```

---

## Output format

```
Before: <unsorted sequence>
After:  <sorted sequence>
Time to process a range of N elements with std::vector : X.XXXXX us
Time to process a range of N elements with std::deque  : X.XXXXX us
```

- Time includes **both** data management and sorting
- Precision must clearly show the difference between containers
- Use `clock()`, `gettimeofday()`, or similar C++98-compatible timing

---

## Rules recap

- Input: positive integers only (no negatives, no non-numeric)
- Must handle at least **3000** elements
- Must use **two different containers** (not map, not stack — used in ex00/ex01)
- Implement the algorithm **separately for each container** (no generic function)
- Duplicates: handling left to your discretion
- Errors → `std::cerr`
