/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphDataSetCompomentManager.h
 * @brief  集合、表面渲染数据管理器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-07
 *********************************************************************/
#ifndef _GraphDataSetCompomentManager_H
#define _GraphDataSetCompomentManager_H

#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

#include <QObject>
#include <QMutexLocker>
#include <QHash>

class vtkUnstructuredGrid;
class vtkIntArray;
namespace Interface {
    class FITKModelSet;
    class FITKMeshSurface;
    class FITKMeshSurfaceNode;
    class FITKMeshSurfaceElement;
    class FITKAbstractElement;
}
namespace Radioss {
    class FITKRadiossPart;
    class FITKRadiossNodes;
    class FITKRadiossMeshModel;
    class FITKRadiossSurfaceSeg;
}
namespace Exchange
{
    /**
     * @brief  集合、表面渲染数据管理器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-07
     */
    class GraphDataSetCompomentManager : public QObject
    {
        //单例定义
        DeclSingleton(GraphDataSetCompomentManager);
    public:
        /**
         * @brief    获取集合渲染数据
         * @param[i] setId                                  集合id
         * @param[i] instanceId                             实例id[缺省]
         * @param[i] update                                 是否更新数据[缺省]
         * @return   vtkUnstructuredGrid*                   渲染数据
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        vtkUnstructuredGrid* getSetGridData(int setId, int instanceId = -1, bool update = false);
        /**
         * @brief    获取表面渲染数据
         * @param[i] surfId                                 表面id
         * @param[i] instanceId                             实例id[缺省]
         * @param[i] update                                 是否更新数据[缺省]
         * @return   vtkUnstructuredGrid*                   渲染数据
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        vtkUnstructuredGrid* getSurfGridData(int surfId, int instanceId = -1, bool update = false);
        /**
         * @brief    删除集合渲染数据
         * @param[i] setId      集合id
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        void deleteSetGridData(int setId);
        /**
         * @brief    删除表面渲染数据
         * @param[i] surfId     表面id
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        void deleteSurfGridData(int surfId);
        /**
         * @brief  获取当前Part节点在全局node的索引
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-14
         */
        static int getPartNodeIdByRadiossNodes(Radioss::FITKRadiossPart* part);
    private:
        /**
         * @brief    构造函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        void initialize();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-28
         */
        void finalize();
        /**
         * @brief    创建集合网格
         * @param[i] set                                    集合
         * @param[i] instanceId                             实例id[缺省]C
         * @return   vtkUnstructuredGrid*                   集合网格
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        vtkUnstructuredGrid* generateSetGrid(Interface::FITKModelSet* set, int instanceId);
        /**
         * @brief    创建部件集合
         * @param[i] part                            部件对象
         * @param[i] type                            类型( 1: Points, 2: Cells )
         * @param[i] ids                             点集或单元集
         * @return   vtkUnstructuredGrid*            集合渲染数据
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        vtkUnstructuredGrid* generateSetDataSetWithMesh(Radioss::FITKRadiossMeshModel* meshModel, int type, const QList<int> & ids);
        /**
         * @brief    创建表面网格
         * @param[i] surface                                 表面
         * @param[i] instanceId                              实例id[缺省]
         * @return   vtkUnstructuredGrid*                    表面网格
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-22
         */
        vtkUnstructuredGrid* generateSurfaceGrid(Interface::FITKMeshSurface* surface, int instanceId = -1);
        /**
         * @brief    生成节点表面
         * @param[i] nodeSurf                                节点表面对象
         * @param[i] instanceId                              实例id[缺省]
         * @return   vtkUnstructuredGrid*                    节点表面网格
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-30
         */
        vtkUnstructuredGrid* generateNodeSurface(Interface::FITKMeshSurfaceNode* nodeSurf, int instanceId = -1);
        /**
         * @brief    生成单元表面分割
         * @param[i] eleSurf 
         * @param[i] instanceId 
         * @return   vtkUnstructuredGrid *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        vtkUnstructuredGrid* generateElementSurfaceSeg(Radioss::FITKRadiossSurfaceSeg* eleSurfSeg, int instanceId = -1);
        /**
         * @brief    获取全局节点对象
         * @return   Radioss::FITKRadiossNodes *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-26
         */
        static Radioss::FITKRadiossNodes* getGlobalNodes();
    private:
        /**
         * @brief    集合渲染数据映射关系
         *           key:集合id,
         *           value:中的 key为所属模型id（part or instance）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        QHash<int, QHash<int, vtkUnstructuredGrid*>> _setGridHash = {};
        /**
         * @brief    表面渲染数据映射关系
         *           key:集合id,
         *           value:中的 key为所属模型id（part or instance）
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        QHash<int, QHash<int, vtkUnstructuredGrid*>> _surfGridHash = {};
    };
}

#endif