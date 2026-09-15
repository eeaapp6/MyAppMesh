/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFFVSchemesWriter.h
 * @brief  fvSchemes 写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-27
 *********************************************************************/
#ifndef __FITKFLOWOFFVSCHEMESWRITER_H__
#define __FITKFLOWOFFVSCHEMESWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface 
{
    class FITKOFDiscretization;
}

namespace DICT
{
    class FITKDictVector;
    class FITKDictArray;
    class FITKDictGroup;
}

namespace Core
{
    class FITKParameter;
}

namespace IO
{
    /**
     * @brief ControlMeshDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFFVSchemesWriter : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFFVSchemesWriter();
        virtual ~FITKOFFVSchemesWriter();

        /**
         * @brief 设置文件路径
         * @param[i] filePath
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void setFilePath(const QString& filePath) override;
        /**
         * @brief 重写run函数
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        bool run() override;
        /**
         * @brief 获取文件
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString getFileName() override;

    private:
        /**
         * @brief 设置字典里的FoamFile
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void dictObjectFoamFile(QString location);

        bool writeFluidRegionSchemes();

        bool writeSolidRegionSchemes();

        bool writeEmptySchemes();

        bool fvSchemesW();
        /**
         * @brief    写出时间一阶导项(非定常项)格式
         * @param[i] timeOption 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        bool writeDDTSchemes(Core::FITKParameter* timeOption);
        /**
         * @brief    写出梯度项格式
         * @param[i] gradient 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        bool writeGradSchemes(Core::FITKParameter* gradient);

        bool writeDivSchemesSimple(Interface::FITKOFDiscretization* discretization);
        bool writeDivSchemesInter(Interface::FITKOFDiscretization* discretization);
        bool writeDivSchemesSHTMultiRegion(Interface::FITKOFDiscretization* discretization);

        QString getConvectionValue(Core::FITKParameter* vPara);
        /**
         * @brief    写出拉普拉斯项格式
         * @param[i] discretization 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        bool writeLaplacianSchemes(Interface::FITKOFDiscretization* discretization);
        /**
         * @brief    写出插值格式
         * @param[i] interpolation 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        bool writeInterpolationSchemes(Core::FITKParameter* interpolation);
        /**
         * @brief    写出面法向梯度格式
         * @param[i] gradient 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        bool writesnGradSchemes(Core::FITKParameter* gradient);
        /**
         * @brief    写出需要用来计算通量的场
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        bool writeFluxWall();

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};
    };
}



#endif
