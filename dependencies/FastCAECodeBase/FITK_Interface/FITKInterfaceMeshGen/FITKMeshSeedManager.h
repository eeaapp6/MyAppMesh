/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMeshSeedManager.h
 * @brief  种子点数据
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-05-22
 *********************************************************************/
#ifndef _FITKMeshSeedManager_H_
#define _FITKMeshSeedManager_H_

#include "FITKInterfaceMeshGenAPI.h"
#include "FITKMeshSeed.h"

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    class FITKAbsGeoCommand;
    /**
     * @brief  种子点数据
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-05-22
     */
    class FITKInterfaceMeshGenAPI FITKMeshSeedManager : public Core::FITKAbstractNDataObject,
        public Core::FITKAbstractDataManager<FITKMeshSeed>
    {
        FITKCLASS(Interface, FITKMeshSeedManager);
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        FITKMeshSeedManager();
        /**
         * @brief  析构函数
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        virtual ~FITKMeshSeedManager();
        /**
         * @brief    移除种子点[overrid]
         * @param[i] obj                        种子点对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-08
         */
        virtual void removeDataObj(FITKAbstractDataObject* obj) override;
        /**
         * @brief    移除种子点[overrid]
         * @param[i] index                      索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-08
         */
        virtual void removeDataByIndex(int index) override;
        /**
         * @brief    移除种子点[overrid]
         * @param[i] id                         ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-08
         */
        virtual void removeDataByID(int id) override;
        /**
         * @brief    通过虚拓扑ID获取种子点
         * @param[i] virTopoID                  虚拓扑ID
         * @return   FITKMeshSeed*              种子点
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        FITKMeshSeed* getMeshSeedByVirTopo(int virTopoID);
        /**
         * @brief    通过类型获取种子点
         * @param[i] type                       类型
         * @return   QList<FITKMeshSeed*>        种子点列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        QList<FITKMeshSeed*> getMeshSeedsByType(FITKMeshSeed::FITKSeedType type);
        /**
         * @brief    获取所有合并边的种子点
         * @return   QList<FITKMeshSeed*>       种子点列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-05
         */
        QList<FITKMeshSeed*> getMeshSeedsByMergeEdge();
        /**
         * @brief    获取所有非合并边的种子点
         * @return   QList<FITKMeshSeed*>       种子点列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-05
         */
        QList<FITKMeshSeed*> getMeshSeedsByNoMergeEdge();
        /**
         * @brief    计算种子点
         * @param[i] seedObj                    种子点对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-23
         */
        virtual void calculateSeedPoint(FITKMeshSeed* seedObj);
        /**
         * @brief    获取当前所有的边id（查询结果包含几何边与合并边，但不包含合并子边）
         * @param[i] geoCom                     几何命令
         * @return   QList<int>                 查询结构
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-07-23
         */
        QList<int> getCurGeoEdgeIds(Interface::FITKAbsGeoCommand* geoCom);

        /**
         * @brief       根据给定模型查找失效边种子点数据并移除。
         * @param[in]   cmd：模型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-07
         */
        void removeInvalidEdgeSeeds(Interface::FITKAbsGeoCommand* cmd);
        /**
         * @brief    设置全局种子点标签
         * @return   bool                       是否创建
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-08
         */
        bool getGlobalSeedDataTag();
        /**
         * @brief    设置全局种子数据标签
         * @param[i] seedDataTag                标签
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-08
         */
        void setGlobalSeedDataTag(bool seedDataTag);
        /**
         * @brief    获取全局种子点尺寸
         * @return   double                     尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-08
         */
        double getGlobalSeedSize();
        /**
         * @brief    设置全局种子点尺寸
         * @param[i] size                       尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-08
         */
        void setGlobalSeedSize(double size);
    protected:
        /**
         * @brief    通过单元数量计算无偏移的种子点
         * @param[i] seedObj                    种子点对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-23
         */
        virtual void calNoneBiasSeedPointByEleNum(FITKMeshSeed* seedObj);
        /**
         * @brief    通过单元大小计算种子点
         * @param[i] seedObj                                    种子点对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-30
         */
        virtual void calSeedPointByEleSize(FITKMeshSeed* seedObj);
    protected:
        /**
         * @brief  全局种子点标签
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-08-08
         */
        bool _globalSeedDataTag = false;
        /**
         * @brief  全局种子点尺寸
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-08-08
         */
        double _globalSeedEleSize = 3.0;
    };
}
#endif