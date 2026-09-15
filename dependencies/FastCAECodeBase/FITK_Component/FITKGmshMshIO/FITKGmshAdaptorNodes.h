/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshAdaptorNodes.h
 * @brief  Gmsh msh 节点读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef __FITKGMSHADAPTORNODES_R_H__
#define __FITKGMSHADAPTORNODES_R_H__

#include "FITKGmshAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Gmsh
{
    /**
     * @brief  Gmsh msh 节点读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKGmshAdaptorNodes : public FITKGmshAbstractAdaptor
    {
    public:
        explicit FITKGmshAdaptorNodes() = default;
        virtual ~FITKGmshAdaptorNodes() = default;

        /**
         * @brief    获取适配器数据类型名
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        QString getAdaptorClass() override;

        /**
         * @brief    适配器读取
         * @return   状态 true成功， false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool adaptR() override;

        /**
         * @brief    适配器写出
         * @return   状态 true成功， false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取所有节点的信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readNodesInfo();
        /**
         * @brief    读取节点
         * @param[i] info
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readNode(QStringList info);

    private:

    };

    Register2FITKIOAdaptorFactory(Gmsh, FITKGmshAdaptorNodes, FITKGmshAdaptorNodes)
}


#endif
