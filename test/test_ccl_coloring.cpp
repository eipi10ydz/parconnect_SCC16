/**
 * @file    check_ccl_coloring_undirected.cpp
 * @ingroup group
 * @author  Chirag Jain <cjain7@gatech.edu>
 * @brief   GTest Unit Tests for connected component labeling using coloring
 *
 * Copyright (c) 2015 Georgia Institute of Technology. All Rights Reserved.
 */

#include <mpi.h>

#include "coloring/labelProp.hpp"
#include "mxx/comm.hpp"
#include "gtest.h"

INITIALIZE_EASYLOGGINGPP

/**
 * @brief       coloring of undirected graph with 3 components
 * @details     builds a small test graph with three components, 
 *              test if program returns 3 as the component count
 */
TEST(connColoring, smallUndirected) {

  mxx::comm c = mxx::comm();

  //Declare a edgeList vector to save edges
  std::vector< std::pair<int64_t, int64_t> > edgeList;

  using edgeIdType = int64_t;
  using pIdType = uint32_t;

  //Start adding the edges
  if (c.rank() == 0) {

    //First component (2,3,4,11)
    //2--11
    edgeList.emplace_back(2,11);
    edgeList.emplace_back(11,2);

    //2--3
    edgeList.emplace_back(2,3);
    edgeList.emplace_back(3,2);

    //2--4
    edgeList.emplace_back(2,4);
    edgeList.emplace_back(4,2);

    //3--4
    edgeList.emplace_back(3,4);
    edgeList.emplace_back(4,3);

    //Second component (5,6,8,10)
    //5--6
    edgeList.emplace_back(5,6);
    edgeList.emplace_back(6,5);

    //5--8
    edgeList.emplace_back(5,8);
    edgeList.emplace_back(8,5);

    //6--10
    edgeList.emplace_back(6,10);
    edgeList.emplace_back(10,6);

    //6--8
    edgeList.emplace_back(6,8);
    edgeList.emplace_back(8,6);

    //Third component (chain 50-51-...1000)
    for(int i = 50; i < 52 ; i++)
    {
      edgeList.emplace_back(i, i+1);
      edgeList.emplace_back(i+1, i);
    }
  }

  std::random_shuffle(edgeList.begin(), edgeList.end());

  //Since we have a graph of small size, use less processes
  c.with_subset(c.rank() < 4, [&](const mxx::comm& comm){
      conn::coloring::ccl<pIdType, edgeIdType> cclInstance(edgeList, comm);
      cclInstance.compute();
      auto component_count = cclInstance.getComponentCount();
      ASSERT_EQ(3, component_count);
      });
}

/**
 * @brief       coloring of undirected graph with 3 components
 * @details     builds a medium test graph with three components, 
 *              test if program returns 3 as the component count
 */
TEST(connColoring, mediumUndirected) {

  mxx::comm c = mxx::comm();

  //Declare a edgeList vector to save edges
  std::vector< std::pair<int64_t, int64_t> > edgeList;

  using edgeIdType = int64_t;
  using pIdType = uint32_t;

  //Start adding the edges
  if (c.rank() == 0) {

    //First component (2,3,4,11)
    //2--11
    edgeList.emplace_back(2,11);
    edgeList.emplace_back(11,2);

    //2--3
    edgeList.emplace_back(2,3);
    edgeList.emplace_back(3,2);

    //2--4
    edgeList.emplace_back(2,4);
    edgeList.emplace_back(4,2);

    //3--4
    edgeList.emplace_back(3,4);
    edgeList.emplace_back(4,3);

    //Second component (5,6,8,10)
    //5--6
    edgeList.emplace_back(5,6);
    edgeList.emplace_back(6,5);

    //5--8
    edgeList.emplace_back(5,8);
    edgeList.emplace_back(8,5);

    //6--10
    edgeList.emplace_back(6,10);
    edgeList.emplace_back(10,6);

    //6--8
    edgeList.emplace_back(6,8);
    edgeList.emplace_back(8,6);

    //Third component (chain 50-51-...1000)
    for(int i = 50; i < 1000 ; i++)
    {
      edgeList.emplace_back(i, i+1);
      edgeList.emplace_back(i+1, i);
    }
  }

  std::random_shuffle(edgeList.begin(), edgeList.end());
  conn::coloring::ccl<pIdType, edgeIdType> cclInstance(edgeList, c);
  cclInstance.compute();
  auto component_count = cclInstance.getComponentCount();
  ASSERT_EQ(3, component_count);
}

