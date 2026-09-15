/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GraphDataVTKPartSetManager.h
 * @brief  渲染部件数据管理器
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-05-22
 *********************************************************************/
#ifndef _GraphDataVTKPartSetManager_H_
#define _GraphDataVTKPartSetManager_H_

#include "GraphDataVTKAdaptorAPI.h"

#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

#include <QMutex>
#include <QHash>
#include <QObject>

#define VTKID_BY_NODEID_ARRAY_NAME              "VTKIdByNodeIdArray"
#define NODEID_BY_VTKID_ARRAY_NAME              "NodeIdByVTKIdArray"

class vtkUnstructuredGrid;
class vtkPolyData;
class vtkDataSet;
class vtkDataSetAlgorithm;
class vtkPolyDataAlgorithm;
class vtkUnstructuredGridAlgorithm;
class FITKShellFeatureEdges;

namespace Interface {
    class FITKGeoMeshVSPt;
    class FITKGeoMeshVSEdgeEntity;
    class FITKGeoMeshVSFaceEntity;
    class FITKAbsGeoCommand;
    class FITKMeshFeatureVTK;
}
namespace Radioss{
    class FITKRadiossNodes;
}
namespace Core {
    class FITKAbstractDataObject;
}
namespace Exchange
{
    /**
     * @brief  部件相关信息
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-05-22
     */
    struct GraphDataVTKPartSetInfo
    {
    public:
        GraphDataVTKPartSetInfo();
        ~GraphDataVTKPartSetInfo();
        //清空数据
        void clearData();
    public:
        //几何命令ID
        int _geoCommandID = -1;
        //点网格
        vtkUnstructuredGrid* _pointGrid = nullptr;
        //线网格
        vtkUnstructuredGrid* _lineGrid = nullptr;
        //面网格
        vtkPolyData* _faceGrid = nullptr;
        //体网格
        vtkPolyData* _solidGrid = nullptr;
        // 点虚拓扑ID到虚拓扑索引的映射
        QHash<int, int> _pointVirTopoIdToIndex;
        // 线虚拓扑ID到虚拓扑索引的映射
        QHash<int, int> _lineVirTopoIdToIndex;
        // 面虚拓扑ID到虚拓扑索引的映射
        QHash<int, int> _faceVirTopoIdToIndex;
        // 体虚拓扑ID到虚拓扑索引的映射
        QHash<int, int> _solidVirTopoIdToIndex;
        /**
         * @brief  点VTK与形状映射(VTKID，形状ID)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, int> _point_VTKId_CellID = {};
        /**
         * @brief  线VTK与形状映射(VTKID，形状ID)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, int> _line_VTKId_CellID = {};
        /**
         * @brief  面VTK与形状映射(VTKID，形状ID)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, int> _face_VTKId_CellID = {};
        /**
         * @brief  体VTK与形状映射(VTKID，形状ID)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-09-15
         */
        QHash<int, int> _solid_VTKId_CellID = {};
        /**
         * @brief  几何命令中的点数据(虚拓扑，点)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, Interface::FITKGeoMeshVSPt*> _partPointHash = {};
        /**
         * @brief  几何命令中的线数据(虚拓扑，线)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, Interface::FITKGeoMeshVSEdgeEntity*> _partLineHash = {};
        /**
         * @brief  几何命令中的面数据(虚拓扑，面)
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, Interface::FITKGeoMeshVSFaceEntity*> _partFaceHash = {};
    };
    struct GraphDataVTKAdaptorAPI PartMeshDataVTK
    {
        int _partId = -1;
        QList<int> _originalNodeIds;
        QHash<int, int> _nodeIdToPointIndex;
        vtkDataSet* _meshData = nullptr;
        Interface::FITKMeshFeatureVTK* _meshFeature{nullptr};
        vtkDataSetAlgorithm* _filterSurface{ nullptr };
        vtkPolyDataAlgorithm* _filterSurfVert{ nullptr };
        vtkPolyDataAlgorithm* _filterVertex = nullptr;
    };
    /**
     * @brief  渲染部件数据管理器
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-05-22
     */
    class GraphDataVTKAdaptorAPI GraphDataVTKPartSetManager : public QObject
    {
        Q_OBJECT;
        //单例定义
        DeclSingleton(GraphDataVTKPartSetManager);
    public:
        /**
         * @brief    更新几何命令数据
         * @param[i] geoCommandId          几何命令ID
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void updateGeoCommand(int geoCommandId);
        /**
         * @brief    获取几何命令信息
         * @param[i] geoCommandId          几何命令ID
         * @return   GraphDataVTKPartSetInfo
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        GraphDataVTKPartSetInfo* getGeoCommandInfo(int geoCommandId);
        /**
         * @brief    根据虚拓扑id获取点数据信息
         * @param[i] virTopoId                             虚拓扑id
         * @return   Interface::FITKGeoMeshVSPt *          点数据信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        Interface::FITKGeoMeshVSPt* getGeoPointData(int virTopoId);
        /**
         * @brief    根据虚拓扑id获取线数据信息
         * @param[i] virTopoId                             虚拓扑id
         * @return   Interface::FITKGeoMeshVSPt *          线数据信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        Interface::FITKGeoMeshVSEdgeEntity* getGeoLineData(int virTopoId);
        /**
         * @brief    根据虚拓扑id获取面数据信息
         * @param[i] virTopoId                             虚拓扑id
         * @return   Interface::FITKGeoMeshVSPt *          面数据信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        Interface::FITKGeoMeshVSFaceEntity* getGeoFaceData(int virTopoId);
        /**
         * @brief    获取部件网格数据
         * @param[i] partId          部件ID
         * @param[i] ioData          部件网格数据
         * @param[i] update          是否更新数据
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-19
         */
        bool getPartMeshData(int partId, PartMeshDataVTK*& ioData, bool update = false);
    private:
        /**
         * @brief    初始化
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void initialize();
        /**
         * @brief    结束，析构函数调用
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void finalize();
        /**
         * @brief    创建点数据
         * @param[i] partInfo     部件信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void createGeoPointGrid(GraphDataVTKPartSetInfo* partInfo);
        /**
         * @brief    创建线数据
         * @param[i] partInfo                     部件信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void createGeoLineGrid(GraphDataVTKPartSetInfo* partInfo);
        /**
         * @brief    创建面数据
         * @param[i] partInfo                     部件信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void createGeoFaceGrid(GraphDataVTKPartSetInfo* partInfo);
        /**
         * @brief    创建体数据
         * @param[i] partInfo 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-15
         */
        void createGeoSolidGrid(GraphDataVTKPartSetInfo* partInfo);
        /**
         * @brief    计算点法向
         * @param[i] polyData                         数据对象
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        void calculatePointNormals(vtkPolyData* polyData);
        /**
         * @brief    获取Radioss节点数据
         * @return   Radioss::FITKRadiossNodes *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-19
         */
        Radioss::FITKRadiossNodes* getRadiossNodes();
        /**
         * @brief    创建部件网格数据VTK
         * @param[i] partId 
         * @param[i] outData 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-21
         */
        bool createPartMeshDataVTK(int partId, PartMeshDataVTK*& outData);
        /**
        * @brief  创建未使用节点的Part
        * @param  ioData 输出数据
        * @return bool
        * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
        * @date   2025-12-25
        */
        bool createUnusedNodesPartVTK(PartMeshDataVTK*& ioData);

        /**
         * @brief    更新VTK对象
         * @param[i] obj 
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-19
         */
        template <class T>
        void modifyVtkObj(T* obj)
        {
            if (obj)
            {
                obj->Modified();
            }
        }
        /**
         * @brief    删除VTK对象
         * @param[i] obj 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-11-19
         */
        template <class T>
        void deleteVtkObj(T*& obj)
        {
            if (obj)
            {
                obj->Delete();
            }

            obj = nullptr;
        }

    private slots:
        void slot_partDeleted(Core::FITKAbstractDataObject* objDel);
    private:
        /**
         * @brief  几何命令信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-05-22
         */
        QHash<int, GraphDataVTKPartSetInfo*> _geoCommandInfos = {};
        /**
         * @brief  网格数据信息
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-11-19
         */
        QHash<int, PartMeshDataVTK*> _partMeshDataHash = {};
    };
}

#endif
