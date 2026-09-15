/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

 
#ifndef _FITKAbaqusRadiossDataMapper_API_API_H_
#define _FITKAbaqusRadiossDataMapper_API_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKAbaqusRadiossDataMapper_API)
#define FITKAbaqusRadiossDataMapperAPI Q_DECL_EXPORT
#else
#define FITKAbaqusRadiossDataMapperAPI Q_DECL_IMPORT
#endif

#ifndef FITKAbaqusRadiossDataMapperVersion
#define FITKAbaqusRadiossDataMapperVersion "0.1.1"
#endif

#endif
