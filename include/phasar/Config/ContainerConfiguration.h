/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * ContainerConfiguration.h
 *
 *  Created on: 02.05.2017
 *      Author: philipp
 */

#ifndef PHASAR_CONFIG_CONTAINER_CONFIGURATION_H_
#define PHASAR_CONFIG_CONTAINER_CONFIGURATION_H_

#include <map>
#include <set>

#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/small_vector.hpp>

namespace psr {
// check if we forgot some more useful container implementations

// define the std::set implementation to use for the ICFG classes
// ------------------
template <typename T> using ICFGSet = boost::container::flat_set<T>;
// ----------------------------------------------------------------------------

// define the std::set implementation to use for the flow functions
// ----------------
#define FFSetPreAllocSize 10

template <typename T>
using FFSet = boost::container::small_vector<T, FFSetPreAllocSize>;
// ----------------------------------------------------------------------------

// define the std::map implementation to use for the special summaries
// -------------
template <typename T, typename U>
using SSMap = boost::container::flat_map<T, U>;
// ----------------------------------------------------------------------------

// define the std::map implementation to use for the dynamic summaries
// -------------
template <typename T, typename U>
using DSMap = boost::container::flat_map<T, U>;
// ----------------------------------------------------------------------------

// MonotoneSolver related container implementations ---------------------------
// define the std::map implementation to use within the MonotoneSolver.hh
// ----------
template <typename T, typename U>
using MonoMap = std::map<T, U>; // boost::container::flat_map<T, U>;
// ----------------------------------------------------------------------------
// define the std::set implementation to use within the MonotoneSolver.hh
// ----------
template <typename T>
using MonoSet = std::set<T>; // boost::container::flat_set<T>;
// ----------------------------------------------------------------------------

} // namespace psr

#endif
