# nut-struct

<img src="data:image/png;base64,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">

## **介绍**
#### 华东师范大学(East China Normal University) 软件工程专业 2022春 数据结构与算法实验课作业 
#### nut-struct 是泛型容器和算法库，使用 RAII 对动态内存进行管理，不保证异常/线程安全。

<br>

## 目录
| 序列容器 |                                             文件                                             |
| :------: | :------------------------------------------------------------------------------------------: |
|   数组   |        [array.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/array.h)        |
|  字符串  | [basic_string.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/basic_string.h) |
|   向量   |       [vector.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/vector.h)       |
| 双向链表 |         [list.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/list.h)         |
| 双端队列 |        [deque.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/deque.h)        |
|    栈    |        [stack.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/stack.h)        |
|   队列   |        [queue.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/queue.h)        |

<br>

| 关联容器 |                                              文件                                              |
| :------: | :--------------------------------------------------------------------------------------------: |
|   集合   |           [set.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/set.h)           |
|  映射表  |           [map.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/map.h)           |
| 无序集合 | [unordered_set.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/unordered_set.h) |
|  无序表  | [unordered_map.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/unordered_map.h) |

<br>

| 迭代器和标准库算法 |                                          文件                                          |
| :----------------: | :------------------------------------------------------------------------------------: |
|       迭代器       |  [iterator.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/iterator.h)  |
|      通用算法      | [algorithm.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/algorithm.h) |

<br>

|  其他组件  |                                           文件                                           |
| :--------: | :--------------------------------------------------------------------------------------: |
|  基本类型  |       [type.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/type.h)       |
|  函数对象  | [functional.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/functional.h) |
|  智能指针  |     [memory.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/memory.h)     |
| 多用途对象 |    [utility.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/utility.h)    |
| 位集 |    [bitset.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/bitset.h)    |
|    元组    |      [tuple.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/tuple.h)      |
|    矩阵    |     [matrix.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/matrix.h)     |
|    异常处理    |     [option.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/option.h)     |
|  范围  |     [range.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/range.h)     |
|  概念约束  |     [concept.h](https://gitee.com/Eplankton/nut-struct/blob/master/include/concept.h)     |

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
