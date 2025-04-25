# Lab work 3: my_vector

Authors (team):

- [Radomyr Husiev](https://github.com/rhusiev)
- [Roman Zaletskyi](https://github.com/Romko-studik) 
- [Bohdan Opyr](https://github.com/rizerphe)

## Prerequisites

-

### Compilation

```
./compile.sh -o
```

`-o` stands for compilation with optimization. Change to `-d` for debug.

### Usage

```
./compile.sh -o; ./bin/test_vector
```

### Results

In this lab we implemented our own versions of standard containers:

1. `VectorTheSerene` - a custom implementation of `std::vector` providing dynamic array functionality with automatic memory management
2. `ArrayTheSteadfast` - a custom implementation of `std::array` offering fixed-size array functionality with bounds checking

Through this work, we deepened our understanding of memory management, templates, and container design. We implemented various features including:
- Element access (at, [], front, back)
- Capacity control (reserve, shrink_to_fit, clear)
- Modifiers (push_back, emplace_back, insert, erase)
- Iterators (begin/end, rbegin/rend)
- Copy/move semantics 