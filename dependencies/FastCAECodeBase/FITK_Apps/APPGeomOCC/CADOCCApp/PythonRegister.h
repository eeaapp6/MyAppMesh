/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file PythonRegister.h
 * @brief Python接口注册
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-10-17
 *
 */
#ifndef _PY_REGISTER_H___
#define _PY_REGISTER_H___

#include "FITK_Kernel/FITKAppFramework/FITKAbstractPythonRegister.h"

namespace Python
{
    class FITKPythonInterface;
}

/**
 * @brief python接口注册器声明
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-10-17
 */
class  PyRegister : public  AppFrame::FITKAbstractPythonRegister
{
public:
     /**
      * @brief Construct a new Py Register object
      * @author fulipeng (fulipengqd@yeah.net)
      * @date 2024-10-17
      */
    explicit PyRegister();
    /**
     * @brief Destroy the Py Register object
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-10-17
     */
    virtual ~PyRegister() = default;
    /**
     * @brief 注册Python装饰器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-10-17
     */
    void registWapper() override;

private:
    /**
     * @brief Python代理器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-10-17
     */
    Python::FITKPythonInterface* _pyInterface{};
};

#endif
