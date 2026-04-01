# Exercise 00 — Bitcoin Exchange

**Program:** `btc`
**Container:** `std::map<std::string, float>` _(banned in ex01 & ex02 after this)_
**Files:** `Makefile`, `main.cpp`, `BitcoinExchange.cpp`, `BitcoinExchange.hpp`

---

## What it does

Takes a CSV database of historical bitcoin prices and an input file of
`date | value` pairs. For each input line, looks up the exchange rate on that date
(or the closest **earlier** date) and outputs `value × rate`.

---

## Database format (`data.csv`)

```
date,exchange_rate
2009-01-02,0
2011-01-03,0.3
2011-01-09,0.32
2012-01-11,7.1
...
```

1612 entries, comma-separated, sorted chronologically.

---

## Input file format (`input.txt`)

```
date | value
2011-01-03 | 3
2011-01-03 | 2
2011-01-03 | 1
2011-01-03 | 1.2
2011-01-09 | 1
2012-01-11 | -1
2001-42-42
2012-01-11 | 1
2012-01-11 | 2147483648
```

---

## Expected output

```
2011-01-03 => 3 = 0.9          ← 3   × 0.3  = 0.9
2011-01-03 => 2 = 0.6          ← 2   × 0.3  = 0.6
2011-01-03 => 1 = 0.3          ← 1   × 0.3  = 0.3
2011-01-03 => 1.2 = 0.36       ← 1.2 × 0.3  = 0.36
2011-01-09 => 1 = 0.32         ← 1   × 0.32 = 0.32
Error: not a positive number.   ← -1 is negative
Error: bad input => 2001-42-42  ← invalid date (month 42)
2012-01-11 => 1 = 7.1          ← 1   × 7.1  = 7.1
Error: too large a number.      ← 2147483648 > 1000
```

---

## Validation rules

| Check               | Condition                            | Error message                     |
|----------------------|--------------------------------------|-----------------------------------|
| Missing file         | no argument or can't open            | `Error: could not open file.`     |
| Bad format           | line doesn't match `date \| value`   | `Error: bad input => <line>`      |
| Invalid date         | month/day out of range               | `Error: bad input => <date>`      |
| Negative value       | `value < 0`                          | `Error: not a positive number.`   |
| Value too large      | `value > 1000`                       | `Error: too large a number.`      |
| Date before DB start | no earlier date exists in map        | error (implementation-defined)    |

---

## Key STL concept: `std::map::lower_bound()`

```
map:  2011-01-03  2011-01-06  2011-01-09  2011-01-12
                                  ↑
      Query: 2011-01-09  →  exact match found, use rate

      Query: 2011-01-07  →  lower_bound returns 2011-01-09
                             go back one (--it) → 2011-01-06 ✓
                             (closest EARLIER date)

      Query: 2008-01-01  →  lower_bound returns begin()
                             no earlier date → error
```
