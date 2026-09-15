/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

 
#ifndef _FITKEASTPARAM_API_H_
#define _FITKEASTPARAM_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKEasyParam_API)
#define FITKEasyParamAPI Q_DECL_EXPORT
#else
#define FITKEasyParamAPI Q_DECL_IMPORT
#endif

#ifndef FITKEasyParamVersion
#define FITKEasyParamVersion "1.0.0"
#endif

#endif
