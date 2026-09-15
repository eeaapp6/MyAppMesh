/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFInitialConditions.h
 * @brief  initial Conitions数据对象
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-23
 *********************************************************************/
#ifndef _FITK_OF_Initial_Conditions__H___
#define _FITK_OF_Initial_Conditions__H___
 
#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKOFEnum.hpp"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKFluidRegionsMesh;
    /**
     * @brief  初始化区域属性
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-17
     */
    class FITKInterfaceFlowOFAPI FITKOFInitPropRegions
    {
    public:
        explicit FITKOFInitPropRegions();
        virtual ~FITKOFInitPropRegions();
        /**
         * @brief    获取区域网格类型
         * @return   FITKOFSolverTypeEnum::FITKOFRegionMeshType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        FITKOFSolverTypeEnum::FITKOFRegionMeshType getRegionMeshType();
        /**
         * @brief    设置区域网格类型
         * @param[i] type 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        void setRegionMeshType(FITKOFSolverTypeEnum::FITKOFRegionMeshType type);
        /**
         * @brief    获取区域参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        Core::FITKParameter* getRegionPara();
        /**
         * @brief    设置区域网格ID
         * @param[i] regionID                    网格区域id
         * @param[i] meshId                      网格边界id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-14
         */
        void setRegionMeshID(int regionID);
        /**
         * @brief    获取区域网格ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-26
         */
        int getRegionMeshID();
        /**
         * @brief    获取区域网格数据
         * @return   FITKFluidRegionsMesh *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        FITKFluidRegionsMesh* getRegionMeshObj();

    private:
        /**
         * @brief  网格区域ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-07-03
         */
        int _regionMeshID{ -1 };
        /**
         * @brief  区域类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        FITKOFSolverTypeEnum::FITKOFRegionMeshType _regionType{ FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid };
        /**
         * @brief  区域参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        Core::FITKParameter* _regionPara{};
    };
    /**
     * @brief  几何补丁
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-03
     */
    class FITKInterfaceFlowOFAPI FITKOFGeometryPatch
    {
    public:
        explicit FITKOFGeometryPatch(int geoId, FITKOFSolverInitialEnum::FITKPatchApplyObjectType type);
        virtual ~FITKOFGeometryPatch();

        /**
         * @brief 获取几何Id
         * @return int
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-23
         */
        int getGeometryId();
        /**
         * @brief 获取几何数据对象
         * @return Interface::FITKAbsGeoCommand*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-26
         */
        Interface::FITKAbsGeoCommand* getGeometryModel();
        /**
         * @brief    获取参数
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-04
         */
        Core::FITKParameter* getFieldPara();
        /**
         * @brief    获取类型
         * @return   FITKOFSolverInitialEnum::FITKPatchApplyObjectType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-04
         */
        FITKOFSolverInitialEnum::FITKPatchApplyObjectType getAppltTo();

    private:
        /**
         * @brief  几何命令ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        int _geometryId{ 0 };
        /**
         * @brief  场参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        Core::FITKParameter* _fieldPara{};
        /**
         * @brief  几何补丁类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        FITKOFSolverInitialEnum::FITKPatchApplyObjectType _applyType{ FITKOFSolverInitialEnum::FITKPatchApplyObjectType::Cells };
    };
    /**
     * @brief  初始条件数据对象
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-03
     */
    class FITKInterfaceFlowOFAPI FITKOFInitialConditions : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKOFInitialConditions);
    public:
        explicit FITKOFInitialConditions();
        virtual ~FITKOFInitialConditions();
        /**
         * @brief    获取是否初始化区域属性
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        bool getInitPropRegions();
        /**
         * @brief    设置是否初始化区域属性
         * @param[i] isPropReg 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void setInitPropRegions(bool isPropReg);
        /**
         * @brief    获取区域属性个数
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        int getInitPropRegionCount();
        /**
         * @brief    通过index获取区域属性
         * @param[i] index 
         * @return   FITKOFInitPropRegions *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        FITKOFInitPropRegions* getInitPropRegionByIndex(int index);
        /**
         * @brief    通过区域类型获取数据
         * @param[i] type 
         * @return   QList<FITKOFInitPropRegions*>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        QList<FITKOFInitPropRegions*> getInitPropRegionByType(FITKOFSolverTypeEnum::FITKOFRegionMeshType type);
        /**
         * @brief    通过区域网格ID获取区域属性
         * @param[i] regionMeshID 
         * @return   FITKOFInitPropRegions *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        FITKOFInitPropRegions* getInitPropRegionByRegionMeshID(int regionMeshID);
        /**
         * @brief    获取基础初始化
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        Core::FITKParameter* getBasicData();
        /**
         * @brief    获取几何补丁数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        int getPatchCount();
        /**
         * @brief    获取几何补丁
         * @param[i] index 
         * @return   FITKOFGeometryPatch *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        FITKOFGeometryPatch* getPatch(int index);
        /**
         * @brief    移除几何补丁
         * @param[i] index 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void removePatch(int index);
        /**
         * @brief    添加几何补丁
         * @param[i] patch 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-17
         */
        void appendPatch(FITKOFGeometryPatch* patch);
        /**
         * @brief    添加初始化区域属性
         * @param[i] regionProp 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        void addInitPropRegion(FITKOFInitPropRegions* regionProp);
        /**
         * @brief    移除初始化参数
         * @param[i] regionMeshID 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        void removeInitPropRegion(int regionMeshID);
        /**
         * @brief    清理全部初始化区域属性数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-03
         */
        void clearAllInitPropRegion();

    protected:
    private:
        /**
         * @brief  是否初始化区域属性
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        bool _isInitPropRegions{ false };
        /**
         * @brief  基础初始化参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-03
         */
        Core::FITKParameter* _basic{};
        /**
         * @brief  补丁数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        QList<FITKOFGeometryPatch*> _patchs{};
        /**
         * @brief  区域数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-17
         */
        QList<FITKOFInitPropRegions*> _regions{};
    };
}


#endif
