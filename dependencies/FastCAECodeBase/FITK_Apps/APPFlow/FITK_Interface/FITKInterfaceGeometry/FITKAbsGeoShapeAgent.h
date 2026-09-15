/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbsGeoShapeAgent.h
 * @brief 抽象几何形状代理器，用于几何形状管理与维护
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-30
 *
 */
#ifndef _FITK_ABS_GEOSHAPE_AGENT_H___
#define _FITK_ABS_GEOSHAPE_AGENT_H___

#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITKInterfaceGeometryAPI.h"
#include "FITKGeoEnum.h"

#include <QVariant>

namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKVirtualTopoManager;
    class FITKGeometryMeshVS;

    /**
     * @brief 抽象几何形状代理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-30
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoShapeAgent :
        public Interface::FITKAbstractGeoModel
    {
        FITKCLASS(Interface, FITKAbsGeoShapeAgent);
    public:
        /**
         * @brief Construct a new FITKAbsGeoShapeAgent object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-30
         */
        explicit FITKAbsGeoShapeAgent(FITKAbsGeoCommand* command);
        /**
         * @brief Destroy the FITKAbsGeoShapeAgent object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-30
         */
        virtual ~FITKAbsGeoShapeAgent() = 0;
        /**
        * @brief 获取几何引擎类型
        * @return FITKGeoEnum::FITKGeoEngine
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-12
        */
        virtual FITKGeoEnum::FITKGeoEngine getGeoEngine() = 0;

        /**
         * @brief       判断模型组件是否有效。[虚函数][重写]
         * @param[in]   compType：组件类型
         * @param[in]   members：组件成员ID
         * @return      是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-11
         */
        virtual bool isComponentValid(FITKModelEnum::FITKModelSetType compType, const QList<int> & members) override;

        /**
        * @brief 获取代理器对应的命令
        * @return FITKAbsGeoCommand
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-31
        */
        FITKAbsGeoCommand* getGeoCommand();

        template <class T>
        /**
       * @brief 获取代理器对应的命令，并进行类型转换
       * @return T*
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-07-31
       */
        T* getTGeoCommand()
        {
            return dynamic_cast<T*> (_command);
        }

        /**
         * @brief       清除命令内的模型数据。（非必须）[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-22
         */
        virtual void clearShape();

        /**
         * @brief 创建虚拓扑
         * @param keepTopos：是否保留未变化的拓扑
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-13
         */
        virtual void buildVirtualTopo(bool keepTopos = false) = 0;
        /**
         * @brief 获取虚拓扑管理器
         * @return FITKVirtualTopoManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-13
         */
        FITKVirtualTopoManager* getVirtualTopoManager();
        /**
         * @brief 三角化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-13
         */
        virtual void triangulation() = 0;

        /**
         * @brief       刷新可视化相关数据。（重构虚拓扑与三角面片信息。）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-03
         */
        void updateVS();

        /**
         * @brief 获取三角化的可视化网格
         * @return FITKGeometryMeshVS*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-13
         */
        FITKGeometryMeshVS* getMeshVS();

        /**
         * @brief       写出STL文件。
         * @param[in]   filePath：文件路径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        virtual bool writeSTLFile(QString filePath) override;

        /**
         * @brief       根据一组坐标计算坐标所在平面法向。[静态]
         * @param[in]   points：节点坐标[引用]
         * @param[out]  normal：法向（已归一化）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-22
         */
        static void GetFaceNormal(QList<QVector3D> & points, double* normal);

        /**
         * @brief       获取模型维度。[纯虚函数]
         * @return      维度（0-3）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-13
         */
        virtual int getDim() = 0;

        /**
         * @brief       创建并获取形状状态标记。[纯虚函数]
         * @param[out]  stateVal：形状状态标记[引用]
         * @return      是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        virtual bool createShapeState(QVariant & stateVal) = 0;

        /**
         * @brief       判断模型状态值是否相同。[虚函数]
         * @param[in]   stateVal_1：形状状态标记1[引用]
         * @param[in]   stateVal_2：形状状态标记2[引用]
         * @return      是否相同
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        virtual bool isSameState(QVariant & stateVal_1, QVariant & stateVal_2);

        /**
         * @brief       当前几何是否可用。（几何引擎层数据被引用模型视为不可用状态，
         *              需要在界面隐藏或跳过数据处理）[虚函数]
         * @return      是否可用
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-15
         */
        virtual bool isValid();

        /**
         * @brief   获取几何形状状态ID
         * @return  是否可用
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-27
         */
        int getShapeStateID();

        /**
         * @brief   设置几何形状状态ID
         * @param   stateID 状态ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-27
         */
        void setShapeStateID(int stateID);

        /**
         * @brief       获取几何点坐标。[虚函数][重写]
         * @param[in]   pointID：节点拓扑ID
         * @param[out]  coor：坐标
         * @param[in]   modelIndex：模型索引（未使用）[缺省]
         * @return      该节点是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-30
         */
        virtual bool getPointCoor(int pointID, double* coor, int modelIndex = 0) override;

    protected:
        /**
         * @brief 代理器对应的命令
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-31
         */
        FITKAbsGeoCommand* _command{};
        /**
         * @brief 虚拓扑管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-13
         */
        FITKVirtualTopoManager* _vtmanager{};
        /**
         * @brief 三角化的可视化网格
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-13
         */
        FITKGeometryMeshVS* _meshVS{};

        /**
         * @brief   几何状态ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-27
         */
        int _geoStateID = -1;

    };

}


#endif
