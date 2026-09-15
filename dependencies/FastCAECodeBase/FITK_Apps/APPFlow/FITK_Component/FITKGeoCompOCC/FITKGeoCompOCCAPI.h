/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

 
#ifndef _FITKGeoCompOCC_API_API_H_
#define _FITKGeoCompOCC_API_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKGeoCompOCC_API)
#define FITKGeoCompOCCAPI Q_DECL_EXPORT
#else
#define FITKGeoCompOCCAPI Q_DECL_IMPORT
#endif

#ifndef FITKGEOCOMPOCCAPIVERSION
#define FITKGEOCOMPOCCAPIVERSION "1.0.0"
#endif

#endif // _FITKGeoCompOCC_API_API_H_