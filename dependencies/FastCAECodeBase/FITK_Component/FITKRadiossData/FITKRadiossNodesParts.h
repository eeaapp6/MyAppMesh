/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRadiossNodesParts.h
 * @brief  Radioss节点和零件数据类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-05-30
 * 
 */
#ifndef FITK_RADIOSS_NODES_PARTS_H
#define FITK_RADIOSS_NODES_PARTS_H

#include <QColor>
#include "FITKRadiossDataAPI.h"
#include "FITKAbstractRadiossData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKNodeList.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementList.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include <QPair>

namespace Radioss
{
    /**
     * @brief Radioss节点数据类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-05-30
     */
    class FITKRadiossDataAPI FITKRadiossNodes : public Core::FITKAbstractDataObject, public Interface::FITKNodeList,
          public FITKAbstractRadiossData
    {
        FITKCLASS(Radioss, FITKRadiossNodes);
        RadiossKeyWord(FITKRadiossNodes,  /NODE)
    public:
        explicit FITKRadiossNodes() = default;
        virtual ~FITKRadiossNodes() = default;
    };
    /**
     * @brief Radioss零件数据类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-05-30
     */
    class FITKRadiossDataAPI FITKRadiossPart : public Interface::FITKAbstractModel,
          public Interface::FITKElementList, public FITKAbstractRadiossData
    {
        FITKCLASS(Radioss, FITKRadiossPart);
        RadiossKeyWord(FITKRadiossPart, /PART)
    public:
        explicit FITKRadiossPart();
        virtual ~FITKRadiossPart() = default;

        /**
         * @brief 获取模型类型
         * @return AbsModelType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual Interface::FITKModelEnum::AbsModelType getAbsModelType() override;
        /**
         * @brief 设置零件属性ID
         * @param propertyID 属性ID
         */
        void setPropertyID(int propertyID);
        /**
         * @brief 获取零件属性ID
         * @return int 属性ID
         */
        int getPropertyID() const;
        /**
         * @brief 设置零件材料ID
         * @param materialID 材料ID
         */
        void setMaterialID(int materialID);
        /**
         * @brief 获取零件材料ID
         * @return int 材料ID
         */
        int getMaterialID() const;
        /**
         * @brief 设置零件子集ID
         * @param subsetID 子集ID
         */
        void setSubsetID(int subsetID);
        /**
         * @brief 获取零件子集ID
         * @return int 子集ID
         */
        int getSubsetID() const;
        /**
         * @brief 设置零件厚度
         * @param thickness 厚度值
         */
        void setThickness(double thickness);
        /**
         * @brief 获取零件厚度
         * @return double 厚度值
         */
        double getThickness() const;
        /**
         * @brief 设置零件颜色
         * @param color 颜色值
         */
        void setColor(const QColor &color);
        
        /**
         * @brief 获取零件颜色
         * @return QColor 颜色值
         */
        QColor getColor() const;
        /**
         * @brief 添加单元并分配唯一ID
         * @param element 待添加的单元指针
         * @return int 分配的单元ID，失败返回-1
         */
        int addElement(Interface::FITKAbstractElement *element);
        /**
         * @brief 添加单元并设置ID
         * @param element 待添加的单元指针
         * @param id 单元id
         * @author libaojun
         */
        void addElement(const int id, Interface::FITKAbstractElement* element);
        /**
         * @brief 获取部件包含的全部节点ID列表
         * @return QList<int> 节点ID列表
         */
        //QList<int> getPartNodeIDs() const;
        /**
         * @brief 获取部件包含的全部元素ID列表
         * @return QList<int> 元素ID列表
         */
        QList<int> getPartElementIDs() const;
        /**
         * @brief  转化SPH粒子模型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-04
         */
        void transformationSPHModel();

   private:
        int m_propertyID{-1}; // Property ID for the part
        int m_materialID{-1}; // Material ID for the part
        int m_subsetID{-1}; // Subset ID for the part
        double m_thickness{0.0}; // Thickness of the part
        QColor m_color{Qt::white}; // Color of the part, default is white

        static int MAXELEID; // Static member to track the maximum element ID
        
   };
    
    /**
     * @brief Radioss零件管理器类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-05-30
     */
   class FITKRadiossDataAPI FITKRadiossPartManager : 
        public Core::FITKAbstractDataManager<FITKRadiossPart>
   {
   public:
       explicit FITKRadiossPartManager() = default;
       virtual ~FITKRadiossPartManager() = default;
       /**
        * @brief 根据单元ID获取单元
        * @param[i]  eleID          单元ID
        * @return QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> 
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-09-17
        */
       QPair<FITKRadiossPart*, Interface::FITKAbstractElement*> 
                   getElementByID(int eleID);

   private:
   };
    /**
     * @brief Radioss零件子集数据类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-06-17
     */
   class FITKRadiossDataAPI FITKRadiossSubSet : public Core::FITKAbstractNDataObject, 
        public FITKAbstractRadiossData, public Core::FITKAbstractDataManager<FITKRadiossSubSet>
   {
       friend class RadReadInformationMapper;
       friend class FITKRadiossAdaptorSubset;
       FITKCLASS(Radioss, FITKRadiossSubSet);
       RadiossKeyWord(FITKRadiossSubSet, /SUBSET)
   public:
       explicit FITKRadiossSubSet() = default;
       virtual ~FITKRadiossSubSet() = default;
   
       /**
        * @brief 获取零件ID列表
        * @return QList<int> 零件ID列表
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-06-17
        */
       QList<int> getPartIDs();
     
       
       /**
        * @brief 添加零件ID
        * @param partID 零件ID
        * @return bool 添加成功返回true，如果ID已存在返回false
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-06-17
        */
       bool addPartID(int partID);
       
       /**
        * @brief 移除零件ID
        * @param partID 零件ID
        * @return bool 移除成功返回true，如果ID不存在返回false
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-06-17
        */
       bool removePartID(int partID);
       
       /**
        * @brief 检查零件ID是否存在
        * @param partID 零件ID
        * @return bool 存在返回true，不存在返回false
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-06-17
        */
       bool containsPartID(int partID);
       
       /**
        * @brief 清空所有零件ID
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-06-17
        */
       void clearPartIDs();
       
       /**
        * @brief 获取零件ID数量
        * @return int 零件ID数量
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2025-06-17
        */
       int getPartIDCount();
   
       /**
        * @brief 是否为嵌套子集
        * @author wangning (2185896382@qq.com)
        * @date   2025-10-11
        */
       bool isCombination();
   
   private:
       QList<int> m_partIDs{}; // List of part IDs in this subset,当子集为嵌套子集时，该对象无效

   private:
       QList<int> m_subsetRadIDs{};//rad文件读取时的临时映射
   };


    /**
     * @brief Radioss零件子集管理器类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-06-17
     */
   class FITKRadiossDataAPI FITKRadiossSubSetManager :
       public Core::FITKAbstractDataManager<FITKRadiossSubSet>
   {
   public:
       explicit FITKRadiossSubSetManager() = default;
       virtual ~FITKRadiossSubSetManager() = default;

   private:
   };
} // namespace Radioss

#endif // FITK_RADIOSS_NODES_PARTS_H
