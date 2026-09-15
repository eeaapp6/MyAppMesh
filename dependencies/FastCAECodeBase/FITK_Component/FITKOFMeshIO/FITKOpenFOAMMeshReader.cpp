/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOpenFOAMMeshReader.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementHex.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementPolygon.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementQuad.h"

namespace IO {


    FITKOpenFOAMMeshReader::FITKOpenFOAMMeshReader() = default;

    FITKOpenFOAMMeshReader::~FITKOpenFOAMMeshReader() = default;

    void FITKOpenFOAMMeshReader::run()
    {
        if (_fluidMesh == nullptr || _boundaryManager == nullptr) return;
        read();
    }

    void FITKOpenFOAMMeshReader::consoleMessage(int level, const QString & str)
    {
        //判断消息等级
        switch (level)
        {
        case 1:AppFrame::FITKMessageNormal(str); break;
        case 2: AppFrame::FITKMessageWarning(str); break;
        case 3:AppFrame::FITKMessageError(str); break;
        default: AppFrame::FITKMessageError(str); break;
        }
    }

    void FITKOpenFOAMMeshReader::setDataObject(Core::FITKAbstractDataObject * data)
    {
        _fluidOFMesh = dynamic_cast<Interface::FITKUnstructuredFluidMeshVTK*>(data);
    }

    bool FITKOpenFOAMMeshReader::read()
    {
        if (_fluidOFMesh == nullptr) return false;
        if (_fileName.isEmpty()) return false;
        QDir dir(_fileName + QDir::separator());
        QFileInfoList info_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        if (info_list.isEmpty())
        {
            consoleMessage(3, tr("err ! There is no polyMesh data"));
            return false;
        }
        QListIterator<QFileInfo> iterator(info_list);
        while (iterator.hasNext())
        {
            QFileInfo info = iterator.next();
            //获取文件夹名称
            QString name = info.fileName();
            QString path = info.absoluteFilePath();
            if (name != "polyMesh")
            {
                path = path + QDir::separator() + "polyMesh";
                QFileInfo fileInfo(path);
                if (!fileInfo.isDir())
                    continue;
            }
            //清理读取的数据
            this->clearData();
            //创建区域网格
            Interface::FITKFluidRegionsMesh* fluidRegion = new Interface::FITKFluidRegionsMesh;
            _fluidMesh = fluidRegion->getFieldMesh();
            _boundaryManager = fluidRegion->getBoundaryMeshManager();

            fluidRegion->setDataObjectName(name);
            if (!this->readPolyMesh(path))
            {
                delete fluidRegion;
            }
            else
            {
                _fluidOFMesh->appendDataObj(fluidRegion);
            }
        }
        consoleMessage(1, tr("read succeed ! %1").arg(_fileName));
        return true;
    }

    bool FITKOpenFOAMMeshReader::readPolyMesh(QString polyMeshDir)
    {
        QFileInfo fileInfo(polyMeshDir + QDir::separator());
        if (!fileInfo.isDir()) return false;


        auto dir = fileInfo.dir();
        _dirPath = dir.absolutePath();
        if (!dir.exists()) return false;

        if (!dir.exists("points") || !dir.exists("faces") || !dir.exists("owner") || !dir.exists("neighbour") || !dir.exists("boundary")) return false;

        printf("Reading points...\n");
        if (!readPoints(dir.filePath("points"))) return false;
        printf("Reading faces...\n");
        if (!readFaces(dir.filePath("faces"))) return false;
        printf("Reading owner...\n");
        if (!readOwner(dir.filePath("owner"))) return false;
        printf("Reading neighbour...\n");
        if (!readNeighbour(dir.filePath("neighbour"))) return false;
        printf("Reading boundary...\n");
        if (!readBoundary(dir.filePath("boundary"))) return false;
        //printf("Setup 2D cells...\n");
        //if (!setup2DCells()) return false;
        printf("Setup boundarys...\n");
        if (!setupBoundarys()) return false;

        printf("Finish reading. Node: %d, Element: %d\n", _fluidMesh->getNodeCount(), _fluidMesh->getElementCount());
        return true;
    }

    bool FITKOpenFOAMMeshReader::processLine(QString& line, bool inComment)
    {
        auto beginPos = line.indexOf("/*");
        auto endPos = line.indexOf("*/");
        auto singlePos = line.indexOf("//");
        // 当前行为注释内容
        if (inComment) {
            // 注释尚未结束
            if (endPos < 0) {
                line.clear();
                return true;
            }
            // 注释结束
            else {
                line = line.mid(endPos + 2);
                return false;
            }
        }
        else {
            // 当前行包含注释且在行内结束
            if (beginPos >= 0 && endPos >= 0) {
                line = line.mid(0, beginPos) + line.mid(endPos);
                return false;
            }
            // 当前行包含注释尚未在行内结束
            else if (beginPos >= 0) {
                line = line.mid(0, beginPos);
                return true;
            }
            // 当前行包含当行注释
            else if (singlePos >= 0) {
                line = line.mid(0, singlePos);
                return false;
            }
            else {
                return false;
            }
        }
    }

    bool FITKOpenFOAMMeshReader::readFoamFileHeader(QFile& file, FoamFileHeader& header) {
        QString line;
        bool isInt{ false };
        do
        {
            line = file.readLine().simplified();
            // 读到整数表示异常
            line.toInt(&isInt);
            if (isInt) return false;
            auto strs = line.split(' ', Qt::SkipEmptyParts);
            if (strs.size() != 2) continue;
            if (strs.at(0) == "format") {
                header.format = strs.at(1).chopped(1);
            }
            else if (strs.at(0) == "class") {
                header.clazz = strs.at(1).chopped(1);
            }
            else if (strs.at(0) == "location") {
                header.location = strs.at(1).chopped(1);
            }
            else if (strs.at(0) == "object") {
                header.object = strs.at(1).chopped(1);
            }

        } while (!line.startsWith("}"));
        return true;
    }

    bool FITKOpenFOAMMeshReader::readPointsData(QFile& file) {
        if (_fluidMesh == nullptr) return false;
        QString line;
        bool ok1, ok2, ok3;
        double x, y, z;
        int pid = 0;
        do
        {
            line = file.readLine().simplified();
            if (line.isEmpty()) continue;
            if (line == "(") continue;
            if (line == ")") break;
            // 移除括号
            line = line.chopped(1).remove(0, 1);
            auto strs = line.split(' ', Qt::SkipEmptyParts);
            if (strs.size() != 3) continue;
            x = strs.at(0).toDouble(&ok1);
            y = strs.at(1).toDouble(&ok2);
            z = strs.at(2).toDouble(&ok3);
            if (ok1 && ok2 && ok3) {
                _fluidMesh->addNode(++pid, x, y, z);
            }
            else {
                return false;
            }
        } while (!file.atEnd());
        return true;
    }

    bool FITKOpenFOAMMeshReader::readPoints(const QString &points)
    {
        if (_fluidMesh == nullptr) return false;

        QFile pointsFile(points);

        if (!pointsFile.exists()) return false;

        if (!pointsFile.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

        QString line;
        bool inComment{ false };
        bool isInt{ false };
        while (!pointsFile.atEnd())
        {
            line = pointsFile.readLine();
            inComment = processLine(line, inComment);
            line = line.simplified();
            if (line.isEmpty()) continue;
            // 读取文件头信息
            if (line.startsWith("FoamFile")) {
                FoamFileHeader header;
                if (!readFoamFileHeader(pointsFile, header)) return false;
                // 只支持文本格式
                if (header.format != "ascii") return false;
            }

            // 读取点数据
            _pointsNum = line.toInt(&isInt);
            if (isInt) {
                if (!readPointsData(pointsFile)) return false;
            }
        }
        pointsFile.close();
        return true;
    }

    bool FITKOpenFOAMMeshReader::readFacesData(QFile& file) {
        QString line{}, multiLine{};
        bool ok;
        int i = -1;
        bool atDataArea{ false };
        bool inBracket{ false };
        do
        {
            line = file.readLine().simplified();
            if (line.isEmpty()) continue;
            // 第一个括号
            if (line == "(" && !atDataArea) {
                atDataArea = true;
                continue;
            }
            else if (line == "(") {
                inBracket = true;
            }
            else if (line == ")" && !inBracket) {
                break;
            }

            // 单行数据
            if (!inBracket && line.endsWith(")")) {
                multiLine = line.chopped(1);
            }
            // 跨行数据
            else if (!line.endsWith(")")) {
                multiLine.append(" " + line);
                continue;
            }
            else {
                multiLine.append(" " + line.chopped(1));
                inBracket = false;
            }

            auto strs = multiLine.split('(', Qt::SkipEmptyParts);
            if (strs.size() != 2) continue;

            QList<QString> indexStrs = strs.at(1).split(' ', Qt::SkipEmptyParts);
            if (indexStrs.size() != strs.at(0).toInt()) return false;
            QList<int> pIDs{};
            for (auto str : indexStrs)
            {
                auto val = str.toInt(&ok) + 1;
                if (ok) pIDs.append(val);
                else return false;
            }
            multiLine.clear();
            _faces[++i] = pIDs;
        } while (!file.atEnd());

        return true;
    }

    bool FITKOpenFOAMMeshReader::readFaces(const QString &faces)
    {
        QFile facesFile(faces);

        if (!facesFile.exists()) return false;

        if (!facesFile.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

        QString line;
        bool inComment{ false };
        bool isInt{ false };
        while (!facesFile.atEnd())
        {
            line = facesFile.readLine();
            inComment = processLine(line, inComment);
            line = line.simplified();
            if (line.isEmpty()) continue;
            // 读取文件头信息
            if (line.startsWith("FoamFile")) {
                FoamFileHeader header;
                if (!readFoamFileHeader(facesFile, header)) return false;
                // 只支持文本格式
                if (header.format != "ascii") return false;
            }

            // 读取面数据
            _facesNum = line.toInt(&isInt);
            if (isInt) {
                _faces.resize(_facesNum);
                if (!readFacesData(facesFile)) return false;
            }
        }
        facesFile.close();

        return true;
    }

    bool FITKOpenFOAMMeshReader::readOwnerData(QFile& file) {
        QString line;
        bool ok;
        int faceIndex = -1;
        do
        {
            line = file.readLine().simplified();
            if (line.isEmpty()) continue;
            if (line == "(") continue;
            if (line == ")") break;

            auto ownerIndex = line.toInt(&ok);
            if (ok) {
                _owner[ownerIndex].append(++faceIndex);
            }
            else {
                return false;
            }
        } while (!file.atEnd());
        return true;
    }

    bool FITKOpenFOAMMeshReader::readOwner(const QString &owner)
    {
        QFile ownerFile(owner);

        if (!ownerFile.exists()) return false;

        if (!ownerFile.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

        QString line;
        bool inComment{ false };
        bool isInt{ false };
        int ownerNum{};
        while (!ownerFile.atEnd())
        {
            line = ownerFile.readLine();
            inComment = processLine(line, inComment);
            line = line.simplified();
            if (line.isEmpty()) continue;
            // 读取文件头信息
            if (line.startsWith("FoamFile")) {
                FoamFileHeader header;
                if (!readFoamFileHeader(ownerFile, header)) return false;
                // 只支持文本格式
                if (header.format != "ascii") return false;
            }

            // 读取Owner数据
            ownerNum = line.toInt(&isInt);
            if (isInt) {
                if (!readOwnerData(ownerFile)) return false;
            }
        }
        ownerFile.close();

        return true;
    }

    bool FITKOpenFOAMMeshReader::readNeighbourData(QFile& file)
    {
        QString line;
        bool ok;
        int faceIndex = -1;
        do
        {
            line = file.readLine().simplified();
            if (line.isEmpty()) continue;
            if (line == "(") continue;
            if (line == ")") break;

            auto neighbourIndex = line.toInt(&ok);
            if (ok) {
                _neighbour[++faceIndex] = neighbourIndex;
                _tempNeighbour[neighbourIndex].append(faceIndex);
            }
            else {
                return false;
            }
        } while (!file.atEnd());
        return true;
    }

    bool FITKOpenFOAMMeshReader::readNeighbour(const QString &neighbour)
    {
        QFile neighbourFile(neighbour);

        if (!neighbourFile.exists()) return false;

        if (!neighbourFile.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

        QString line;
        bool inComment{ false };
        bool isInt{ false };
        int neighbourNum{};
        while (!neighbourFile.atEnd())
        {
            line = neighbourFile.readLine();
            inComment = processLine(line, inComment);
            line = line.simplified();
            if (line.isEmpty()) continue;
            // 读取文件头信息
            if (line.startsWith("FoamFile")) {
                FoamFileHeader header;
                if (!readFoamFileHeader(neighbourFile, header)) return false;
                // 只支持文本格式
                if (header.format != "ascii") return false;
            }

            // 读取Owner数据
            neighbourNum = line.toInt(&isInt);
            if (isInt) {
                if (!readNeighbourData(neighbourFile)) return false;
            }
        }
        neighbourFile.close();

        return true;
    }

    bool FITKOpenFOAMMeshReader::readSingleBoundaryData(QFile & file, Boundary*  boundary)
    {

        QString line;

        do
        {
            line = file.readLine().simplified();
            if (line.isEmpty()) continue;
            if (line == "{") continue;
            if (line == "}") break;

            if (line.startsWith("type")) {
                boundary->type = line.mid(4).simplified().chopped(1);
            }
            else if (line.startsWith("inGroups")) {
                boundary->group = line.mid(8).simplified().chopped(1);
            }
            else if (line.startsWith("nFaces")) {
                boundary->nFaces = line.mid(6).simplified().chopped(1).toInt();
            }
            else if (line.startsWith("startFace")) {
                boundary->startFace = line.mid(9).simplified().chopped(1).toInt();
            }
        } while (!file.atEnd());
        return true;
    }

    bool FITKOpenFOAMMeshReader::readBoundaryData(QFile & file)
    {
        QString line;
        bool ok;
        int faceIndex = -1;
        Boundary boundary{};
        do
        {
            line = file.readLine().simplified();
            if (line.isEmpty()) continue;
            if (line == "(") continue;
            if (line == ")") break;

            auto boundary = new Boundary;
            boundary->name = line;
            readSingleBoundaryData(file, boundary);
            if (!boundary->isValid()) {
                delete boundary;
                boundary = nullptr;
                break;
            }
            else {
                _Boundarys.append(boundary);
            }

        } while (!file.atEnd());
        return true;
    }

    bool FITKOpenFOAMMeshReader::readBoundary(const QString & boundary)
    {
        QFile boundaryFile(boundary);

        if (!boundaryFile.exists()) return false;

        if (!boundaryFile.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

        QString line;
        bool inComment{ false };
        bool isInt{ false };
        int boundaryNum{};
        while (!boundaryFile.atEnd())
        {
            line = boundaryFile.readLine();
            inComment = processLine(line, inComment);
            line = line.simplified();
            if (line.isEmpty()) continue;
            // 读取文件头信息
            if (line.startsWith("FoamFile")) {
                FoamFileHeader header;
                if (!readFoamFileHeader(boundaryFile, header)) return false;
                // 只支持文本格式
                if (header.format != "ascii") return false;
            }

            // 读取boundary数据
            boundaryNum = line.toInt(&isInt);
            if (isInt) {
                if (!readBoundaryData(boundaryFile)) return false;
            }
        }
        boundaryFile.close();

        return true;
    }

    bool FITKOpenFOAMMeshReader::getHex8IdsByFace(QList<int> face, int inIdPrev, int inIdNext, int& outIdPrev, int& outIdNext)
    {
        int idNextPos = face.indexOf(inIdNext);
        int idPrevPos = face.indexOf(inIdPrev);
        // id1必须在id2后面，且必须相邻
        if (idPrevPos == 0 && idNextPos == 1) {
            outIdNext = face.at(3);
            outIdPrev = face.at(2);
        }
        else if (idPrevPos == 1 && idNextPos == 2) {
            outIdNext = face.at(0);
            outIdPrev = face.at(3);
        }
        else if (idPrevPos == 2 && idNextPos == 3) {
            outIdNext = face.at(1);
            outIdPrev = face.at(0);
        }
        else if (idPrevPos == 3 && idNextPos == 0) {
            outIdNext = face.at(2);
            outIdPrev = face.at(1);
        }
        return true;
    }

    Interface::FITKElementHex8* FITKOpenFOAMMeshReader::setupHex8Cell(int eleIndex, QVector<QList<int>> points) {

        auto faceNum = points.size();

        // 假定第一个面为底面
        QList<int> bottomFace = points.at(0);
        if (bottomFace.size() != 4) return nullptr;


        int id0 = bottomFace.at(0);
        int id1 = bottomFace.at(1);
        int id2 = bottomFace.at(2);
        int id3 = bottomFace.at(3);

        int id4{ -1 }, id5{ -1 }, id6{ -1 }, id7{ -1 };

        QList<int> leftFace{};
        QList<int> rightFace{};
        QList<int> frontFace{};
        QList<int> backFace{};

        // 只有三个面不足以确定所有顶点，需要添加邻面
        if (faceNum < 5) {
            auto faces = _tempNeighbour[eleIndex];
            for (auto faceIndex : faces) {
                auto face = _faces[faceIndex];
                if (face.size() != 4) return nullptr;
                points.append({ face.at(3), face.at(2), face.at(1), face.at(0) });
            }
        }
        faceNum = points.size();

        for (int i = 1; i < faceNum; ++i) {
            auto face = points.at(i);
            if (face.contains(id0) && face.contains(id1)) {
                leftFace = std::move(face);
            }
            else if (face.contains(id1) && face.contains(id2)) {
                backFace = std::move(face);
            }
            else if (face.contains(id2) && face.contains(id3)) {
                rightFace = std::move(face);
            }
            else if (face.contains(id3) && face.contains(id0)) {
                frontFace = std::move(face);
            }
        }

        // 分析面确定顶面的节点顺序
        if (!leftFace.empty())
        {
            getHex8IdsByFace(leftFace, id1, id0, id4, id7);
        }
        if (!rightFace.empty())
        {
            getHex8IdsByFace(rightFace, id3, id2, id6, id5);
        }
        if ((id4 == -1 || id5 == -1) && !frontFace.empty())
        {
            getHex8IdsByFace(frontFace, id0, id3, id5, id4);
        }
        if ((id6 == -1 || id7 == -1) && !backFace.empty())
        {
            getHex8IdsByFace(backFace, id2, id1, id7, id6);
        }

        // 只有三个face的单元缺一个顶点，需要从neighbour中读取
        if (id4 == -1 || id5 == -1 || id6 == -1 || id7 == -1)
        {
            return nullptr;
        }
        // OpenFOAM的face节点是按右手螺旋定则排序的
        auto hex8Cell = new Interface::FITKElementHex8;
        hex8Cell->setEleID(eleIndex);
        hex8Cell->setNodeID({ id0, id3, id2, id1, id4, id5, id6, id7 });
        return hex8Cell;
    }

    Interface::FITKElementPolygon * FITKOpenFOAMMeshReader::setupPolygonCell(QList<int> face)
    {
        // OpenFOAM的face节点是按右手螺旋定则排序的
        auto polygonCell = new Interface::FITKElementPolygon;
        polygonCell->setNodeID(face);
        return polygonCell;
    }

    Interface::FITKAbstractElement* FITKOpenFOAMMeshReader::setupCell(int eleIndex, QList<int> faces) {
        Interface::FITKAbstractElement* cell{};

        // 单元包含面的个数
        auto facesNum = faces.size();

        // 包含每个面的点索引的面数组
        QVector<QList<int>> points{};

        // 包含每个面的点数的面数组
        QVector<int> pointNumOfFace{};

        // 遍历面
        for (auto fIndex : faces) {
            auto pointsInFace = _faces[fIndex];
            points.append(pointsInFace);

            pointNumOfFace.append(pointsInFace.size());
        }

        // 面数组里面单个面中点数的最大值和最小值
        int minPointNum = pointNumOfFace.at(0);
        int maxPointNum = minPointNum;

        for (int i = 1, n = 0; i < facesNum; ++i) {
            n = pointNumOfFace.at(i);
            if (minPointNum > n) minPointNum = n;
            if (maxPointNum < n) maxPointNum = n;
        }

        // 六面体单元(最少有3个面，最多有6个面)
        if (minPointNum == 4 && maxPointNum == 4) {
            cell = setupHex8Cell(eleIndex, points);
        }
        return cell;
    }

    bool FITKOpenFOAMMeshReader::setup3DCells() {
        if (_fluidMesh == nullptr) return false;

        auto eleNum = _owner.size();
        for (int i = 0; i < eleNum; ++i)
        {
            auto cell = setupCell(i, _owner[i]);
            if (cell == nullptr) {
                return false;
            }
            _fluidMesh->appendElement(cell);
        }
        return true;
    }
    bool FITKOpenFOAMMeshReader::setup2DCells()
    {
        if (_fluidMesh == nullptr) return false;

        auto eleNum = _faces.size();
        for (int i = 0; i < eleNum; ++i)
        {
            auto cell = setupPolygonCell(_faces.at(i));
            if (cell == nullptr) {
                return false;
            }
            _fluidMesh->appendElement(cell);
        }
        return true;
    }
    bool FITKOpenFOAMMeshReader::setupBoundarys()
    {
        if (_boundaryManager == nullptr) return false;
        _fluidMesh->setUserData(Core::FITKUserData::FITKUserRole + 2, _dirPath);
        for (auto boundary : _Boundarys) {
            auto b = new Interface::FITKBoundaryMeshVTK(_fluidMesh);
            //记录边界网格信息 libaojun@20240906
            b->setUserData(Core::FITKUserData::FITKUserRole, boundary->startFace);
            b->setUserData(Core::FITKUserData::FITKUserRole + 1, boundary->nFaces);
            b->setUserData(Core::FITKUserData::FITKUserRole + 2, _dirPath);
            //********************
            for (int i = 0; i < boundary->nFaces; ++i)
            {
                const QList<int>& face = _faces[boundary->startFace + i];
                auto ele = new Interface::FITKElementPolygon;
                ele->setNodeCount(face.size());
                ele->setNodeID(face);
                b->appendElement(ele);
            }
            b->setDataObjectName(boundary->name);
            _boundaryManager->appendDataObj(b);
        }
        return true;
    }

    void FITKOpenFOAMMeshReader::clearData()
    {
        _fluidMesh = nullptr;
        _boundaryManager = nullptr;
        _pointsNum = 0;
        _facesNum = 0;
        _faces.clear();
        _owner.clear();
        _neighbour.clear();
        _tempNeighbour.clear();
        _Boundarys.clear();
    }
}
