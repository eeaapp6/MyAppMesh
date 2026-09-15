/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaRefPoint.h
 * @brief 参考点及参考点管理器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-07
 *
 */
#ifndef __FITKREFABAPOINTS_H___
#define __FITKREFABAPOINTS_H___

#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModelComponent.h"
#include "FITKInterfaceStructuralAPI.h"

namespace Interface
{
    class FITKModelSet;
    /**
     * @brief 参考点
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-07
     */
    class FITKInterfaceStructuralAPI FITKAbaRefPoint :
        public Interface::FITKAbstractModelComponent
    {
        FITKCLASS(Interface, FITKAbaRefPoint);
    public:
        /**
         * @brief Construct a new FITKAbaRefPoint object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        explicit FITKAbaRefPoint();
        /**
         * @brief Destroy the FITKAbaRefPoint object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        virtual ~FITKAbaRefPoint() = default;
        /**
         * @brief 设置集合id
         * @param[i]  id             集合id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        void setComponentID(int id);
        /**
         * @brief 获取集合id
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        int getComponentID();
        /**
         * @brief 获取集合对象
         * @return FITKModelSet*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        FITKModelSet* getSet();
        /**
         * @brief 获取节点ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        int getNodeID();

        /**
         * @brief 设置集合属于的模型id，装配实例等
         * @param[i]  modelID        模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-25
         */
        void setComponentModelID(const int modelID);
        /**
         * @brief 获取集合属于的模型id
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-25
         */
        int getComponentModelID();
        /**
         * @brief 获取参考点坐标
         * @param[i]  coor       坐标
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        bool getCoor(double* coor);
    

    private:
         /**
          * @brief 集合id
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-04-07
          */
        int _setID{ -1 };
        /**
          * @brief 集合所属模型id
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-04-07
          */
        int _modelID{ -1 };
    };

    /**
     * @brief 参考点管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-07
     */
    class FITKInterfaceStructuralAPI FITKAbaRefPointManager : public Core::FITKAbstractObject,
        public Core::FITKAbstractDataManager<FITKAbaRefPoint>
    {
    public:
        explicit FITKAbaRefPointManager() = default;
        virtual ~FITKAbaRefPointManager() = default;
        /**
        * @brief 移除对象
        * @param[i]  obj            需要移除的对象
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-22
        */
        void removeDataObj(Core::FITKAbstractDataObject* obj) override;
        /**
         * @brief 移除第index个对象
         * @param[i]  index          对象索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void removeDataByIndex(int index) override;
        /**
        * @brief 根据数据ID移除对象
        * @param[i]  id            数据ID
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-22
        */
        void removeDataByID(int id) override;
        /**
         * @brief 根据名称移除对象
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByName(const QString &name, bool compSens = false) override;

    private:
        /**
         * @brief 移除集合
         * @param[i]  ref            参考点对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        void removeSet(FITKAbaRefPoint* ref);
    };

}


#endif
