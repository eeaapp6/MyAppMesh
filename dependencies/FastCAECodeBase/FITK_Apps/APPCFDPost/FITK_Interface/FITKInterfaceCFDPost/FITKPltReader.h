/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FCGNSREADER_H__
#define _FCGNSREADER_H__

#include <vtkMultiBlockDataSetAlgorithm.h>
#include <vector>
#include <QString>
#include <QDebug>
#include <QStringList>

class vtkPointData;
class vtkPoints;
class vtkCellData;
class vtkUnstructuredGrid;

namespace Interface
{
    enum TecZoneType
    {
        ORDERED,
        FE_LINE_SEG,
        FE_TRIANGLE,
        FE_QUAD,
        FE_TET,
        FE_BRICK,
        FE_POLYGON,         //szplt not support polygon
        FE_POLYHDFRON,      //szplt not support polyhedron
    };


    enum ErrorCode
    {
        NoError = 1,
        PointError = 2,
        CellError = 3,
    };

    struct Vector2
    {
        double min = 0;
        double max = 0;
        Vector2()
        {
            min = 0;
            max = 0;
        }

        Vector2(double min_c, double max_c)
        {
            min = min_c;
            max = max_c;
        }
    };

    struct Vector3
    {
        float x = 0;
        float y = 0;
        float z = 0;
        Vector3()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        Vector3(float x_c, float y_c, float z_c)
        {
            x = x_c;
            y = y_c;
            z = z_c;
        }
    };

    struct HanderInfo
    {
        QByteArray Version{};
        QString DataTitle{};
        //维度
        int dim = 0;
        QStringList VariableNames{};

        void print()
        {
            qDebug() << "Version : " << Version;
            qDebug() << "Title : " << DataTitle;
            qDebug() << "Dim : " << dim;
            qDebug() << "VariableNames : " << VariableNames;
        }
    };

    struct ZoneInfo
    {
        //分区名称
        QString ZoneName{};
        //场变量类型
        QList<int> VariableTypes{};
        //点的数量
        int numberPoints = 0;
        //单元的数量
        int numberCells = 0;
        //分区类型
        int zoneType = 0;
        //分区颜色
        int zoneColor = 0;
    };

    struct DataInfo
    {
        //int dim = 0;//标记二三维

        //X方向最大最小值
        Vector2 nodeXMinMax{};
        //Y方向最大最小值
        Vector2 nodeYMinMax{};
        //Z方向最大最小值
        Vector2 nodeZMinMax{};
        //所有变量的最大最小值
        QList<Vector2> VariablesMinMax{};
        //点数据
        QList<Vector3> Points{};
        QList<QList<float>> variablesData{}; //所有属性数据 暂用float 因为输出端都是float
        QList<int> Cells{}; //所有单元拓扑

    };

    class  FITKPltReader : public vtkMultiBlockDataSetAlgorithm
    {
    public:
        static FITKPltReader* New();
        vtkTypeMacro(FITKPltReader, vtkMultiBlockDataSetAlgorithm);
        void PrintSelf(ostream& os, vtkIndent indent) override;

        vtkSetStringMacro(FileName);
        vtkGetStringMacro(FileName);



        int ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
        int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*);


    private:
        FITKPltReader();
        ~FITKPltReader();

        /**
         * @brief    向非结构网格中添加单元数据
         * @param[i] zoneIndex
         * @param[i] numberCells 分区
         * @param[i] cellTypeStr
         * @param[i] unstrctGrid
         * @param[i] multZone
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-10
         */
        void GetUnstructuredGridCells(int zoneIndex, int numberCells, int cellTypeStr, vtkUnstructuredGrid* unstrctGrid, vtkMultiBlockDataSet* multZone);
        void GetArraysFromBlockPackingZone(int zoneIndex, int numNodes, int numCells, vtkPoints * theNodes, vtkPointData * nodeData, vtkCellData * cellData, vtkMultiBlockDataSet* multZone);
        QByteArray ReadBytes(QByteArray &data, size_t typeSize, size_t readLengh);
        /**
         * @brief    读取指定字节的数据
         * @param[i] data            数据源
         * @param[i] readLengh       长度
         * @return   QByteArray      数据结果
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-10
         */
        QByteArray ReadBytes(QByteArray &data, size_t readLengh);
        /**
         * @brief    读取指定字节的数据设置拷贝到指定数据
         * @param[i] data            数据源
         * @param[o] backData        拷贝数据结果
         * @param[i] readLengh       长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-10
         */
        template<typename T2>
        void ReadBytes(QByteArray &data, T2 &backData, size_t readLengh);
        /**
         * @brief IsReadEnd:是否读取结尾
         * @param data:读取的流
         * @return:isEnd
         */
        bool IsReadEnd(QByteArray & data);
        /**
         * @brief    读取
         * @param[i] data     源数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-10
         */
        void Reader(QByteArray &data);
        /**
         * @brief    读取分区数据
         * @param[i] data    源数据
         * @return   float   读取位置
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-10
         */
        float ReadZones(QByteArray &data);
        /**
         * @brief    读取数据
         * @param[i] data    源数据
         * @return   float   读取位置
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-10
         */
        float ReadDataSection(QByteArray &data);
        /**
         * @brief    通过单元类型获取点数
         * @param[i] cellType
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-10
         */
        int GetNumberOfPointSize(int cellType);
    private:
        /**
         * @brief  头部信息
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-10
         */
        HanderInfo handerInfo{};
        /**
         * @brief  分区信息
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-10
         */
        QList<ZoneInfo> zoneInfos{};
        /**
         * @brief  数据信息
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-10
         */
        QList<DataInfo> dataInfos{};
        /**
         * @brief  场数量
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-10
         */
        int NumberOfVariables = 0;

        /**
         * @brief readZoneIndex:读取zone块索引
         */
        int readZoneIndex = 0;
        /**
         * @brief readDataIndex:读取数据快索引
         */
        int readDataIndex = 0;

        /**
         * @brief readErrorCode:读取数据错误标记 默认1 读取成功
         */
        enum ErrorCode _readErrorCode;
        /**
         * @brief  当前所读取到的位置
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-10
         */
        long long DirectionHead = 0;
        std::string DataTitle;
        std::vector<int> CellBased;

        std::vector<std::string> Variables;

        char* FileName = nullptr;
        //vtkMultiBlockDataSet* mudataset{};
        int TopologyDim;
        int GeometryDim;
        char* dataSetTitle = nullptr;
        int32_t numVars;
        QStringList varNames;
        int32_t fileType;
        int32_t numZones;
        void* inputFileHandle = nullptr;//
        std::vector <int32_t> shareConnectivity;
        std::vector<std::vector<std::vector<double>>> varAllZones;//store all the variables of all zones
    };
}
#endif

