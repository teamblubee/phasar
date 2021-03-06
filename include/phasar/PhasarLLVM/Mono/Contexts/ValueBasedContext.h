/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Nicolas Bellec and others
 *****************************************************************************/

/*
 * ValueBasedContext.h
 *
 *  Created on: 19.06.2018
 *      Author: nicolas
 */

// Generic implementation, could be far more precise if the analysis
// takes into account the variables as we can reduce the number of
// args copied to match exactly the function arguments + the global variables
// used by the function (+ static variables ?)

#ifndef PHASAR_PHASARLLVM_MONO_CONTEXTS_VALUEBASEDCONTEXT_H_
#define PHASAR_PHASARLLVM_MONO_CONTEXTS_VALUEBASEDCONTEXT_H_

#include <map>
#include <ostream>

// #include <phasar/Config/ContainerConfiguration.h>

#include <phasar/PhasarLLVM/Mono/Contexts/ContextBase.h>

namespace psr {

/*  N = Node in the CFG
 *  Domain = Domain of the results
 */
template <typename N, typename Domain>
class ValueBasedContext
    : public ContextBase<N, Domain, ValueBasedContext<N, Domain>> {
public:
  using Node_t = N;
  using Domain_t = Domain;

protected:
  Domain_t args;
  Domain_t prev_context;

public:
  ValueBasedContext() = default;

  virtual void enterFunction(const Node_t src, const Node_t dest,
                             const Domain_t &In) override {
    prev_context.swap(args); // O(1)
    args = In;               // O(|In|)
  }

  virtual void exitFunction(const Node_t src, const Node_t dest,
                            const Domain_t &In) override {
    args.swap(prev_context); // O(1)
    prev_context.clear();    // O(|prev_context|)
  }

  virtual bool isUnsure() override { return false; }

  virtual bool isEqual(const ValueBasedContext &rhs) const override {
    if (rhs.args.size() != args.size())
      return false;

    return std::equal(args.cbegin(), args.cend(), rhs.args.cbegin());
  }

  virtual bool isDifferent(const ValueBasedContext &rhs) const override {
    return !isEqual(rhs);
  }

  virtual bool isLessThan(const ValueBasedContext &rhs) const override {
    if (args.size() < rhs.args.size())
      return true;

    auto lhs_it = args.cbegin();
    const auto lhs_end = args.cend();

    auto rhs_it = rhs.args.cbegin();
    const auto rhs_end = rhs.args.cend();

    while (lhs_it != lhs_end && rhs_it != rhs_end) {
      if (*lhs_it < *rhs_it)
        return true;
      if (*lhs_it > *rhs_it)
        return false;
      ++lhs_it;
      ++rhs_it;
    }

    return false;
  }

  virtual void print(std::ostream &os) const override {
    // TODO
  }
};

/*  N = Node in the CFG
 *  Key = Key type of the map
 *  Value = Value type of the map
 *  IdGenerator = Generator of Id from the source and destination (in that
 * order) Map = Partial type of Map used. The full type will be Map<Key, Value>
 */
template <typename Node, typename Key, typename Value, typename IdGenerator,
          template <class, class, class...> class Map = std::map>
class MappedValueBasedContext
    : public ContextBase<
          Node, Map<Key, Value>,
          MappedValueBasedContext<Node, Key, Value, IdGenerator, Map>> {
public:
  using Node_t = Node;
  using Key_t = Key;
  using Value_t = Value;
  using IdGen_t = IdGenerator;

  using Domain_t = Map<Key_t, Value_t>;

protected:
  Domain_t args;
  Domain_t prev_context;
  IdGen_t IdGen;

public:
  MappedValueBasedContext() = default;
  MappedValueBasedContext(IdGen_t &_IdGen) : IdGen(_IdGen) {}
  template <class T>
  MappedValueBasedContext(T &&_args, T &&_prev_context)
      : args(std::forward<T>(_args)),
        prev_context(std::forward<T>(_prev_context)) {}

  virtual void enterFunction(const Node_t src, const Node_t dest,
                             const Domain_t &In) override {
    prev_context.swap(args); // O(1)

    args.clear(); // O(|args|)
    auto ids = SrcIdGen(src, dest);

    for (auto id : ids) {
      if (In.count(id) == 0) {
        // An argument is not in the given entry, check if the analysis doesn't
        // become unsound
      }
      args[id] = In[id];
    }
  }

  virtual void exitFunction(const Node_t src, const Node_t dest,
                            const Domain_t &In) override {
    args.swap(prev_context); // O(1)
    prev_context.clear();    // O(|prev_context|)
  }

  virtual bool isUnsure() override { return false; }

  virtual bool isEqual(const MappedValueBasedContext &rhs) const override {
    if (rhs.args.size() != args.size())
      return false;

    return std::equal(args.cbegin(), args.cend(), rhs.args.cbegin());
  }

  virtual bool isDifferent(const MappedValueBasedContext &rhs) const override {
    return !isEqual(rhs);
  }

  virtual bool isLessThan(const MappedValueBasedContext &rhs) const override {
    if (args.size() < rhs.args.size())
      return true;

    auto lhs_it = args.cbegin();
    const auto lhs_end = args.cend();

    auto rhs_it = rhs.args.cbegin();
    const auto rhs_end = rhs.args.cend();

    while (lhs_it != lhs_end && rhs_it != rhs_end) {
      if (*lhs_it < *rhs_it)
        return true;
      if (*lhs_it > *rhs_it)
        return false;
      ++lhs_it;
      ++rhs_it;
    }

    return false;
  }

  virtual void print(std::ostream &os) const override {
    // TODO
  }
};

} // namespace psr

#endif
