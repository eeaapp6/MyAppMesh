/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

 /**********************************************************************
  * @file   FITKGmshMshIOAPI.h
  * @brief  外部接口声明
  * @author liuzhonghua (liuzhonghuaszch@163.com)
  * @date   2024-11-28
  *********************************************************************/
#ifndef _FITKIGMSH_MSHIO_API_H_
#define _FITKIGMSH_MSHIO_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKGmshMshIO_API)
#define FITKGmshMshIOAPI Q_DECL_EXPORT
#else
#define FITKGmshMshIOAPI Q_DECL_IMPORT
#endif

#ifndef FITKGMSHMSHIOVERSION
#define FITKGMSHMSHIOVERSION "2.0.0"
#endif

#endif // _FITKIGMSH_MSHIO_API_H_