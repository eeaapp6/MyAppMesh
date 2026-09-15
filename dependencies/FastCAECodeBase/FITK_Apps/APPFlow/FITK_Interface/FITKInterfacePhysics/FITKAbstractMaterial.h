/**
 *
 * @file FITKAbstractMaterial.h
 * @brief 抽象材料类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-29
 *
 */
#ifndef _FITKABSTRACTMATERIAL_H___
#define _FITKABSTRACTMATERIAL_H___

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKInterfacePhysicsAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    /**
     * @brief 抽象材料类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-29
     */
    class FITKInterfacePhysicsAPI FITKAbstractMaterial : public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKAbstractMaterial();
        virtual ~FITKAbstractMaterial() = 0;
        /**
         * @brief 获取材料id，注意：该值与管理器的中使用的id无关，与其他对象的关联也无关
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        int getMaterialID() const;
        /**
         * @brief 获取材料描述
         * @return QString
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-13
         */
        QString getMaterialDescribe() const;
        /**
         * @brief 设置材料描述
         * @param[i] des 材料描述
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-13
         */
        void setMaterialDescribe( const QString& des);
    private:
        /**
         * @brief 材料id，与管理器无关，不能被使用
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        int _materialID{ -1 };
        /**
        * @brief 材料描述
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-06-13
        */
        QString _describe{};
    };

    /**
     * @brief 材料管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfacePhysicsAPI FITKMaterialManager : public Core::FITKAbstractDataObject
        , public Core::FITKAbstractDataManager<FITKAbstractMaterial>
    {
    public:
        explicit FITKMaterialManager() = default;
        virtual ~FITKMaterialManager() = default;
    };

}

#endif // !_FITKABSTRACTMATERIAL_H___
