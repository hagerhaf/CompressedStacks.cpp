#ifndef COMPONENT
#define COMPONENT

/*==============================================================================
  Includes
==============================================================================*/
#include "sign.hpp"
#include "stack.hpp"
#include <memory>
#include <string>

/*==============================================================================
  Class      : template (T context, D datas)
  Extensions :
  Aliases    :
  Friends   -> CompressedStack
            <- Data, Signature
==============================================================================*/
template <class T, class D>
class CompressedStack; // Required for the friendship
template <class T, class D> class Component {
  friend class CompressedStack<T, D>;

private:
  Component<T, D>(int space, int depth);

  // Setters
  void clearExplicit(int space);

  // Push and pop
  void pushExplicit(SPData<T, D> elt);
  void push(Signature<T, D> sign, int lvl);
  Data<T, D> top();
  Signature<T, D> top(int lvl);
  int topIndex(int lvl = 0);

  // IO
  std::string toString();

  // State
  bool isempty();
  bool isempty(int lvl);
  bool isExplicitEmpty();
  bool single(int lvl);

  Levels<T, D> mPartial;
  ExplicitPointer<T, D> mExplicit;
  Signature<T, D> mSign;
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D>
Component<T, D>::Component(int space, int depth)
    : mSign(0, std::streampos(0), std::shared_ptr<T>(nullptr),
            Buffer<T, D>(0)) {

  Levels<T, D> partial = initLevels<T, D>(space, depth);
  mPartial = partial;

  ExplicitPointer<T, D> xplicit = initExplicitPointer<T, D>();
  xplicit.reserve(space);
  mExplicit = xplicit;
}

/*==============================================================================
  IO : levelsToStringInComponent, toString
==============================================================================*/
template <class T, class D> std::string levelsToString(Levels<T, D> levels) {
  std::string str;
  str = "";
  int index = 0;
  for (typename Levels<T, D>::iterator it = levels.begin(); it != levels.end();
       ++it) {
    index++;
    str += "\t\t\tLevel " + std::to_string(index) + "   ->";
    str += " " + blockToString(*it) + "\n";
  }
  return str;
}

template <class T, class D> std::string Component<T, D>::toString() {
  std::string str;
  str = levelsToString(mPartial);
  str += "\t\t\tExplicit  -> ";
  str += explicitPointerToString(mExplicit);
  str += "\n\t\t\tSignature -> ";
  str += mSign.toString() + "\n";
  return str;
}

/*==============================================================================
  Stack Functions: push, pop, top, topIndex, isempty, isExplicitEmpty
==============================================================================*/
template <class T, class D> bool Component<T, D>::isempty() {
  bool b = true;
  for (int lvl = 0; lvl <= int(mPartial.size()); lvl++) {
    b = b && isempty(lvl);
  }
  return b;
}

template <class T, class D> bool Component<T, D>::isempty(int lvl) {
  bool b;
  if (lvl >= int(mPartial.size())) {
    b = isExplicitEmpty();
  } else {
    b = (mPartial[lvl]).empty();
  }
  return b;
}

template <class T, class D> bool Component<T, D>::isExplicitEmpty() {
  return (mExplicit.empty());
}

template <class T, class D>
void Component<T, D>::pushExplicit(SPData<T, D> elt) {
  mExplicit.push_back(elt);
}
template <class T, class D>
void Component<T, D>::push(Signature<T, D> sign, int lvl) {
  mPartial[lvl].push_back(sign);
}

template <class T, class D> Data<T, D> Component<T, D>::top() {
  return *(mExplicit.back());
}
template <class T, class D> Signature<T, D> Component<T, D>::top(int lvl) {
  return mPartial[lvl].back();
}

template <class T, class D> int Component<T, D>::topIndex(int lvl) {
  if (lvl == 0) {
    return top().mIndex;
  } else {
    return top(lvl).mLast;
  }
}

template <class T, class D> bool Component<T, D>::single(int lvl) {
  if (mPartial[lvl].empty()) {
    return false;
  }
  return (mPartial[lvl].back().single());
}

/*==============================================================================
  Setters
==============================================================================*/
template <class T, class D> void Component<T, D>::clearExplicit(int space) {
  mExplicit.clear();
  mExplicit.reserve(space);
}

#endif /* COMPONENT */
