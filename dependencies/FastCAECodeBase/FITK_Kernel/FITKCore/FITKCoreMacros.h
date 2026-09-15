/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKCoreMacros.h
 * @brief 核心模块宏定义
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef __FITKCIREMACROS_H__
#define __FITKCIREMACROS_H__

/**
 * @brief 前置声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2023-03-28
 */
#define ForwardDeclar( aClass ) class aClass;

 /**
  * @brief 带命名空间的前置声明
  * @author LiBaojun (libaojunqd@foxmail.com)
  * @date 2023-03-28
  */
#define ForwardDeclarNS( aNamespace, aClass ) \
    namespace aNamespace                      \
    {                                         \
        class aClass;                         \
    }


  /**
   * @brief 强制类型转换
   * @author LiBaojun (libaojunqd@foxmail.com)
   * @date 2023-03-28
   */
#define FITKSafeDownCastA( thisClass, superClass )                  \
public:                                                             \
    static thisClass* SafeDownCast##thisClass##( superClass * obj ) \
    {                                                               \
        return dynamic_cast< thisClass* >( obj );                   \
    }

   /**
    * @brief 定义单例
    * @author LiBaojun (libaojunqd@foxmail.com)
    * @date 2023-03-28
    */
#define DeclSingleton( thisClass )        \
private:                                  \
    static thisClass* _instance;          \
    static QMutex m_mutex;                \
    thisClass( ) { this->initialize( ); } \
    ~thisClass( ) {this->finalize(); }    \
                                          \
public:                                   \
    static thisClass* getInstance( )      \
    {                                     \
        QMutexLocker l(&m_mutex);         \
        if ( _instance == nullptr )       \
            _instance = new thisClass;    \
        return _instance;                 \
    }

/**
 * @brief 添加类名判断
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 */
#define  FITKCLASS(NS,thisClass)            \
public:                                  \
virtual bool isFITKClass(const QString& className) override \
{                                        \
    return className == #thisClass;      \
}                                        \
virtual QString getClassName() override  \
{                                        \
    QString sNamespace(#NS);             \
    QString className(#thisClass);       \
    if(sNamespace.isEmpty())             \
        return #thisClass;               \
    return sNamespace+"::"+className;    \
}

#endif
