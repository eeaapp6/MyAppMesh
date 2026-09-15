/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractGeoModel.h
 * @brief 几何模型相关抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-05
 *
 */
#ifndef __FITKABSTRACT_GEO_MODEL_H__
#define __FITKABSTRACT_GEO_MODEL_H__

#include "FITKAbstractModel.h"
#include "FITKAbstractModelComponent.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"


namespace Interface
{
    class FITKGeoComponentManager;
    /**
     * @brief 抽象几何对象
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-05
     */
    class FITKInterfaceModelAPI FITKAbstractGeoModel : public FITKAbstractModel
    {
    public:
        /**
         * @brief Construct a new FITKAbstractGeoModel object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKAbstractGeoModel();
        /**
         * @brief Destroy the FITKAbstractGeoModel object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKAbstractGeoModel() = 0;

        /**
         * @brief 获取形状的数量
         * @param type 形状类别
         * @return int 返回指定形状的数量，错误时返回-1
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-29
         */
        virtual int getShapeCount(const Interface::FITKModelEnum::FITKModelSetType type);

        /**
       * @brief 获取三角形，用于绘制图形
       * @param[o] mesh 网格
       * @param[i] id   图元ID
       * @param[i] addotional   附加信息
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-05-29
       */
        virtual void getTriangleMeshVS(FITKAbstractDataObject* mesh, int id, void * additional = nullptr);

        /**
         * @brief       写出STL文件。
         * @param[in]   filePath：文件路径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        virtual bool writeSTLFile(QString filePath);

        /**
         * @brief 获取组件管理器
         * @return FITKGeoComponentManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual FITKGeoComponentManager* getGeoComponentManager();
        /**
       * @brief 获取模型包围盒
       * @param[o] minPt 最小点位置
       * @param[o] maxPt 最大点位置
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-07-30
       */
        virtual bool getBoundaryBox(double* min, double* max);

    protected:
        /***
         * @brief 设置数据对象ID，需谨慎使用
         * @param ID 数据对象id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-18
         */
        void setAbsDataID(const int id) override;

    protected:
       /**
        * @brief 组件管理器
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-06-05
        */
        FITKGeoComponentManager* _compManager{};

    };
    /**
     * @brief 集合组件
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-05
     */
    class FITKInterfaceModelAPI FITKGeoComponent : public FITKAbstractModelComponent
    {
    public:
        /**
         * @brief Construct a new FITKGeoComponent object
         * @param type 集合类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKGeoComponent(const FITKModelEnum::FITKModelSetType type = FITKModelEnum::FMSSurface);
        /**
         * @brief Destroy the FITKGeoComponent object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKGeoComponent() = default;
         /**
          * @brief 设置成员
          * @param[i]  member         成员列表
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-06-05
          */
        void setMember(QList<int> & member);
        /**
         * @brief 追加成员
         * @param[i]  mem           成员
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void appendMember(const int mem);
        /**
         * @brief 清空成员
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void clear();
        /**
         * @brief 移除成员
         * @param[i]  mem            待移除的成员
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        void removeMember(const int mem);
        /**
         * @brief 获取全部成员
         * @return QList<int>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        QList<int> getMember() const;
        /**
         * @brief 获取集合类型
         * @return FITKModelEnum::FITKModelSetType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        FITKModelEnum::FITKModelSetType getGeoType() const;

    private:
        /**
        * @brief 集合类型
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-27
        */
        FITKModelEnum::FITKModelSetType _type{ FITKModelEnum::FMSNone };
        /**
        * @brief 成员列表
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-27
        */
        QList <int> _member{};
    };
    /**
     * @brief 几何组件管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-05
     */
    class FITKInterfaceModelAPI FITKGeoComponentManager:
        public Core::FITKAbstractDataManager<FITKGeoComponent>
    {
    public:
        /**
         * @brief Construct a new FITKGeoComponentManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        explicit FITKGeoComponentManager() = default;
        /**
         * @brief Destroy the FITKGeoComponentManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual ~FITKGeoComponentManager() = default;
        /**
         * @brief 追加成员对象
         * @param[i]  item           成员对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual void appendDataObj( Core::FITKAbstractDataObject* item) override;
        /**
         * @brief 获取集合组件
         * @param[i]  type           集合类型
         * @return QList<FITKGeoComponent*>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual QList<FITKGeoComponent*> getGeoComponent(const FITKModelEnum::FITKModelSetType type = FITKModelEnum::FMSSurface);
        /**
         * @brief 获取全部集合成员
         * @param[i]  type           集合类型
         * @return QList<int>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-05
         */
        virtual QList<int> getAllComponentMember(const FITKModelEnum::FITKModelSetType type = FITKModelEnum::FMSSurface);
    };
}


#endif
