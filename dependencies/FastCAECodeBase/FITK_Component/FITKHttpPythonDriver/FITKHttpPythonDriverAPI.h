/**
 * @file FITKHttpPythonDriverAPI.h
 * @brief FITKHttpPythonDriver 组件导出宏定义
 * @author libaojunAI
 * @date 2026-05-26
 */
#ifndef __FITK_HTTP_PYTHON_DRIVER_API_H__
#define __FITK_HTTP_PYTHON_DRIVER_API_H__

#include <QtCore/QtGlobal>

#ifdef FITKHttpPythonDriver_API
#define FITKHTTPPYTHONDRIVERAPI Q_DECL_EXPORT
#else
#define FITKHTTPPYTHONDRIVERAPI Q_DECL_IMPORT
#endif

#ifndef FITKHTTPPYTHONDRIVER_VERSION
#define FITKHTTPPYTHONDRIVER_VERSION "1.0.0"
#endif

#endif
