/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCModelImportFlattenAssembly.h
 * @brief       拆分平铺装配模式读取几何文件。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-27
 *********************************************************************/

#ifndef __FITKOCCMODELIMPORTFLATTENASSEMBLY_H__
#define __FITKOCCMODELIMPORTFLATTENASSEMBLY_H__

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImportFlattenAssembly.h"
#include "FITKAbstractOCCModel.h"
#include "FITKGeoCompOCCAPI.h"

// 前置声明
class TDF_Label;
class TopLoc_Location;
class XCAFDoc_ShapeTool;

namespace OCC
{
    /**
     * @brief       拆分平铺装配模式读取几何文件。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-27
     */
    class FITKGeoCompOCCAPI FITKOCCModelImportFlattenAssembly : public Interface::FITKAbsGeoModelImportFlattenAssembly, public OCCShapeAgent
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        explicit FITKOCCModelImportFlattenAssembly();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        virtual ~FITKOCCModelImportFlattenAssembly() = default;

        /**
         * @brief       更新命令。[重写]
         * @return      是否更新成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool update() override;

        /**
         * @brief       获取由本命令创建出的所有新命令ID列表。[重写]
         * @return      ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        QList<int> getCreatedCommandIds() override;

    private:
        /**
         * @brief       OCAF读取工具方法，获取文档内几何形状根节点标签。
         * @param[in]   root：文档根节点[引用]
         * @param[out]  label：几何形状根节点[引用]
         * @return      是否存在形状节点
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool getShapeLabel(TDF_Label & root, TDF_Label & label);

        /**
         * @brief       OCAF读取工具方法，拆分形状，处理引用与变换。
         * @param[in]   shapeTool：形状工具
         * @param[in]   parent：父节点[引用]
         * @param[in]   loc：父节点变换
         * @return      是否拆分成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        bool splitShapeLabel(XCAFDoc_ShapeTool* shapeTool, const TDF_Label & parent, TopLoc_Location loc);

    private:
        /**
         * @brief       新创建的命令ID列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-27
         */
        QList<int> m_newCmdIds;

    };
}


#endif //!__FITKOCCMODELIMPORTFLATTENASSEMBLY_H__
