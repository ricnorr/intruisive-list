#pragma once

#include <algorithm>
#include <valarray>

class ListElement {
 public:
  ListElement() {}

  bool IsLinked() const {
    return left_ != nullptr || right_ != nullptr;
  }

  void Unlink() {
    if (left_ != nullptr) {
      left_->right_ = right_;
    }
    if (right_ != nullptr) {
      right_->left_ = left_;
    }
    left_ = nullptr;
    right_ = nullptr;
  }


 virtual ~ListElement() {
    Unlink();
  }

  ListElement(const ListElement &) = delete;

 private:
  template<class T>
  friend
  class List;

  ListElement *left_ = nullptr;
  ListElement *right_ = nullptr;


  void LinkAfter(ListElement *other) {
    auto after_other = other->right_;
    other->right_ = this;
    left_ = other;
    right_ = after_other;
    if (after_other != nullptr) {
      after_other->left_ = this;
    }
  }
};

template<typename T>
class List {
 private:
  ListElement *head_ = new ListElement();
  ListElement *tail_ = head_;
 public:
  class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:

    explicit Iterator(ListElement *val) : it_(val) {}

    Iterator &operator++() {
      it_ = it_->right_;
      return *this;
    }

    Iterator operator++(int) {
      auto result = Iterator(&*it_);
      ++(*this);
      return result;
    }

    T &operator*() const {
      return static_cast<T &>(*it_);
    }

    T *operator->() const {
      return &operator*();
    }

    bool operator==(const Iterator &rhs) const {
      return rhs.it_ == it_;
    }
    bool operator!=(const Iterator &rhs) const {
      return rhs.it_ != it_;
    }
   private:
    ListElement *it_;
  };

  
  ~List() {
    auto current = head_->right_;
    while (current != nullptr) {
      current->left_->right_ = nullptr;
      current->left_ = nullptr;
      current = current->right_;
    }
    tail_ = head_;
  }

  List() {}

  List(const List &) = delete;

  List &operator=(List &&other) {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    return *this;
  }

  List(List &&other) {
    *this = std::forward<List>(other);
  }

  List &operator=(const List &) = delete;

  bool IsEmpty() const {
    return Size() == 0;
  }


  size_t Size() const {
    size_t size = 0;
    auto current = head_->right_;
    while (current != nullptr) {
      size++;
      current = current->right_;
    }
    return size;
  }



  void PushBack(T *elem) {
    elem->LinkAfter(&*tail_);
    tail_ = tail_->right_;
  }

  void PushFront(T *elem) {
    elem->LinkAfter(&*head_);
    if (tail_->left_ == nullptr) {
      tail_ = head_->right_;
    }
  }

  T &Front() {
    if (!head_->IsLinked()) {
      throw std::runtime_error("front from empty list");
    }
    return static_cast<T &>(*(head_->right_));
  }

  const T &Front() const {
    if (!head_->IsLinked()) {
      throw std::runtime_error("front from empty list");
    }
    return static_cast<T &>(*(head_->right_));
  }

  T &Back() {
    if (!head_->IsLinked()) {
      throw std::runtime_error("back from empty list");
    }
    return static_cast<T &>(*tail_);
  }

  const T &Back() const {
    if (!head_->IsLinked()) {
      throw std::runtime_error("back from empty list");
    }
    return static_cast<const T &>(*tail_);
  }

  void PopBack() {
    if (!head_->IsLinked()) {
      throw std::runtime_error("pop back from empty list");
    }
    auto temp = tail_->left_;
    tail_->Unlink();
    tail_ = temp;
  }

  void PopFront() {
    if (!head_->IsLinked()) {
      throw std::runtime_error("pop front from epty list");
    }
    auto val = head_->right_;
    if (val == tail_) {
      tail_ = head_;
    }
    val->Unlink();
  }

  Iterator Begin() {
    return Iterator(&*head_->right_);
  }
  Iterator End() {
    return Iterator(nullptr);
  }


  Iterator IteratorTo(T *element) {
    return Iterator(element);
  }
};

