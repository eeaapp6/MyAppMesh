/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphModelProvider.h
 * @brief  三维模型可视化数据管理
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-05-22
 *********************************************************************/
#ifndef __GRAPHMODELPROVIDER_H__
#define __GRAPHMODELPROVIDER_H__

#include "GraphProviderBase.h"

#include "GraphDataProviderAPI.h"

#include <QHash>

 // 前置声明

namespace GraphData
{
    /**
     * @brief  三维模型可视化数据管理
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-05-22
     */
    class GraphDataProviderAPI GraphModelProvider : public GraphProviderBase
    {
        Q_OBJECT;
        // 友元，防止外部手动创建或析构。
        friend class GraphProviderManager;
    public:
        /**
         * @brief    获取当前算例已实例化的可视化对象
         * @return   QList<Exchange::GraphVTKObject3D*>  可视化对象列表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QList<Exchange::GraphVTKObject3D*> getCurrentGraphObjs() override;

        /**
         * @brief   获取类名。[重写]
         * @return  类名
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QString getClassName() override;

        /**
         * @brief   根据数据ID获取模型（几何）可视化对象。（没有则创建）
         * @param   dataObjId：数据ID
         * @return  可视化对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        Exchange::GraphVTKObject3D* getModelGraphObject(int dataObjId);
        /**
         * @brief    根据数据id获取Part可视化对象
         * @param[i] dataObjId                                 数据id
         * @return   Exchange::GraphVTKObject3D*               可视化对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        Exchange::GraphVTKObject3D* getPartGraphObject(int dataObjId);
        /**
         * @brief    根据数据id获取刚体墙可视化对象
         * @param[i] dataObjId 
         * @return   Exchange::GraphVTKObject3D *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-27
         */
        Exchange::GraphVTKObject3D* getRWallGraphObject(int dataObjId);
        /**
         * @brief    根据数据id获取重力载荷可视化对象
         * @param[i] dataObjId 
         * @return   Exchange::GraphVTKObject3D *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-28
         */
        Exchange::GraphVTKObject3D* getGravityGraphObject(int dataObjId);
        /**
         * @brief    根据数据id获取初始场可视化对象
         * @param[i] dataObjId 
         * @return   Exchange::GraphVTKObject3D *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-05
         */
        Exchange::GraphVTKObject3D* getFiledGraphObject(int dataObjId);
        /**
         * @brief    根据数据id获取边界条件可视化对象
         * @param[i] dataObjId 
         * @return   Exchange::GraphVTKObject3D *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-27
         */
        Exchange::GraphVTKObject3D* getBCSGraphObject(int dataObjId);
        /**
         * @brief    根据数据id获取相互作用可视化对象
         * @param[i] dataObjId 
         * @return   Exchange::GraphVTKObject3D *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-10
         */
        Exchange::GraphVTKObject3D* getInteractionGraphObject(int dataObjId);
        /**
         * @brief    根据数据id获取连接可视化对象
         * @param[i] dataObjId 
         * @return   Exchange::GraphVTKObject3D *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-10
         */
        Exchange::GraphVTKObject3D* getConnectionGraphObject(int dataObjId);
        /**
         * @brief    根据id获取集合渲染对象
         * @param[i] dataObjId                         数据id
         * @param[i] parentId                          集合所在对象id（Part、Instance、Assembly）
         * @return   Exchange::GraphVTKObject3D*       渲染对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        Exchange::GraphVTKObject3D* getModelSetGraphObject(int dataObjId, int parentId);
        /**
         * @brief    根据id获取实例渲染对象
         * @param[i] dataObjId                         数据id
         * @return   Exchange::GraphVTKObject3D *      渲染对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        Exchange::GraphVTKObject3D* getInstanceGraphObject(int dataObjId);
        /**
         * @brief    获取集合或表面渲染对象
         * @param[i] dataObjId                         数据id
         * @param[i] modeId                            集合所属模型id
         * @return   Exchange::GraphVTKObject3D*       渲染对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        Exchange::GraphVTKObject3D* getGraphObjSetOrSurface(int dataObjId, int modeId);
        /**
         * @brief  获取探针渲染对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-12-04
         */
        Exchange::GraphVTKObject3D* getProbeGraphObject(int dataObjId);
        /**
         * @brief    获取所有几何可视化对象
         * @return   QList<Exchange::GraphVTKObject3D*>          可视化对象列表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QList<Exchange::GraphVTKObject3D*> getAllGeoGraphObjects();
        /**
         * @brief    获取所有网格可视化对象
         * @return   QList<Exchange::GraphVTKObject3D*>
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-10-28
         */
        QList<Exchange::GraphVTKObject3D*> getAllMeshGraphObjects();
        /**
         * @brief    获取所有实例可视化对象
         * @return   QList<Exchange::GraphVTKObject3D*>          可视化对象列表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QList<Exchange::GraphVTKObject3D*> getAllInstanceGraphObjects();
        /**
         * @brief    获取全部集合可视化对象
         * @return   QList<Exchange::GraphVTKObject3D*>          可视化对象列表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, QHash<int, Exchange::GraphVTKObject3D*>> getAllModelSetGraphObjects();
        /**
         * @brief    获取所有可视化对象
         * @return   QList<Exchange::GraphVTKObject3D*>          可视化对象列表
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QList<Exchange::GraphVTKObject3D*> getAllGraphObjects();
        /**
         * @brief   根据数据ID更新可视化对象，没有则跳出。
         * @param   dataObjId：数据ID
         * @param   info：附加信息[缺省]
         * @return  是否存在该数据ID所对应可视化对象。
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        bool updateObjById(int dataObjId, QVariant info = QVariant());

        /**
         * @brief    设置当前所有实例化模型可视化对象是否可拾取点。
         * @param    dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setGeoVertPickable(int dataObjId = -1);

        /**
         * @brief    设置当前所有实例化模型可视化对象是否可拾取线。
         * @param    dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setGeoEdgePickable(int dataObjId = -1);

        /**
         * @brief    设置当前所有实例化模型可视化对象是否可拾取面。
         * @param    dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setGeoFacePickable(int dataObjId = -1);

        /**
         * @brief    设置当前所有实例化模型可视化对象是否可拾取体。
         * @param    dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setGeoSolidPickable(int dataObjId = -1);
        /**
         * @brief    设置网格节点可被拾取
         * @param[i] dataObjId                被拾取对象ID（-1为所有对象）
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setMeshVerPickable(int dataObjId = -1);
        /**
         * @brief    设置网格表面节点是否可被拾取
         * @param[i] dataObjid                被拾取对象ID（-1为所有对象）
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setMeshVerSurfPickable(int dataObjId = -1);
        /**
         * @brief    设置网格单元是否可被拾取
         * @param[i] dataObjid                被拾取对象ID（-1为所有对象）
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setMeshCellPickable(int dataObjId = -1);
        /**
         * @brief    设置网格表面单元是否可被拾取
         * @param[i] dataObjid                被拾取对象ID（-1为所有对象）
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setMeshCellSurfPickable(int dataObjId = -1);
        /**
         * @brief    关闭模型拾取状态。
         * @param    dataObjId：生效数据ID（-1则对所有数据生效）[缺省]
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void setNonePickable(int dataObjId = -1);

    private:
        /**
         * @brief   构造函数。
         * @param   graphWidget：可视化窗口
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        GraphModelProvider(Comp::FITKGraph3DWindowVTK* graphWidget);

        /**
         * @brief   析构函数。
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        ~GraphModelProvider();
        /**
         * @brief    集合对象删除事件
         * @param[i] parentId                  父对象id
         * @param[i] dataObjId                 对象id
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void modelSetDeleteEvent(int parentId, int dataObjId);
    private:
        /**
         * @brief  模型可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, Exchange::GraphVTKObject3D*> _modelObjHash;
        /**
         * @brief  网格可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, Exchange::GraphVTKObject3D*> _meshObjHash;
        /**
         * @brief  刚性墙可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        QHash<int, Exchange::GraphVTKObject3D*> _rWallObjHash;
        /**
         * @brief  重力载荷可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-28
         */
        QHash<int, Exchange::GraphVTKObject3D*> _gravityObjHash;
        /**
         * @brief  初始化场可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-05
         */
        QHash<int, Exchange::GraphVTKObject3D*> _fieldObjHash;
        /**
         * @brief  边界条件可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-10-27
         */
        QHash<int, Exchange::GraphVTKObject3D*> _bcsObjHash;
        /**
         * @brief  相互作用可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-11-10
         */
        QHash<int, Exchange::GraphVTKObject3D*> _interactionObjHash;
        /**
         * @brief  连接可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-11-10
         */
        QHash<int, Exchange::GraphVTKObject3D*> _connectionObjHash;
        /**
         * @brief  集合可视化对象字典
         * <SetID<PartID/InstanceID/AssemblyID，渲染对象>>
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, QHash<int, Exchange::GraphVTKObject3D*>> _modelSetObjHash;
        /**
         * @brief  实例可视化对象字典
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, Exchange::GraphVTKObject3D*> _instanceHash;
        /**
        * @brief  探针可视化对象字典
        * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
        * @date   2025-05-22
        */
        QHash<int, Exchange::GraphVTKObject3D*> _probeHash;
        /**
         * @brief  集合与表面可视化对象数据字典。( QHash<集合/表面ID, QHash<模型ID, 可视化对象>> )
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, QHash<int, Exchange::GraphVTKObject3D*>> _setSurfObjHash;
    };
}   // namespace GraphData

#endif // __GRAPHMODELPROVIDER_H__
