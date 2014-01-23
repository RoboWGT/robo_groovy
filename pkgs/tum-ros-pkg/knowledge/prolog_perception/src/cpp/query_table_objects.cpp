/*
 * Copyright (c) 2009 Dejan Pangercic <pangercic -=- cs.tum.edu>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: query_table_objects.cpp 17089 2009-06-15 18:52:12Z pangercic $
 *
 */

/**
@mainpage

@htmlinclude manifest.html

@query_table_objects is a middleware between (ros)prolog and ros nodes.
Compilled into a shared library it
in general:
a)accepts prolog foreign function calls and
b)invokes ros ServiceClient calls

and in particular:
c)returns unique identifiers for found tables and clusters located on tables
in a PointCloud 
Return type:
[table_seq, [[cl_x, cl_y, cl_z], [], [], ...]
**/



#include "ros/ros.h"
#include "ros/node_handle.h"
#include <mapping_srvs/FindTableId.h>
#include <cstdlib>
#include <ctype.h>
#include <SWI-Prolog.h>
extern "C"
{
  using namespace mapping_srvs;    

  foreign_t
  pl_getPlaneClustersROS(term_t l)    
  {
    int argc;
    argc=1;
    char **argv;
    argv = (char **)malloc(sizeof(char*));
    argv[0] = (char *)malloc(sizeof(char) *2);
    argv[0][0] = 'a';
    argv[0][1] = '\0';
    ros::init(argc, argv, "query_table_objects");
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<FindTableId>("/table_object_detector_sick/table_object_detector");
    FindTableId srv;
    term_t tmp = PL_new_term_ref();
    float cluster[3];
    //    ROS_DEBUG(" srv.response.table.header.seq %u",  srv.response.table.header.seq);
    if (client.call(srv))
      {
        ROS_DEBUG("srv.response.id %ld",  srv.response.id);
        //add response variables of choice
        if (!PL_unify_list(l, tmp, l) ||
         !PL_unify_integer(tmp, srv.response.id))
        PL_fail;
        //TODO: integrate table time stamps
        //ROS_DEBUG("Table seq: %d",  srv.response.table.header.seq);
        // if (!PL_unify_list(l, tmp, l) ||
        //    !PL_unify_float(tmp,  float(srv.response.table.header.stamp)))
        //  PL_fail;
        //ROS_DEBUG("Table stamp: %f",  srv.response.table.header.stamp);

        for (unsigned int j = 0; j < srv.response.table.objects.size(); j++)
          {
            cluster[0] = srv.response.table.objects[j].center.x;
            cluster[1] = srv.response.table.objects[j].center.y;
            cluster[2] = srv.response.table.objects[j].center.z;
            ROS_DEBUG("cluster %f, %f, %f",  cluster[0], cluster[1], cluster[2]);
            for (int i = 0; i < 3; i++)
              {
                if(!PL_unify_list(l, tmp, l) ||  !PL_unify_float(tmp, cluster[i]))
                  {
                    PL_fail;
                    break;
                  }
              }
          }      
        return PL_unify_nil(l);
      }
    else
      {
        ROS_ERROR("Failed to call service /table_object_detector_sick/table_object_detector");
        return -1;
      }
    free(argv[0]);
    free(argv);
    return 0;
  }

  /////////////////////////////////////////////////////////////////////////////
  // register foreign functions
  ////////////////////////////////////////////////////////////////////////////
  install_t
  install()
  { 
    PL_register_foreign("getPlaneClustersROS", 1, (void *)pl_getPlaneClustersROS, 0);
  }
}
