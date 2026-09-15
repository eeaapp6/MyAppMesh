/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsInterfaceFactory.h
 * @brief       抽象（工具）工厂接口。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-12-20
 *********************************************************************/

#ifndef __FITKABSINTERFACEFACTORY_H__
#define __FITKABSINTERFACEFACTORY_H__

#include "FITKInterfaceModelAPI.h"

#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

#include <QMutex>
#include <QObject>

namespace Interface
{
    // 前置声明。
    class FITKAbsAlgorithmToolsCreator;

    /**
     * @brief       抽象（工具）工厂接口。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-21
     */
    class FITKInterfaceModelAPI FITKInterfaceAlgorithmFactory : public QObject
    {
        //声明为单例
        DeclSingleton(FITKInterfaceAlgorithmFactory);

    public:
        /**
         * @brief       设置算法工具生成器。
         * @param[in]   creator：算法工具生成器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        void setAlgToolsCreator(FITKAbsAlgorithmToolsCreator* creator);

        /**
         * @brief       获取算法工具生成器。
         * @return      算法工具生成器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        FITKAbsAlgorithmToolsCreator* getAlgToolsCreator();

    private:
        /**
         * @brief       初始化。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        void initialize() {}

        /**
         * @brief       销毁对象。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        void finalize();

    private:
        /**
         * @brief       算法生成器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-20
         */
        FITKAbsAlgorithmToolsCreator* m_toolsCreator{ nullptr };

    };
}

#endif //!__FITKABSINTERFACEFACTORY_H__
