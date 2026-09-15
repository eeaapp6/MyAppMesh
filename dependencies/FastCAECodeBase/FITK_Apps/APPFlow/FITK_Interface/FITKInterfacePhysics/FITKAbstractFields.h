/**
 *
 * @file FITKAbstractFields.h
 * @brief 场数据抽象类与管理类声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-07-11
 *
 */
#ifndef _FITKABSTRACT_FIELDS_H___
#define _FITKABSTRACT_FIELDS_H___

#include <QString>
#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    /**
     * @brief 场数据抽象类
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-07-11
     */
    class FITKInterfacePhysicsAPI FITKAbstractFiled :
        public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 场类型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        enum FITKFieldType
        {
            FFTNone,        ///< 未定义
            FFTDiscrete,    ///< 离散场
            FFTAnalytical,  ///< 解析场
        };
        Q_ENUM(FITKFieldType);
    public:
        /**
         * @brief Construct a new FITKAbstractFiled object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        explicit FITKAbstractFiled() = default;
        /**
         * @brief Destroy the FITKAbstractFiled object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        virtual ~FITKAbstractFiled() = 0;
        /**
         * @brief 获取场类型
         * @return FITKFieldType
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        virtual FITKFieldType getFieldType() = 0;
        /**
         * @brief 设置描述
         * @param des  场描述
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void setDescribe(const QString & des);
        /**
         * @brief 获取描述
         * @return QString
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        QString getDescribe() const;
        /**
        * @brief 复制数据对象,深拷贝，数据id不拷贝
        * @param[i] obj 被拷贝的对象
        * @return true
        * @return false
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-07-12
        */
        virtual bool copy(FITKAbstractDataObject* obj) override;

    private:
        /**
         * @brief 描述
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        QString _describe{};

    };

    /**
     * @brief 场管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-07-11
     */
    class  FITKInterfacePhysicsAPI FITKFieldsManager :
        public Core::FITKAbstractDataObject,public Core::FITKAbstractDataManager<FITKAbstractFiled>
    {
    public:
        /**
         * @brief Construct a new FITKFieldsManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        explicit FITKFieldsManager() = default;
        /**
         * @brief Destroy the FITKFieldsManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        virtual ~FITKFieldsManager() = default;
        /**
         * @brief 根据类型获取场对象
         * @param[i]  t              类型
         * @return QList<FITKAbstractFiled*>
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        QList<FITKAbstractFiled*> getFields(FITKAbstractFiled::FITKFieldType t);
    };

}


#endif
