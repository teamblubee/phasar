/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * AbstractEdgeFunction.h
 *
 *  Created on: 04.08.2016
 *      Author: pdschbrt
 */

#ifndef PHASAR_PHASARLLVM_IFDSIDE_EDGEFUNCTION_H_
#define PHASAR_PHASARLLVM_IFDSIDE_EDGEFUNCTION_H_

#include <iostream> // std::cout in dump, better replace it with a ostream
#include <memory>
#include <string>

namespace psr {

template <typename V> class EdgeFunction {
public:
  virtual ~EdgeFunction() = default;

  virtual V computeTarget(V source) = 0;

  virtual std::shared_ptr<EdgeFunction<V>>
  composeWith(std::shared_ptr<EdgeFunction<V>> secondFunction) = 0;

  virtual std::shared_ptr<EdgeFunction<V>>
  joinWith(std::shared_ptr<EdgeFunction<V>> otherFunction) = 0;

  virtual bool equal_to(std::shared_ptr<EdgeFunction<V>> other) const = 0;

  virtual void print(std::ostream &OS, bool isForDebug = false) const {
    OS << "edge_function";
  }

  std::string toString() { return "edge function"; }
};

template <typename V>
static inline bool operator==(const EdgeFunction<V> &F,
                              const EdgeFunction<V> &G) {
  return F.equal_to(G);
}

template <typename V>
static inline std::ostream &operator<<(std::ostream &OS,
                                       const EdgeFunction<V> &F) {
  F.print(OS);
  return OS;
}

} // namespace psr

#endif
