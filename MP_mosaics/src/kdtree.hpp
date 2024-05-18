/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>
#include <iterator>
#include <cmath>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second, int curDim)
{
 if (first[curDim] < second[curDim]) {
        return true;
    } else if (second[curDim] < first[curDim]) {
        return false;
    } else {
    // breaking ties 
        return first < second;
    }
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,  Point<Dim>& currentBest, const Point<Dim>& potential)
{
    double currentBestDist = 0.0;
    double potentialDist = 0.0;
    for (int i = 0; i < Dim; ++i)
    {
        double diffCB = currentBest[i] - target[i];
        double diffPB = potential[i] - target[i];

        currentBestDist += diffCB * diffCB;
        potentialDist += diffPB * diffPB;
    }

    if (potentialDist < currentBestDist || (potentialDist == currentBestDist && potential < currentBest))
    {
        return true;
    }
    else
    {
        return false;
    }
}




template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
   std::vector<Point<Dim>> points = newPoints; 
    root = buildTree(points, 0, 0, points.size() - 1);
}
  //helper

template<int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(std::vector<Point<Dim>>& points, int depth, int start, int end) {
    if (start > end) {
        return nullptr; // Base 
    }

    int dimension = depth % Dim;
    int medianIndex = (start + end) / 2;

    // Lambda function 
    auto cmp = [dimension](const Point<Dim>& a, const Point<Dim>& b) {
        return a[dimension] < b[dimension] || (a[dimension] == b[dimension] && a < b);
    };
    select(points.begin() + start, points.begin() + end + 1, points.begin() + medianIndex, cmp);

    KDTreeNode* node = new KDTreeNode(points[medianIndex]);
    node->left = buildTree(points, depth + 1, start, medianIndex - 1);
    node->right = buildTree(points, depth + 1, medianIndex + 1, end);

    return node;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other)  {
   root = copyhelper(other.root);
}
//helper 
template<int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copyhelper(const KDTreeNode* node) {
    if (node == nullptr) {
        return nullptr; 
    }
    // Create a new node copy
    KDTreeNode* newNode = new KDTreeNode(node->point);
    newNode->left = copyhelper(node->left); // left subtree
    newNode->right = copyhelper(node->right); //right subtree
    return newNode;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  if (this != &rhs) {
        deleter(root);
        // Copy 
        root = copyhelper(rhs.root);
    }
    return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  deleter(root);
}
// helper
template <int Dim>
void KDTree<Dim>::deleter(typename KDTree<Dim>::KDTreeNode*& current) {
    if (current == nullptr) {
        return;
    }
    deleter(current->left);
    deleter(current->right);
    delete current;
    current = nullptr;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
   if (!root){
    return Point<Dim>();
   }
    return Nearesthelper(root, query, 0);
}
// helpers 
template <int Dim>
Point<Dim> KDTree<Dim>::Nearesthelper(KDTreeNode* node, const Point<Dim>& query, int depth) const {
    if (!node) return Point<Dim>();

    Point<Dim> currentBest;
    if (!node->left && !node->right) { // Leaf node
        return node->point;
    }

    int dim = depth % Dim;
    bool goLeft = query[dim] < node->point[dim];
    if (goLeft) {
        currentBest = node->left ? Nearesthelper(node->left, query, depth + 1) : node->point;
    } else {
        currentBest = node->right ? Nearesthelper(node->right, query, depth + 1) : node->point;
    }

    // Update current best 
    if (Replace(query, currentBest, node->point)) {
        currentBest = node->point;
    }

    // Check if we need to explore the other subtree
    double radius = 0;
    for (int i = 0; i < Dim; i++) {
        radius += (query[i] - currentBest[i]) * (query[i] - currentBest[i]);
    }
    double split = (query[dim] - node->point[dim]) * (query[dim] - node->point[dim]); // splitting distance

    if (split <= radius) {
        Point<Dim> otherBest = goLeft && node->right ? Nearesthelper(node->right, query, depth + 1) :
                               !goLeft && node->left ? Nearesthelper(node->left, query, depth + 1) : currentBest;
        if (Replace(query, currentBest, otherBest)) {
            currentBest = otherBest;
        }
    }

    return currentBest;
}

template <int Dim>
bool KDTree<Dim>::Replace(const Point<Dim>& target, const Point<Dim>& currentBest, const Point<Dim>& potential) const {
    double currentDist = 0, potentialDist = 0;
    for (int i = 0; i < Dim; ++i) {
        currentDist += (currentBest[i] - target[i]) * (currentBest[i] - target[i]);
        potentialDist += (potential[i] - target[i]) * (potential[i] - target[i]);
    }
    return potentialDist < currentDist || (potentialDist == currentDist && potential < currentBest);
}


template<typename RandIter, typename Comparator>
RandIter Partition(RandIter begin, RandIter end, RandIter pivot, Comparator cmp) {
    auto pivotValue = *pivot;
    std::iter_swap(pivot, end - 1);
    RandIter storeIndex = begin;
    for (RandIter it = begin; it != end - 1; ++it) {
        if (cmp(*it, pivotValue)) {
            std::iter_swap(it, storeIndex);
            ++storeIndex;
        }
    }
    std::iter_swap(storeIndex, end - 1);
    return storeIndex;
}

template<typename RandIter, typename Comparator>
void select(RandIter begin, RandIter end, RandIter k, Comparator cmp) {
    if (begin >= end) return; 
    // our pivot to be selecting on 
    RandIter pivot = begin; 

    // Partition and find new pivot position
    RandIter newPivot = Partition(begin, end, pivot, cmp);

    // Recurse accordingly
    if (k == newPivot) {
        // Found kth smallest
        return;
    } else if (k < newPivot) {
        select(begin, newPivot, k, cmp); // left
    } else {
        select(newPivot + 1, end, k, cmp); // right
    }
}
