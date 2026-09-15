/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorPartElements.h
 * @brief  Part单元读取适配器
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-11
 *********************************************************************/
#ifndef FITK_RADIOSS_ADAPTOR_PART_ELEMENTS_H
#define FITK_RADIOSS_ADAPTOR_PART_ELEMENTS_H

#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"
#include <QList>
#include <QHash>

namespace Radioss
{
    class FITKRadiossRadReader;
    class FITKRadiossRadWriter;
    class FITKRadiossPart;

    /**
     * @brief  Part单元读取适配器
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-11
     */
    class FITKRadiossAdaptorPartElements
    {
    public:
        FITKRadiossAdaptorPartElements(FITKRadiossRadReader* reader, FITKRadiossRadWriter *writer, FITKRadiossPart* part);
        virtual ~FITKRadiossAdaptorPartElements() = default;
        /**
         * @brief    读取单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-11
         */
        bool readElements();
        /**
         * @brief    写出单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-13
         */
        bool writeElements();
    private:
        /**
         * @brief    写出弹簧单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-14
         */
        bool writeSpringElements();
        /**
         * @brief    写出三角形单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-13
         */
        bool writeSH3NElements();
        /**
         * @brief    写出四边形单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-13
         */
        bool writeShellElements();
        /**
         * @brief    写出六面体单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-08-14
         */
        bool writeBrickElements();
        /**
        * @brief    写出10节点4面体网格单元
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-13
        */
        bool writeTETRA10Elements();
        /**
         * @brief    写出4面体网格单元
         * @return   bool
         * @author   wangning (2185896382@qq.com)
         * @date     2025-09-13
         */
        bool writeTETRA4Elements();
        /**
        * @brief    写出梁单元
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-13
        */
        bool writeBEAMElements();
        /**
        * @brief    写出粒子单元
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-13
        */
        bool writeSPHCELElements();

    private:
        /**
         * @brief    读取弹簧单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-11
         */
        bool readSpringElements();
        /**
         * @brief    读取四边形单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-11
         */
        bool readShellElements();
        /**
         * @brief    读取三角形单元
         * @return   bool
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-11
         */
        bool readSH3NElements();
        /**
         * @brief    读取六面体网格单元
         * @return   bool 
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-11
         */
        bool readBrickElements();
        /**
        * @brief    读取10节点4面体网格单元
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-13
        */
        bool readTETRA10Elements();
        /**
        * @brief    读取4面体网格单元
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-22
        */
        bool readTETRA4Elements();
        /**
        * @brief    读取4面体网格单元
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-22
        */
        bool readBEAMElements();
        /**
        * @brief    读取粒子单元
        * @return   bool
        * @author   wangning (2185896382@qq.com)
        * @date     2025-09-22
        */
        bool readSPHCELElements();

    private:
        /**
         * @brief  Radioss文件读取器
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-11
         */
        FITKRadiossRadReader* _reader{};
        /**
         * @brief  文件写出器
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-08-13
         */
        FITKRadiossRadWriter* _writer{};
        /**
         * @brief  part数据对象
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-11
         */
        FITKRadiossPart* _part{};
        /**
         * @brief  存储单元类型的数量
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-08-13
         */
        QHash<Interface::FITKModelEnum::FITKEleType, QList<int>> _eleTypeIds{};
    };
}

#endif // FITK_RADIOSS_ADAPTOR_PART_ELEMENTS_H