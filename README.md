# Nutstruct

#### **介绍**
#### Nutstruct 是泛型容器和算法库，使用 RAII 对动态内存进行管理。



## 目录
| 序列容器 |                                             文件                                             |
| :------: | :------------------------------------------------------------------------------------------: |
|   数组   |        [array.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/array.h)        |
|  字符串  | [basic_string.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/basic_string.h) |
|   向量   |       [vector.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/vector.h)       |
| 双向链表 |         [list.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/list.h)         |
| 双端队列 |        [deque.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/deque.h)        |
|    栈    |        [stack.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/stack.h)        |
|   队列   |        [queue.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/queue.h)        |

<br>

| 关联容器 |                                              文件                                              |
| :------: | :--------------------------------------------------------------------------------------------: |
|   集合   |           [set.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/set.h)           |
|  映射表  |           [map.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/map.h)           |
| 无序集合 | [unordered_set.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/unordered_set.h) |
|  无序表  | [unordered_map.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/unordered_map.h) |

<br>

| 迭代器和标准库算法 |                                          文件                                          |
| :----------------: | :------------------------------------------------------------------------------------: |
|       迭代器       |  [iterator.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/iterator.h)  |
|      通用算法      | [algorithm.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/algorithm.h) |

<br>

|  其他组件  |                                           文件                                           |
| :--------: | :--------------------------------------------------------------------------------------: |
|  基本类型  |       [type.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/type.h)       |
|  函数对象  | [functional.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/functional.h) |
|  智能指针  |     [memory.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/memory.h)     |
| 多用途对象 |    [utility.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/utility.h)    |
|    元组    |      [tuple.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/tuple.h)      |
|    矩阵    |     [matrix.h](https://gitee.com/Eplankton/nut-struct/blob/master/cpp_type/matrix.h)     |

<br>

#### **安装**

 1. `git clone https://gitee.com/Eplankton/nut-struct.git  ` 克隆到本地，在代码中引用相应版本的头文件即可

#### **实现原理**

1.数据封装

​	如果需要，则`数据节点`是储存数据的基础元素，是储存的**最小单位**，如 `链表节点`类，包含储存的数据值、前后指针和构造函数。类似于节点这样的类型不需要提供非平凡的析构函数，因为在节点类中没有申请动态内存的行为，自然不需要任何释放行为，若使用自己的析构函数也无需加入`delete`关键字，事实上`data`变量的生命周期和实例化的对象的生命周期是一致的。

<br>

2.中间对象

​	`中间对象` 是一个抽象的概念，它只存在于文本形式上的说明，实际上代码中并不允许孤立的中间对象，中间对象代表数据的一个无序或有序集合，这样一个集合存在于**堆**中时，若没有任何**管理者**来管理它，就会造成**内存泄露**或**二次释放**问题。任何情况下，一个非空的中间对象必须有**至少一个**管理者与它绑定。

<br>

3.管理者

​	`管理者` 是用户在**栈**中创建的，负责直接操纵数据的实体，如 `list<T>` 类对象，该对象储存了链表的头尾节点指针，以及一个储存链表长度的值 `length`，管理类对外暴露操纵数据元素的**接口**，泛型算法和迭代器只需根据管理类的接口来设计，而与储存的数据类型无关。

​	声明一个管理者时，可以把它即时绑定到一个生成了的中间对象上，也可以仅仅声明而不绑定，一些构造函数允许根据指定的数量和初始化值来创建数据节点的集合（生成中间对象），并绑定到某一管理者。

​	当一个管理者的**作用域**结束时，系统将自动调用**析构函数**（**至少两种**析构函数，一种负责析构数据元素，另一种负责析构管理者），销毁这个管理类所管理的中间对象（**在堆上**）以及管理者本身（**在栈上**）并回收内存。但如果用户已经在此之前手动调用过 `destroy()` 或类似函数销毁了**所有**数据，则系统将只负责析构管理类本身。

​	一个管理者可以在作用域结束之前放弃它对中间对象**所有权**，并将所有权**移动**给别的管理者(`move()`）。也可以使多个管理者**共享**所有权（声明为**引用**即可），这样的多个管理者只会被析构一次，一次析构，管理者全部失效。

​	管理者可以使用拷贝构造函数来 **深拷贝** 数据，创建一个完全独立的新中间对象并与之绑定。

<!-- ```cpp
template <class T>
    class ListNode
    {
    protected:
        ListNode<T> *prev = NULL;
        ListNode<T> *next = NULL;
    public:
        T data;
        ListNode() : prev(NULL), next(NULL){};
        explicit ListNode(const T &userInputData) 
            : data(userInputData), prev(NULL), next(NULL){};

        template <class>
        	friend class LINKED_LIST;
    };
``` -->

<!-- ```cpp
template <class T>
    class LINKED_LIST
    {
    protected:
        ListNode<T> *head = NULL;
        ListNode<T> *tail = NULL;
        unsigned int length = 0;

    public:
        ListNode<T> *begin() const
        ListNode<T> *end() const;
        bool empty() const;
        
        LINKED_LIST() : head(NULL), tail(NULL), length(0){};
        explicit LINKED_LIST(unsigned int userInputlength);
        explicit LINKED_LIST(const T &userInputData, unsigned int userInputlength = 1);
        LINKED_LIST(ListNode<T> *start_node, ListNode<T> *end_node);
        LINKED_LIST(const LINKED_LIST<T> &obj);
        ~LINKED_LIST()
        {
            if (!this->empty())
            {
                this->destroy();
            }
            head = NULL;
            tail = NULL;
        }

        void print();
        unsigned int get_length() const
        unsigned int get_length(ListNode<T> *start_node) const;

        class iterator
        {
            ...
        };

        bool remove(ListNode<T> *start_node, unsigned int N = 0);
        unsigned int remove_all(bool (*condition)(T &stock_val));
        bool destroy();
        bool destroy(ListNode<T> *start_node, ListNode<T> *end_node);
        ListNode<T> *push_back();
        ListNode<T> *push_back(const T &obj, unsigned int num = 1);
        ListNode<T> *push_front(); 
        ListNode<T> *push_front(const T &obj, unsigned int num = 1);
        LL& merge(LINKED_LIST<T> &after);
        ListNode<T> *insert(const ListNode<T> *position, const T &obj,int num = 1);
        ListNode<T> *find(bool (*condition)(T &stock_val));
        
    };
``` -->




