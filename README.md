# nut-struct

## **介绍**
#### 华东师范大学(East China Normal University) 软件工程学院 2022春 数据结构与算法实验课
#### nut-struct 是简易 STL 库，使用 RAII 对动态内存进行管理，不保证异常/线程安全
#### [ 主要设计缺陷：尾迭代器位置指向 end()-1 ]

<br>

## 目录
| 序列型容器 |                                             文件                                             |
| :------: | :------------------------------------------------------------------------------------------: |
|   数组   |        [array.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/array.h)        |
|  字符串  | [basic_string.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/basic_string.h) |
|   动态数组   |       [vector.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/vector.h)       |
| 双向链表 |         [list.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/list.h)         |
| 双端队列 |        [deque.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/deque.h)        |
|    栈    |        [stack.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/stack.h)        |
|   队列   |        [queue.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/queue.h)        |

<br>

| 关联型容器 |                                              文件                                              |
| :------: | :--------------------------------------------------------------------------------------------: |
|  有序集合  |           [set.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/set.h)           |
|  有序表  |           [map.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/map.h)           |
| 无序集合 | [unordered_set.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/unordered_set.h) |
|  无序表  | [unordered_map.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/unordered_map.h) |

<br>

| 迭代器与算法 |                                          文件                                          |
| :----------------: | :------------------------------------------------------------------------------------: |
|       迭代器       |  [iterator.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/iterator.h)  |
|      算法      | [algorithm.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/algorithm.h) |

<br>

|  其他组件  |                                           文件                                           |
| :--------: | :--------------------------------------------------------------------------------------: |
|  基本类型  |       [type.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/type.h)       |
|  函数对象  | [functional.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/functional.h) |
|  智能指针  |     [memory.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/memory.h)     |
| 多用途对象 |    [utility.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/utility.h)    |
| 位集 |    [bitset.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/bitset.h)    |
|    矩阵    |     [matrix.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/matrix.h)     |
|    异常    |     [option.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/option.h)     |
|  范围  |     [range.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/range.h)     |
|  概念  |     [concept.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/concept.h)     |

<br>

## **Benchmark 测试**
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

## **安装**

 1. `git clone https://gitee.com/Eplankton/nut-struct.git  ` 在代码中引用` include/ `下头文件即可

<br>

## **实现原理**

1.数据封装

​	如果需要，则`数据节点`是储存数据的基础元素，是储存的 **最小单位**，如 `链表节点`类，包含储存的数据值、前后指针和构造函数。类似于节点这样的类型不需要提供非平凡的析构函数，因为在节点类中没有申请动态内存的行为，自然不需要任何释放行为，若使用自己的析构函数也无需加入`delete`关键字，事实上`data`变量的生命周期和实例化的对象的生命周期是一致的。

<br>

2.中间对象

​	`中间对象` 是一个抽象的概念，它只存在于文本形式上的说明，实际上代码中并不允许孤立的中间对象，中间对象代表数据的一个无序或有序集合，这样一个集合存在于 **堆** 中时，若没有任何 **管理者** 来管理它，就会造成 **内存泄露** 或 **二次释放** 问题。任何情况下，一个非空的中间对象必须有 **至少一个** 管理者与它绑定。

<br>

3.管理者

​	`管理者` 是用户创建的，负责直接操纵数据的实体，如 `list<T>` 对象，该对象储存了链表的头尾节点指针，以及一个储存链表长度的值 `length`，管理类对外暴露操纵数据元素的 **接口**，泛型算法和迭代器只需根据管理类的接口来设计，而与储存的数据类型无关。

​	声明一个管理者时，可以把它即时绑定到一个生成了的中间对象上，也可以仅仅声明而不绑定，一些构造函数允许根据指定的数量和初始化值来创建数据节点的集合（生成中间对象），并绑定到某一管理者。

​	当一个管理者的 **作用域** 结束时 ，系统将自动调用析构函数 **至少两种 **析构函数，一种负责析构数据元素，另一种负责析构管理者），销毁这个管理类所管理的中间对象 **在堆上** 以及管理者本身 **在栈上** 并回收内存。但如果用户已经在此之前手动调用过 `destroy()` 或类似函数销毁了 **所有** 数据，则系统将只负责析构管理类本身。

​	一个管理者可以在作用域结束之前放弃它对中间对象 **所有权 **，并将所有权 **移动** 给别的管理者  ` move()`。也可以使多个管理者 **共享** 所有权（声明为 **引用** 即可），这样的多个管理者只会被析构一次，一次析构，管理者全部失效。

​	管理者可以使用拷贝构造函数来 **深拷贝** 数据，创建一个完全独立的新中间对象并与之绑定。
