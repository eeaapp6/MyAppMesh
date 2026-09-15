/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMeshDataProcessToCGNSData.h
 * @brief  网格数据转换CGNS数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-08
 *********************************************************************/

#ifndef _FITKMESHDATAPROCESSTOCGNSDATA_H__
#define _FITKMESHDATAPROCESSTOCGNSDATA_H__

#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

#include <QList>
#include <QHash>
#include <QVector>
#include <QSet>

#include <cgnslib.h>

namespace Interface
{
    class FITKModelSet;
    class FITKAbstractMesh;
    class FITKAbstractElement;
}

namespace CGNS
{
    /**
     * @brief  节点数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-08
     */
    struct NodeCGNSCoordData
    {
        QVector<double> node_coordX{};
        QVector<double> node_coordY{};
        QVector<double> node_coordZ{};
    };
    /**
     * @brief  单元数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-08
     */
    struct ElementCGNSSectionData
    {
        QString name_section{};//名称section
        CGNS_ENUMT(ElementType_t) element_type = CGNS_ENUMV(ElementTypeNull);//单元类型
        QVector<cgsize_t> pnts{};//单元的起始id，终止id
        QVector<cgsize_t> elements{};//单元的拓扑数据
    };
    /**
     * @brief  边界数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-08
     */
    struct SetCGNSBCData
    {
        QString name_bc{};//名称bc
        QVector<cgsize_t> pnts{};//起始id，终止id
        CGNS_ENUMT(GridLocation_t) location = CGNS_ENUMV(GridLocationNull);//边界的位置信息
    };
    /**
     * @brief  数据处理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-08
     */
    class FITKMeshDataProcessToCGNSData
    {
    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-07-29
         */
        explicit FITKMeshDataProcessToCGNSData() = default;
        /**
         * @brief  析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-07-29
         */
        ~FITKMeshDataProcessToCGNSData() = default;
        /**
         * @brief    初始化数据
         * @param[i] mesh 
         * @param[i] componentMgr 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool init(Interface::FITKAbstractMesh* mesh, QList<Interface::FITKModelSet*> componentMgr);
        /**
         * @brief    计算体积是否都满足正数
         * @para[i]  
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-11
         */
        bool calCellVolume(QString filePath);

    public:
        /**
         * @brief  网格类型（结构还是非结构）
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        CGNS_ENUMT(ZoneType_t) m_mesh_type { CGNS_ENUMV(ZoneTypeNull) };
        /**
         * @brief  区域名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QString m_name_zone{};
        /**
         * @brief  单元的最高维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        int m_element_max_dim{ 0 };
        /**
         * @brief  节点坐标
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        NodeCGNSCoordData m_node_coord{};
        /**
         * @brief  单元数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QList<ElementCGNSSectionData> m_element_section{};
        /**
         * @brief  边界数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QList<SetCGNSBCData> m_set_BC{};

    private:
        /**
         * @brief    处理非结构数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool processUnstructuredData();
        /**
         * @brief    处理结构数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool processStructuredData();
        /**
         * @brief    处理节点
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool processUnstructuredMesh_Node();
        /**
         * @brief    处理单元
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool processUnstructuredMesh_Element();
        /**
         * @brief    处理边界
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool processUnstructuredMesh_BC();
        /**
         * @brief    处理无边界的单元数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool processUnstructuredMesh_NoBCElement_Section();
        /**
         * @brief    处理有边界的单元数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        bool processUnstructuredMesh_Element_BC();
        /**
         * @brief    获取CGNS的单元类型
         * @param[i] type 
         * @param[i] eleName 
         * @return   CGNS_ENUMT(ElementType_t)
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        CGNS_ENUMT(ElementType_t) getCGNSElementType(int type, QString& eleName);
        /**
         * @brief    数据清理
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-08
         */
        void dataClear();

    private:
        /**
         * @brief  网格数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        Interface::FITKAbstractMesh* _mesh = nullptr;
        /**
         * @brief  边界数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QList<Interface::FITKModelSet*> _componentMgr{};
        /**
         * @brief  节点映射器<oldID, newID>
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QHash<int, int> _nodeMapping{};
        /**
         * @brief  映射器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QHash<Interface::FITKModelEnum::FITKEleType, QSet<int>> _eleType_eleOldIDs_Mapping;
        /**
         * @brief  映射器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QHash<int, QList<Interface::FITKModelEnum::FITKEleType>> _eleDim_eleType_Mapping;
        /**
         * @brief  映射器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QHash<int, QList<cgsize_t>> _eleOldID_nodeNewIDs_Mapping;
        /**
         * @brief  顺序链表
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QList<QString> _bcNameList;
        /**
         * @brief  边界映射器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QHash<QString, QSet<int>> _bcName_eleOIldIDs_Mapping;
        /**
         * @brief  单元最大ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        int _element_max_ID = 0;
    };
}

#endif