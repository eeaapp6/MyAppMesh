/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphVTKObjectRigidWall.h
 * @brief  刚性墙渲染对象
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-27
 *********************************************************************/
#ifndef _GRAPHVTKOBJECTRIGIDWALL_H__
#define _GRAPHVTKOBJECTRIGIDWALL_H__

#include "GraphDataVTKAdaptorAPI.h"
#include "GraphVTKObjectModelBase.h"

namespace Radioss {
    class FITKAbstractRWall;
}
namespace Interface {
    class FITKMeshFeatureVTK;
}
namespace Core {
    class FITKPoint;
}
class vtkUnstructuredGrid;
class FITKGraphActor;

namespace Exchange
{
    /**
     * @brief  刚性墙渲染对象
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-27
     */
    class GraphDataVTKAdaptorAPI GraphVTKObjectRigidWall : public GraphVTKObjectModelBase
    {
        Q_OBJECT;
        FITKGraphObjectRegist(GraphVTKObjectRigidWall, GraphVTKObjectModelBase);
        FITKCLASS(Exchange, GraphVTKObjectRigidWall);
    public:
        /**
         * @brief    构造函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        GraphVTKObjectRigidWall(Radioss::FITKAbstractRWall* wall);

        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        virtual ~GraphVTKObjectRigidWall();

        /**
         * @brief    更新
         * @param[i] forceUpdate 是否强制刷新
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        virtual void update(bool forceUpdate = false) override;
        /**
         * @brief    设置显示隐藏
         * @param[i] isVisible 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-26
         */
        virtual void setVisible(bool isVisible) override;

        /**
         * @brief    获取数据可见
         * @return   bool 是否可见
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        virtual bool getVisibility() override;

        /**
         * @brief  更新可见
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-27
         */
        virtual void updateVisibility() override;

        /**
         * @brief    根据形状网格类型获取网格数据
         * @param[i] pickType                       拾取类型
         * @return   vtkDataSet*                    网格数据
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        virtual vtkDataSet* getMesh(GraphVTKCommons::ShapePickMode pickType) override;
    private:
        /**
         * @brief    初始化
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        void init();

        /**
         * @brief    初始化演员
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        void initActor();

        /**
         * @brief    更新图形数据
         * @return   bool               是否成功
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        bool updateGraphData();
        /**
         * @brief    计算墙体角坐标点
         * @return   bool
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-01
         */
        bool calculateWallCornerPoints();
        /**
         * @brief    计算圆柱壁面
         * @return   bool
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-25
         */
        bool calculateCylinderWall();

        /**
         * @brief    创建网格单元
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        void createCellGrid();
    private:
        /**
         * @brief  网格演员
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-27
         */
        FITKGraphActor* _meshCellActor = nullptr;      // 单元演员

        /**
         * @brief  网格数据
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-27
         */
        vtkUnstructuredGrid* _meshCellData = nullptr;        // 单元数据
        Interface::FITKMeshFeatureVTK* _feature = nullptr;      // 特征数据
    };
}

#endif // !_GRAPHVTKOBJECTRIGIDWALL_H__