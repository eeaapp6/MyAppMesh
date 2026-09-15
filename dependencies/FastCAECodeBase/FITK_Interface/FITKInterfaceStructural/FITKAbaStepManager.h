/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file   FITKAbaStepManager.h
 * @brief  分析步管理器
 * @author LiJin (1003680421@qq.com)
 * @date   2024-03-29
 *
 */
#ifndef __FITKSTRUCTURALMANAGER_H___
#define __FITKSTRUCTURALMANAGER_H___

#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbaAbstractStep.h"

namespace Interface
{
    /**
     * @brief 分析步管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-07
     */
    class FITKInterfaceStructuralAPI  FITKAbaStepManager: public Core::FITKAbstractNDataObject,
         public Core::FITKAbstractDataManager<FITKAbaAbstractStep>
    {
    public:
        /**
         * @brief  构造函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        explicit FITKAbaStepManager() ;
        /**
         * @brief  析构函数
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        virtual ~FITKAbaStepManager() = default;

        /**
        * @brief 插入数据对象，索引越界追加，索引为负数插入到最前面
        * @param[i] index           插入的索引
        * @param[i] item            待插入的数据对象
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-22
        */
        virtual void insertDataObj(int index, Core::FITKAbstractDataObject* item) override;
        /**
         * @brief 移除对象
         * @param[i]  obj            需要移除的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataObj(Core::FITKAbstractDataObject* obj) override;

        /**
         * @brief 移除第index个对象
         * @param[i]  index          对象索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByIndex(int index) override;

        /**
         * @brief 根据数据ID移除对象
         * @param[i]  id            数据ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByID(int id) override;

        /**
         * @brief 根据名称移除对象
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByName(const QString &name, bool compSens = false) override;
        /**
         * @brief 获取指定类型的分析步
         * @param[i]  type           分析步类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual QList<FITKAbaAbstractStep*> getStepByType(FITKAbaAbstractStep::FITKAbaStepType type );


    };

}



#endif // !__FITKSTRUCTURALMANAGER_H___



