# Exercise 01 — Reverse Polish Notation

**Program:** `RPN`
**Container:** `std::stack<int>` _(banned in ex02 after this)_
**Files:** `Makefile`, `main.cpp`, `RPN.cpp`, `RPN.hpp`

---

## Normal (Infix) vs Reverse Polish (Postfix) Notation

### What's the difference?

```
┌─────────────────────────────────────────────────────────────────────┐
│                                                                     │
│  INFIX (normal)              POSTFIX (RPN)                          │
│  ─────────────               ─────────────                          │
│  Operator BETWEEN operands   Operator AFTER operands                │
│  Needs parentheses           No parentheses needed                  │
│  Ambiguous without rules     Always unambiguous                     │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### Conversion examples

```
  Infix (what you write)         RPN (what the program reads)
  ──────────────────────         ───────────────────────────
  8 × 9 - 9 - 9 - 9 - 4 + 1    8 9 * 9 - 9 - 9 - 4 - 1 +
  7 × 7 - 7                     7 7 * 7 -
  ((1 × 2) / 2 × 2) + (2 - 4)  1 2 * 2 / 2 * 2 4 - +
```

### Visual walkthrough: `8 9 * 9 - 9 - 9 - 4 - 1 +`

```
Read    Stack              Operation
────    ─────              ─────────
 8      [8]                push 8
 9      [8, 9]             push 9
 *      [72]               pop 9, pop 8  → 8*9 = 72
 9      [72, 9]            push 9
 -      [63]               pop 9, pop 72 → 72-9 = 63
 9      [63, 9]            push 9
 -      [54]               pop 9, pop 63 → 63-9 = 54
 9      [54, 9]            push 9
 -      [45]               pop 9, pop 54 → 54-9 = 45
 4      [45, 4]            push 4
 -      [41]               pop 4, pop 45 → 45-4 = 41
 1      [41, 1]            push 1
 +      [42]               pop 1, pop 41 → 41+1 = 42
                                           ══════
Result: 42                                 Answer
```

### Visual walkthrough: `7 7 * 7 -`

```
Read    Stack              Operation
────    ─────              ─────────
 7      [7]                push 7
 7      [7, 7]             push 7
 *      [49]               pop 7, pop 7  → 7*7 = 49
 7      [49, 7]            push 7
 -      [42]               pop 7, pop 49 → 49-7 = 42
                                           ══════
Result: 42                                 Answer
```

### Visual walkthrough: `1 2 * 2 / 2 * 2 4 - +`

```
Read    Stack              Operation
────    ─────              ─────────
 1      [1]                push 1
 2      [1, 2]             push 2
 *      [2]                pop 2, pop 1  → 1*2 = 2
 2      [2, 2]             push 2
 /      [1]                pop 2, pop 2  → 2/2 = 1
 2      [1, 2]             push 2
 *      [2]                pop 2, pop 1  → 1*2 = 2
 2      [2, 2]             push 2
 4      [2, 2, 4]          push 4
 -      [2, -2]            pop 4, pop 2  → 2-4 = -2
 +      [0]                pop -2, pop 2 → 2+(-2) = 0
                                           ══════
Result: 0                                  Answer
```

---

## Expected outputs from subject

```
$> ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
42
$> ./RPN "7 7 * 7 -"
42
$> ./RPN "1 2 * 2 / 2 * 2 4 - +"
0
$> ./RPN "(1 + 1)"
Error
```

---

## Rules

- Numbers are always **single digits** (less than 10) in the input
- Intermediate results and final result **can** be larger
- Tokens: `+` `-` `/` `*` and single-digit numbers
- Brackets and decimals → **Error**
- Errors go to **standard error** (`std::cerr`)

---

## Algorithm (pseudocode)

```
for each token in input:
    if token is a number:
        stack.push(number)
    else if token is an operator:
        if stack.size() < 2: ERROR
        b = stack.pop()
        a = stack.pop()
        stack.push(a OP b)     ← note: a is the FIRST operand
    else:
        ERROR (invalid token)

if stack.size() != 1: ERROR
print stack.top()
```
