/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossPostData.h
 * @brief  后处理数据
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-08
 *********************************************************************/
#ifndef __FITKRADIOSSPOST_DATA_H__
#define __FITKRADIOSSPOST_DATA_H__

#include "FITKRadiossDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    class FITKStruPost3DManager;
}

namespace Radioss
{
    class FITKRadiossPost2DManager;
    /**
     * @brief  Radioss Post Data
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class FITKRadiossDataAPI FITKRadiossPostData
        : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief  Construct a new FITKRadiossPostData object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        explicit FITKRadiossPostData();
        /**
         * @brief  Destroy the FITKRadiossPostData object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        virtual ~FITKRadiossPostData();
        /**
         * @brief  从app框架获取后处理数据
         * @return FITKRadiossPostData*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        static FITKRadiossPostData* GetDataFromAPPFrame();
        /**
         * @brief  获取三维后处理数据管理
         * @return Interface::FITKStruPost3DManager*
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        Interface::FITKStruPost3DManager* getPost3DManager();
        /**
         * @brief    获取二维后处理数据管理
         * @return   FITKRadiossPost2DManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        FITKRadiossPost2DManager* getPost2DManager();
        /**
         * @brief    获取当前后处理3维数据ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        int getCurrentPostDataID();
        /**
         * @brief    设置当前后处理3维数据ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        void setCurrentPostDataID(const int id);
        /**
         * @brief    获取动画时间间隔
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        int getAnimTimeInterval();
        /**
         * @brief    设置动画时间间隔
         * @param[i] time 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        void setAnimTimeInterval(int time);

    private:

        /**
         * @brief  三维对象数据管理
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        Interface::FITKStruPost3DManager* _post3DManager{};
        /**
         * @brief  二维对象数据管理
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-21
         */
        FITKRadiossPost2DManager* _post2DManager{};
        /**
         * @brief  当前后处理3D数据ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-21
         */
        int _current3DDataID{ -1 };
        /**
         * @brief  动画时间间隔 1000ms
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        int _timeInterval{ 1000 };
    };
    /**
     * @brief  2维后处理数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-21
     */
    class FITKRadiossDataAPI FITKRadiossPost2DData : public Core::FITKAbstractNDataObject
    {
    public:
        /**
         * @brief  Construct a new FITKRadiossPost2DData object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        explicit FITKRadiossPost2DData() = default;
        /**
         * @brief  Destroy the FITKRadiossPost2DData object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        virtual ~FITKRadiossPost2DData() = default;
        /**
         * @brief    设置算例ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        void setCaseDataID(int id);
        /**
         * @brief    获取算例ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        int getCaseDataID();
        /**
         * @brief    设置解决方案ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        void setSolutionID(int id);
        /**
         * @brief    获取解决方案ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        int getSolutionID();
        /**
         * @brief    设置文件名称
         * @param[i] file 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        void setFileName(QString file);
        /**
         * @brief    设置文件名称
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        QString getFileName();

    private:
        /**
         * @brief  算例ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-21
         */
        int _caseDataID{ -1 };
        /**
         * @brief  求解方案ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-21
         */
        int _solutionID{ -1 };
        /**
         * @brief  文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-21
         */
        QString _fileName{};
    };
    /**
     * @brief  2维后处理数据管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-21
     */
    class FITKRadiossDataAPI FITKRadiossPost2DManager : public Core::FITKAbstractDataManager<FITKRadiossPost2DData>
    {
    public:
        /**
         * @brief  Construct a new FITKRadiossPost2DManager object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        explicit FITKRadiossPost2DManager() = default;
        /**
         * @brief  Destroy the FITKRadiossPost2DManager object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        virtual ~FITKRadiossPost2DManager() = default;
    };
}

#endif
