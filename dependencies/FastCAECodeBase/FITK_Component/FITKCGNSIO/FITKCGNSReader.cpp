/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCGNSReader.h"
#include "FITKCGNSIOInterface.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKStructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

namespace IO
{
    FITKCGNSReader::FITKCGNSReader(FITKCGNSIOInterface* io) :
        _compInterface(io)
    {

    }

    FITKCGNSReader::~FITKCGNSReader()
    {
        
    }

    void FITKCGNSReader::run()
    {
        if (!_resultMark) return;
        *_resultMark = false;
        this->consoleMessage(1, tr("Reading CGNS File : %1").arg(_fileName));
        //打开cgns文件
        if (!this->openCGNSFile())
        {
            this->consoleMessage(3, tr("Reading CGNS Error ! Open file err %1").arg(_fileName));
            this->closeFile();
            return;
        }
        //读取cgns数据
        *_resultMark = read();
        //信息打印
        if (*_resultMark)
            this->consoleMessage(1, tr("Reading CGNS File Complete : %1").arg(_fileName));
        else
            this->consoleMessage(3, tr("Reading CGNS File Err : %1").arg(_fileName));
        this->closeFile();
    }

    void FITKCGNSReader::consoleMessage(int level, const QString& str)
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

    bool FITKCGNSReader::read()
    {
        if (!_compInterface) return false;
        //读取CGNS信息
        this->readCGNSInfo();

        // 获取文件中的 CGNS 基节点数
        int nbases = 0;
        if (CG_OK != cg_nbases(m_index_file, &nbases))return false;
        for (int iBase = 0; iBase < nbases; ++iBase)
        {
            //获取基节点索引号
            m_index_base = iBase + 1;

            this->readBase();
        }


        return true;
    }

    bool FITKCGNSReader::readBase()
    {
        if (!_compInterface || m_index_base <= 0) return false;
        //获取基本信息
        char base_name[33];//base的名称
        int cell_dim = -1, phys_dim = -1;//网格单元维度、物理空间维度
        if (CG_OK != cg_base_read(m_index_file, m_index_base, base_name, &cell_dim, &phys_dim))return false;

        //获取基节点的描述
        cg_goto(m_index_file, m_index_base, NULL);
        int ndescriptors = 0;
        cg_ndescriptors(&ndescriptors);
        for (int iDescriptor = 0; iDescriptor < ndescriptors; ++iDescriptor)
        {
            int index_descriptor = iDescriptor + 1;
            char descriptor_name[33]{};
            char *descr_text = nullptr;     // 存储描述符文本内容
            if (CG_OK != cg_descriptor_read(index_descriptor, descriptor_name, &descr_text))
                return false;
            if (descr_text == nullptr) continue;
            this->consoleMessage(1, QString("%1 %2 Descriptor : %3").arg(base_name).arg(descriptor_name).arg(descr_text));
            delete[] descr_text;
        }

        //读取族
        int nfamily = 0;
        cg_nfamilies(m_index_file, m_index_base, &nfamily);
        for (int iFamily = 0; iFamily < nfamily; ++iFamily)
        {
            //获取族节点索引号
            m_index_family = iFamily + 1;
            if (!readBase_Family())
                return false;
        }

        //获取 base 中的区域数
        int nzones = 0;
        if (CG_OK != cg_nzones(m_index_file, m_index_base, &nzones))return false;
        for (int iZone = 0; iZone < nzones; ++iZone)
        {
            //获取区域节点索引号
            m_index_zone = iZone + 1;
            //获取区域类型（结构化或非结构化）
            CGNS_ENUMT(ZoneType_t) zonetype;
            if (CG_OK != cg_zone_type(m_index_file, m_index_base, m_index_zone, &zonetype)) return false;
            //创建数据空间
            if (!this->createMeshData())
                continue;

            //读取结构数据
            if (zonetype == CGNS_ENUMV(Structured))
                this->readBase_ZoneStructured();
            //读取非结构数据
            else if (zonetype == CGNS_ENUMV(Unstructured))
                this->readBase_ZoneUnstructured();
        }
        return true;
    }

    bool FITKCGNSReader::readBase_Family()
    {
        if (!_compInterface || m_index_base <= 0 || m_index_family <= 0) return false;
        //获取族数据
        char family_name[64] = { 0 };//Family的名称
        int nboco = 0, ngeos = 0;//Family关联的边界条件数量,  Family关联的几何实体数量
        if (CG_OK != cg_family_read(m_index_file, m_index_base, m_index_family, family_name, &nboco, &ngeos)) return false;

        //获取族的描述
        cg_goto(m_index_file, m_index_base, "Family_t", m_index_family, "end");
        int ndescriptors = 0;
        cg_ndescriptors(&ndescriptors);
        for (int iDescriptor = 0; iDescriptor < ndescriptors; ++iDescriptor)
        {
            int index_descriptor = iDescriptor + 1;
            char descriptor_name[33]{};
            char *descr_text = nullptr;     // 存储描述符文本内容
            if (CG_OK != cg_descriptor_read(index_descriptor, descriptor_name, &descr_text))
                return false;
            if (descr_text == nullptr) continue;
            this->consoleMessage(1, QString("%1 %2 Descriptor : %3").arg(family_name).arg(descriptor_name).arg(descr_text));
            delete[] descr_text;
        }

        //获取族边界
        for (int iBoco = 0; iBoco < nboco; ++iBoco)
        { 
            //获取边界条件索引号
            int index_Boco = iBoco + 1;
            CGNS_ENUMT(BCType_t) bocotype;
            char fambcName[33] = { 0 };
            if (CG_OK != cg_fambc_read(m_index_file, m_index_base, m_index_family, index_Boco, fambcName, &bocotype))
                return false;
        }

        //获取族几何数据
        for (int iGeo = 0; iGeo < ngeos; ++iGeo)
        {
            //获取几何索引号
            int index_Geo = iGeo + 1;
            char geo_name[66] = { 0 }, *geo_file = nullptr, cadName[66] = { 0 };//几何实体的名称、关联的几何文件路径、CAD模型名称
            int npart = 0;//该几何包含的部件数量
            if (CG_OK != cg_geo_read(m_index_file, m_index_base, m_index_family, index_Geo, geo_name, &geo_file, cadName, &npart))
                return false;
        }

        //获取名称数量
        int nNames = 0;
        if (CG_OK != cg_nfamily_names(m_index_file, m_index_base, m_index_family, &nNames));
        for (int iName = 0; iName < nNames; ++iName)
        {
            //获取名称索引号
            int index_Name = iName + 1;
            char name[33] = { 0 }, family[33] = { 0 };//存储与 Family 关联的具体名称（如边界、几何的名称)、存储对应的 Family 名称
            if (CG_OK != cg_family_name_read(m_index_file, m_index_base, m_index_family, index_Name, name, family))
                return false;
        }
        
        return true;
    }

    bool FITKCGNSReader::readBase_ZoneStructured()
    {
        return false;
    }

    bool FITKCGNSReader::readBase_ZoneUnstructured()
    {
        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_compInterface->getCGNSDataIO()._mesh[m_current_index_mesh]);
        if (!unstructuredMesh) return false;
        //获取区域名称
        char zone_name[33] = { 0 };
        cgsize_t zone_ijk[3] = { 0 };
        if (CG_OK != cg_zone_read(m_index_file, m_index_base, m_index_zone, zone_name, zone_ijk))
            return false;
        //设置名称
        unstructuredMesh->setDataObjectName(zone_name);
        //获取 CGNS 区域的索引维度
        int dim = 3;
        if (CG_OK != cg_index_dim(m_index_file, m_index_base, m_index_zone, &dim))
            return false;
        //获取节点信息
        int vertex_number = zone_ijk[0];
        cgsize_t range_from[3] = { 1, 1, 1 };
        cgsize_t range_to[3] = { vertex_number, vertex_number, vertex_number };
        int cellNumber = zone_ijk[1];
        //读取节点数据
        if (!this->readGridCoordinate(vertex_number, range_from, range_to, unstructuredMesh))
            return false;
        //获取元素集数量
        int nsections = 0;
        cg_nsections(m_index_file, m_index_base, m_index_zone, &nsections);
        //读取单元
        for (int iSection = 0; iSection < nsections; iSection++)
        {
            //获取元素索引号
            int index_section = iSection + 1;
            this->readBase_ZoneUnstructured_Section(index_section);
        }
        this->consoleMessage(1, QString("ZoneMesh %1 : Node %2  Element %3").arg(zone_name).arg(unstructuredMesh->getNodeCount()).arg(unstructuredMesh->getElementCount()));
        //获取边界数量
        int bcCount = 0;
        if (CG_OK != cg_nbocos(m_index_file, m_index_base, m_index_zone, &bcCount)) return false;
        for (int iBc = 0; iBc < bcCount; ++iBc)
        {
            //获取边界索引号
            int index_BC = iBc + 1;
            if (!readBase_ZoneUnstructured_BC(index_BC))
                return false;
        }
        //获取体单元的FamilyName
        cg_goto(m_index_file, m_index_base, "Zone_t", m_index_zone, "end");
        char family_name[33];
        cg_famname_read(family_name);
        return true;
    }

    bool FITKCGNSReader::readBase_ZoneUnstructured_Section(int index_section)
    {
        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_compInterface->getCGNSDataIO()._mesh[m_current_index_mesh]);
        if (!unstructuredMesh) return false;
        //获取元素信息
        char section_name[33] = { 0 };//单元段名称
        /*int*/ cgsize_t istart = 0, iend = 0;//第一个单元的索引, 最后一个单元的索引
        int nbndry = 0, iparent_flag = 0;//包含的边界单元数量, 父单元标识（用于嵌套网格，通常为0）
        CGNS_ENUMT(ElementType_t) itype;//单元类型
        if (cg_section_read(m_index_file, m_index_base, m_index_zone, index_section, section_name, &itype, &istart, &iend, &nbndry, &iparent_flag))
            return false;
        //获取元素数量
        /*int*/ cgsize_t element_data_size = 0;
        if (cg_ElementDataSize(m_index_file, m_index_base, m_index_zone, index_section, &element_data_size))
            return false;
        if (element_data_size == 0) return true;
        int size = 4 * (iend - istart + 1);
        QVector<cgsize_t> elements(element_data_size, 0);//存储单元的节点数据
        cgsize_t *element_parent = nullptr;//存储父单元关联数据,通常为NULL
        if (cg_elements_read(m_index_file, m_index_base, m_index_zone, index_section, elements.data(), element_parent))
        {
            return false;
        }
        //多边形网格
        if (cg_poly_elements_read(m_index_file, m_index_base, m_index_zone, index_section, elements.data(), 0, element_parent))
        {
            return false;
        }
        //读取单元数据
        this->readElementCells(unstructuredMesh, istart, iend, itype, elements.data(), element_data_size);
        return true;
    }

    bool FITKCGNSReader::readBase_ZoneStructured_BC(int index_BC)
    {
        return false;
    }

    bool FITKCGNSReader::readBase_ZoneUnstructured_BC(int index_BC)
    {
        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_compInterface->getCGNSDataIO()._mesh[m_current_index_mesh]);
        if (!unstructuredMesh) return false;
        //获取边界信息
        char bcName[33]{};//边界名称
        CGNS_ENUMT(BCType_t) type = CGNS_ENUMV(BCTypeNull);//边界类型
        CGNS_ENUMT(PointSetType_t) ptType = CGNS_ENUMV(PointSetTypeNull);//点集类型
        cgsize_t npnts;//点集中的点数量
        int NormalIndex;//法向量索引
        cgsize_t NormalListSize;//法向量列表的大小
        CGNS_ENUMT(DataType_t) dataType = CGNS_ENUMV(DataTypeNull);//法向量数据的类型
        int dataSetCount;//数据集的数量
        if (CG_OK != cg_boco_info(m_index_file, m_index_base, m_index_zone, index_BC, bcName, &type, &ptType, &npnts, &NormalIndex, &NormalListSize, &dataType, &dataSetCount))
            return false;
        //获取边界类型数据
        char family_name[33];
        if (type == CGNS_ENUMV(FamilySpecified))
        {
            cg_goto(m_index_file, m_index_base, "Zone_t", m_index_zone, "ZoneBC_t", 1, "BC_t", index_BC, "end");
            cg_famname_read(family_name);
        }
        //获取边界数据
        QVector<cgsize_t> pnts(npnts, 0);
        if (CG_OK != cg_boco_read(m_index_file, m_index_base, m_index_zone, index_BC, pnts.data(), nullptr))
        {
            return false;
        }
        //点集链表
        if (ptType == CGNS_ENUMV(PointList))
        {
        }
        //点集范围
        else if (ptType == CGNS_ENUMV(PointRange))
        {
            int min = pnts[0];
            int max = pnts[1];
            //边界id
            pnts.resize(max - min + 1);
            for (int i = 0; i < pnts.size(); ++i)
            {
                pnts[i] = min + i;
            }
        }
        //获取网格位置信息
        CGNS_ENUMT(GridLocation_t) gridType;
        if (CG_OK != cg_boco_gridlocation_read(m_index_file, m_index_base, m_index_zone, index_BC, &gridType))
        {
            return false;
        }
        //顶点
        Interface::FITKModelSet* elementSet = nullptr;
        if (gridType == Vertex)
        {
            elementSet = new Interface::FITKModelSet(Interface::FITKModelEnum::FMSNode);
        }
        //单元中心等
        else if (gridType == CellCenter || gridType == FaceCenter || gridType == IFaceCenter ||
            gridType == JFaceCenter || gridType == KFaceCenter || gridType == EdgeCenter)
        {
            elementSet = new Interface::FITKModelSet(Interface::FITKModelEnum::FMSElem);
        }
        if (elementSet == nullptr) return false;
        QList<int> idsMember;
        for (int i = 0; i < pnts.size(); ++i)
            idsMember.append(pnts[i]);
        elementSet->setDataObjectName(bcName);
        elementSet->setAbsoluteMember(idsMember);
        elementSet->setModel(unstructuredMesh->getDataObjectID());
        _compInterface->getCGNSDataIO()._componentManager[m_current_index_mesh].append(elementSet);
        this->consoleMessage(1, QString("BC %1 : Element %3").arg(bcName).arg(idsMember.size()));
        return true;
    }

    bool FITKCGNSReader::readGridCoordinate(int vertext_num, cgsize_t range_from[3], cgsize_t range_to[3], Interface::FITKNodeList * nodeList)
    {
        if (nodeList == nullptr) return false;

        auto voidToDouble = [&](QVector<double>& result, void* value, int vertext_num, CGNS_ENUMV(DataType_t) type)
        {
            //数据转换
            if (type == CGNS_ENUMV(RealSingle))
            {
                float* fPos = static_cast<float*>(value);
                for (int i = 0; i < vertext_num; ++i)
                    result[i] = fPos[i];
            }
            else if (type == CGNS_ENUMV(RealDouble))
            {
                double* dPos = static_cast<double*>(value);
                for (int i = 0; i < vertext_num; ++i)
                    result[i] = dPos[i];
            }
        };

        //获取方向轴数量
        int coordinate_number = 0;
        if (CG_OK != cg_ncoords(m_index_file, m_index_base, m_index_zone, &coordinate_number))
            return false;
        //坐标
        QVector<double> coordX(vertext_num, 0);
        QVector<double> coordY(vertext_num, 0);
        QVector<double> coordZ(vertext_num, 0);
        //获取节点坐标
        for (int iCoor = 0; iCoor < coordinate_number; ++iCoor)
        {
            //获取坐标索引号
            int index_coor = iCoor + 1;
            //获取方向轴信息
            char coordinate_name[33] = { 0 };//方向轴名称
            CGNS_ENUMV(DataType_t) datatype;//数据类型
            if (CG_OK != cg_coord_info(m_index_file, m_index_base, m_index_zone, index_coor, &datatype, coordinate_name))
                return false;
            //创建数据空间用于获取一维坐标轴的数据
            void* xyz = nullptr;
            if (datatype == CGNS_ENUMV(RealSingle))
                xyz = new float[vertext_num];
            else if (datatype == CGNS_ENUMV(RealDouble))
                xyz = new double[vertext_num];
            if (xyz == nullptr)
                return false;
            //获取坐标轴参数
            if (CG_OK != cg_coord_read(m_index_file, m_index_base, m_index_zone, coordinate_name, datatype, range_from, range_to, xyz))
            {
                delete[] xyz;
                return false;
            }
            //按坐标抽名称设置顺序
            if (!strcmp(coordinate_name, "CoordinateX"))
                voidToDouble(coordX, xyz, vertext_num, datatype);
            else if (!strcmp(coordinate_name, "CoordinateY"))
                voidToDouble(coordY, xyz, vertext_num, datatype);
            else if (!strcmp(coordinate_name, "CoordinateZ"))
                voidToDouble(coordZ, xyz, vertext_num, datatype);
            delete[] xyz;
        }
        //添加点数据
        for (int i = 0; i < vertext_num; ++i)
        {
            int nID = nodeList->addNode(coordX[i], coordY[i], coordZ[i]);
        }
        return true;
    }

    void FITKCGNSReader::readElementCells(Interface::FITKElementList * elementList, int start_index, int end_index, int type, cgsize_t * elements, int element_data_size)
    {
        if (CGNS_ENUMT(TRI_3) == type) //三角形
        {
            int cell_number = element_data_size / 3;
            assert(0 == element_data_size % 3);
            assert(end_index - start_index + 1 == cell_number);

            for (int iCnt = 0; iCnt < cell_number; ++iCnt)
            {
                Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType::Tri3);
                if (!element) continue;
                element->setEleID(start_index + iCnt);
                element->setNodeID(QList<int>() << elements[iCnt * 3 + 0] << elements[iCnt * 3 + 1] << elements[iCnt * 3 + 2]);
                elementList->appendElement(element);
            }
        }
        else if (CGNS_ENUMT(BAR_2) == type) //二节点梁单元
        {
            int cell_number = element_data_size / 2;
            assert(0 == element_data_size % 2);
            assert(end_index - start_index + 1 == cell_number);

            for (int iCnt = 0; iCnt < cell_number; ++iCnt)
            {
                Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType::Line2);
                if (!element) continue;
                element->setEleID(start_index + iCnt);
                element->setNodeID(QList<int>() << elements[iCnt * 2 + 0] << elements[iCnt * 2 + 1]);
                elementList->appendElement(element);
            }
        }
        else if (CGNS_ENUMT(TETRA_4) == type) //四节点四面体
        {
            int cell_number = element_data_size / 4;
            assert(0 == element_data_size % 4);
            assert(end_index - start_index + 1 == cell_number);

            for (int iCnt = 0; iCnt < cell_number; ++iCnt)
            {
                Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType::Tet4);
                if (!element) continue;
                element->setEleID(start_index + iCnt);
                element->setNodeID(QList<int>() << elements[iCnt * 4 + 0] << elements[iCnt * 4 + 1] << elements[iCnt * 4 + 2] << elements[iCnt * 4 + 3]);
                elementList->appendElement(element);
            }
        }
        else if (CGNS_ENUMT(QUAD_4) == type) //四节点四边形
        {
            int cell_number = element_data_size / 4;
            assert(0 == element_data_size % 4);
            assert(end_index - start_index + 1 == cell_number);

            for (int iCnt = 0; iCnt < cell_number; ++iCnt)
            {
                Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType::Quad4);
                if (!element) continue;
                element->setEleID(start_index + iCnt);
                element->setNodeID(QList<int>() << elements[iCnt * 4 + 0] << elements[iCnt * 4 + 1] << elements[iCnt * 4 + 2] << elements[iCnt * 4 + 3]);
                elementList->appendElement(element);
            }
        }
        else if (CGNS_ENUMT(HEXA_8) == type) //八节点六面体
        {
            int cell_number = element_data_size / 8;
            assert(0 == element_data_size % 8);
            assert(end_index - start_index + 1 == cell_number);

            for (int iCnt = 0; iCnt < cell_number; ++iCnt)
            {
                Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType::Hex8);
                if (!element) continue;
                element->setEleID(start_index + iCnt);
                element->setNodeID(QList<int>() << elements[iCnt * 8 + 0] << elements[iCnt * 8 + 1] << elements[iCnt * 8 + 2] << elements[iCnt * 8 + 3] <<
                    elements[iCnt * 8 + 4] << elements[iCnt * 8 + 5] << elements[iCnt * 8 + 6] << elements[iCnt * 8 + 7]);
                elementList->appendElement(element);
            }
        }
        else if (CGNS_ENUMT(PENTA_6) == type) //六节点三棱柱
        {
            int cell_number = element_data_size / 6;
            assert(0 == element_data_size % 6);
            assert(end_index - start_index + 1 == cell_number);

            for (int iCnt = 0; iCnt < cell_number; ++iCnt)
            {
                Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType::Wedge6);
                if (!element) continue;
                element->setEleID(start_index + iCnt);
                element->setNodeID(QList<int>() << elements[iCnt * 6 + 0] << elements[iCnt * 6 + 1] << elements[iCnt * 6 + 2] << elements[iCnt * 6 + 3] <<
                    elements[iCnt * 6 + 4] << elements[iCnt * 6 + 5]);
                elementList->appendElement(element);
            }
        }
        else if (CGNS_ENUMV(PYRA_5) == type) //金字塔类型
        {
            int cell_number = element_data_size / 5;
            assert(0 == element_data_size % 5);
            assert(end_index - start_index + 1 == cell_number);

            for (int iCnt = 0; iCnt < cell_number; ++iCnt)
            {

            }
        }
        else if (CGNS_ENUMV(MIXED) == type)//混合单元类型
        {

        }
    }

    bool FITKCGNSReader::readCGNSInfo()
    {
        //获取 文件的CGNS 版本
        float fileVersion = -1;
        if (CG_OK != cg_version(m_index_file, &fileVersion))return false;
        //获取 CGNS文件精度
        int precision = -1;
        if (CG_OK != cg_precision(m_index_file, &precision))return false;
        //是否启用压缩算法
        int compress = -1;
        if (CG_OK != cg_get_compress(&compress)) return false;
        this->consoleMessage(1, tr("Reading CGNS File Version : %1, Precision : %2").arg(fileVersion).arg(precision));
        return true;
    }

    bool FITKCGNSReader::createMeshData()
    {
        //创建数据存储空间
        Interface::FITKAbstractMesh* mesh = _compInterface->getCGNSDataIO().CreateMeshFun();
        if (!mesh) return false;
        _compInterface->getCGNSDataIO()._mesh.append(mesh);
        _compInterface->getCGNSDataIO()._componentManager.append(QList<Interface::FITKModelSet*>());
        ++m_current_index_mesh;
        return true;
    }

}

