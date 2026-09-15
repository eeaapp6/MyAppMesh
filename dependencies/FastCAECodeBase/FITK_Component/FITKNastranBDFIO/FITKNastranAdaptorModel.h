/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKNastranAdaptorModel.h
 * @brief  Nastran bdf 文件 模型读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-03-13
 *********************************************************************/
#ifndef _FITKNASTRANADAPTORMODEL_H
#define _FITKNASTRANADAPTORMODEL_H

#include "FITKNastranAbstractAdaptor.h"

namespace Interface
{
    class FITKAbstractElement;
}

namespace Nastran
{
    /**
     * @brief  节点读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2026-02-10
     */
    class FITKNastranAdaptorModelNode : public FITKNastranAbstractAdaptor
    {
    public:
        explicit FITKNastranAdaptorModelNode() = default;
        virtual ~FITKNastranAdaptorModelNode() = default;

        /**
         * @brief  适配器读取
         * @return bool 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-12
         */
        bool adaptR() override;

        /**
         * @brief  适配器写出
         * @return bool 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-12
         */
        bool adaptW() override;

    private:

    };
    /**
     * @brief  单元读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2026-02-10
     */
    class FITKNastranAdaptorModelElement : public FITKNastranAbstractAdaptor
    {
    public:
        explicit FITKNastranAdaptorModelElement() = default;
        virtual ~FITKNastranAdaptorModelElement() = default;

        /**
         * @brief  适配器读取
         * @return bool 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-12
         */
        bool adaptR() override;

        /**
         * @brief  适配器写出
         * @return bool 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-03-12
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取梁单元
         * @param[i] data 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool readCBAR(QStringList data);
        /**
         * @brief    读取三角形单元
         * @param[i] data 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool readCTRIA3(QStringList data);
        /**
         * @brief    读取四边形单元
         * @param[i] data 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool readCQUAD4(QStringList data);
        /**
         * @brief    读取六面体单元
         * @param[i] data 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool readCHEXA(QStringList data);
        /**
         * @brief    读取四面体单元
         * @param[i] data 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool readCTETRA(QStringList data);
        /**
         * @brief    读取高阶梁单元
         * @param[i] data 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool readCBEAM3(QStringList data);
        /**
         * @brief    写出梁单元
         * @param[i] element 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool writeCBAR(Interface::FITKAbstractElement* element);
        /**
         * @brief    写出三角形单元
         * @param[i] element 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool writeCTRIA3(Interface::FITKAbstractElement* element);
        /**
         * @brief    写出四边形单元
         * @param[i] element 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool writeCQUAD4(Interface::FITKAbstractElement* element);
        /**
         * @brief    写出六面体单元
         * @param[i] element 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool writeCHEXA(Interface::FITKAbstractElement* element);
        /**
         * @brief    写出四面体单元
         * @param[i] element 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool writeCTETRA(Interface::FITKAbstractElement* element);
        /**
         * @brief    写出高阶梁单元
         * @param[i] element 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-02-10
         */
        bool writeCBEAM3(Interface::FITKAbstractElement* element);
    };

    Register2FITKIOAdaptorFactory(BDF, NastranModelNode, FITKNastranAdaptorModelNode);
    Register2FITKIOAdaptorFactory(BDF, NastranModelElement, FITKNastranAdaptorModelElement);
} // namespace Nastran

#endif // FITKNASTRANADAPTORPART_H
