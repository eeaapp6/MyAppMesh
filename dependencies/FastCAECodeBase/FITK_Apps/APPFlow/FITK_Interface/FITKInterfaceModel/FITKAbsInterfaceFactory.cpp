/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsInterfaceFactory.h"

// Tools
#include "FITKAbsAlgorithmTools.h"

namespace Interface
{
    // 初始化静态变量。
    QMutex FITKInterfaceAlgorithmFactory::m_mutex;
    FITKInterfaceAlgorithmFactory* FITKInterfaceAlgorithmFactory::_instance = nullptr;

    void FITKInterfaceAlgorithmFactory::setAlgToolsCreator(FITKAbsAlgorithmToolsCreator* creator)
    {
        // 保存工具生成器。
        m_toolsCreator = creator;
    }

    FITKAbsAlgorithmToolsCreator* FITKInterfaceAlgorithmFactory::getAlgToolsCreator()
    {
        // 获取工具生成器。
        return m_toolsCreator;
    }

    void FITKInterfaceAlgorithmFactory::finalize()
    {
        // 析构。
        if (m_toolsCreator)
        {
            delete m_toolsCreator;
            m_toolsCreator = nullptr;
        }
    }
}
