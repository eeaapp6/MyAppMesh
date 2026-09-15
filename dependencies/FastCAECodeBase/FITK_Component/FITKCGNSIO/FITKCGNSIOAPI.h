/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCGNSIOAPI.h
 * @brief  网格文件格式cgns读写外部接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-19
 *********************************************************************/
#ifndef FITKCGNSIOAPI_H
#define FITKCGNSIOAPI_H


#include <QtCore/QtGlobal>


#ifdef FITKCGNSIO_API
#define FITKCGNSIOAPI Q_DECL_EXPORT
#else
#define FITKCGNSIOAPI Q_DECL_IMPORT
#endif

#endif // FITKCGNSIOAPI_H
