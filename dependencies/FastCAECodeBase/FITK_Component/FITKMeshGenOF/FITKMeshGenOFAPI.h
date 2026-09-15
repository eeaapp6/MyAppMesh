/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKMeshGenOFAPI.h
 * @brief 接口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-17
 *
 */
#ifndef _FITK_MESH_GEN_OF_API_H_
#define _FITK_MESH_GEN_OF_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKMeshGenOF_API)
#define FITKMeshGenOFAPI Q_DECL_EXPORT
#else
#define FITKMeshGenOFAPI Q_DECL_IMPORT
#endif

#endif
