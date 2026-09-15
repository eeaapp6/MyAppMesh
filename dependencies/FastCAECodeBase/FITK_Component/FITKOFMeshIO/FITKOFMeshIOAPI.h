/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKOFMESHIOAPI_H
#define FITKOFMESHIOAPI_H


#include <QtCore/QtGlobal>


#ifdef FITKOFMeshIO_API
#define FITKOFMeshIOAPI Q_DECL_EXPORT
#else
#define FITKOFMeshIOAPI Q_DECL_IMPORT
#endif

#endif // FITKOFMESHIOAPI_H
