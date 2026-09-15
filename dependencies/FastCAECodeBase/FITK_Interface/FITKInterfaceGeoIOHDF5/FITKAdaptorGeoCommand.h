/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoCommand.h
 * @brief  几何适配器读写
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOCOMMAND_H
#define FITKADAPTORGEOCOMMAND_H

#include "FITKAbstractGeoHDF5Adaptor.h"
#include "FITKInterfaceGeoIOHDF5API.h"
#include "FITKGeoHDF5KeyWords.h"

namespace H5
{
    class Group;
}

namespace Interface
{
    /**
     * @brief  几何适配器读写
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoCommand : public FITKAbstractGeoHDF5Adaptor
    {
    public:
        explicit FITKAdaptorGeoCommand() = default;
        virtual ~FITKAdaptorGeoCommand() = 0;
        /**
         * @brief    适配器读取
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool adaptR() override;
        /**
         * @brief    适配器写出
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool adaptW() override;

    protected:
        /**
         * @brief    写出NDataObject数据
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool writeNamedDataObject(H5::Group& g);
        /**
         * @brief    读取NDataObject数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool readNamedDataObject();
        /**
         * @brief    写出命令数据
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool writeCommandDataObject(H5::Group& g);
        /**
         * @brief    读取命令数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool readCommandDataObject();
        /**
         * @brief    写出虚拓扑形状数据
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool writeShape(H5::Group & g) = 0;
        /**
         * @brief    读取虚拓扑形状数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool readShape() = 0;
        /**
         * @brief    写出虚拓扑管理器数据
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool writeVirtualShape(H5::Group& g);
        /**
         * @brief    读取虚拓扑管理器数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool readVirtualShape();
        /**
         * @brief    写出参数数据
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool writeParameters(H5::Group & g) = 0;
        /**
         * @brief    读取参数数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool readParameters() = 0;


    };

}

#endif // FITKADAPTORGEOCOMMAND_H

