/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaModelInfo.h
 * @brief 集合部件等信息获取
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-04-03
 *
 */
#ifndef __FITKMODELINFO_H___
#define __FITKMODELINFO_H___

#include "FITKAbaqusDataAPI.h"



namespace Interface
{
    class FITKAbstractAssInstance;
    class FITKA;
    class FITKModelSet;
    class FITKModelSurface;
    class FITKAbaRefPoint;
    class FITKAbstractModelComponent;
    class FITKBoundaryLocation;
}

namespace AbaqusData
{
    class FITKAbaqusPart;
    class FITKAbaqusAssembly;
    /**
     * @brief Abaqus与模型相关的信息
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-04-03
     */
    struct  AbaModelInfo
    {
        AbaqusData::FITKAbaqusAssembly* _assembly{};
        Interface::FITKAbstractAssInstance* _instance{};
        AbaqusData::FITKAbaqusPart* _part{};
        Interface::FITKModelSet* _set{};
        Interface::FITKModelSurface* _surface{};
        Interface::FITKAbaRefPoint* _refPoint{};
    };

    /**
     * @brief 获取模型相关信息
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-04-03
     */
    class FITKAbaqusDataAPI FITKAbaModelInfo
    {
    public:
        /**
         * @brief 获取部件的所属信息
         * @param[i]  comp       部件对象，可以是集合 表面 参考点
         * @return AbaModelInfo
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        static AbaModelInfo GetModelInfo(Interface::FITKAbstractModelComponent* comp);
        /**
         * @brief 获取边界位置相关信息
         * @param[i]  loc            边界载荷等位置
         * @return AbaModelInfo
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        static AbaModelInfo GetModelInfo(Interface::FITKBoundaryLocation* loc);
        /**
         * @brief 获取部件的所属信息
         * @param[i]  compid       部件对象id，可以是集合 表面 参考点
         * @return AbaModelInfo
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-10
         */
        static AbaModelInfo GetModelInfo(const int componentid);
        /**
         * @brief 获取部件的所属信息
         * @param[i]  compid       部件对象id，可以是集合 表面 参考点
         * @param[i]  modelID      模型id
         * @return AbaModelInfo
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-10
         */
        static AbaModelInfo GetModelInfo(const int componentid, int modelID);
    private:
        /**
         * @brief 获取集合与表面信息
         * @param[o]  info           模型信息
         * @param[i]  set            部件对象 集合
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        static void getSetSurfInfo(AbaModelInfo* info, Interface::FITKAbstractModelComponent* set);
        /**
         * @brief 获取集合与表面信息
         * @param[o]  info           模型信息
         * @param[i]  surf            部件对象，表面
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        static void getRefPtInfo(AbaModelInfo* info, Interface::FITKAbaRefPoint* surf);

    private:
        FITKAbaModelInfo() = default;
        FITKAbaModelInfo(const FITKAbaModelInfo&) = default;
        ~FITKAbaModelInfo() = default;
    };
}
#endif
