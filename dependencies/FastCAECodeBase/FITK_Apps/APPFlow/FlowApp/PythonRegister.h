/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/


#ifndef _PY_REGISTER_H___
#define _PY_REGISTER_H___

#include "FITK_Kernel/FITKAppFramework/FITKAbstractPythonRegister.h"

namespace Python {
    class FITKPythonInterface;
}

 

/**
 * @brief python接口注册器声明
 */
class PyRegister : public AppFrame::FITKAbstractPythonRegister
{
public:
    /**
     * @brief 构造函数
     */
    explicit PyRegister();

    /**
     * @brief 析构函数
     */
    virtual ~PyRegister() = default;

    /**
     * @brief 注册Python装饰器与类
     */
    void registWapper() override;

private:
    /**
     * @brief Python接口代理器
     */
    Python::FITKPythonInterface* _pyInterface{};
};

#endif
