#include <iostream>

using namespace std;

template<typename T>
struct forward_list_node {
    forward_list_node(T&& t = T()) 
        : data(std::forward<T>(t)) {}
    T data;
    forward_list_node* next = nullptr;
};

template<class T, class Ref, class Ptr>
struct forward_list_iterator {
  typedef Ref reference;
  typedef Ptr pointer;
  
  typedef forward_list_iterator<T, Ref, Ptr> self;
  typedef forward_list_node<T> node;
  
  forward_list_iterator(node* node = nullptr)
    : node_(node) {}
  
  bool operator==(const self& rhs){
      return node_ == rhs.node_;
  }
  
  bool operator!=(const self& rhs){
      return !(operator==(rhs));
  }
  
  reference operator*() { return node_->data; }
  pointer operator->() { return &(operator*()); }
  
  self& operator++() {
      node_ = node_->next;
      return *this;
  }
  
  self operator++(int){
      self tmp = *this;
      node_ = node_->next;
      return tmp;
  }

  node* node_ = nullptr;
};

template<typename T>
class forward_list {
public:
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* pointer;
  typedef const T* const_pointer;

  typedef forward_list_iterator<T, T&, T*> iterator;
  typedef forward_list_iterator<T, const T&, const T*> const_iterator;

  typedef forward_list_node<T> node;

  forward_list() : node_(new node()) {
      node_->next = node_;
  }
  ~forward_list() {
      //clear();
      delete node_;
      node_ = nullptr;
  }

  bool empty() const { return begin() == end(); }

  reference front() { return *begin(); }
  const_reference front() const { return begin(); }

  void push_front(T&& t) {
      insert_after(end(), std::forward<T>(t));
  }
  void pop_front() {
      erase_after(end());
  }

  iterator begin() { return node_->next; }
  const_iterator begin() const { return node_->next; }
  iterator end() { return node_; }
  const_iterator end() const { return node_; }

  iterator insert_after(iterator pos, T&& t){
      node* pn = new node(std::forward<T>(t));
      pn->next = pos.node_->next;
      pos.node_->next = pn;
      return pn;
  }
  iterator erase_after(iterator pos){
      node* tmp = pos.node_->next;
      pos.node_->next = tmp->next;
      delete tmp;
      return pos.node_->next;
  }
  void clear() {
      while(!empty()) {
          erase_after(end());
      }
  }
  void show(){
      node* cur = node_->next;
      while(cur != node_){
          cout << cur->data << ", ";
          cur = cur->next;
      }
      cout << "\n";
  }
  void reverse(){
      node* pre = node_;
      node* cur = node_->next;
      while(cur != node_){
          node* next = cur->next;
          cur->next = pre;
          pre = cur;
          cur = next;
      }
      node_->next = pre;
  }

private:
  node* node_ = nullptr;
};

int main()
{
    forward_list<int> l;
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    l.push_front(4);

    l.show();
    l.reverse();
    l.show();

    cout << "{";
    for(auto&& e : l){ cout << e <<  ", "; }
    cout << "}\n";

    return 0;
}
