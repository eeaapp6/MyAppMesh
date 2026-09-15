/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPltReader.h
 * @brief  Tecplot 二进制 plt 文件读取器声明
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-07-03
 *********************************************************************/
#ifndef _FCGNSREADER_H__
#define _FCGNSREADER_H__

#include <vtkMultiBlockDataSetAlgorithm.h>
#include <vector>
#include <array>
#include <QString>
#include <QDebug>
#include <QStringList>

class vtkPointData;
class vtkPoints;
class vtkCellData;
class vtkUnstructuredGrid;

namespace Interface
{
    /**
     * @brief    Tecplot 分区类型枚举。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
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

    /**
     * @brief    plt 读取错误码枚举。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    enum ErrorCode
    {
        NoError = 1,
        PointError = 2,
        CellError = 3,
    };

    /**
     * @brief    二维最值区间。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    struct Vector2
    {
        /** @brief 最小值 */
        double min = 0;
        /** @brief 最大值 */
        double max = 0;

        /**
         * @brief    默认构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        Vector2()
        {
            min = 0;
            max = 0;
        }

        /**
         * @brief    使用最小值和最大值构造区间
         * @param[i] min_c 最小值
         * @param[i] max_c 最大值
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        Vector2(double min_c, double max_c)
        {
            min = min_c;
            max = max_c;
        }
    };

    /**
     * @brief    三维点坐标。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    struct Vector3
    {
        /** @brief X 坐标 */
        double x = 0;
        /** @brief Y 坐标 */
        double y = 0;
        /** @brief Z 坐标 */
        double z = 0;

        /**
         * @brief    默认构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        Vector3()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        /**
         * @brief    使用三维坐标构造点
         * @param[i] x_c X 坐标
         * @param[i] y_c Y 坐标
         * @param[i] z_c Z 坐标
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        Vector3(float x_c, float y_c, float z_c)
        {
            x = x_c;
            y = y_c;
            z = z_c;
        }
    };

    /**
     * @brief    plt 文件头信息。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    struct HanderInfo
    {
        /** @brief Tecplot 文件版本号原始字节 */
        QByteArray Version{};
        /** @brief 数据集标题 */
        QString DataTitle{};
        /** @brief 数据集几何维度 */
        int dim = 0;
        /** @brief 变量名称列表 */
        QStringList VariableNames{};

        /**
         * @brief    输出头信息到调试日志
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        void print()
        {
            qDebug() << "Version : " << Version;
            qDebug() << "Title : " << DataTitle;
            qDebug() << "Dim : " << dim;
            qDebug() << "VariableNames : " << VariableNames;
        }
    };

    /**
     * @brief    单个 Tecplot 分区的头信息。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    struct ZoneInfo
    {
        /** @brief 分区名称 */
        QString ZoneName{};
        /** @brief 分区头标记，299 表示 v112，298 表示 v191 */
        int zoneMarker = 299;
        /** @brief 变量位置列表，0 为节点变量，1 为单元变量 */
        QList<int> VariableTypes{};
        /** @brief 分区点数量 */
        int numberPoints = 0;
        /** @brief 分区单元数量 */
        int numberCells = 0;
        /** @brief 分区类型 */
        int zoneType = 0;
        /** @brief 分区默认颜色 */
        int zoneColor = 0;
        /** @brief ORDERED 分区 I 方向点数 */
        int orderedIMax = 1;
        /** @brief ORDERED 分区 J 方向点数 */
        int orderedJMax = 1;
        /** @brief ORDERED 分区 K 方向点数 */
        int orderedKMax = 1;
        /** @brief 是否包含 raw local face neighbor */
        int hasRawLocalFaceNeighbors = 0;
        /** @brief 用户自定义 face neighbor 连接数量 */
        int numMiscFaceConnections = 0;
        /** @brief 用户自定义 face neighbor 模式 */
        int faceNeighborMode = -1;
        /** @brief 有限元面邻接是否完整指定 */
        int faceNeighborsCompletelySpecified = 0;
        /** @brief polygon/polyhedron 的面数量 */
        qint64 numFaces = 0;
        /** @brief polygon/polyhedron 的总面节点数量 */
        qint64 totalFaceNodes = 0;
        /** @brief 边界面数量 */
        int totalBoundaryFaces = 0;
        /** @brief 边界连接数量 */
        int totalBoundaryConnections = 0;
    };

    /**
     * @brief    单个分区的数据缓存。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    struct DataInfo
    {
        /** @brief X 方向最值 */
        Vector2 nodeXMinMax{};
        /** @brief Y 方向最值 */
        Vector2 nodeYMinMax{};
        /** @brief Z 方向最值 */
        Vector2 nodeZMinMax{};
        /** @brief 各变量最小最大值 */
        QList<Vector2> VariablesMinMax{};
        /** @brief 点坐标缓存 */
        QList<Vector3> Points{};
        /** @brief 各变量原始数值缓存 */
        QList<QList<double>> variablesData{};
        /** @brief 单元拓扑节点索引 */
        QList<int> Cells{};

    };

    /**
     * @brief    Tecplot 二进制 plt 文件读取器。
     * @details  负责解析 Tecplot v112/v191 二进制头段和数据段，
     *           并转换为 VTK 可用的多块数据对象。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    class FITKPltReader : public vtkMultiBlockDataSetAlgorithm
    {
    public:
        /**
         * @brief    创建 vtk 对象实例
         * @return   FITKPltReader* 读取器实例
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        static FITKPltReader* New();
        vtkTypeMacro(FITKPltReader, vtkMultiBlockDataSetAlgorithm);

        /**
         * @brief    输出对象调试信息
         * @param[i] os 输出流
         * @param[i] indent 缩进信息
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        void PrintSelf(ostream& os, vtkIndent indent) override;

        vtkSetStringMacro(FileName);
        vtkGetStringMacro(FileName);
        /**
         * @brief    处理 VTK 管线请求
         * @param[i] request 请求对象
         * @param[i] inputVector 输入信息
         * @param[i] outputVector 输出信息
         * @return   int 是否处理成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        int ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);

        /**
         * @brief    执行 plt 文件读取并生成输出数据
         * @param[i] vtkInformation VTK 请求信息
         * @param[i] vtkInformationVector 输入信息
         * @param[i] vtkInformationVector 输出信息
         * @return   int 是否读取成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*);


    private:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        FITKPltReader();

        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
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

        /**
         * @brief    按变量位置把分区变量写入 VTK 点数据或单元数据
         * @param[i] zoneIndex 分区索引
         * @param[i] numNodes 点数量
         * @param[i] numCells 单元数量
         * @param[i] theNodes 点对象
         * @param[o] nodeData 点属性数据
         * @param[o] cellData 单元属性数据
         * @param[i] multZone 多块数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        void GetArraysFromBlockPackingZone(int zoneIndex, int numNodes, int numCells, vtkPoints * theNodes, vtkPointData * nodeData, vtkCellData * cellData, vtkMultiBlockDataSet* multZone);

        /**
         * @brief    读取以 INT32 字符编码的字符串或指定长度字节流
         * @param[i] data 源数据
         * @param[i] typeSize 单个元素字节数
         * @param[i] readLengh 指定读取长度，0 表示读取到字符串结束符
         * @return   QByteArray 读取结果
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
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
         * @brief    读取 Tecplot INT32 字符串
         * @param[i] data    源数据
         * @return   QString 读取结果
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        QString ReadInt32String(QByteArray &data);
        /**
         * @brief    判断当前读取指针是否已超过数据末尾
         * @param[i] data 读取的二进制流
         * @return   bool 是否到达末尾
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool IsReadEnd(QByteArray & data);
        /**
         * @brief    解析 Tecplot 二进制文件的头段和数据段
         * @param[i] data     源数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        void Reader(QByteArray &data);
        /**
         * @brief    读取单个分区头定义
         * @param[i] data    源数据
         * @param[i] zoneMarker 分区标记
         * @return   bool    是否读取成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool ReadZoneHeader(QByteArray &data, float zoneMarker);
        /**
         * @brief    读取单个分区的数据段描述和变量值
         * @param[i] data    源数据
         * @param[i] zoneIndex 分区索引
         * @param[i] zoneMarker 分区标记
         * @return   bool    是否读取成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool ReadZoneData(QByteArray &data, int zoneIndex, float zoneMarker);
        /**
         * @brief    通过单元类型获取点数
         * @param[i] cellType
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-10
         */
        int GetNumberOfPointSize(int cellType);
        /**
         * @brief    获取几何维度
         * @param[i] zoneInfo 分区信息
         * @return   int      几何维度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        int GetGeometryDimension(const ZoneInfo &zoneInfo);
        /**
         * @brief    获取有序区点数量
         * @param[i] zoneInfo 分区信息
         * @return   int      点数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        int GetOrderedPointCount(const ZoneInfo &zoneInfo);
        /**
         * @brief    获取有序区单元数量
         * @param[i] zoneInfo 分区信息
         * @return   int      单元数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        int GetOrderedCellCount(const ZoneInfo &zoneInfo);
        /**
         * @brief    获取指定变量的数据数量
         * @param[i] zoneInfo      分区信息
         * @param[i] variableIndex 变量索引
         * @return   int           数据数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        int GetVariableValueCount(const ZoneInfo &zoneInfo, int variableIndex);
        /**
         * @brief    读取数值并转换为 double
         * @param[i] data          源数据
         * @param[i] variableType  Tecplot 数据类型
         * @return   double        数值结果
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        double ReadNumericValueAsDouble(QByteArray &data, int variableType);
        /**
         * @brief    读取 32/64 位整数
         * @param[i] data    源数据
         * @param[i] isInt64 是否 64 位
         * @return   qint64  整数结果
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        qint64 ReadIntegerValue(QByteArray &data, bool isInt64);
        /**
         * @brief    构造当前分区的点坐标和变量缓存
         * @param[i] zoneIndex      分区索引
         * @param[i] zoneVariables  原始变量数据
         * @param[o] dataInfo       输出数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        void BuildZoneData(int zoneIndex, const QList<QList<double>> &zoneVariables, DataInfo &dataInfo);
        /**
         * @brief    读取或跳过当前分区的拓扑与面邻接数据
         * @param[i] data      源数据
         * @param[i] zoneIndex 分区索引
         * @param[i] shareZone 共享连接分区
         * @param[o] dataInfo  输出数据
         * @return   bool      是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool ReadConnectivity(QByteArray &data, int zoneIndex, int shareZone, DataInfo &dataInfo);
        /**
         * @brief    跳过几何定义
         * @param[i] data    源数据
         * @return   bool    是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipGeometry(QByteArray &data);
        /**
         * @brief    跳过文本定义
         * @param[i] data    源数据
         * @return   bool    是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipText(QByteArray &data);
        /**
         * @brief    跳过自定义标签
         * @param[i] data    源数据
         * @return   bool    是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipCustomLabel(QByteArray &data);
        /**
         * @brief    跳过用户记录
         * @param[i] data    源数据
         * @return   bool    是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipUserRecord(QByteArray &data);
        /**
         * @brief    跳过数据集辅助数据
         * @param[i] data    源数据
         * @return   bool    是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipDataSetAuxData(QByteArray &data);
        /**
         * @brief    跳过变量辅助数据
         * @param[i] data    源数据
         * @return   bool    是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipVariableAuxData(QByteArray &data);
        /**
         * @brief    跳过分区辅助数据
         * @param[i] data    源数据
         * @return   bool    是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipZoneAuxData(QByteArray &data);
        /**
         * @brief    跳过面邻接连接数据
         * @param[i] data            源数据
         * @param[i] connectionCount 连接数量
         * @param[i] faceNeighborMode 连接模式
         * @return   bool            是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipFaceNeighborConnections(QByteArray &data, int connectionCount, int faceNeighborMode);
        /**
         * @brief    按类型跳过数值数组
         * @param[i] data          源数据
         * @param[i] variableType  Tecplot 数据类型
         * @param[i] valueCount    数据数量
         * @return   bool          是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-07-03
         */
        bool SkipNumericValues(QByteArray &data, int variableType, qint64 valueCount);
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
        /** @brief 文件中的变量数量 */
        int NumberOfVariables = 0;

        /** @brief 当前读取到的分区头索引 */
        int readZoneIndex = 0;
        /** @brief 当前读取到的数据段索引 */
        int readDataIndex = 0;

        /** @brief 当前读取错误码，默认 NoError 表示成功 */
        enum ErrorCode _readErrorCode;
        /**
         * @brief  当前所读取到的位置
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-10
         */
        long long DirectionHead = 0;
        /** @brief 兼容旧实现保留的数据集标题缓存 */
        std::string DataTitle;
        /** @brief 兼容旧实现保留的单元变量标记缓存 */
        std::vector<int> CellBased;

        /** @brief 兼容旧实现保留的变量名缓存 */
        std::vector<std::string> Variables;

        /** @brief VTK 文件名接口使用的文件路径 */
        char* FileName = nullptr;
        /** @brief 当前读取结果的拓扑维度 */
        int TopologyDim;
        /** @brief 当前读取结果的几何维度 */
        int GeometryDim;
        /** @brief 兼容旧实现保留的数据集标题指针 */
        char* dataSetTitle = nullptr;
        /** @brief 兼容旧实现保留的变量数量缓存 */
        int32_t numVars;
        /** @brief 兼容旧实现保留的变量名列表 */
        QStringList varNames;
        /** @brief Tecplot 文件类型 */
        int32_t fileType;
        /** @brief 兼容旧实现保留的分区数量缓存 */
        int32_t numZones;
        /** @brief 兼容旧实现保留的输入句柄 */
        void* inputFileHandle = nullptr;
        /** @brief 兼容旧实现保留的连接共享信息 */
        std::vector <int32_t> shareConnectivity;
        /** @brief 兼容旧实现保留的所有分区变量缓存 */
        std::vector<std::vector<std::vector<double>>> varAllZones;
    };
}
#endif

