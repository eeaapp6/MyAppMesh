/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKMeshGenInterface.h
 * @brief 网格生成接口类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-17
 *
 */
#ifndef _FITKMESHGEN_INTERFACE_H___
#define _FITKMESHGEN_INTERFACE_H___

#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include <QHash>
#include <QString>
#include <QMutex>
#include <QObject>

namespace Interface
{
    //前置声明
    class FITKGlobalMeshSizeInfo;
    class FITKRegionMeshSizeManager;
    class FITKAbstractMeshSizeInfoGenerator;
    class FITKAbstractGeometryMeshSizeGenerator;
    class FITKAbstractMesherDriver;
    class FITKAbstractMeshProcessor;
    class FITKGeometryMeshSizeManager;
    class FITKZonePointManager;
    class FITKRegionGeometryRefineManager;
    class FITKGlobalMeshGenerateAlgorithmInfo;
    class FITKMeshGenInterfaceDeleator;
    class FITKAbstractMeshAlgorithmInfoGenerator;
    class FITKAbstractMeshingParameter;
    class FITKMeshParameterManager;
    class FITKMeshGenInterfaceData;
    class FITKMeshSeedManager;
    
    /**
     * @brief 网格生成接口类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-12
     */
    class FITKInterfaceMeshGenAPI FITKMeshGenInterface : public QObject
    {
        friend FITKMeshGenInterfaceDeleator;
        //声明为单例
        DeclSingleton(FITKMeshGenInterface);
    public:
        /**
         * @brief 获取全局网格尺寸信息
         * @param[i]  mesher 网格引擎名称
         * @return FITKGlobalMeshSizeInfo*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKGlobalMeshSizeInfo* getGlobalMeshSizeInfo(const QString & mesher = "");
        /**
         * @brief 注册网格尺寸生成器
         * @param[i] genator
         * @param[i]  mesher 网格引擎名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        void regMeshSizeGenerator(FITKAbstractMeshSizeInfoGenerator* genator, const QString & mesher = "");
        /**
         * @brief    获取全局网格生成的算法信息
         * @param[i]  mesher 网格引擎名称
         * @return   FITKGlobalMeshGenerateAlgorithmInfo *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-15
         */
        FITKGlobalMeshGenerateAlgorithmInfo* getGlobalMeshGenerateAlgorithmInfo(const QString & mesher = "");
        /**
         * @brief  注册网格算法生成器
         * @param  genator
         * @param[i]  mesher 网格引擎名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-15
         */
        void regMeshAlgorithmGenerator(FITKAbstractMeshAlgorithmInfoGenerator* genator, const QString & mesher = "");
        /**
        * @brief 获取网格尺寸生成器
        * @param[i]  mesher 网格引擎名称
        * @return FITKAbstractGeometryMeshSizeGenerator*
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-12
        */
        FITKAbstractMeshSizeInfoGenerator* getMeshSizeGenerator(const QString & mesher = "");
        /**
         * @brief 注册几何网格尺寸生成器
         * @param[i]  mesher 网格引擎名称
         * @param genator FITKAbstractMeshSizeInfoGenerator*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-24
         */
        void regGeometryMeshSizeGenerator(FITKAbstractGeometryMeshSizeGenerator* genator,const  QString & mesher = "");
        /**
         * @brief 获取几何网格尺寸生成器
         * @param[i]  mesher 网格引擎名称
         * @return FITKAbstractGeometryMeshSizeGenerator*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-24
         */
        FITKAbstractGeometryMeshSizeGenerator* getGeometryMeshSizeGenerator(const QString & mesher = "");
        /**
        * @brief 注册网格生成驱动器
        * @param driver
        * @param[i]  mesher 网格引擎名称
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-12
        */
        void regMesherDriver(FITKAbstractMesherDriver* driver, const QString & mesher = "");
        /**
         * @brief 获取网格生成驱动器
         * @param[i]  mesher 网格引擎名称
         * @return FITKAbstractMesherDriver*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKAbstractMesherDriver* getMesherDriver(const QString & mesher = "");
        /**
         * @brief 注册网格处理器
         * @param p
         * @param[i]  mesher 网格引擎名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        void regMeshProcessor(FITKAbstractMeshProcessor* p,const QString & mesher = "");
        /**
         * @brief 获取网格处理器
         * @param[i]  mesher 网格引擎名称
         * @return FITKAbstractMeshProcessor*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKAbstractMeshProcessor* getMeshProcessor(const QString & mesher = "");
        /**
         * @brief 获取区域网格尺寸管理器
         * @return FITKRegionMeshSizeManager*
         *  @param[i]  mesher 网格引擎名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKRegionMeshSizeManager* getRegionMeshSizeMgr(const QString & mesher = "");
        /**
         * @brief 获取几何网格区域尺寸管理器
         *  @param[i]  mesher 网格引擎名称
         * @return FITKGeometryMeshSizeManager*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-24
         */
        FITKGeometryMeshSizeManager* getGeometryMeshSizeManager(const QString & mesher = "");
        /**
         * @brief    获取网格划分参数管理器
         * @param[i] dataObjId                  数据id
         * @param[i] mesher                     网格驱动器标签
         * @param[i] isCreate                   不存在是否创建
         * @return   FITKMeshParameterManager*  网格划分参数管理器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-26
         */
        FITKMeshParameterManager* getMeshParameterManager(int dataObjId, const QString & mesher = "", bool isCreate = true);
        /**
         * @brief    移除网格划分参数管理器
         * @param[i] dataObjId                  数据id
         * @param[i] mesher                     网格驱动器标签
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-27
         */
        void removeMeshParameterManager(int dataObjId, const QString & mesher = "");
        /**
         * @brief    获取网格划分种子点管理器
         * @param[i] dataObjId                  数据id
         * @param[i] mesher                     网格驱动器标签
         * @param[i] isCreate                   不存在是否创建
         * @return   FITKMeshSeedManager*       网格划分种子点管理器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-26
         */
        FITKMeshSeedManager* getMeshSeedManager(int dataObjId, const QString & mesher = "", bool isCreate = true);
        /**
         * @brief    移除网格划分种子点管理器
         * @param[i] dataObjId                  数据id
         * @param[i] mesher                     网格驱动器标签
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-27
         */
        void removeMeshSeedManager(int dataObjId, const QString & mesher = "");
        /**
         * @brief 获取网格区域材料点
         *  @param[i]  mesher 网格引擎名称
         * @return FITKAbstractGeometryMeshSizeGenerator*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-26
         */
        FITKZonePointManager* getZonePointManager(const QString & mesher = "");
        /**
         * @brief    获取几何区域优化管理器
         *  @param[i]  mesher 网格引擎名称
         * @return   FITKRegionGeometryRefineManager*
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-02
         */
        FITKRegionGeometryRefineManager* getRegionGeometryRefineManager(const QString & mesher = "");
        /**
         * @brief  移除网格引擎相关数据
         * @param[i]  mesher 网格引擎名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-07
         */
        void removeMeshGenInterfaceData(const QString& mesher = "");

        /**
         * @brief       获取网格组件接口名称列表。
         * @return      网格组件接口名称列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-27
         */
        QStringList getMeshInterfaceNames();
        /**
         * @brief    清空所所有参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-28
         */
        void clear(const QString& mesher = "");
    private:
        /**
         * @brief 初始化
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        void initialize();
        /**
         * @brief 析构
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        void finalize();
        /**
         * @brief 获取一个有效的数据集合，不存在则创建新的
         * @param[i]  mesher        网格引擎名称
         * @return FITKMeshGenInterfaceData* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-25
         */
        FITKMeshGenInterfaceData* getValidData(const QString mesher);

    private:
        /**
         * @brief 名称与网格划分相关数据的集合
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-25
         */
        QHash<QString, FITKMeshGenInterfaceData*> _interfaceData{};
    };


    class FITKMeshGenInterfaceDeleator
    {
    public:
        explicit FITKMeshGenInterfaceDeleator() = default;
        virtual ~FITKMeshGenInterfaceDeleator();

    };
    /**
     * @brief 网格划分相关的参数管理
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-02-25
     */
    class FITKMeshGenInterfaceData
    {
    public:
        explicit FITKMeshGenInterfaceData();
        virtual ~FITKMeshGenInterfaceData();
        /**
         * @brief 网格尺寸生成器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKAbstractMeshSizeInfoGenerator* _meshSizeGenor{};
        /**
         * @brief  网格算法生成器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-15
         */
        FITKAbstractMeshAlgorithmInfoGenerator* _meshAlgorithmGenor{};
        /**
         * @brief 几何网格尺寸生成器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-24
         */
        FITKAbstractGeometryMeshSizeGenerator* _geoMeshSizeGenor{};
        /**
         * @brief 网格尺寸生成驱动器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKAbstractMesherDriver* _mesherDriver{};
        /**
         * @brief 网格文件处理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKAbstractMeshProcessor* _meshProcessor{};

        /**
         * @brief 网格划分参数
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-12-13
         */
        FITKAbstractMeshingParameter* _meshingParameter{};

        /**
         * @brief 全局网格尺寸信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKGlobalMeshSizeInfo* _globalSize{};
        /**
         * @brief  全局网格生成的算法信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-15
         */
        FITKGlobalMeshGenerateAlgorithmInfo* _globalMeshAlgorithm{};
        /**
         * @brief 区域网格尺寸管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        FITKRegionMeshSizeManager* _regionMeshMgr{};
        /**
         * @brief 几何网格区域尺寸管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-24
         */
        FITKGeometryMeshSizeManager* _geoMeshSizeMgr{};
        /**
         * @brief  网格划分参数管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-01-26
         */
        QHash<int, FITKMeshParameterManager*> _meshParamManager = {};
        /**
         * @brief  网格划分种子点管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-01-28
         */
        QHash<int, FITKMeshSeedManager*> _meshSeedManager = {};
        /**
         * @brief 网格区域分点生成器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-26
         */
        FITKZonePointManager* _zonePointsManager{};
        /**
         * @brief  加密区域管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-02
         */
        FITKRegionGeometryRefineManager* _regionGeoRefineMgr{};
    };
}


#endif

