/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAlgDatReader.h
 * @brief  Tecplot中dat文件格式读取
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-09
 *********************************************************************/
#ifndef _FITKAlgDatReader_H_
#define _FITKAlgDatReader_H_

#include "FITKInterfaceCFDPostAPI.h"
#include <vtkSmartPointer.h>
#include <vtkMultiBlockDataSetAlgorithm.h>
#include <QHash>
#include <QStringList>
#include <vtkDataSetAlgorithm.h>
#include <vtkUnstructuredGridAlgorithm.h>

class vtkMultiBlockDataSet;
class vtkAlgorithmOutput;
class vtkInformation;
class vtkDataSet;

namespace Interface
{
    /**
     * @brief 补充属性信息结构体
     * @since Version: 1.0.0
     **/
    typedef struct tpArrayInfo
    {
        int components;
        int tuples;
    } tArrayInfo;

    class FITKInterfaceCFDPostAPI FITKAlgDatReader : public vtkUnstructuredGridAlgorithm
    {
    public:
        /**
         * @brief 创建Plot3DReader对象
         * @since Version: 1.0.0
         **/
        static FITKAlgDatReader *New();
        /**
         * @brief 信息打印函数
         * @since Version: 1.0.0
         **/
        void PrintSelf(ostream &os, vtkIndent indent);

        void setFileName(QString fileName);
        QString getFileName();

        int getNumberOfBlocks();

        void setVisible(int blockIndex, bool vis);

        QStringList getBlockNames();

        QStringList getBlockBCTypes();

        /**
         * @brief 设置输出信息
         * @since Version: 1.0.0
         **/
        int FillOutputPortInformation(int nPort, vtkInformation *info);

    protected:
        FITKAlgDatReader();
        ~FITKAlgDatReader();
        /**
         * @brief 请求数据函数
         * @note  需重写
         * @since Version: 1.0.0
         **/
        int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

        /**
         * @brief 处理请求函数
         * @note  需重写
         * @since Version: 1.0.0
         **/
        int ProcessRequest(vtkInformation *request, vtkInformationVector **inputVector,
            vtkInformationVector *outputVector);

    private:
        /**
         * @note  防止拷贝，使类创建统一走New()函数
         * @since Version: 1.0.0
         **/
        FITKAlgDatReader(const FITKAlgDatReader &);
        void operator=(const FITKAlgDatReader &);

        /**
         * @brief 递归遍历数据块
         * @since Version: 1.0.0
         **/
        void getBlocks(vtkDataObject *blockData);
    protected:
        /**
         * @brief  文件名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-09
         */
        QString _fileName = "";
    private:
        /**
         * @brief 拆分成的数据链表
         * @note  将multiblock拆分成dataset数据,存入链表
         * @since Version: 1.0.0
         **/
        QList<vtkDataSet *> _blockList;

        /**
         * @brief 属性全集哈希
         * @note  QString-属性名称, int-属性维度
         * @since Version: 1.0.0
         **/
        QHash<QString, tArrayInfo> _attributesPointList;
        QHash<QString, tArrayInfo> _attributesCellList;

        QList<bool> _visibleStates{};
        QStringList _blockNames{};
    };
}
#endif
