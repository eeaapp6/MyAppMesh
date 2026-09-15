/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaMeshPartVTK.h
 * @brief vtk表示的非结构网格
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-30
 *
 */
#ifndef _FITKABAMESHPARTVTK_H__
#define _FITKABAMESHPARTVTK_H__

#include "FITKInterfaceStructuralAPI.h"
#include "FITKAbaSurfaceLabel.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"

namespace Interface
{
    struct AbaEleSurface
    {
        //类型与vtkCellType一致
        int _faceType{ -1 };
        //全局节点ID
        QList<int> _nodeIDs{};
    };

    class FITKInterfaceStructuralAPI FITKAbaMeshPartVTK
        : public Interface::FITKUnstructuredMeshVTK
    {
        Q_OBJECT
    public:
        /**
         * @brief Abaqus单元类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        enum AbaEleType
        {
            AbaEleNone = 0,
            B31,
            B31H,
            B32,
            B32H,
            T3D2,
            T3D2H,
            T3D3,
            T3D3H,
            S3,
            S3R,
            S3RS,
            STRI3,
            CPS3,
            STRI65,
            SFM3D6,
            S4,
            S4R,
            S4R5,
            S4RS,
            S4RSW,
            S8R,
            S8R5,
            C3D4,
            C3D4H,
            C3D6,
            C3D8,
            C3D8I,
            C3D8R,
            C3D8H,
            C3D8T,
            C3D10,
            C3D10H,
            C3D10M,
            C3D15,
            C3D20,
            C3D20H,
            C3D20R,
            R3D4,
            CAX3, //轴对称三节点单元
            CAX4  //轴对称四节点单元
        };
        Q_ENUM(AbaEleType);

        using Interface::FITKUnstructuredMeshVTK::FITKUnstructuredMeshVTK;
       /**
        * @brief Destroy the FITKAbaMeshPartVTK object
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-30
        */
        virtual ~FITKAbaMeshPartVTK();
        /**
         * @brief 追加单元
         * @param[i]  eleType        单元类型
         * @param[i]  eleID          单元ID
         * @param[i]  nodeIDList     节点ID列表
         * @param[i]  nativeFlag     几何关联标记
         * @param[i]  tag            标记
         * @return  是否追加成功
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        bool appendElement(AbaEleType eleType, const int eleID, const QList<int>& nodeIDList, bool nativeFlag = false, int tag = 0);

        /**
         * @brief       移除全部单元。[重写]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-04
         */
        void removeAllElement() override;

        /**
         * @brief       根据索引移除单元。[重写]
         * @param[in]   index：网格单元索引
         * @return      单元ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-04
         */
        int removeElementAt(const int index) override;

        /**
         * @brief       根据ID移除单元。[重写]
         * @param[in]   id：网格单元ID
         * @return      单元索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-04
         */
        int removeElementByID(const int id) override;

        /**
         * @brief 获取单元类型
         * @param[i]  index          第index个单元
         * @return AbaEleType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        AbaEleType getAbaElementType(const int index) const;
        
        /**
         * @brief 分配单元类型
         * @param[i]  index          单元索引
         * @param[i]  eleTypes       单元类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-04
         */
        void assignAbaElementType(const int index, const AbaEleType& eleTypes);
        /**
         * @brief 分配单元类型
         * @param[i]  eleTypes       单元类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-06-04
         */
        void assignAbaElementType( const AbaEleType& eleTypes);

        /**
         * @brief 获取单元类型
         * @param[i]  eleid           单元id
         * @return AbaEleType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-23
         */
        AbaEleType getAbaElementTypeByID(const int eleid) const;
        /**
         * @brief  获取表面信息
         * @param[i]  cellid         单元ID
         * @param[i]  label          表面标记
         * @return AbaEleSurface
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-11
         */
        AbaEleSurface getEleFace(const int cellid, FITKAbaSurfaceLabel::AbaSurfLabel label);
        /**
         * @brief    清空所有网格
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-10-15
         */
        virtual void clearMesh() override;
        /**
         * @brief    清空关联网格。（存在几何映射关系）[虚函数][重写]
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-10-15
         */
        virtual void clearNativeMesh() override;
         /**
          * @brief    清空独立网格。（不存在几何映射关系）[虚函数][重写]
          * @return   void
          * @author   BaGuijun (baguijun@163.com)
          * @date     2025-10-15
          */
        virtual void clearOrphanMesh() override;

        /**
         * @brief       替换单元。
         * @param[in]   orgIndex：原始单元索引
         * @param[in]   newEle：新单元
         * @param[in]   eleType：新单元类型
         * @param[in]   delOrg：是否删除原始单元[缺省]
         * @return      是否存在指定ID的单元
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-06
         */
        virtual bool replaceAbaElementAt(int orgIndex, FITKAbstractElement* newEle, const AbaEleType & eleType, bool delOrg = true);
        /**
         * @brief    获取所有的Abaqus所有单元类型
         * @return   const QList<Interface::FITKAbaMeshPartVTK::AbaEleType>&   单元类型列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-12-25
         */
        const QList<Interface::FITKAbaMeshPartVTK::AbaEleType>& getAbaEleTypes();
    private:
        /**
         * @brief Create a Element object
         * @param[i]  abaEleType     单元类型
         * @return FITKAbstractElement*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        FITKAbstractElement* createElement(AbaEleType abaEleType);
    private:
        /**
         * @brief 单元类型列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-30
         */
        QList<AbaEleType> _elementTypes{};
    };
}




#endif
