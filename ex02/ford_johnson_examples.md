# Ford-Johnson: Two Worked Examples

## Example 1: Jacobsthal is Overhead (4 elements)

**Input:** `[8, 2, 5, 1]`

### Step 1 — Pair & compare

```
(8, 2) → winner: 8   loser: 2
(5, 1) → winner: 5   loser: 1

No straggler (even count).
                                          comparisons: 2
```

### Step 2 — Recursively sort winners

```
Winners: [8, 5]  →  [5, 8]    (1 comparison: 5 < 8)

Matched pairs after sort:
  a1=5 ↔ b1=1
  a2=8 ↔ b2=2
                                          comparisons: 1
```

### Step 3 — Build chain, insert b1

```
b1=1 < a1=5 guaranteed → insert at front:
Chain:  [1, 5, 8]

Pend:   [b2=2]    ← just one element
```

### Step 4 — Insert pend via Jacobsthal order

```
Pend has 1 element. Jacobsthal machinery fires up:

  Compute boundaries: t₁=1, t₂=3, t₃=5, t₄=11, ...
  Group 1: b-values from t₂=3 down to t₁+1=2 → just b₂
  Mark b₂ as added. Check remaining: none.
  Result: insertion order = [b₂]

    ┌─────────────────────────────────────────────┐
    │  All that computation to produce [b₂].      │
    │  We could have just... inserted b₂.         │
    │  With 1 pend element, the answer is always  │
    │  trivially "insert the one element."         │
    └─────────────────────────────────────────────┘

Insert b₂=2 into chain [1, 5, 8]:
  We know b₂ < a₂=8, so search window: [1, 5]

  Compare: 2 > 1 → search [5]
  Compare: 2 < 5 → insert before 5
  Chain: [1, 2, 5, 8]  ✓
                                          comparisons: 2
```

### Total: 5 comparisons

Any insertion strategy gives exactly 5 comparisons here.
Jacobsthal computed boundaries, iterated groups in reverse, tracked
a boolean array — all to produce the same trivial answer.

For ≤5 input elements (≤2 pairs → ≤1 pend element), this is always
the case. The algorithm is correct but the optimization has nothing
to optimize.

---

## Example 2: Jacobsthal Earns its Keep (12 elements)

**Input:** `[7, 1, 11, 5, 9, 3, 12, 2, 10, 6, 8, 4]`

### Step 1 — Pair & compare

```
(7,  1)  → winner:  7   loser: 1
(11, 5)  → winner: 11   loser: 5
(9,  3)  → winner:  9   loser: 3
(12, 2)  → winner: 12   loser: 2
(10, 6)  → winner: 10   loser: 6
(8,  4)  → winner:  8   loser: 4

No straggler (even count).
                                          comparisons: 6
```

### Step 2 — Recursively sort winners

```
Winners: [7, 11, 9, 12, 10, 8]

After recursive Ford-Johnson:
Sorted:  [7, 8, 9, 10, 11, 12]

Matched pairs (loser tracks its winner):
  a1= 7 ↔ b1=1
  a2= 8 ↔ b2=4
  a3= 9 ↔ b3=3
  a4=10 ↔ b4=6
  a5=11 ↔ b5=5
  a6=12 ↔ b6=2
                                 (recursive sort costs omitted)
```

### Step 3 — Build chain, insert b1

```
b1=1 < a1=7 guaranteed → insert at front:

Chain:  [1, 7, 8, 9, 10, 11, 12]
         b1 a1 a2 a3  a4  a5  a6

Pend:   [b2=4, b3=3, b4=6, b5=5, b6=2]
```

### Step 4 — Insert pend via Jacobsthal order

```
Jacobsthal boundaries: t₁=1, t₂=3, t₃=5, t₄=11, ...

Group 1: b₃, b₂          (from t₂=3 down to t₁+1=2)
Group 2: b₅, b₄          (from t₃=5 down to t₂+1=4)
Group 3: b₆              (remaining)

Insertion order: b₃, b₂, b₅, b₄, b₆
```

Why this order? Each group is sized so the binary search window
hits exactly $2^k - 1$ elements — the **optimal** size for binary search:

```
┌───────────┬───────────────────────┬─────────────────────┐
│ Insert    │ Search window         │ Window size         │
├───────────┼───────────────────────┼─────────────────────┤
│ b₃ first  │ [1, 7, 8]  (< a3=9)  │ 3  = 2² - 1  ✓     │
│ b₂ next   │ [1, 3, 7]  (< a2=8)  │ 3  = 2² - 1  ✓     │
│ b₅ first  │ [1,3,4,7,8,9,10](<11)│ 7  = 2³ - 1  ✓     │
│ b₄ next   │ [1,3,4,5,7,8,9](<10) │ 7  = 2³ - 1  ✓     │
│ b₆ last   │ everything before 12  │ 10 (not optimal)    │
└───────────┴───────────────────────┴─────────────────────┘

Window size 2^k - 1 means binary search uses exactly k comparisons.
No comparison is ever "wasted" on a lopsided split.
```

#### Walking through each insertion:

**Insert b₃=3** into `[1, 7, 8, 9, 10, 11, 12]`
```
Search window: [1, 7, 8]    (b₃ < a₃=9)
  mid=7:  3 < 7 → search [1]
  mid=1:  3 > 1 → insert after 1
Chain: [1, 3, 7, 8, 9, 10, 11, 12]              2 comparisons
```

**Insert b₂=4** into `[1, 3, 7, 8, 9, 10, 11, 12]`
```
Search window: [1, 3, 7]    (b₂ < a₂=8)
  mid=3:  4 > 3 → search [7]
  mid=7:  4 < 7 → insert before 7
Chain: [1, 3, 4, 7, 8, 9, 10, 11, 12]           2 comparisons
```

**Insert b₅=5** into `[1, 3, 4, 7, 8, 9, 10, 11, 12]`
```
Search window: [1, 3, 4, 7, 8, 9, 10]    (b₅ < a₅=11)
  mid=7:  5 < 7 → search [1, 3, 4]
  mid=3:  5 > 3 → search [4]
  mid=4:  5 > 4 → insert after 4
Chain: [1, 3, 4, 5, 7, 8, 9, 10, 11, 12]        3 comparisons
```

**Insert b₄=6** into `[1, 3, 4, 5, 7, 8, 9, 10, 11, 12]`
```
Search window: [1, 3, 4, 5, 7, 8, 9]    (b₄ < a₄=10)
  mid=5:  6 > 5 → search [7, 8, 9]
  mid=8:  6 < 8 → search [7]
  mid=7:  6 < 7 → insert before 7
Chain: [1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]     3 comparisons
```

**Insert b₆=2** into `[1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]`
```
Search window: [1, 3, 4, 5, 6, 7, 8, 9, 10, 11]    (b₆ < a₆=12)
  mid=6:   2 < 6 → search [1, 3, 4, 5]
  mid=3:   2 < 3 → search [1]
  mid=1:   2 > 1 → insert after 1
Chain: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]  3 comparisons
```

### Insertion total: 2 + 2 + 3 + 3 + 3 = **13 comparisons**

---

### What if we inserted naively (b₂, b₃, b₄, b₅, b₆)?

```
┌───────────┬───────────────────────────┬─────────────────────┐
│ Insert    │ Search window             │ Window size         │
├───────────┼───────────────────────────┼─────────────────────┤
│ b₂ first  │ [1, 7]      (< a₂=8)     │ 2  (not 2ᵏ-1)      │
│ b₃ next   │ [1, 4, 7, 8] (< a₃=9)   │ 4  (not 2ᵏ-1)      │
│ b₄ next   │ [1,3,4,7,8,9] (< a₄=10) │ 6  (not 2ᵏ-1)      │
│ b₅ next   │ [1,3,4,6,7,8,9,10](<11)  │ 8  (not 2ᵏ-1)      │
│ b₆ last   │ 10 elements              │ 10 (not 2ᵏ-1)      │
└───────────┴───────────────────────────┴─────────────────────┘

Sizes 2, 4, 6, 8, 10 — none are 2ᵏ-1.
Binary search on 4 elements can take 3 comparisons (same as 7 elements!).
You PAY for 3 comparisons but only ELIMINATE 4 candidates instead of 7.
```

**Naive insertion:** windows 2, 4, 6, 8, 10 → up to 1+3+3+4+4 = **15 comparisons**
**Jacobsthal insertion:** windows 3, 3, 7, 7, 10 → exactly 2+2+3+3+3 = **13 comparisons**

```
┌──────────────────────────────────────────────────────────┐
│                                                          │
│  Jacobsthal saved 2 comparisons on 5 pend insertions.   │
│  With thousands of elements, this adds up significantly. │
│                                                          │
│  The core insight: binary search on 2ᵏ-1 elements       │
│  costs exactly k comparisons. On 2ᵏ elements it ALSO    │
│  costs k comparisons in the worst case — so that extra   │
│  slot is "free" if you pack the window to 2ᵏ-1.         │
│  Jacobsthal ordering achieves exactly this packing.      │
│                                                          │
└──────────────────────────────────────────────────────────┘
```
