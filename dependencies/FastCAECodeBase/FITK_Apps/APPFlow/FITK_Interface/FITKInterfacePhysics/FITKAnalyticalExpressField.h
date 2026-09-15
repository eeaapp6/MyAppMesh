/**
 *
 * @file FITKAnalyticalExpressField.h
 * @brief 解析场，表达式定义
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-07-11
 *
 */
#ifndef  __FITK_ANALY_EXPRESS_FIELD_H__
#define  __FITK_ANALY_EXPRESS_FIELD_H__


#include "FITKAbstractFields.h"

namespace Interface
{
    /**
     * @brief 解析场表达式
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-07-11
     */
    class FITKInterfacePhysicsAPI FITKFieldAanlyticalExpress :
        public FITKAbstractFiled
    {
    public:
        /**
         * @brief Construct a new FITKFieldAanlyticalExpress object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        explicit FITKFieldAanlyticalExpress() = default;
        /**
         * @brief Destroy the FITKFieldAanlyticalExpress object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        virtual ~FITKFieldAanlyticalExpress() = default;
        /**
         * @brief 场类型
         * @return FITKFieldType
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        virtual FITKFieldType getFieldType() override;
        /**
         * @brief 设置表达式
         * @param express
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        void setExpression(const QString& express);
        /**
         * @brief 获取表达式
         * @return QString
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        QString getExpression() const;


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
         * @brief 表达式
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-07-11
         */
        QString _expression{};

    };

}


#endif
