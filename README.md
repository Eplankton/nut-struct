# Nutstruct

#### **介绍**
#### Nutstruct 是一组泛型容器和算法，包含数据增删查改实现 ，模拟了所有权的概念，使用 RAII 对内存进行管理。
#### **安装**

 1. `git clone https://gitee.com/Eplankton/nut-struct.git  ` 克隆到本地，在代码中引用相应版本的头文件即可

#### **实现原理**

1.数据封装

​	`数据节点`是储存数据的基础元素，是储存的**最小单位**，如以下代码中的 `链表节点`类，包含储存的数据值、前后指针和构造函数。

类似于节点这样的自定义泛型数据类型不需要提供析构函数，因为在节点类中没有任何一个成员函数有过申请动态内存的行为，自然不需要任何释放行为，若使用自己的析构函数也无需加入`delete`关键字，事实上，`data`变量的生存期和实例化的对象的生存期是一致的。

```cpp
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
```

2.中间对象

​	`中间对象` 是一个抽象的概念，它只存在于文本形式上的说明，实际上代码中并**不允许**中间对象的独立形成，中间对象代表基础数据节点的一个无序或有序集合（如用指针连接），这样一个集合存在于**堆**中时，若没有任何**管理者**来管理它，就会造成**内存泄露**和二次释放问题。任何情况下，一个非空的中间对象必须有**至少一个**管理者与它绑定，否则便会造成**内存泄漏**。

3.管理者

​	`管理者` 是用户在**栈**中创建的，负责直接操纵数据元素的实体，如下文的 `LINKED_LIST` 类对象，该对象储存了链表的头尾节点指针，以及一个储存链表长度的值 `length`，管理类对外提供操纵数据元素的**接口**，泛型算法和迭代器只需根据管理类的接口来设计，而与储存的数据类型无关。

​	管理类通常被声明为数据节点类的**友元类**，使得其能够访问数据节点的成员变量和构造函数。声明一个管理者时，可以把它即时绑定到一个生成了的中间对象上，也可以仅仅声明而不绑定，一些构造函数允许根据指定的数量和初始化值来创建数据节点的集合（生成中间对象），并绑定到某一管理者。

​	当一个管理者的**作用域**结束时，系统将自动调用**析构函数**（**至少两种**析构函数，一种负责析构数据元素，另一种负责析构管理者），销毁这个管理类所管理的中间对象（**在堆上**）以及管理者本身（**在栈上**）并回收内存。但如果用户已经在此之前手动调用过 `clear()` 函数或类似的函数销毁了**所有**数据节点，则系统将只负责回收管理类本身，避免二次释放问题。

​	一个管理者可以达作用域结束之前放弃它对中间对象**所有权**，并将所有权**转交**给别的管理者(或者由别的还未绑定的管理者夺取它的管理权)。也可以使多个管理者共享所有权（声明为**引用**），这样的多个管理者只会被销毁一次实体，一次销毁，全部失效。

​	管理者可以使用拷贝构造函数来 **深拷贝** 数据，创建一个完全独立的新中间对象并与之绑定。

​	多个独立的管理者之间可以合并，合并后只剩下一个主管理者，其它的全部失效。相似的，也可以把一个主管理者分解为多个从管理者，然后主管理者失效。

```cpp
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
```



#### **使用**

**一、节点操作**

1.  `ListNode()`	构造函数，构造一个空节点单元，使用时需要指明类型 `Type`
2.  `explicit ListNode(const T &userInputData) : data(userInputData), prev(NULL), next(NULL){}`    构造函数，用输入数据构造一个节点单元，使用时需要指明类型 `Type`

**二、链表操作**

4.  `LINKED_LIST()`    构造函数，构造一个空单链表，使用时需要指明类型 `Type`
5.  `explicit LINKED_LIST(unsigned int userInputlength)`    构造函数，构造一个长度为 `userInputlength` 空链表，使用时需要指明类型 `Type`
6.  `explicit LINKED_LIST(const T &userInputData, unsigned int userInputlength = 1)`    构造函数，以 ` obj` 为初始化值构造一个长度为 `userInputlength` 的链表，使用时需要指明类型 `Type`
7.  `LINKED_LIST(const LL<T> &obj)`    构造函数，**深拷贝**另一个链表，使用时需要指明类型 `Type`
8.  `~LINKED_LIST()`   析构函数，销毁实体和管理类
9.  `void print(void)`    打印函数，把一个非空链表打印到屏幕
10.  `ListNode<T> *begin(void)`    返回头节点指针
11.  `ListNode<T> *end(void)`    返回尾节点指针
12.  `bool empty()` 返回链表是否为空
13.  `unsigned int get_length(void)`    返回链表的长度
14.  `unsigned int get_length(const ListNode<T> *start_node)`    返回从 ` start_node` 开始链表的长度
15.  `bool remove(ListNode<T> *start_node, int N)`    删除从 ` start_node ` 数起第 N 个节点
16.  `int remove_all(bool (*condition)(T stock_val))`    删除链表中所有符合 `condition` 条件的节点，返回被删除的数量
17.  `bool destroy(void)`    删除一个非空链表，失败返回 `false`  
18.  `bool destroy(ListNode<T> *start_node, ListNode<T> *end_node)`    以 ` start_node` 为首节点，以 ` end_node` 为尾节点删除一个非空链表的一部分，失败返回 `false`  
19.  `ListNode<T> *push_back(unsigned int num = 1)`    在链表最后加上 `num` 个空节点，返回尾指针
20.  `ListNode<T> *push_back(const T &obj, unsigned int num = 1)`    在链表最后加上 `num` 个节点，初始化值为`obj`，返回尾指针
21.  `ListNode<T> *push_front(unsigned int num = 1)`    在链表最前加上 `num` 个空节点，返回头指针
22.  `ListNode<T> *push_front(const T &obj, unsigned int num = 1)`    在链表最前加上 `num` 个节点，初始化值为`obj`，返回头指针
23.  `ListNode<T> *find(bool (*condition)(T stock_val))`    查找符合 `condition` 条件的第一个节点，返回指针
25.  迭代器 `iterator` 操作 ` ==  !=  ++  --  *`
