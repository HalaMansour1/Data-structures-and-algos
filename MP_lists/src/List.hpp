/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0 ;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(NULL, tail_);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
   ListNode* current = head_;
        ListNode* next;

        while (current != nullptr) {
            next = current->next;
            delete current;
            current = next;
        }

        // After deletion, make sure to set head to nullptr
        head_ = nullptr;

}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  
  if(head_ == NULL){
    newNode->prev = nullptr;
    newNode->next = nullptr;
   head_ = newNode ; 
   tail_ = head_; 

  }else{
    newNode->prev = nullptr;
    newNode->next = head_;
    head_ -> prev = newNode; 
    head_ = newNode ;
  }
   

  // if (head_ != NULL) {
  //   head_ -> prev = newNode;
  // }
  // head_ = newNode ;
  

  length_++;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {


  ListNode * newNode = new ListNode(ndata);
  newNode -> next = NULL;
  newNode -> prev = tail_;
  if(head_ == NULL) {
    tail_ = newNode ;
    head_ = newNode ; 
  }else if (tail_ != NULL) {
    tail_->next = newNode ; 
    // newNode->prev = tail_;
    // newNode->next = NULL;
    tail_ = newNode; 
  }
   length_++;
  //  cout << head_->data << endl;
  //  cout << tail_->data << endl;

}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
 // No list
  ListNode* current = start;
    if (start == nullptr) {
        return nullptr;
    }

    if (splitPoint <= 0) {
        return start;
    }


    for (int i = 0; i < splitPoint && current != nullptr; ++i) {
        current = current->next;
    }

    if (current != nullptr) {
        // Update 
        if (current->prev != nullptr) {
            current->prev->next = nullptr;
        }

        current->prev = nullptr;
        return current;
    }
   return NULL;

}


/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
if (length_ == 0 || length_ == 1) {
  return;
}
ListNode *curr = head_->next;

while (curr != nullptr && curr->next != nullptr) {
    ListNode *temp = curr->next->next;

    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    curr->prev = tail_;
    curr->next = nullptr;
    tail_->next = curr;
    tail_ = curr;

    curr = temp;
}
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  if (startPoint == endPoint  || endPoint == nullptr ||startPoint == nullptr) {
    return;
  }

  ListNode* temp;
  ListNode* curr = startPoint;
  ListNode* endtemp = endPoint->next;
  //loop thru 
   ListNode* starttemp = startPoint->prev;
  // cout<< starttemp <<endl;
  ListNode* Start = startPoint;
  ListNode* End = endPoint;
  while (curr != endtemp) {
    temp = curr->prev;
    curr->prev = curr->next;
    curr->next = temp;
    curr = curr->prev;
  }  
  End->prev = starttemp;
  Start->next = endtemp;

  // edge case checks 
  if (endtemp != nullptr) {
    endtemp->prev = Start;
  }
  if (starttemp!= nullptr) {
    starttemp->next = End;
  } 
  // cout<< starttemp <<endl;
  if (endtemp != nullptr) {
    endtemp->prev = Start;
  }
   endPoint = Start;
  startPoint = End;
  // cout<< Start<<endl;
  // cout<< End <<endl;
  if (tail_ ==End) {
    tail_ = Start; 
  }
  if (head_ == Start) {
    head_ = End;
  }
  
}





/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
 template <typename T>
 void List<T>::reverseNth(int n) {
  if (n <= 0 || head_ == nullptr) {
    return;
  }
  int Start = 0;
  int End = 0;
  ListNode *startPoint = nullptr;
  ListNode *endPoint = nullptr ;

  while (End != length_) {
    Start = End + 1;
    End = End + n;
    if (End > length_) {
      End = length_;
    }
    ListNode *cur = head_;
    //look for n starttig point
    for (int i = 1; i < Start && cur != nullptr; i++) { 
      cur = cur->next; 
    }
    // add nullptr caase for every cur iteration
    if (cur == nullptr) break; 
    startPoint = cur;

    cur = head_;
    for (int i = 1; i < End && cur != nullptr; i++) { 
      cur = cur->next; 
    }
    if (cur == nullptr) break; 
    endPoint = cur;

    // reverse the nodes
    reverse(startPoint, endPoint);
  }
}



/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
   if (!first) return second;
  if (!second) return first;

  ListNode* merged = nullptr;
  ListNode* tail = nullptr;

  while (first && second) {
    ListNode*& smaller = (first->data < second->data) ? first : second;
    
    if (!merged) {
      merged = tail = smaller;
    } else {
      tail->next = smaller;
      smaller->prev = tail;
      tail = smaller;
    }

    smaller = smaller->next;
  }

  tail->next = (first) ? first : second;
  if (tail->next) tail->next->prev = tail;

  return merged;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in mp_lists part 2
   if (start == nullptr || start->next == nullptr) {
    return start;
  }
  int s = chainLength / 2;
  ListNode* secondHalf = split(start, s);

  ListNode* sortedFirstHalf = mergesort(start, s);
  ListNode* sortedSecondHalf = mergesort(secondHalf, chainLength - s);
  return merge(sortedFirstHalf, sortedSecondHalf);
}
