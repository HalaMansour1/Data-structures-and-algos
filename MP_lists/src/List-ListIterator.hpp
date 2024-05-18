
class ListIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
  private:
    // @TODO: graded in mp_lists part 1
    ListNode* position_;
    ListNode* tail_;

  public:
    ListIterator() : position_(NULL) { }
    ListIterator(ListNode* x) : position_(x) { }
    ListIterator(ListNode* x, ListNode* y) : tail_(y) { 
        position_ = x;
    }
    


    // Pre-Increment, ++iter
    ListIterator& operator++() {
    
     position_ = position_->next;
    
    
    return *this;
    }
    
    // Post-Increment, iter++
    ListIterator operator++(int) {
        
    ListNode * temp = position_;
   
    position_ = position_->next;
    
    return ListIterator(temp);
    }

    // Pre-Decrement, --iter
    ListIterator& operator--() {

        if (position_ == NULL) {
            position_ = tail_;
        } else if (position_ != nullptr) {
        position_ = position_->prev;
    }
    return *this;
      
    }
        // get tail from constructor and if psotion not equal to null pointer then head = taill 
    // Post-Decrement, iter--
    ListIterator operator--(int) {

    ListNode * temp = position_;
    if (position_ == NULL){
            position_ = tail_; 
     } else {
        position_ = position_->prev;
    }
    return ListIterator(temp);
   
    }

    bool operator!=(const ListIterator& rhs) {
        return position_ != rhs.position_;
    }

    bool operator==(const ListIterator& rhs) {
        return !(*this != rhs);
    }

    const T& operator*() {
        return position_->data;
    }

    const T* operator->() {
        return &(position_->data);
    }
};
