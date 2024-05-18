#include <cmath>
#include <iterator>
#include <iostream>
#include <climits>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h*h) + (s*s) + (l*l));
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
   work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    work_list.push_front(point);
  }

  /**
  * Removes the current_ Point in the bfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if (!work_list.empty()) {
   work_list.pop_front(); 
   }
  
  }

  /**
  * Removes the current_ Point in the dfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if (!work_list.empty()) {
    work_list.pop_front(); 
  }
  }

  /**
   * Returns the current_ Point in the bfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current_ Point 
   */
  Point bfs_peek(std::deque<Point> & work_list) {
    if (!work_list.empty()) {
    return work_list.front(); // Next point for BFS.
  }
  return Point(0,0);
  }

  /**
   * Returns the current_ Point in the dfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current_ Point 
   */
  Point dfs_peek(std::deque<Point> & work_list) {
    if (!work_list.empty()) {
    return work_list.front(); 
  }
  return Point(0,0); 
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current_ point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param fns the set of functions describing a traversal's operation
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {
    iter.png_ = png;
    iter.start_ = start;
    iter.tolerance_ = tolerance;
    iter.functions_ = fns;
    iter.current_ = start;
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {

    ImageTraversal myiter(iter.png_,iter.start_,iter.tolerance_,iter.functions_);
    myiter.iter.visited.resize(iter.png_.width()*iter.png_.height());
    return myiter.iter;
 
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    Iterator myiter;
    myiter.current_ = Point(-1,-1);
    return myiter;
  }
  
  // my own iterator constructor 
//   Iterator(const PNG& png, const Point& start, double tolerance, TraversalFunctions fns)
// : png_(png), start_(start), tolerance_(tolerance), functions_(fns), current_(start) {}

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
  // nothing 
  
}

 bool ImageTraversal::Iterator::helper(Point p){
  unsigned width = png_.width();
  unsigned height = png_.height();

    if (p.x < width && p.y < height && p.x >= 0 && p.y >= 0) {
        double delta = calculateDelta(png_.getPixel(p.x, p.y), png_.getPixel(start_.x, start_.y));
        if (delta < tolerance_) {
            return !visited
            [p.y * width + p.x];
        }
    }
    return false;
  }

/**
  * Iterator increment operator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  
visited[current_.y * png_.width() + current_.x ] = true;

    Point neighbors[4] = {
        Point(current_.x + 1, current_.y), // Right
        Point(current_.x , current_.y + 1), // Below
        Point(current_.x - 1, current_.y), // Left
        Point(current_.x, current_.y - 1)  // Above
    };
// for looping
    for (auto &neighbor : neighbors) {
        if (helper(neighbor)) {
            functions_.add(work_list_, neighbor);
        }
    }
// search for next point 

    do {
        if (work_list_.empty()) {
            current_ = Point(-1, -1); 
            return *this;
        }
        current_ = work_list_.front();
        functions_.pop(work_list_);
    } while (visited[current_.y * png_.width() + current_.x ]); 

    return *this;
}
   
 

  /**
  * Iterator accessor operator.
  *
  * Accesses the current_ Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
     return current_;
  }


  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
     return current_.x != other.current_.x || current_.y != other.current_.y;
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
   return work_list_.empty();
  }
  
}
