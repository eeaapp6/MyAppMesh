/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOpenFOAMMeshReader.h
 * @brief  OpenFOAM网格文件读取类
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-07-17
 */
#ifndef FITKOPENFOAMMESHREADER_H
#define FITKOPENFOAMMESHREADER_H

#include "FITKOFMeshIOAPI.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractIO.h"

class QFile;

namespace Interface {
    class FITKUnstructuredFluidMeshVTK;
    class FITKUnstructuredMeshVTK;
    class FITKAbstractElement;
    class FITKBoundaryMeshVTKManager;
    class FITKElementHex8;
    class FITKElementPolygon;
}
namespace IO {
    /**
     * @brief  OpenFOAM文件头信息
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-07-17
     */
    typedef struct {
        QString format{};
        QString clazz{};
        QString location{};
        QString object{};
    }FoamFileHeader;
    /**
     * @brief  边界
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-07-22
     */
    typedef struct {
        QString name{};
        QString type{};
        QString group{};
        int nFaces{0};
        int startFace{0};

        bool isValid() {
            return !name.isEmpty() && !type.isEmpty() && nFaces != 0 && startFace != 0;
        }
    } Boundary;
    /**
     * @brief  OpenFOAM网格文件读取类
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-07-17
     */
    class FITKOFMeshIOAPI FITKOpenFOAMMeshReader : public Interface::FITKAbstractIO
    {
    public:
        /**
         * @brief  构造函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        FITKOpenFOAMMeshReader();
        /**
         * @brief  析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        ~FITKOpenFOAMMeshReader() override;
        /**
         * @brief  重写线程启动函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        void run() override;
        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 消息
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        void consoleMessage(int level, const QString &str) override;
        /**
         * @brief  设置数据
         * @param  data Interface::FITKUnstructureFluidMeshVTK类型对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-25
         */
        void setDataObject(Core::FITKAbstractDataObject* data) override;
        /**
         * @brief  读取网格文件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool read();

    protected:
        /**
         * @brief    读取网格文件夹
         * @param[i] polyMeshDir 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-12
         */
        bool readPolyMesh(QString polyMeshDir);

        /**
         * @brief  处理行数据
         * @param  line 行数据
         * @param  inComment 上一次读取完行数据是否在注释中
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool processLine(QString& line, bool inComment = false);
        /**
         * @brief  读取OpenFOAM文件头信息
         * @param  file 文件
         * @param[out]  header 头信息结构体
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readFoamFileHeader(QFile& file, FoamFileHeader& header);
        /**
         * @brief  读取points文件中的点数据
         * @param  file 文件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readPointsData(QFile& file);
        /**
         * @brief  读取points文件
         * @param  points points文件路径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readPoints(const QString &points);
        /**
         * @brief  读取faces文件中的面数据
         * @param  file 文件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readFacesData(QFile& file);
        /**
         * @brief  读取faces文件
         * @param  points faces文件路径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readFaces(const QString &faces);
        /**
         * @brief  读取owner文件中的所属关系数据
         * @param  file 文件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readOwnerData(QFile& file);
        /**
         * @brief  读取owner文件
         * @param  points owner文件路径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readOwner(const QString &owner);
        /**
         * @brief  读取neighbour文件中的相邻关系数据
         * @param  file 文件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readNeighbourData(QFile& file);
        /**
         * @brief  读取neighbour文件
         * @param  points neighbour文件路径
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool readNeighbour(const QString &owner);
        /**
         * @brief  读取boundary文件中的单个边界信息
         * @param  file 文件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-22
         */
        bool readSingleBoundaryData(QFile& file, Boundary*);
        /**
         * @brief  读取boundary文件中的边界信息
         * @param  file 文件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-22
         */
        bool readBoundaryData(QFile& file);
        /**
         * @brief  读取boundary文件
         * @param  file 文件
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-22
         */
        bool readBoundary(const QString & boundary);
        /**
         * @brief  计算hex8单元的节点信息
         * @param  face 面（节点列表）
         * @param  inIdPrev 已知节点1（按OpenFOAM右手法则排序在前）
         * @param  inIdNext 已知节点2（按OpenFOAM右手法则排序在后）
         * @param[out]  outIdPrev 未知节点1（按OpenFOAM右手法则排序在前）
         * @param[out]  outIdNext 未知节点2（按OpenFOAM右手法则排序在后）
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool getHex8IdsByFace(QList<int> face, int inIdPrev, int inIdNext, int& outIdPrev, int& outIdNext);
        /**
         * @brief  装配hex8单元
         * @param  eleIndex 单元索引
         * @param  points 每个面上点的列表
         * @return 装配完的hex8单元或者空指针
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        Interface::FITKElementHex8* setupHex8Cell(int eleIndex, QVector<QList<int>> points);
        /**
         * @brief  装配多边形单元
         * @param  eleIndex 单元索引
         * @param  points 每个面上点的列表
         * @return 装配完的单元或者空指针
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        Interface::FITKElementPolygon* setupPolygonCell(QList<int> face);
        /**
         * @brief  装配单元
         * @param  eleIndex 单元索引
         * @param  faces 面索引列表
         * @return 装配完的单元或空指针
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        Interface::FITKAbstractElement* setupCell(int eleIndex, QList<int> faces);
        /**
         * @brief  装配3D单元
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool setup3DCells();
        /**
         * @brief  装配2D单元
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-17
         */
        bool setup2DCells();
        /**
         * @brief  装配边界
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-25
         */
        bool setupBoundarys();
        /**
         * @brief    清理读取的数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-12
         */
        void clearData();

    private:
        /**
         * @brief  流体网格
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-12
         */
        Interface::FITKUnstructuredFluidMeshVTK* _fluidOFMesh{};
        /**
         * @brief  内部网格数据
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-16
         */
        Interface::FITKUnstructuredMeshVTK* _fluidMesh{};
        /**
         * @brief  边界管理器
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-25
         */
        Interface::FITKBoundaryMeshVTKManager* _boundaryManager{};
        /**
         * @brief  文件夹路径
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-12
         */
        QString _dirPath{};
        /**
         * @brief  节点总数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-16
         */
        int _pointsNum{ 0 };
        /**
         * @brief  face总数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-16
         */
        int _facesNum{ 0 };
        /**
         * @brief  face列表<节点索引列表>
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-16
         */
        QVector<QList<int>> _faces{};
        /**
         * @brief  owner哈希<所属单元owner索引，face索引列表>
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-16
         */
        QMap<int, QList<int>> _owner{};
        /**
         * @brief  邻居单元<face索引, 邻居单元索引>
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-16
         */
        QMap<int, int> _neighbour{};
        /**
         * @brief  邻居单元<单元索引, face索引列表>
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-16
         */
        QMap<int, QList<int>> _tempNeighbour{};
        /**
         * @brief  边界
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-07-22
         */
        QList<Boundary*> _Boundarys{};
    };
}
#endif // FITKOPENFOAMMESHREADER_H
