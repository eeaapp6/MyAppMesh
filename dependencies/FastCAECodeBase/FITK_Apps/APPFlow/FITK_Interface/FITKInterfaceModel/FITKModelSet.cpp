/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKModelSet.h"
#include "FITKAbstractAssInstance.h"
#include "FITKAbstractPart.h"
#include "FITKUnstructuredMesh.h"
#include "FITKAbstractElement.h"

namespace Interface
{
    //可以表示多种类型
    FITKModelSet::FITKModelSet(FITKModelEnum::FITKModelSetType type)
        :_setType(type)
    {
    }

    Interface::FITKModelEnum::FITKModelSetType FITKModelSet::getModelSetType()
    {
        //不是复合集合类型
        if (!isCombination())
            return _setType;
        //复合集合
        const int n = this->getDataCount();
        if (n < 2) return FITKModelEnum::FMSNone;
        FITKModelSet* set = this->getDataByIndex(0);
        if (set == nullptr) return FITKModelEnum::FMSNone;
        //第一个集合类型
        auto firstType = set->getModelSetType();
        for (int i = 0; i < n; ++i)
        {
            set = this->getDataByIndex(i);
            if (set == nullptr) continue;
            auto t = set->getModelSetType();
            if (t != firstType) return FITKModelEnum::FMSMIX;
        }
        return firstType;
    }

    bool FITKModelSet::isGeometrySet() {
        auto t = getModelSetType();

        const QList<FITKModelEnum::FITKModelSetType> geomTypes = {
            FITKModelEnum::FMSPoint,    ///< 几何点
            FITKModelEnum::FMSEdge,     ///< 几何线
            FITKModelEnum::FMSSurface,  ///< 几何面
            FITKModelEnum::FMSSolid,    ///< 几何实体
            FITKModelEnum::FMSComb    ///< 几何装配
        };

        if (geomTypes.contains(t)) return true;
        else if (t != FITKModelEnum::FMSMIX) return false;
        else {
            auto set = this->getDataByIndex(0);
            return geomTypes.contains(set->getModelSetType());
        }
    }


    bool FITKModelSet::containsType(FITKModelEnum::FITKModelSetType t)
    {
        if (t == this->getModelSetType()) return true;
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            auto set = this->getDataByIndex(i);
            if (set == nullptr) continue;
            auto st = set->getModelSetType();
            if (t == st) return true;
        }
        return false;
    }

    bool FITKModelSet::containsTypes(QList<FITKModelEnum::FITKModelSetType> types)
    {
        if (types.contains(this->getModelSetType())) return true;
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            auto set = this->getDataByIndex(i);
            if (set == nullptr) continue;
            auto st = set->getModelSetType();
            if (types.contains(st)) return true;
        }
        return false;
    }

    bool FITKModelSet::containsUniqueTypes(QList<FITKModelEnum::FITKModelSetType> types)
    {
        if (types.contains(this->getModelSetType())) return true;
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            auto set = this->getDataByIndex(i);
            if (set == nullptr) continue;
            auto st = set->getModelSetType();
            if (!types.contains(st)) return false;
        }
        return n > 1;
    }

    void FITKModelSet::setModelSetType(FITKModelEnum::FITKModelSetType t)
    {
        //类型
        _setType = t;
    }

    bool FITKModelSet::isInternal()
    {
        if (!isCombination())
            return _internal;
        const int n = this->getDataCount();
        for (int i = 1; i < n; ++i)
        {
            auto set = this->getDataByIndex(i);
            if (set == nullptr) continue;
            if (!set->isInternal())
                return false;
        }
        return true;
    }

    bool FITKModelSet::isGenerated()
    {
        return _generated;
    }

    void FITKModelSet::setInternal(bool internal /*= true */)
    {
        _internal = internal;
    }

    void FITKModelSet::setGenerated(bool generated /*= true*/)
    {
        _generated = generated;
    }

    void FITKModelSet::clearMember()
    {
        //清空
        _setMember.clear();
    }

    void FITKModelSet::appendMember(const int & mem)
    {
        //在成员中记录
        _setMember.append(mem);
    }

    void FITKModelSet::setAbsoluteMember(const QList<int>& mem)
    {
        _setMember = mem;
        qSort(_setMember);
        //判断是否generate
        const int nCount = _setMember.size();

        if (nCount < 3) return;
        const int start = _setMember.first();
        const int end = _setMember.last();
        const int intv = _setMember.at(1) - start;
        //假设是generate
        _generated = true;
        for (int i = 2; i < nCount; ++i)
        {
            if (intv != _setMember[i] - _setMember[i - 1])
            {
                //两个数值之间的差值不一样就不是generate
                _generated = false;
                return;
            }
        }
        //重新记录参数
        if (_generated)
        {
            _setMember.clear();
            _setMember.append(start);
            _setMember.append(end);
            _setMember.append(intv);
        }

    }

    const QList<int> & FITKModelSet::getMember() const
    {
        //获取链表
        return _setMember;
    }

    int FITKModelSet::getMemberCount()
    {
        //generate类型展开
        if (_generated)
            return getAbsoluteMember().size();
        //链表长度
        return _setMember.size();
    }

    const QList<int> FITKModelSet::getAbsoluteMember() const
    {
        if (!_generated) return _setMember;
        //generate的成员数量是3的倍数
        if (_setMember.size() % 3 != 0) return QList<int>();
        const int groupCount = _setMember.size() / 3;
        QList<int> mem;
        //对每组进行遍历细分
        for (int i = 0; i < groupCount; i++)
        {
            const int start = _setMember.at(i * 3);
            const int end = _setMember.at(i * 3 + 1);
            const int step = _setMember.at(i * 3 + 2);

            for (int num = start; num <= end; num += step)
                mem.append(num);
        }
        return mem;
    }

    bool FITKModelSet::isCombination()
    {
        return this->getDataCount() > 0;
    }

    void FITKModelSet::appendDataObj(FITKAbstractDataObject* item)
    {
        Core::FITKAbstractDataManager<FITKModelSet>::appendDataObj(item);
        //默认为内部集合，有一个不是内部集合的则认为是非内部集合
        _internal = true;
        for (int i = 0; i < this->getDataCount(); ++i)
        {
            FITKModelSet* set = this->getDataByIndex(i);
            if (set == nullptr || set->isInternal()) continue;
            //非内部集合
            _internal = false;
            break;
        }

    }

    void FITKModelSet::getAbsoluteSet(QList<FITKModelSet*> & sets, FITKModelEnum::FITKModelSetType t)
    {
        //不是复合集合
        if (!this->isCombination())
        {
            if (_setType == t) sets.append(this);
        }
        else  //复合集合
        {
            //递归遍历成员
            const int nSet = this->getDataCount();
            for (int i = 0; i < nSet; ++i)
            {
                auto asubSet = this->getDataByIndex(i);
                if (asubSet == nullptr) continue;
                //递归遍历
                asubSet->getAbsoluteSet(sets, t);
            }
        }
    }

    void FITKModelSet::setDataObjectName(const QString & name)
    {
        FITKAbstractNDataObject::setDataObjectName(name);
        //递归遍历成员
        const int nSet = this->getDataCount();
        for (int i = 0; i < nSet; ++i)
        {
            auto asubSet = this->getDataByIndex(i);
            if (asubSet == nullptr) continue;
            //递归修改
            asubSet->setDataObjectName(name);
        }
    }

    int FITKModelSet::getFirstMemberID()
    {
        if (_setMember.isEmpty()) return -1;
        return _setMember.first();
    }

    bool FITKModelSet::isValid()
    {
        // 判断模型是否存在。
        FITKAbstractModel* model = getAbstractModel();
        if (!model)
        {
            return false;
        }

        bool isValid = true;
        int nSubSets = getDataCount();
        if (nSubSets == 0)
        {
            isValid = model->isComponentValid(this->getModelSetType(), this->getAbsoluteMember());
        }
        else
        {
            for (int i = 0; i < nSubSets; i++)
            {
                FITKModelSet* subSet = getDataByIndex(i);
                if (!subSet)
                {
                    return false;
                }

                isValid &= subSet->isValid();
            }
        }

        return isValid;
    }
}
