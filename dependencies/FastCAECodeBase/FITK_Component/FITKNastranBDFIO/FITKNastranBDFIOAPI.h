/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

 
#ifndef _FITKIBASTRAN_BDFIO_API_H_
#define _FITKIBASTRAN_BDFIO_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKNastranBDFIO_API)
#define FITKNastranBDFIOAPI Q_DECL_EXPORT
#else
#define FITKNastranBDFIOAPI Q_DECL_IMPORT
#endif

#ifndef FITKNastranBDFIOVersion 
#define FITKNastranBDFIOVersion "1.0.1"
#endif

#endif
