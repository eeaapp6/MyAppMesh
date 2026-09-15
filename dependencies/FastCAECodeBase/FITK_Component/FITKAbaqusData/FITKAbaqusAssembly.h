/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbaqusAssembly.h
 * @brief       Abaqus装配体数据。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-04-02
 *********************************************************************/

#ifndef __FITKABAQUSASSEMBLY_H__
#define __FITKABAQUSASSEMBLY_H__

#include "FITK_Interface/FITKInterfaceStructural/FITKAbaAssembly.h"

#include "FITKAbaqusDataAPI.h"

 // 前置声明
 //namespace Interface
 //{
 //    class FITKDatumList;
 //}

namespace AbaqusData
{
    /**
     * @brief       Abaqus装配体数据。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-04-02
     */
    class FITKAbaqusDataAPI FITKAbaqusAssembly : public Interface::FITKAbaAssembly
    {
        //FITKSafeDownCastA(FITKAbaqusAssembly, Interface::FITKAbaAssembly);
        // FITKCLASS(AbaqusData, FITKAbaqusAssembly);

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
        explicit FITKAbaqusAssembly();

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
        virtual ~FITKAbaqusAssembly();

        /**
         * @brief       析构当前装配基准元素。（移除装配体时调用）
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-03
         */
        void removeDatums();

        /**
         * @brief       获取基准元素管理器。
         * @return      基准元素管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
         // Interface::FITKDatumList* getDatumManager();

         /**
          * @brief       获取基准元素列表。
          * @return      基准元素ID列表
          * @author      ChengHaotian (yeguangbaozi@foxmail.com)
          * @date        2025-04-22
          */
        const QList<int> & getDatumList();

        /**
         * @brief       添加基准元素ID。
         * @param[in]   id：基准元素数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-22
         */
        void addDatum(int id);

        /**
         * @brief       移除基准元素ID，析构对象。
         * @param[in]   id：基准元素数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-22
         */
        void removeDatum(int id);

        /**
         * @brief       装配是否有几何。
         * @return      是否有几何
         * @author      YanZhiHui (chanyuantiandao@126.com)
         * @date        2025-09-01
         */
        bool hasGeometry();

        /**
         * @brief       装配是否有网格。
         * @return      是否有网格
         * @author      YanZhiHui (chanyuantiandao@126.com)
         * @date        2025-09-01
         */
        bool hasMesh();


        /**
         * @brief       装配是否有独立网格。
         * @return      是否有网格
         * @author      YanZhiHui (chanyuantiandao@126.com)
         * @date        2025-09-01
         */
        bool hasOrphanMesh();

    private:
        /**
         * @brief       初始化数据管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
        void init();

    private:
        /**
         * @brief       装配体基准元素管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-02
         */
         // Interface::FITKDatumList* m_datumManager{ nullptr };

         /**
          * @brief       部件基准元素ID。
          * @author      ChengHaotian (yeguangbaozi@foxmail.com)
          * @date        2025-04-22
          */
        QList<int> m_datumIDs;

    };
}

#endif //!__FITKABAQUSASSEMBLY_H__
