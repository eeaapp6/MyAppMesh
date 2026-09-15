/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKAbaqusRadiossDataMapperInterface.h
 * @brief  Abaqus与Radioss数据映射器组件接口类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-10-21
 * 
 */
#ifndef _FITKAbaqusRadiossDataMapperInterface_H_
#define _FITKAbaqusRadiossDataMapperInterface_H_

#include "FITKAbaqusRadiossDataMapperAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace AbaqusData
{
    class FITKDataCase;
}

namespace Radioss
{
    class FITKRadiossCase;
}

namespace AbaqusRadiossDataMapper
{
    /**
     * @brief  Abaqus与Radioss数据映射器组件接口类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-10-21
     */
    class FITKAbaqusRadiossDataMapperAPI FITKAbaqusRadiossDataMapperInterface
        : public AppFrame::FITKComponentInterface
    {
    public:
        /**
         * @brief Construct a new FITKAbaqusRadiossDataMapperInterface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        explicit FITKAbaqusRadiossDataMapperInterface() = default;
        /**
         * @brief Destroy the FITKAbaqusRadiossDataMapperInterface object
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        virtual ~FITKAbaqusRadiossDataMapperInterface() = default;
        /**
         * @brief 设置Abaqus数据对象
         * @param[i]  caseData      Abaqus数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        void setAbaqusCase(AbaqusData::FITKDataCase* caseData);
        /**
         * @brief 设置Radioss数据对象
         * @param[i]  caseData      Radioss数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)
         */
        void setRadiossCase(Radioss::FITKRadiossCase* caseData);
        /**
         * @brief 获取部件名称，不能重复 
         * @return QString AbaqusRadiossDataMapper
         * @author LiBaojun 
         * @date 2025-10-21
         */
        QString getComponentName() override;
        /**
         * @brief 执行组件
         * @param[i]  indexPort      抽象接口定义  1-Abaqus到Radioss映射 2-Radioss到Abaqus映射
         * @return true  执行成功
         * @return false  执行失败
         * @author LiBaojun 
         * @date 2025-10-21
         */
        bool exec(const int indexPort) override;
        /**
         * @brief Abaqus到Radioss映射
         * @return true  映射成功
         * @return false  映射失败
         * @author LiBaojun 
         * @date 2025-10-21
         */
        bool abaqus2RadiossMapping();
        /**
         * @brief Radioss到Abaqus映射
         * @return true  映射成功
         * @return false  映射失败
         * @author LiBaojun 
         * @date 2025-10-21
         */
        bool radioss2AbaqusMapping();

    private:
        
        /**
         * @brief Abaqus数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-21
         */
        AbaqusData::FITKDataCase* _abaqusCase {};
        /**
         * @brief Radioss数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-10-21
         */
        Radioss::FITKRadiossCase* _radiossCase{};
         

    };
}


#endif