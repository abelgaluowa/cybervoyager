# 面向对象关键设计

# S.O.L.I.D

- **S**ingle-responsibility principle（单一功能原则）：对象应该仅具有一种单一功能；

- **O**pen–closed principle（开闭原则）：软件应该对于扩展开放，对于修改封闭；

- **L**iskov substitution principle（里氏替换原则）：程序中的对象应该可以在不改变程序正确性的前提下被它的子类所替换；

- **I**nterface segregation principle（接口隔离原则）：多个特定客户端接口要好于一个宽泛用途的接口；

- **D**ependency inversion principle（依赖反转原则）：方法应该遵从依赖于抽象而不是一个实例；

  

## 接口类中不应该有实现

接口类是模块之间的协议，不应该有具体的闪现。不然不够抽象，污染全局代码。

优先考虑组合。

可以考虑使用一个类屏蔽变化，成员变量指向抽象类或者接口，外部不扩散这个继承体系。



## 正方形到底是不是矩形？

子类应该继承抽象类，正方形不应该继承矩形。  __实体类只能继承抽象类，不可继承实体类__。

计算机世界和显示世界不是完全对等，为了构造系统，有其自身的约束。

内存只存必要的属性，其他可以推出的走接口。甚至于在系统设计中也是data + view的设计，参考 taskflow 设计。

且特征属性不能修改，否则违反开闭原则。 不提供 setters。

其他模块使用抽象类 Shape，否则扩展功能时需要修改源码，也违反开放封闭原则。

现实中完全的闭合是不可能，需要叠加其他方法提高扩展性，比如数据驱动。

```c++
class Shape {
    public:
    ~Shape() = default;
    
    double area() const = 0;
    double length() const = 0;
    void draw() const = 0;
};

class Rectangle : public Shape {
    public:
    Rectangle(double width, double height);
    /// implement
};

class Square : public Shape {
    public:
    Square(double sideLength);
    /// implement
};

// Square heriting Rectangle obey Liskov substitution principle!!!

class Painter {
    public:
    Painter(std::function<void()> draw): draw_(draw){}
    void paint() {
        draw_();
    }
    private:
    std::function<void()> draw_;
};

int main()
{
    Shape* pShape = new Square(2);
    
    /// smart pointer better.
    Painter painter([pShape](){
        pShape->draw();
    });
    painter.paint();
    
    delete pShape;
}
```



