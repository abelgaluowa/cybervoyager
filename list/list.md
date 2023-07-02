# list 杂谈

list 非常具有学习价值：

- nice domain
- complex algorithm
- pointer intensive
- visualization

## 实现 tips

- **changing a pointer using a reference pointer **

  pass a pointer to the "value of interest" that needs to be changed.

  push_back 的三种实现方式，各有优劣， special case是工程级实现，另两种简化了处理条件，变更了variant.

  ```c++
  void changeToNull(struct node** headRef) {
  	*headRef = NULL;
  }
  
  /// 使用 reference pointer 而不仅仅是 pointer
  /// 使用 pointer 修改的仅仅是 stack 上的 local variable 
  void push_front(struct node** headRef, int data)
  {
      struct node* pn = new node(data);
      pn->next = *headRef->next;
      *headRef->next = pn;
  }
  
  
  /// push_back 构造 list 三种不同方式
  struct node* head = NULL;
  struct node* tail = NULL;  
  /// 1：special case
  void build(struct node** headRef, struct node** tailRef, int data) {
      struct node* pn = new node(data);
      pn->next = NULL;
      
      if(*head == NULL) {
          *headRef = pn;
          *tailRef = pn;
      }
      else {
          *tail->next = pn;
          *tail = pn;
      }
  }
  
  /// 2: Temporary Dummy node as first node
  /// 增加一个 dummy node， push_back 均变成在 tail->next 添加，无 special case
  struct node dummy;
  dummy.next = NULL; // as head
  tail = &dummy;
  void build(struct node** tailRef, int data) {
      struct node* pn = new node(data);
      pn->next = NULL;
      *tail->next = pn;
      *tail = pn;
  }
  
  /// local reference
  /// 使用指向最后一个节点指针 替代 指向最后一个节点
  struct node* head = NULL;
  struct node** lastPtrRef = &head;
  void build(struct node&** lastPtrRef, int data) {
      struct node* pn = new node(data);
      pn->next = NULL;
      
      *lastPtrRef->next = pn;
      lastPtrRef = &((*lastPtrRef)->next);
  }                          
  ```

 ## pointer and local memory

- 函数执行与 local memory

  ```c++
  // Local storage example
  int Square(int num) {
      int result;
      result = num * num;
      return result;
  }
  ```

  线程有其独有的 stack， 当运行到 Squre 函数是，为 locals 分配内存（num 带值， result 随机).

  在函数内部持续可见。

- 关于 pointer 的字面量

  NULL - 0x00000000 **or** 0x00000000'00000000 (32 vs 64 bit)
  NUL - 0x00 **or** 0x0000 (ascii vs 2byte unicode)
  '0' - 0x20

  | NULL | #define NULL 0<br /> #define NULL ((void*)0） |
  | ---- | --------------------------------------------- |
  | '\0' | 0x00                                          |
  | "\0" | 两个空字符， size is 2， strlen == 0          |

- nullptr 与 NULL区别

  c++11 引入nullptr 解决 NULL 可能造成的重载二义性，不能区别 int 与指针优先级。

  c 中 NULL 定义为 （(void\*)0） c++ 定义为 0。因为c++ 中  void\*不支持隐式转换为其他类型。

  ```c++
  const class nullptr_t
  {
  public:
      template<class T>
      inline operator T*() const
      { return 0; }

      template<class C, class T>
      inline operator T C::*() const
      { return 0; }

  private:
      void operator&() const;
  } nullptr = {};
  ```

## 仿 std::forward_list 

接口非常不错，作为练习。

[forward_list](./forward_list.cpp)

接口设计问题：

1. 模板类中 type 在类创建时已确定，不能在成员函数中用 T&& 标识 universal reference。

   必须提供 const T& 与 T&& 两套接口；或者成员函数增加 template<typename T>.

   [完美转发std::forward](./forwardTest.cpp)

2. iterator 接口

   对不变接口增加 const 约束。

   为什么 template<class T, class Ref, class Ptr> 需要单独传递 Ref、Ptr？

   由于底层需要直接操作 T\* , 无法通过第一个参数推断Ref、Ptr。

3. forward_list 接口

   返回值不能重载，同名函数 const 约束可以实现重载。

   所有实现中不做入参有效性判断，由使用者保证。

   接口按需提供 const T& 与 T&& 两套接口。或者使用1 中解决方案。

   

## list 算法

1. 逆序

   递归、前后指针法。

2. 环检测

   快慢指针，相遇时快指针是慢指针跑过的2倍。 

   重置一指针从头开始，相遇时到达环节点。

