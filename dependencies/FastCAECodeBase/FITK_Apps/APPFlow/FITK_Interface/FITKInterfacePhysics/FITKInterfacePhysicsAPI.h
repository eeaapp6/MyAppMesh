 /**
 *
 * @file FITKInterfacePhysicsAPI.h
 * @brief  声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-29
 *
 */
#ifndef _FITKINTERFACEPHYSICS_API_H_
#define _FITKINTERFACEPHYSICS_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfacePhysics_API)
#define FITKInterfacePhysicsAPI Q_DECL_EXPORT
#else
#define FITKInterfacePhysicsAPI Q_DECL_IMPORT
#endif

#ifndef FITKInterfacePhysicsVersion
#define FITKInterfacePhysicsVersion "0.2.0"
#endif

#endif
