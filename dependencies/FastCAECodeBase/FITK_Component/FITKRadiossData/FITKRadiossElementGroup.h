/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRadiossElementGroup.h
 * @brief Element group representation for Radioss
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-09-24
 * 
 */
#ifndef FITK_RADIOS_ELEMENT_GROUP_H
#define FITK_RADIOS_ELEMENT_GROUP_H

#include "FITKAbstractRadiossData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"


namespace Radioss
{
    class FITKRadiossElementGroup;

    /**
     * @class FITKRadiossElementGroupRadIOInfo
     * @brief Radioss单元组的RadIO信息类
     * @details 此类用于存储和管理Radioss节点组的RadIO相关信息，包括关键字和ID列表。
     *          提供设置和获取这些信息的接口。
     * @author wangning (2185896382@qq.com)
     * @date 2025-09-28
     */

    class FITKRadiossDataAPI FITKRadiossElementGroupRadIOInfo : public QObject
    {
        friend class FITKRadiossElementGroup;
        Q_OBJECT
    public:
        //次类型  wangning  2025-09-28
        enum ElementWayType
        {
            SameProp,   //同属性，比如/GRQUAD/QUAD,/GRSHEL/SHEL,后四个字母分别相同
            Equal,      //与ElementGroupType同名类型，比如/GRSHEL/GRSHEL, /GRQUAD/GRQUAD
            BOX,
            GEN_INCR,
            GENE,
            MAT,
            PART,
            PROP,
            SUBMODEL,
            SUBSET,
        }; Q_ENUM(ElementWayType)
    public:
        explicit FITKRadiossElementGroupRadIOInfo(FITKRadiossElementGroup* eleGroup);
        virtual ~FITKRadiossElementGroupRadIOInfo() {};

        /**
         * @brief 设置Radioss关键字
         * @param[i] keyWords 关键字
         */
        void setRadiossKeyWord(const QString& keyWords);

        ElementWayType getElementWayType() { return _elementWayType; }
        /**
         * @brief 设置Radioss ID列表
         * @param[i] ids ID列表
         */
        void setRadiossIDs(const QList<int>& ids) { _radiossIDs = ids; };;
        QList<int> getRadiossIDs() { return _radiossIDs; }

    private:
        QString _radiossKeyWord; ///< Radioss关键字
        QList<int> _radiossIDs; ///< Radioss ID列表   part 或者 surface，根据关键字决定
        FITKRadiossElementGroup* _eleGroup{};
        ElementWayType _elementWayType{ ElementWayType::SameProp };
    };

    class RadReadInformationMapper;
    class FITKRadiossAdaptorElementGroup;
    class FITKRadiossMeshModel;
    /**
     * @brief Element group representation for Radioss
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-09-24
     */
    class FITKRadiossDataAPI FITKRadiossElementGroup
        : public Interface::FITKModelSet, public FITKAbstractRadiossData
    {
        friend class RadReadInformationMapper;
        friend class FITKRadiossAdaptorElementGroup;
        Q_OBJECT
        FITKCLASS(Radioss, FITKRadiossElementGroup);
    public:
        /**
         * @brief Element group type enumeration
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-24
         */
        enum ElementGroupType
        {
            GRNone,
            GRBRIC,
            GRBEAM,
            GRTRUS,
            GRQUAD,
            GRSH3N,
            GRSHEL,
            GRSPRI,
            GRTRIA,
        }; Q_ENUM(ElementGroupType)
    public:
        /**
         * @brief Construct a new FITKRadiossElementGroup object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-24
         */
        explicit FITKRadiossElementGroup();
        virtual ~FITKRadiossElementGroup();
        /**
         * @brief Get the Element Group Key Words object
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-24
         */
        QString getElementGroupKeyWords();
        /**
         * @brief Get the Element Group Type object
         * @return ElementGroupType 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-24
         */
        ElementGroupType getElementGroupType() const;
        /**
         * @brief Set the Element Group Type object
         * @param[i] type 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-24
         */
        void setElementGroupType(const ElementGroupType & type);    
   
    private:
        FITKRadiossElementGroupRadIOInfo* getRadIOInfo();

        void transformRadIOInfo(FITKRadiossMeshModel* model);


    private:        
        /**
         * @brief Element group type
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-24
         */
        ElementGroupType _elementGroupType{ ElementGroupType::GRNone };
        ///< Radioss RadIO信息
        FITKRadiossElementGroupRadIOInfo* _radIOInfo{ nullptr };
    };



}



#endif