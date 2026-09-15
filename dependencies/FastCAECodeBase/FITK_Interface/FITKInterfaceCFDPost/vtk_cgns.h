/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*=========================================================================

  Program:   ParaView
  Module:    vtk_cgns.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtk_cgns_h
#define vtk_cgns_h

/* Use the cgns library configured for ParaView.  */
#define VTK_MODULE_USE_EXTERNAL_ParaView_cgns 1

#if VTK_MODULE_USE_EXTERNAL_ParaView_cgns
# include <cgnslib.h>
# include <cgns_io.h>
#else
# include <vtkcgns/src/cgnslib.h>
# include <vtkcgns/src/cgns_io.h>
#endif

#endif
