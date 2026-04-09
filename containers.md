# STL Containers — Overview & Comparison

## The Three Categories

```
STL Containers
├── Sequence Containers      ← ordered by insertion
│   ├── vector
│   ├── deque
│   └── list
├── Associative Containers   ← ordered by key (sorted)
│   ├── set
│   ├── multiset
│   ├── map
│   └── multimap
└── Container Adaptors       ← wrappers around other containers
    ├── stack
    ├── queue
    └── priority_queue
```

---

## Sequence Containers

Elements are stored in the order you insert them. Access patterns differ.

### `std::vector`
**What:** Dynamic array, contiguous memory.

> **Naming confusion:** This has nothing to do with a vector in math/physics (an n-dimensional coordinate array). Here "vector" just means "growable array" — a sequence of elements stored contiguously in memory. Think of it as a resizable `int arr[]`, not a direction in space.

```cpp
std::vector<int> v;
v.push_back(10);     // append
v.push_back(20);
v[0];                // 10  — O(1) random access
v.size();            // 2
```

| Operation | Complexity |
|-----------|-----------|
| Access by index | O(1) |
| Push/pop back | O(1) amortized |
| Insert/erase middle | O(n) — shifts elements |

**Use when:** You need random access and mostly add/remove at the end. The default go-to container.

---

### `std::deque`
**What:** Double-ended queue. Like vector but efficient at both ends.

```cpp
std::deque<int> d;
d.push_back(10);
d.push_front(5);     // efficient! (vector can't do this)
d[0];                // 5
d[1];                // 10
```

| Operation | Complexity |
|-----------|-----------|
| Access by index | O(1) |
| Push/pop front or back | O(1) |
| Insert/erase middle | O(n) |

**Use when:** You need fast insertion at both ends. Underlying container for `std::stack` and `std::queue`.

> **deque vs list:** The key difference is that deque is still **indexed** — you can do `d[3]` in O(1) just like a vector. A list has no indexing at all; you must walk through it with iterators. In return, list gives you O(1) insert/erase *anywhere* (given an iterator), while deque only does that cheaply at the front and back. Think: **deque = double-ended vector**, **list = chain of links**.

---

### `std::list`
**What:** Doubly-linked list. No contiguous memory.

```cpp
std::list<int> l;
l.push_back(10);
l.push_front(5);
l.push_back(20);
// l: 5 <-> 10 <-> 20

std::list<int>::iterator it = l.begin();
++it;                 // points to 10
l.insert(it, 7);     // l: 5 <-> 7 <-> 10 <-> 20  — O(1)!
```

| Operation | Complexity |
|-----------|-----------|
| Access by index | **Not supported** — no `[]` operator |
| Push/pop front or back | O(1) |
| Insert/erase anywhere (with iterator) | O(1) |
| Find element | O(n) — must traverse |

**Use when:** You frequently insert/erase in the middle. No random access — you traverse with iterators.

---

## Associative Containers

Elements are automatically **sorted by key**. Internally use balanced binary trees (red-black trees). Not relevant for `easyfind` (subject says skip these).

### `std::set`
**What:** Collection of unique keys, sorted.

```cpp
std::set<int> s;
s.insert(30);
s.insert(10);
s.insert(20);
s.insert(10);        // ignored — already exists
// s: {10, 20, 30}   — auto-sorted

s.find(20);          // O(log n) — returns iterator
s.count(10);         // 1 (exists) or 0 (doesn't)
```

**Use when:** You need unique sorted values with fast lookup.

### `std::multiset`
Same as `set` but **allows duplicates**.

> **Not really a "set" in the math sense.** A mathematical set has unique elements by definition. `multiset` is more like a **sorted occurrence list** — it tracks how many times each value appears while keeping everything in order. The name is misleading.

```cpp
std::multiset<int> ms;
ms.insert(10);
ms.insert(10);
ms.insert(10);
ms.count(10);        // 3
```

### `std::map`
**What:** Key-value pairs, sorted by key. Each key is unique.

```cpp
std::map<std::string, int> m;
m["alice"] = 90;
m["bob"] = 85;
m["alice"] = 95;     // overwrites — key "alice" already exists
// m: {"alice":95, "bob":85}

m.find("bob");       // O(log n)
m["charlie"];        // creates entry with default value (0)
```

**Use when:** You need a dictionary / lookup table.

> **Python equivalent:** `std::map` is essentially a **`dict`** in Python — key-value pairs with unique keys. The difference: C++ `map` keeps keys sorted, while Python's `dict` (3.7+) preserves insertion order.

### `std::multimap`
Same as `map` but **allows duplicate keys**. No `[]` operator.

> **In math terms:** A regular `map` is a **function** — each key maps to exactly one value (injective in the key direction). A `multimap` abandons that uniqueness, so it's no longer a function but a **relation**. Practical use case: mapping one entity to multiple others, like a person to their credit cards, or a category to its products.

```cpp
std::multimap<std::string, int> mm;
mm.insert(std::make_pair("alice", 90));
mm.insert(std::make_pair("alice", 95));  // both stored
mm.count("alice");   // 2
```

---

## Container Adaptors

These are **wrappers** around other containers. They restrict the interface to enforce a specific access pattern. You don't get iterators (which is why cpp08/ex02 exists!).

### `std::stack`
**What:** LIFO (Last In, First Out). Wraps `deque` by default.

```cpp
std::stack<int> s;
s.push(1);
s.push(2);
s.push(3);
s.top();             // 3
s.pop();             // removes 3
s.top();             // 2
s.size();            // 2
```

**No iterators. No random access.** You can only see/remove the top.

### `std::queue`
**What:** FIFO (First In, First Out). Wraps `deque` by default.

```cpp
std::queue<int> q;
q.push(1);
q.push(2);
q.push(3);
q.front();           // 1
q.back();            // 3
q.pop();             // removes 1 (front)
```

### `std::priority_queue`
**What:** Max-heap. Largest element is always on top.

```cpp
std::priority_queue<int> pq;
pq.push(3);
pq.push(1);
pq.push(2);
pq.top();            // 3 — always the largest
pq.pop();
pq.top();            // 2
```

---

## Quick Comparison

| Container | Ordered | Sorted | Duplicates | Random Access | Iterators |
|-----------|---------|--------|------------|---------------|-----------|
| vector | by insertion | no | yes | O(1) `[]` | yes |
| deque | by insertion | no | yes | O(1) `[]` | yes |
| list | by insertion | no | yes | no | yes |
| set | by value | yes | no | no | yes |
| multiset | by value | yes | yes | no | yes |
| map | by key | yes | no (keys) | O(log n) `[]` | yes |
| multimap | by key | yes | yes | no | yes |
| stack | LIFO | no | yes | top only | **no** |
| queue | FIFO | no | yes | front/back | **no** |
| priority_queue | by priority | yes (heap) | yes | top only | **no** |

---

## Why `easyfind` skips associative containers

Associative containers have their own `.find()` member function that searches by **key** in O(log n). Using `std::find` (which does linear search by **value**) on them would be inefficient and semantically wrong — they're designed for key-based lookup, not value-based search.

---

## Further reading

- [Microsoft C++ Standard Library Reference](https://learn.microsoft.com/en-us/cpp/standard-library/cpp-standard-library-reference) — Docs with diagrams showing internal data structures
- [VisuAlgo](https://visualgo.net/) — Animated visualizations of data structures (linked lists, BSTs, heaps)
- [LearnCpp](https://learncpp.com/) — Chapters 16-23 cover containers and iterators in depth with examples
