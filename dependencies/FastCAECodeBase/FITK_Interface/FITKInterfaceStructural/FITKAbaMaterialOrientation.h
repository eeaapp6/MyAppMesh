/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaMaterialOrientation.h
 * @brief 材料方向管理器
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-06-06
 *
 */
#ifndef _FITK_MATERIAL_ORIENTATION_H__
#define _FITK_MATERIAL_ORIENTATION_H__

#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{


    class FITKAbstractModel;
    /**
     * @brief 材料方向
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-06-06
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialOrientation :
        public Core ::FITKAbstractNDataObject
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbaMaterialOrientation);
    public:
        enum MaterialOrientationDef
        {
            MOGolbalSys, ///< 全局
            MOCoorSys,   ///< 坐标系
        };
        Q_ENUM(MaterialOrientationDef);

    public:
        /**
         * @brief Construct a new FITKAbaMaterialOrientation object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        explicit  FITKAbaMaterialOrientation() = default;
        /**
         * @brief Destroy the FITKAbaMaterialOrientation object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        virtual ~FITKAbaMaterialOrientation() = default;
        /**
         * @brief 设置材料方向定义方式
         * @param[i]  t              定义方式
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        void setMaterialOrientationDefTpye(MaterialOrientationDef t);
        /**
         * @brief 获取材料方向定义方式
         * @return MaterialOrientationDef
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        MaterialOrientationDef getMaterialOrientationDefTpye() const;

        /**
         * @brief 设置集合ID
         * @param[i]  id             集合ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        void setModelSetID(const int id);
        /**
         * @brief 获取集合ID
         * @return int 集合ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        int getModelSetID() const;
        /**
         * @brief 设置坐标系ID，默认全局坐标系，ID为-1
         * @param[i]  id             材料坐标系ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        void setCsysID(const int id);
        /**
         * @brief 获取坐标系ID
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        int getCsysID() const;
        /**
         * @brief 设置轴线索引，有效值1-3，分别表示第一二三坐标轴
         * @param[i]  index          坐标轴索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        void setRotateAxis(const int index);
        /**
         * @brief 获取轴线索引
         * @return int
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        int getRotateAxis() const;
        /**
         * @brief 设置旋转角度，单位为度
         * @param[i]  angle          旋转角度
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        void setRotateAngle(const double angle);
        /**
         * @brief 获取旋转角度
         * @return double
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        double getRotateAngle() const;
        /**
         * @brief 设置是否启用附加旋转角度 None为不启用，输入值为false
         * @param[i]  en             是否启用
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        void enableAdditionRotationAngle(bool en = true );
        /**
         * @brief 获取是否启用附加旋转角度  fase为不启用
         * @return bool
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        bool isEnableAdditionRotationAngle() const;

        /**
         * @brief 设置材料堆叠方向 1-3分别对应单元坐标系三个方向，4代表壳单元法线
         * @param[i]  dir           堆叠方向
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        void setStackingDir(const int dir);
        /**
         * @brief 获取材料堆叠方向
         * @return int 1-3分别对应单元坐标系三个方向，4代表壳单元法线
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        int getStackingDir() const;
        /**
         * @brief 获取关联的部件
         * @return FITKAbstractModel*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        FITKAbstractModel* getPart();
        /**
         * @brief 拷贝数据对象
         * @param[i]  obj           被拷贝的对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        virtual bool copy(FITKAbstractDataObject* obj) override;

    private:
        MaterialOrientationDef _materialOriDef{ MaterialOrientationDef::MOGolbalSys };
        /**
         * @brief 集合ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        int _setID{ -1 };
        /**
         * @brief 坐标系ID
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        int _csysID{ -1 };
        /**
         * @brief 旋转轴索引
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        int _rotateAxisIndex{ 1 };
        /**
         * @brief 旋转角度是否为None
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        bool _enableRotationAngle{ false };
        /**
         * @brief 旋转角度
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        double _rotateAngle{ 0 };
        /**
         * @brief 堆叠方向
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        int _stackingDir{ 1 };
    };
    /**
     * @brief 材料方向管理器
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-06-06
     */
    class FITKInterfaceStructuralAPI FITKAbaMaterialOrientationManager
        : public Core::FITKAbstractDataManager<FITKAbaMaterialOrientation>
    {
    public:
        /**
         * @brief Construct a new FITKAbaMaterialOrientationManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        explicit FITKAbaMaterialOrientationManager() = default;
        /**
         * @brief Destroy the FITKAbaMaterialOrientationManager object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        virtual ~FITKAbaMaterialOrientationManager() = default;

        /**
         * @brief 根据集合获取材料方向
         * @param[i]  setID          集合ID
         * @return FITKAbaMaterialOrientation*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-06-06
         */
        FITKAbaMaterialOrientation* getMaterialOrientation(const int setID );
    };
}



#endif
