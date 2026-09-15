/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKRADIOSSNODEGROUP_H__
#define FITKRADIOSSNODEGROUP_H__


#include "FITKAbstractRadiossData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"


 
namespace Radioss
{
    class FITKRadiossNodeGroup;
    

    /**
     * @class FITKRadiossNodeGroupRadIOInfo
     * @brief Radioss节点组的RadIO信息类
     * @details 此类用于存储和管理Radioss节点组的RadIO相关信息，包括关键字和ID列表。
     *          提供设置和获取这些信息的接口。
     * @author libaojun
     * @date 2025-04-23
     */

    class FITKRadiossDataAPI FITKRadiossNodeGroupRadIOInfo
    {
        friend class FITKRadiossNodeGroup;

    public:
        explicit FITKRadiossNodeGroupRadIOInfo();
        virtual ~FITKRadiossNodeGroupRadIOInfo();

        /**
         * @brief 设置Radioss关键字
         * @param[i] keyWords 关键字
         * @author LiBaojun
         */
        void setRadiossKeyWord(const QString& keyWords);
        QString getRadiossKeyWord() { return _radiossKeyWord; }
        /**
         * @brief 设置Radioss ID列表
         * @param[i] ids ID列表
         * @author LiBaojun
         */
        void setRadiossIDs(const QList<int>& ids);
        QList<int> getRadiossIDs() { return _radiossIDs; }

    private:
        QString _radiossKeyWord; ///< Radioss关键字
        QList<int> _radiossIDs; ///< Radioss ID列表   part 或者 surface，根据关键字决定

    };

    class RadReadInformationMapper;
    class FITKRadiossAdaptorNodeGroup;
    class FITKRadiossMeshModel;
    
    /**
     * @class FITKRadiossNodeGroup
     * @brief Radioss节点组数据对象
     * @details 此类用于表示Radioss中的节点组数据，继承自FITKModelSet和FITKAbstractRadiossData。
     *          节点组可以包含多个节点，并提供对节点集合的管理和操作功能。
     * @author libaojun
     * @date 2025-04-23
     */
    class FITKRadiossDataAPI FITKRadiossNodeGroup : 
       public Interface::FITKModelSet, public FITKAbstractRadiossData
    {
        friend class RadReadInformationMapper;
        friend class FITKRadiossAdaptorNodeGroup;
        FITKCLASS(Radioss, FITKRadiossNodeGroup);
    public:
        explicit FITKRadiossNodeGroup();
        virtual ~FITKRadiossNodeGroup();

    private:
        FITKRadiossNodeGroupRadIOInfo* getRadIOInfo();

        void transformRadIOInfo(FITKRadiossMeshModel* model);

    private:
        FITKRadiossNodeGroupRadIOInfo* _radIOInfo{nullptr}; ///< Radioss RadIO信息

        // Other member functions and variables
    };
}

#endif // FITKRADIOSSNODEGROUP_H__