/**
 *
 * @file FITKFieldDiscrete.h
 * @brief 离散场声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-07-11
 *
 */
#ifndef __FITK_FIELD_DISCRETE_H__
#define __FITK_FIELD_DISCRETE_H__

#include "FITKAbstractFields.h"
#include <QList>

namespace Interface
{
    //离散场数据
    struct FieldData
    {
        int _id{ -1 };
        double _value{ 0.0 };
    };
    /**
     * @brief 离散场声明
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-07-11
     */
    class FITKInterfacePhysicsAPI FITKFieldDiscrete :
        public FITKAbstractFiled
    {
        Q_OBJECT
    public:
        /**
         * @brief 场位置
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        enum FITKDisFieldLoc
        {
            FDFLNone,
            FDFLEle,
            FDFLNode,
        };
        Q_ENUM(FITKDisFieldLoc);

    public:
        /**
         * @brief Construct a new FITKFieldDiscrete object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        explicit FITKFieldDiscrete() = default;
        /**
         * @brief Destroy the FITKFieldDiscrete object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        virtual ~FITKFieldDiscrete() = default;
        /**
         * @brief 获取场类型
         * @return FITKFieldType
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        virtual FITKFieldType getFieldType() override;
        /**
         * @brief 获取场位置
         * @return FITKDisFieldLoc
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        FITKDisFieldLoc getFieldLoc()const;
        /**
         * @brief 设置场位置
         * @param loc
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void setDisFieldLoc(FITKDisFieldLoc loc);
        /**
         * @brief 获取默认值
         * @return double
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        bool hasDefaultValue() const;
        /**
         * @brief 设置默认值
         * @param v    默认值
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void hasDefaultValue(const bool v);
        /**
         * @brief 设置默认值
         * @param[i] v
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void setDefaultValue(const double v);
        /**
         * @brief 获取默认值
         * @return double
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        double getDefaultValue() const;
        /**
         * @brief 获取离散场数据个数
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        int getFileDataCount();
        /**
         * @brief 获取离散场数据
         * @param index  索引
         * @return FieldData
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        FieldData getFieldData(const int index);
        /**
         * @brief 添加离散场数据
         * @param id
         * @param v  值
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void appendFieldData(int id ,double v);
        /**
         * @brief 插入离散场数据
         * @param index  索引
         * @param id
         * @param v  值
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void insertFieldData(int index, int id, int v);
        /**
         * @brief 移除离散场数据
         * @param index  索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void removeFieldData(int index);
        /**
         * @brief 清除离散场数据
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void clearFieldData();
        /**
        * @brief 复制数据对象,深拷贝，数据id不拷贝
        * @param[i] obj 被拷贝的对象
        * @return true
        * @return false
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-12
        */
        bool copy(FITKAbstractDataObject* obj) override;

    private:
        /**
         * @brief 场位置
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        FITKDisFieldLoc _fieldLoc{ FDFLEle };
        /**
         * @brief 默认值是否被设置
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        bool _hasDefaultValue{ false };
        /**
         * @brief 默认值
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        double _defaultValue{ 0 };
        /**
         * @brief 离散场数据
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        QList<FieldData> _fieldData{};
    };
}


#endif
