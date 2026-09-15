/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMeshSeed.h
 * @brief  种子点
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-05-22
 *********************************************************************/
#ifndef FITKMeshSeed_H_
#define FITKMeshSeed_H_

#include "FITKInterfaceMeshGenAPI.h"

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"

namespace Interface
{
    /**
     * @brief  种子点信息
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-05-26
     */
    struct FITKMeshSeedInfo
    {
        //位置
        Core::FITKPoint _point;
        //在线上的比例（0-1）
        double _ratio = 0.0;
        //边id（实际几何边，不包含合并边id）
        int _edgeId = -1;
        //线上的比例（当前边为合并边时有效）
        double _mergeRatio = 0.0;
        //当前点对应的实际几何点id（无实际点对应id = -1）
        int _nodeID = -1;
    };
    /**
     * @brief  种子点
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-05-22
     */
    class FITKInterfaceMeshGenAPI FITKMeshSeed : public Core::FITKAbstractNDataObject
    {
        FITKCLASS(Interface, FITKMeshSeed);
        Q_OBJECT;
    public:
        /**
         * @brief  种子点类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        enum FITKSeedType{
            TNone,         //空
            TLocal,        //局部
            TGlobal,       //全局
        };
        Q_ENUM(FITKSeedType);
        /**
         * @brief  布种约束
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        enum FITKLocalSeedConstraint {
            CIncreaseOrDecrease,       //允许增加或减少
            COnlyIncrease,             //只允许单元数增加
            CNotChange,                //不允许修改单元数
        };
        Q_ENUM(FITKLocalSeedConstraint);
        /**
         * @brief  布种方法
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        enum FITKLocalSeedMethod{
            MBySize,              //按尺寸
            MByNum,               //按个数
        };
        Q_ENUM(FITKLocalSeedMethod);
        /**
         * @brief  布种偏移方式
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        enum FITKLocalSeedBias {
            BNone,               //无
            BSingle,             //单精度
            BDouble,             //两者
        };
        Q_ENUM(FITKLocalSeedBias);
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        FITKMeshSeed(int virTopoID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        virtual ~FITKMeshSeed();
        /**
         * @brief    获取种子点类型
         * @return   FITKSeedType               类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        FITKMeshSeed::FITKSeedType getSeedType();
        /**
         * @brief    获取虚拓扑ID
         * @return   int                        虚拓扑ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        int getVirTopoId();
        /**
         * @brief    获取种子点
         * @return   QList<Core::FITKPoint>     种子点列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        QList<Core::FITKPoint> getSeedPoints();
        /**
         * @brief    获取种子点信息列表
         * @return   QList<FITKMeshSeedInfo>    种子点列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-26
         */
        QList<FITKMeshSeedInfo> getSeedPointInfos();
        /**
         * @brief    设置种子点信息
         * @param[i] points                     种子点列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setSeedPointInfos(QList<FITKMeshSeedInfo> points);
        /**
         * @brief    设置种子点类型
         * @param[i] type                       类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setSeedType(FITKMeshSeed::FITKSeedType type);
        /**
         * @brief    获取布种约束类型
         * @return   FITKLocalSeedConstraint    布种约束类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        FITKMeshSeed::FITKLocalSeedConstraint getConstraintType();
        /**
         * @brief    设置布种约束类型
         * @param[i] type                       布种约束类型
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setConstraintType(FITKMeshSeed::FITKLocalSeedConstraint type);
        /**
         * @brief    获取布种方法
         * @return   FITKLocalSeedMethod        布种方法
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        FITKMeshSeed::FITKLocalSeedMethod getMethod();
        /**
         * @brief    设置布种方法
         * @param[i] method                     布种方法
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setMethod(FITKMeshSeed::FITKLocalSeedMethod method);
        /**
         * @brief    获取布种偏移
         * @return   FITKLocalSeedBias          偏移方式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        FITKMeshSeed::FITKLocalSeedBias getBias();
        /**
         * @brief    设置布种偏移
         * @param[i] bias                       偏移方式
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setBias(FITKMeshSeed::FITKLocalSeedBias bias);
        /**
         * @brief    设置单元数量
         * @param[i] num                        单元数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setEleNumber(int num);
        /**
         * @brief    获取单元数量
         * @return   int                        单元数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        int getEleNumber();
        /**
         * @brief    设置偏心率
         * @param[i] biasRatio                  偏心率
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-23
         */
        void setBiasRatio(double biasRatio);
        /**
         * @brief    获取偏心率
         * @return   double                     偏心率
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-23
         */
        double getBiasRatio();
        /**
         * @brief    设置单元尺寸
         * @param[i] size                       尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setEleSize(double size);
        /**
         * @brief    获取单元尺寸
         * @return   double                     尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        double getEleSize();
        /**
         * @brief    设置单元最小尺寸
         * @param[i] size                       尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setEleMinSize(double size);
        /**
         * @brief    获取单元最小尺寸
         * @return   double                     尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        double getEleMinSize();
        /**
         * @brief    设置单元最大尺寸
         * @param[i] size                       尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        void setEleMaxSize(double size);
        /**
         * @brief    获取单元最大尺寸
         * @return   double                     尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-05-22
         */
        double getEleMaxSize();
        /**
         * @brief    根据当前种子点计算最小、最大单元尺寸
         * @param[i] minSize                    最小尺寸
         * @param[i] maxSize                    最大尺寸
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-06-20
         */
        void calculateEleSize(double& minSize, double&maxSize);
    protected:
        /**
         * @brief  布种类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        FITKSeedType _seedType = FITKSeedType::TNone;
        /**
         * @brief  虚拓扑ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        int _virTopoID = -1;
        /**
         * @brief  种子点列表
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        QList<FITKMeshSeedInfo> _seedPoints = {};
        /**
         * @brief  布种约束类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        FITKLocalSeedConstraint _conType = FITKLocalSeedConstraint::CNotChange;
        /**
         * @brief  布种方法
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        FITKLocalSeedMethod _method = FITKLocalSeedMethod::MByNum;
        /**
         * @brief  布种偏移方式
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        FITKLocalSeedBias _bias = FITKLocalSeedBias::BNone;
        /**
         * @brief  单元数量
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        int _eleNum = 4;
        /**
         * @brief  偏心率
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        double _biasRatio = 5.0;
        /**
         * @brief  单元尺寸
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        double _eleSize = 3.0;
        /**
         * @brief  单元最小尺寸
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        double _eleMinSize = 0.0;
        /**
         * @brief  单元最大尺寸
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-05-22
         */
        double _eleMaxSize = 0.0;
    };
}

#endif