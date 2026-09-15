/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKNastranAdaptorCase.h
 * @brief  Nastran bdf 文件 读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-03-13
 *********************************************************************/
#ifndef _FITKABAQUSADAPTORCASE_H
#define _FITKABAQUSADAPTORCASE_H

#include "FITKNastranAbstractAdaptor.h"

namespace AbaqusData
{
    class FITKDataCase;
    class FITKAbaqusPart;
}

namespace Nastran
{
    /**
     * @brief  Nastran bdf 文件 读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-03-13
     */
    class FITKNastranAdaptorCase : public FITKNastranAbstractAdaptor
    {
    public:
        /**
        * @brief Create
        * @author shenzhuowei (2931275006@qq.com)
        * @date 2025-03-05
        */
        explicit FITKNastranAdaptorCase();
        /**
        * @brief Destory
        * @author shenzhuowei (2931275006@qq.com)
        * @date 2025-03-05
        */
        virtual ~FITKNastranAdaptorCase();

        /**
         * @brief    设置算例数据
         * @param[i] caseData
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-13
         */
        void setCaseData(AbaqusData::FITKDataCase *caseData);

        /**
         * @brief    设置Part数据
         * @param[i] partData
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-03-13
         */
        void setPartData(AbaqusData::FITKAbaqusPart *partData);

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author zhourp (zhourunping@diso.cn)
         * @date 2024-03-15
         */
        bool adaptR() override;

        /**
         * @brief 适配器写入
         * @return 状态 true成功， false失败
         * @author shenzhuowei (2931275006@qq.com)
         * @date 2025-03-04
         */
        bool adaptW() override;

    private:
        bool readModelNode();
        bool writeModelNode();
        bool readModelElement();
        bool writeModelElement();
        bool readMaterial();
        bool writeMaterial();
        bool readProperty();
        bool writeProperty();

    private:
        /**
         * @brief  算例数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-13
         */
        AbaqusData::FITKDataCase *_caseData{};

        /**
         * @brief  网格数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-13
         */
        AbaqusData::FITKAbaqusPart *_partData{};

        /**
         * @brief  读取适配器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-13
         */
        QHash<QString, FITKNastranAbstractAdaptor*> _adaptorIO{};
    };

    Register2FITKIOAdaptorFactory(BDF, NastranCase, FITKNastranAdaptorCase);
} // namespace Nastran
#endif // _FITKABAQUSADAPTORCASE_H
