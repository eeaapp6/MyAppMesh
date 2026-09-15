/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKModelSet.h
 * @brief 集合类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-17
 *
 */
#ifndef _FITKMODELSET__H__
#define _FITKMODELSET__H__

#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModelComponent.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKModelEnum.h"
#include <QList>

namespace Interface
{
    /**
     * @brief 集合类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-17
     */
    class FITKInterfaceModelAPI FITKModelSet : public FITKAbstractModelComponent,
        public Core::FITKAbstractDataManager<FITKModelSet>
    {
        FITKCLASS(Interface, FITKModelSet);
        //        FITKSafeDownCastA(FITKModelSet, Core::FITKAbstractDataObject);

    public:
        /**
         * @brief Construct a new FITKModelSet object
         * @param[i]  type          集合类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        explicit FITKModelSet(FITKModelEnum::FITKModelSetType type);
        explicit FITKModelSet() = default;
        virtual ~FITKModelSet() = default;
        /**
         * @brief 获取集合类型
         * @return FITKModelEnum::FITKModelSetType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        virtual FITKModelEnum::FITKModelSetType getModelSetType();
        /**
         * @brief 判断是否为几何类型的混合几何（补丁方法）
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2025-11-12
         */
        bool isGeometrySet();
        /**
         * @brief 获取集合所属模型（顶级）维度
         * @return FITKModelEnum::FITKModelSetDim
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date 2025-09-25
         */
         //FITKModelEnum::FITKModelSetDim getModelSetParentDim();
         /**
          * @brief 是否获取集合类型
          * @param[i] t  FITKModelEnum::FITKModelSetType 类型
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-03-17
          */
        bool containsType(FITKModelEnum::FITKModelSetType t);
        bool containsTypes(QList<FITKModelEnum::FITKModelSetType> types);
        bool containsUniqueTypes(QList<FITKModelEnum::FITKModelSetType> types);
        

        /**
         * @brief 设置集合类型
         * @param[i] t  FITKModelEnum::FITKModelSetType 类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        void setModelSetType(FITKModelEnum::FITKModelSetType t);
        /**
         * @brief 是否为内部集合
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        bool isInternal();
        /**
         * @brief 是否为generate格式
         * @return true
         * @return false
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        bool isGenerated();
        /**
         * @brief 设置为内部格式集合
         * @param[i]  internal       内部集合
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        void setInternal(bool internal = true);
        /**
         * @brief 设置为generate格式
         * @param[i]  generated      是否为generate格式
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        void setGenerated(bool generated = true);
        /**
         * @brief 清空成员
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        void clearMember();
        /**
         * @brief 追加对象
         * @param[i]  mem           对象ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        void appendMember(const int & mem);
        /**
         * @brief 设置对象
         * @param[i]  mem           对象ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        void setAbsoluteMember(const QList<int>& mem);
        /**
         * @brief 获取成员
         * @return const QList<int>&
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        const QList<int> & getMember() const;
        /**
         * @brief 获取成员数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-29
         */
        int getMemberCount();

        /**
         * @brief 获取绝对成员，generate会展开所有成员
         * @return const QList<int>&
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        const QList<int>  getAbsoluteMember() const;
        /**
         * @brief 是否为组合集合
         * @return const QList<int>&
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        bool isCombination();
        /**
         * @brief 追加对象
         * @param[i] item          追加的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void appendDataObj(FITKAbstractDataObject* item) override;
        /**
         * @brief 根据类型获取集合，如果是复合集合则会递归获取子对象，返回值中不会存在复合集合
         * @param[o] sets   集合列表
         * @param[i] t      类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-19
         */
        void getAbsoluteSet(QList<FITKModelSet*> & sets, FITKModelEnum::FITKModelSetType t);

        /**
         * @brief 设置名称,复合集合子对象也重命名
         * @param[i]  name         名称
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-19
         */
        virtual void setDataObjectName(const QString & name) override;
        /**
         * @brief 获取第一个成员ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-07
         */
        int getFirstMemberID();

        /**
         * @brief       判断当前集合是否有效（ID对应的数据是否存在）。
         * @return      集合是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-11
         */
        bool isValid();

    protected:
        /**
         * @brief 集合类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        FITKModelEnum::FITKModelSetType _setType{ FITKModelEnum::FMSNone };
        /**
         * @brief 集合成员
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        QList<int> _setMember{};
        /**
         * @brief 内部集合标记
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        bool _internal{ false };
        /**
         * @brief 是否为generate格式
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        bool _generated{ false };
    };
}


#endif
