/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorSet.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

namespace IO
{
    QString FITKInterfaceHDF5AdaptorSet::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorSet";
    }

    void FITKInterfaceHDF5AdaptorSet::setCompomentManager(Interface::FITKComponentManager* compManager)
    {
        _compManager = compManager;
    }


    bool FITKInterfaceHDF5AdaptorSet::adaptR()
    {
        auto set = dynamic_cast<Interface::FITKModelSet*>(_dataObj);
        if (!_reader || !set || !_h5Group || !_compManager) return false;
        bool isR = true;

        isR &= readSet(set, *_h5Group);
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorSet::adaptW()
    {
        auto set = dynamic_cast<Interface::FITKModelSet*>(_dataObj);
        if (!_writer || !set || !_h5Group) return false;
        bool isW = true;

        isW &= writeSet(set, *_h5Group);
        return isW;
    }

    bool FITKInterfaceHDF5AdaptorSet::readSet(Interface::FITKModelSet* set, H5::Group& h5Group)
    {
        if (set == nullptr) return false;
        //临时保存
        Interface::FITKModelSet *tempSet = set;
        bool isR = true;
        bool isValid = false;

        //写出基础信息
        isR &= readNDataObject(set, h5Group);
        //模型ID
        int absModelID = readIntAttribute(h5Group, "AbsModelID");
        set->setModel(absModelID);
        //写出基础信息
        bool isInternal = readBoolAttribute(h5Group, "isInternal");
        set->setInternal(isInternal);
        bool isGenerated = readBoolAttribute(h5Group, "isGenerated");
        set->setGenerated(isGenerated);
        bool isCombination = readBoolAttribute(h5Group, "isCombination");
        //类型
        auto sType = readStrAttribute(h5Group, "Type");
        //字符转换枚举
        Core::FITKEnumTransfer<Interface::FITKModelEnum::FITKModelSetType> fitkSetTypeTrafer;
        auto eTypeStr = fitkSetTypeTrafer.fromString(QString::fromStdString(sType), isValid);
        if (!isValid) return false;
        set->setModelSetType(eTypeStr);
        if (set->getDataObjectID() == 207)
        {
            set->setModelSetType(eTypeStr);
        }

        int ChildCount = readIntAttribute(h5Group, "ChildCount");
        for (int i = 0; i < ChildCount; ++i)
        {
            QString indexName = QString::number(i);

            int n, m;
            if (!getDataSetDim(h5Group, indexName.toStdString(), n, m)) return false;

            auto setDataset = h5Group.openDataSet(indexName.toStdString());

            //复合集合
            if (isCombination)
            {
                Interface::FITKModelSet *cSet = new Interface::FITKModelSet();
                isR &= readNDataObject(cSet, setDataset);
                //模型ID
                int absModelID = readIntAttribute(setDataset, "AbsModelID");
                cSet->setModel(absModelID);
                //写出基础信息
                bool fisInternal = readBoolAttribute(setDataset, "isInternal");
                cSet->setInternal(fisInternal);
                bool fisGenerated = readBoolAttribute(setDataset, "isGenerated");
                cSet->setGenerated(fisGenerated);
                //类型
                auto sType = readStrAttribute(setDataset, "Type");
                //字符转换枚举
                Core::FITKEnumTransfer<Interface::FITKModelEnum::FITKModelSetType> fitkSetTypeTrafer;
                auto eTypeStr = fitkSetTypeTrafer.fromString(QString::fromStdString(sType), isValid);
                if (!isValid) return false;
                set->appendDataObj(cSet);
                //_compManager->appendDataObj(cSet);
                cSet->setModelSetType(eTypeStr);
                tempSet = cSet;
            }

            //Sheet Set Data
            hsize_t       dim1[] = { 1 };
            H5::DataSpace mspace1(1, dim1);
            hsize_t       coordPtData[1][2];
            coordPtData[0][1] = 0;

            hsize_t       dimPtIDData[2]{ n ,1 };// 行数 // 列数
            H5::DataSpace ptIDDataSpace(2, dimPtIDData);
            for (int i = 0; i < dimPtIDData[0]; ++i)
            {
                coordPtData[0][0] = i;
                //Set ID
                int setID = -1;
                ptIDDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
                setDataset.read(&setID, H5::PredType::NATIVE_INT, mspace1, ptIDDataSpace);
                tempSet->appendMember(setID);
            }
        }
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorSet::writeSet(Interface::FITKModelSet* set, H5::Group & h5Group)
    {
        if (set == nullptr) return false;
        bool isW = true;
        bool isValid = false;

        std::string groupName = createParentAttribute(set, h5Group);
        if (groupName.empty())return false;

        //创建集合的数据块
        H5::Group setGroup = h5Group.createGroup(groupName);

        //写出基础信息
        isW &= writeNDataObject(set, setGroup);

        //枚举转换字符
        Core::FITKEnumTransfer<Interface::FITKModelEnum::FITKModelSetType> fitkSetTypeTrafer;
        //写出类型
        QString TypeStr = fitkSetTypeTrafer.toString(set->getModelSetType(), isValid);
        if (!isValid) return false;
        writeStrAttribute(setGroup, "Type", TypeStr.toStdString());

        //写出唯一类型
        writeStrAttribute(setGroup, "UniqueType", "Set");
        //模型ID
        int absModelID = set->getAbsModelID();
        writeIntAttribute(setGroup, "AbsModelID", &absModelID);
        //写出基础信息
        bool isInternal = set->isInternal();
        bool isGenerated = set->isGenerated();
        bool isCombination = set->isCombination();
        writeBoolAttribute(setGroup, "isInternal", &isInternal);
        writeBoolAttribute(setGroup, "isGenerated", &isGenerated);
        writeBoolAttribute(setGroup, "isCombination", &isCombination);

        QList<Interface::FITKModelSet*> sets;
        if (isCombination)
        {
            for (int i = 0; i < set->getDataCount(); i++)
            {
                sets.append(set->getDataByIndex(i));
            }
        }
        else
        {
            sets.append(set);
        }

        int count = sets.size();
        for (int i = 0; i < count; ++i)
        {
            auto mList = sets[i]->getMember();
            int mListCount = mList.count();
            //错误 set下没有任何数据
            //if (mListCount < 1) return false;

            QString indexName = QString::number(i);
            //Sheet Set Data
            hsize_t       dim1[] = { 1 };
            H5::DataSpace mspace1(1, dim1);
            hsize_t       coordPtData[1][2];
            coordPtData[0][1] = 0;

            hsize_t       dimPtIDData[2]{ mListCount ,1 };// 行数 // 列数
            H5::DataSpace ptIDDataSpace(2, dimPtIDData);
            H5::DataSet   ptIDData = setGroup.createDataSet(indexName.toStdString(), H5::PredType::NATIVE_ULLONG, ptIDDataSpace);

            //复合集合
            if (isCombination)
            {
                //枚举转换字符
                Core::FITKEnumTransfer<Interface::FITKModelEnum::FITKModelSetType> fitkSetTypeTrafer;
                //写出类型
                QString scTypeStr = fitkSetTypeTrafer.toString(sets[i]->getModelSetType(), isValid);
                if (!isValid) return false;

                writeStrAttribute(ptIDData, "Type", scTypeStr.toStdString());
                //写出唯一类型
                writeStrAttribute(ptIDData, "UniqueType", "Set");
                //模型ID
                int absModelID = sets[i]->getAbsModelID();
                writeIntAttribute(ptIDData, "AbsModelID", &absModelID);
                //写出基础信息
                bool fisInternal = sets[i]->isInternal();
                bool fisGenerated = sets[i]->isGenerated();
                writeBoolAttribute(ptIDData, "isInternal", &fisInternal);
                writeBoolAttribute(ptIDData, "isGenerated", &fisGenerated);
                isW &= writeNDataObject(sets[i], ptIDData);
            }

            for (int i = 0; i < dimPtIDData[0]; ++i)
            {
                coordPtData[0][0] = i;
                //Set ID
                ptIDDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
                ptIDData.write(&mList[i], H5::PredType::NATIVE_INT, mspace1, ptIDDataSpace);
            }
        }
        writeIntAttribute(setGroup, "ChildCount", &count);
        return isW;
    }
}


