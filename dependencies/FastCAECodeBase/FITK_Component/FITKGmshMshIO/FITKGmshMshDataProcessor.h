/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshMshDataProcessor.h
 * @brief  Gmsh Msh数据处理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-01-27
 *********************************************************************/
#ifndef _FITK_GMSH_MSH_DATAPROCESSOR_READER_H___
#define _FITK_GMSH_MSH_DATAPROCESSOR_READER_H___

#include "FITKGmshMshData.h"
#include "FITKGmshMshIOInterface.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include <QSet>

namespace Interface
{
    class FITKUnstructuredMesh;
    class FITKComponentManager;
}

namespace Gmsh
{
    /**
     * @brief  Gmsh Msh数据读取处理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2026-01-28
     */
    class FITKGmshMshDataReaderProcessor : public FITKGmshMshData
    {
    public:
        FITKGmshMshDataReaderProcessor();
        ~FITKGmshMshDataReaderProcessor();

        /**
         * @brief    设置读取配置
         * @param[i] config
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-03
         */
        void setReaderConfig(const ReaderGmshMshConfig& config);
        /**
         * @brief    获取读取配置
         * @return   const ReaderGmshMshConfig &
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-03
         */
        const ReaderGmshMshConfig& getReaderConfig();

        /**
         * @brief    添加Msh文件里的节点
         * @param[i] nID 
         * @param[i] x 
         * @param[i] y 
         * @param[i] z 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        void addNode(const int nID, const double x, const double y, const double z);
        /**
         * @brief    添加Msh文件里的单元
         * @param[i] eID 
         * @param[i] eType 
         * @param[i] nodeList 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        void addElement(const int eID, const int eType, const int dim, QList<int>& nodeList);
        /**
         * @brief    添加Msh文件里的集合
         * @param[i] dim 
         * @param[i] elementIDs 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        void addElementSet(const int dim, QList<int>& elementIDs);
        /**
         * @brief    将Gmsh mshData更新到meshData里
         * @param[i] meshData 网格数据
         * @param[i] componentMgr 数组数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        virtual void update(Interface::FITKUnstructuredMesh* meshData, Interface::FITKComponentManager* componentMgr = nullptr) override;

    private:
        /**
         * @brief    Gmsh Msh数据检查：将单元的节点ID转化成节点Index，将集合里的单元ID转化成单元Index
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        void mshDtaCheck();
        /**
         * @brief    过滤未使用的节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-04
         */
        void filterUnusedNode();
        /**
         * @brief    过滤低维单元
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-04
         */
        void filterLowDim();

    private:
        /**
         * @brief  读取配置
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-03
         */
        ReaderGmshMshConfig _readConfig{};
        /**
         * @brief  节点数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-04
         */
        int _nodeCount{ 0 };
        /**
         * @brief  节点ID链表
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-04
         */
        QList<int> _nodesId{};
        /**
         * @brief  单元数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-04
         */
        int _elementCount{ 0 };
        /**
         * @brief  单元ID链表
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-04
         */
        QList<int> _elementsId{};
        /**
         * @brief  集合数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-04
         */
        int _setCount{ 0 };
        /**
         * @brief  已使用的节点ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-02-04
         */
        QSet<int> _nodeIdUsed{};
        /**
         * @brief  节点映射器<nodeID, nodeIndex>
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QHash<int, int> _nodeMapping{};
        /**
         * @brief  节点映射器<elementID, elementIndex>
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-08
         */
        QHash<int, int> _elementMapping{};
    };

    /**
     * @brief  Gmsh Msh数据写出处理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2026-01-28
     */
    class FITKGmshMshDataWriterProcessor : public FITKGmshMshData
    {
    public:
        FITKGmshMshDataWriterProcessor();
        ~FITKGmshMshDataWriterProcessor();

    private:

    private:

    };
}

#endif
