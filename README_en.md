## **nut-struct**

## Introduction
This project was originally the lab assignment for `Data Structure (2022 Spring)` course at `East China Normal University (ECNU)`, aiming to practice `C++` programming and understand the application or relationship between data structure, algorithm, operating system and computer architecture.  After a long period of commits and additions, this project gradually became a relatively modern and complete library.  Due to busy daily workload, there's no time for me to fix these parts and errors.


**Waiting list**:
1.  Lack of `EBO` optimization

2.  Many algorithms are missing

3.  `flat_xxx` and `multi_xxx` containers are missing

4.  Missing `btree` and `heap`

5.  A custom `Allocator` for containers is missing

6.  Lack of comments and unit tests

7.  Lack of thread-safety and exception

8.  Missing type-erase and others in `functional`

9.  Missing `linear-probing` and `open-addressing` scheme for resolving collisions in `hashtable`, current usage of `separate-chaining` scheme is not cache-friendly

10. A mess in `basic_string`, please don't use it

11. `Option && Result`, something similar to `Rust` 

12. Many more...

**Bugs**:
1.  The `end()` iterator should point to the `end()+1` position thus we can get a half-open range

2.  `unique_ptr` shouldn't be used in binary tree nodes, which increases a lot of complexity and overheads when handling with subtrees 

3.  A nested array should be used as the central controller of `deque`, but the combination of `list` and `array` is used for simplicity, so the performance of cache degrades and the iterator can't satisfy the requirements of `RandomAccess`

4. Many more...
<br>

## Components

[GitHub](https://github.com/Eplankton/nut-struct)
[Gitee](https://gitee.com/Eplankton/nut-struct)

| Sequence Container |                                                    
| :------: | 
|  [array.h](https://github.com/Eplankton/nut-struct/blob/main/include/array.h)        |
|  [basic_string.h](https://github.com/Eplankton/nut-struct/blob/main/include/basic_string.h) |
|  [vector.h](https://github.com/Eplankton/nut-struct/blob/main/include/vector.h)       |
|  [list.h](https://github.com/Eplankton/nut-struct/blob/main/include/list.h)         |
|  [deque.h](https://github.com/Eplankton/nut-struct/blob/main/include/deque.h)        |
|  [stack.h](https://github.com/Eplankton/nut-struct/blob/main/include/stack.h)        |
|  [queue.h](https://github.com/Eplankton/nut-struct/blob/main/include/queue.h)        |

<br>

| Associative Container |                                                                             
| :------: |
|  [set.h](https://github.com/Eplankton/nut-struct/blob/main/include/set.h)           |
|  [map.h](https://github.com/Eplankton/nut-struct/blob/main/include/map.h)           |
|  [unordered_set.h](https://github.com/Eplankton/nut-struct/blob/main/include/unordered_set.h) |
|  [unordered_map.h](https://github.com/Eplankton/nut-struct/blob/main/include/unordered_map.h) |

<br>

| Iterator && Algorithm |                                                              
| :----------------: |
|  [iterator.h](https://github.com/Eplankton/nut-struct/blob/main/include/iterator.h)  |
|  [algorithm.h](https://github.com/Eplankton/nut-struct/blob/main/include/algorithm.h) |

<br>

|  Others |                                                                          
| :--------: | 
|  [type.h](https://github.com/Eplankton/nut-struct/blob/main/include/type.h)       |
|  [functional.h](https://github.com/Eplankton/nut-struct/blob/main/include/functional.h) |
|  [memory.h](https://github.com/Eplankton/nut-struct/blob/main/include/memory.h)     |
|  [utility.h](https://github.com/Eplankton/nut-struct/blob/main/include/utility.h)    |
|  [bitset.h](https://github.com/Eplankton/nut-struct/blob/main/include/bitset.h)    |
|  [matrix.h](https://github.com/Eplankton/nut-struct/blob/main/include/matrix.h)     |
|  [option.h](https://github.com/Eplankton/nut-struct/blob/main/include/option.h)     |
|  [range.h](https://github.com/Eplankton/nut-struct/blob/main/include/range.h)     |
|  [concept.h](https://github.com/Eplankton/nut-struct/blob/main/include/concept.h)     |

<br>

## **Benchmark**
```
===============================================================================
| complexityN |               ns/op |                op/s |    err% |     total | benchmark
|------------:|--------------------:|--------------------:|--------:|----------:|:----------
|          10 |                8.50 |      117,636,525.52 |    0.4% |      0.01 | `nuts::sort`
|         100 |              999.04 |        1,000,960.61 |    0.3% |      0.01 | `nuts::sort`
|       1,000 |           12,777.65 |           78,261.67 |    0.8% |      0.01 | `nuts::sort`
|      10,000 |          131,387.50 |            7,611.07 |    0.3% |      0.01 | `nuts::sort`
|     100,000 |        1,625,700.00 |              615.12 |    0.1% |      0.02 | `nuts::sort`
|   1,000,000 |       19,580,800.00 |               51.07 |    2.1% |      0.27 | `nuts::sort`
|  10,000,000 |      199,011,500.00 |                5.02 |    2.0% |      2.77 | `nuts::sort`
| 100,000,000 |    2,318,782,200.00 |                0.43 |    2.0% |     31.36 | `nuts::sort`

|   coefficient |   err% | complexity
|--------------:|-------:|------------
| 8.7240808e-10 |   0.5% | O(n log n)
| 2.3154920e-08 |   3.7% | O(n)
| 2.3205422e-16 |  19.7% | O(n^2)
| 2.3189789e-24 |  22.0% | O(n^3)
| 2.9615327e-02 | 206.7% | O(log n)
| 3.1739317e-01 | 239.2% | O(1)
===============================================================================
```

<br>

## **Install**

 1. `git clone https://github.com/Eplankton/nut-struct.git ` 
    then using by` #include <include/xxx.h>`, all the components are in namespace `nuts`, such as `nuts::vector` and `nuts::sort()`