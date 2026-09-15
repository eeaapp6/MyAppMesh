/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelCut.h"

// OCC
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <gp_Dir.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepOffset_MakeOffset.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Precision.hxx>
#include <BRepBndLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Section.hxx>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <ProjLib.hxx>
#include <gp_Pnt2d.hxx>
#include <Bnd_Box2d.hxx>
#include <ShapeAnalysis.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketch2D.h"
#include "FITKOCCModelPart.h"
#include "FITKOCCComandCommon.h"
#include "FITKOCCExtendTool.h"

#include <QSet>
#include <QDebug>

namespace {
    /**
     * @brief  代表草绘图中每个封闭的曲线.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-09-29
     */
    struct CWire {
        /// 包围盒
        Bnd_Box Bound;
        /// TopoDS_Wire
        BRepBuilderAPI_MakeWire MakeWire{};
        /// 起点
        gp_Pnt Start{};
        /// 终点
        gp_Pnt End{};
        /// 封闭曲线的点
        QList<gp_Pnt> Pnts{};
        /**
         * @brief  连接新的边.
         * @param  edge 新边
         * @return 如果成功连接，返回true，否则返回false
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        bool connect(const TopoDS_Edge& edge) {
            Standard_Real first;
            Standard_Real last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
            gp_Pnt p1 = curve->Value(first);
            gp_Pnt p2 = curve->Value(last);
            // 空线段直接添加
            if (!MakeWire.IsDone() || MakeWire.Shape().IsNull()) {
                Start = p1;
                End = p2;
                Pnts.append(p1);
                Pnts.append(p2);
                MakeWire.Add(edge);
                return true;
            }
            // Start与p1重合
            if (Start.Distance(p1) <= Precision::Confusion()) {
                MakeWire.Add(edge);
                Pnts.append(p2);
                Start = p2;
                return true;
            }
            // Start与p2重合
            if (Start.Distance(p2) <= Precision::Confusion()) {
                MakeWire.Add(edge);
                Pnts.append(p1);
                Start = p1;
                return true;
            }
            // End与p1重合
            if (End.Distance(p1) <= Precision::Confusion()) {
                MakeWire.Add(edge);
                Pnts.append(p2);
                End = p2;
                return true;
            }
            // End与p2重合
            if (End.Distance(p2) <= Precision::Confusion()) {
                MakeWire.Add(edge);
                Pnts.append(p1);
                End = p1;
                return true;
            }
            return false;
        }
        /**
         * @brief  是否包含某边.
         * @param  edge 边
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        bool contain(const TopoDS_Edge& edge) {
            for (TopoDS_Iterator iter(MakeWire.Shape()); iter.More(); iter.Next()) {
                if (iter.Value().IsSame(edge)) return true;
            }
            return false;
        }
        /**
         * @brief  当前wire是否为空.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        bool isNull() { return MakeWire.Edge().IsNull(); }
        /**
         * @brief  当前wire是否闭合.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        bool isClosed() { return MakeWire.Shape().Closed(); }
    };
    /**
     * @brief  一组共点的边.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-09-29
     */
    struct EdgePair {
        /// 共点
        gp_Pnt Pnt{};
        /// 共点的边，size必须是2，否则会出现交叉或者未封闭的图形
        QList<TopoDS_Edge> Edges{};
        /**
         * @brief  是否为空.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        bool isNull() { return Edges.size() == 0; }
        /**
         * @brief  判断共点边是否相同.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        friend bool operator ==(const EdgePair &, const EdgePair &) noexcept;
    };
    /**
     * @brief  判断共点边是否相同.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-09-29
     */
    inline bool operator==(const EdgePair &first, const EdgePair &second) noexcept
    {
        return first.Pnt.Distance(second.Pnt) <= Precision::Confusion();
    }
    /**
     * @brief  共点边的组.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-09-29
     */
    struct EdgeGroup {
        /// 共点边列表
        QList<EdgePair> Groups{};
        /**
         * @brief  以点为条件添加边.
         * @param  p 点
         * @param  e 边
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        void add(const gp_Pnt& p, const TopoDS_Edge& e) {
            // 如果已经存在该点的共点边，则添加到已有共点边
            for (auto& g : Groups) {
                if (g.Pnt.Distance(p) <= Precision::Confusion()) {
                    g.Edges.append(e);
                    return;
                }
            }
            // 否则创建新的共点边
            EdgePair ep;
            ep.Pnt = p;
            ep.Edges = { e };
            Groups.append(ep);
        }
        /**
         * @brief  移除某点处的共点边并返回删除对象.
         * @param  p 点
         * @return 返回删除的对象（可能为空isNull() == true）
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        EdgePair removeByPnt(const gp_Pnt& p) {
            EdgePair ret;
            for (auto g : Groups) {
                if (g.Pnt.Distance(p) <= Precision::Confusion()) {
                    ret = g;
                    Groups.removeOne(g);
                    break;
                }
            }
            return ret;
        }
        /**
         * @brief  移除某边所在的共点边并返回删除对象.
         * @param  e 要移除的边
         * @return 返回删除的对象（可能为空isNull() == true）
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        EdgePair removeByEdge(const TopoDS_Edge& e) {
            EdgePair ret;
            for (auto g : Groups) {
                for (auto edge : g.Edges) {
                    if (edge.IsSame(e))
                    {
                        ret = g;
                        Groups.removeOne(g);
                        break;
                    }
                }
            }
            return ret;
        }

    };
    /**
     * @brief  包围盒树结构的节点类.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-09-29
     */
    class WireBoundTreeNode {
    public:
        /// 构造函数
        WireBoundTreeNode(const CWire& cw) : m_Data(cw) {}
        /// 析构函数
        ~WireBoundTreeNode() = default;
        /**
         * @brief  返回节点数据CWire.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        CWire& data() { return m_Data; }
        /**
         * @brief  返回当前节点的父节点.
         * @return 父节点，根节点返回空指针nullptr
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        WireBoundTreeNode* parent() { return m_Parent; }
        /**
         * @brief  是否有子节点.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        bool hasChild() { return m_Childs.size() > 0; }
        /**
         * @brief  返回所有子节点列表.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        QList<WireBoundTreeNode*>& childs() { return m_Childs; }
        /**
         * @brief  返回节点数据CWire的包围盒.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        Bnd_Box bnd() const { return m_Data.Bound; }
        /**
         * @brief  重新设置当前节点的父节点.
         * @param  n 新的父节点(n==nullptr代表根节点)
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        void setParent(WireBoundTreeNode* n) {
            if (n == m_Parent) return;
            // 移除原有的父级关系
            if (m_Parent != nullptr) {
                m_Parent->m_Childs.removeAll(this);
            }
            // 设置新的父子关系
            m_Parent = n;
            if (n != nullptr) {
                n->m_Childs.append(this);
            }
        }

    private:
        /// 节点数据
        CWire m_Data{};
        /// 节点父节点
        WireBoundTreeNode* m_Parent{};
        /// 节点子节点列表
        QList<WireBoundTreeNode*> m_Childs{};
    };
    /**
     * @brief  判断草图包围盒的包含关系.
     * @param  box1 包围盒1
     * @param  box2 包围盒2
     * @note   此处不能用OCC提供的IsOut代替，不符合草图包围的情况会抛异常由调用函数捕捉
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-09-29
     */
    bool isOut(const Bnd_Box& box1, const Bnd_Box& box2) {
        // 草图包围盒不能为空
        if (box1.IsVoid() || box2.IsVoid()) throw "System Error!";

        Standard_Real xMin1, xMax1, yMin1, yMax1, zMin1, zMax1;
        Standard_Real xMin2, xMax2, yMin2, yMax2, zMin2, zMax2;
        box1.Get(xMin1, yMin1, zMin1, xMax1, yMax1, zMax1);
        box2.Get(xMin2, yMin2, zMin2, xMax2, yMax2, zMax2);

        // XOY平面
        if (box1.IsZThin(box1.GetGap()) && box2.IsZThin(box2.GetGap())) {
            return (yMin1 < yMin2) && (xMin1 < xMin2) && (yMax1 > yMax2) && (xMax1 > xMax2);
        }
        // YOZ平面
        else if (box1.IsXThin(box1.GetGap()) && box2.IsXThin(box2.GetGap())) {
            return (yMin1 < yMin2) && (zMin1 < zMin2) && (yMax1 > yMax2) && (zMax1 > zMax2);
        }
        // XOZ平面
        else if (box1.IsYThin(box1.GetGap()) && box2.IsYThin(box2.GetGap())) {
            return (xMin1 < xMin2) && (zMin1 < zMin2) && (xMax1 > xMax2) && (zMax1 > zMax2);
        }
        // 普通平面
        else if (!box2.IsOpen()) {
            return (xMin1 < xMin1) && (yMin1 < yMin2) && (zMin1 < zMin2)
                && (xMax1 > xMax2) && (yMax1 > yMax2) && (zMax1 > zMax2);
        }
        else {
            throw "System Error!";
        }
    }
    /**
     * @brief  包围盒树结构类.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2024-09-29
     */
    class WireBoundTree {
    public:
        /// 构造函数
        WireBoundTree() = default;
        /// 析构函数
        ~WireBoundTree() = default;
        /**
         * @brief  返回根节点列表.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        QSet<WireBoundTreeNode*> roots() { return m_Roots; }
        /**
         * @brief  添加CWire.
         * @param  w CWire
         * @note   该函数负责把CWire放在树结构中合适的位置
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        void addWire(const CWire& w) {
            auto newNode = new WireBoundTreeNode(w);
            // 如果根节点为空，则添加到根节点列表
            if (m_Roots.isEmpty()) { m_Roots.insert(newNode); return; }

            bool isContain = false;

            const auto oldRoots = m_Roots;
            for (WireBoundTreeNode* node : oldRoots) {
                // 新节点包含node
                if (isOut(newNode->bnd(), node->bnd())) {
                    node->setParent(newNode);
                    m_Roots.remove(node);
                    m_Roots.insert(newNode);
                    isContain = true;
                }
                // node包含新节点
                else if (isOut(node->bnd(), newNode->bnd())) {
                    if (!node->hasChild()) { newNode->setParent(node); }
                    else {
                        addChildNode(newNode, node);
                    }
                    isContain = true;
                    return;
                }
            }
            // 并列关系
            if (!isContain) {
                m_Roots.insert(newNode);
            }
        }

    protected:
        /**
         * @brief  比较新节点与子节点的关系，并设置合适的位置.
         * @param  newNode 新节点
         * @param  toNode 子节点
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        void addChildNode(WireBoundTreeNode* newNode, WireBoundTreeNode* toNode) {
            if (newNode == nullptr || toNode == nullptr) return;

            bool isContain = false;
            for (auto& node : toNode->childs()) {
                if (isOut(node->bnd(), newNode->bnd())) {
                    addChildNode(newNode, node);
                    isContain = true;
                    return;
                }
                else if (isOut(newNode->bnd(), node->bnd())) {
                    node->setParent(newNode);
                    newNode->setParent(toNode);
                    isContain = true;
                }
            }
            // 并列关系
            if (!isContain) {
                newNode->setParent(toNode);
            }
        }


    private:
        /**
         * @brief  根节点列表（最外围的草绘并列存在多个，因此根节点需要使用列表）.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-09-29
         */
        QSet<WireBoundTreeNode*> m_Roots{};
    };
}

namespace OCC
{
    FITKOCCModelExtrudeCut::FITKOCCModelExtrudeCut() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelExtrudeCut::update()
    {
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceSurface.CmdId);
        if (cmd == nullptr) return false;

        gp_Vec dir(m_Direction[0], m_Direction[1], m_Direction[2]);
        auto mag = dir.Magnitude();
        if (mag <= Precision::Confusion()) {
            printLog(tr("The direction cannot be a zero vector!"), 3);
            return false;
        }

        TopoDS_Shape topoShape;

        try
        {
            if (m_ExtrudeType == 1)
            {
                dir = dir * m_Length / mag;

                // 草图。
                if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
                {
                    Interface::FITKAbsGeoSketch2D* sketch = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(cmd);
                    if (!sketch)
                    {
                        return false;
                    }

                    double pos3[3], nor3[3], up3[3];
                    sketch->getPlane(pos3, nor3, up3);
                    gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));

                    // 合并草图。
                    cmd->update();

                    FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
                    if (!occModel)
                    {
                        return false;
                    }

                    TopoDS_Shape* sketchShape = occModel->getShape();
                    if (sketchShape->IsNull())
                    {
                        return false;
                    }

                    QList<EncloseWire> wiresList;
                    QList<TopoDS_Face> faces;
                    FITKOCCComandCommon::MakeWires(*sketchShape, wiresList, pln, 1e-5);

                    // New version. 2025/10/24
                    //@{
                    QList<EncloseWire> wiresListFlat;
                    FITKOCCComandCommon::FlattenWires(wiresList, wiresListFlat);

                    TopoDS_Builder builder;
                    TopoDS_Compound compoundSolids;
                    builder.MakeCompound(compoundSolids);

                    for (EncloseWire & ew : wiresListFlat)
                    {
                        const TopoDS_Face & fOut = BRepBuilderAPI_MakeFace(ew.Wire);
                        if (fOut.IsNull())
                        {
                            continue;
                        }

                        BRepPrimAPI_MakePrism mp(fOut, dir);
                        if (!mp.IsDone())
                        {
                            continue;
                        }

                        const TopoDS_Shape & shapeOut = mp.Shape();

                        if (ew.Children.isEmpty())
                        {
                            builder.Add(compoundSolids, shapeOut);
                        }
                        else
                        {
                            TopoDS_Shape cutShape = shapeOut;
                            for (EncloseWire & ewHole : ew.Children)
                            {
                                const TopoDS_Face & fInner = BRepBuilderAPI_MakeFace(ewHole.Wire);
                                if (fInner.IsNull())
                                {
                                    continue;
                                }

                                BRepPrimAPI_MakePrism mpInner(fInner, dir);
                                if (!mpInner.IsDone())
                                {
                                    continue;
                                }

                                const TopoDS_Shape & newShape = BRepAlgoAPI_Cut(cutShape, mpInner);
                                if (!newShape.IsNull())
                                {
                                    cutShape = newShape;
                                }
                            }

                            builder.Add(compoundSolids, cutShape);
                        }
                    }
                    //@}

                    // Old version.
                    //@{
                    //FITKOCCComandCommon::MakeFaces(wiresList, faces);

                    //TopoDS_Builder builder;
                    //TopoDS_Compound compoundSolids;
                    //builder.MakeCompound(compoundSolids);

                    //for (const TopoDS_Face & face : faces)
                    //{
                    //    if (face.IsNull())
                    //    {
                    //        continue;
                    //    }

                    //    const TopoDS_Shape & shape = BRepPrimAPI_MakePrism(face, dir);
                    //    builder.Add(compoundSolids, shape);
                    //}
                    //@}

                    ShapeFix_Shape fix(compoundSolids);
                    fix.Perform();

                    topoShape = fix.Shape();
                }
                // 几何面。
                else
                {
                    auto shape = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface.VirtualTopoIndex);
                    if (shape == nullptr) return false;
                    topoShape = BRepPrimAPI_MakePrism(shape->getTopoShape(), dir);
                }
            }
            else if (m_ExtrudeType == 2)
            {
                // 获取目标面。
                if (m_UpToFace.isNull())
                {
                    return false;
                }

                Interface::FITKAbsGeoCommand* faceCmd = geoCmdList->getDataByID(m_UpToFace.CmdId);
                if (!faceCmd)
                {
                    return false;
                }

                FITKOCCTopoShape* occShape = faceCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_UpToFace.VirtualTopoIndex);
                if (!occShape)
                {
                    printLog(tr("Failed to get face from virtual topo."), 3);
                    return false;
                }

                const TopoDS_Face & upToFace = TopoDS::Face(occShape->getTopoShape());
                if (upToFace.IsNull())
                {
                    return false;
                }

                Handle(Geom_Surface) upToSurf = BRep_Tool::Surface(upToFace);
                if (upToSurf.IsNull())
                {
                    return false;
                }

                // 计算目标面与原始草图总包围盒，计算拉伸长度。
                Bnd_Box box;
                BRepBndLib::Add(upToFace, box, false);

                // 草图。
                if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
                {
                    Interface::FITKAbsGeoSketch2D* sketch = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(cmd);
                    if (!sketch)
                    {
                        return false;
                    }

                    double pos3[3], nor3[3], up3[3];
                    sketch->getPlane(pos3, nor3, up3);
                    gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));

                    // 合并草图。
                    cmd->update();

                    FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
                    if (!occModel)
                    {
                        return false;
                    }

                    TopoDS_Shape* sketchShape = occModel->getShape();
                    if (sketchShape->IsNull())
                    {
                        return false;
                    }

                    QList<EncloseWire> wiresList;
                    QList<TopoDS_Face> faces;
                    FITKOCCComandCommon::MakeWires(*sketchShape, wiresList, pln, 1e-5);

                    for (EncloseWire & ew : wiresList)
                    {
                        BRepBndLib::Add(ew.Wire, box, false);
                    }

                    // 使用包围盒对角线长度1.1倍距离进行拉伸。
                    double extrudeDist = sqrt(box.SquareExtent()) * 1.1;
                    dir = dir * extrudeDist / mag;

                    // 覆盖边。
                    BRepBuilderAPI_MakeFace mkFace;
                    mkFace.Init(upToSurf, false, Precision::Confusion());

                    // New version. 2025/10/24
                    //@{
                    QList<EncloseWire> wiresListFlat;
                    FITKOCCComandCommon::FlattenWires(wiresList, wiresListFlat);

                    TopoDS_Builder builder;
                    TopoDS_Compound compoundSolids;
                    builder.MakeCompound(compoundSolids);

                    for (EncloseWire & ew : wiresListFlat)
                    {
                        const TopoDS_Face & fOut = BRepBuilderAPI_MakeFace(ew.Wire);
                        if (fOut.IsNull())
                        {
                            continue;
                        }

                        BRepPrimAPI_MakePrism mp(fOut, dir);
                        if (!mp.IsDone())
                        {
                            continue;
                        }

                        TopoDS_Shape shape = mp.Shape();

                        if (!ew.Children.isEmpty())
                        {
                            TopoDS_Shape cutShape = shape;
                            for (EncloseWire & ewHole : ew.Children)
                            {
                                const TopoDS_Face & fInner = BRepBuilderAPI_MakeFace(ewHole.Wire);
                                if (fInner.IsNull())
                                {
                                    continue;
                                }

                                BRepPrimAPI_MakePrism mpInner(fInner, dir);
                                if (!mpInner.IsDone())
                                {
                                    continue;
                                }

                                const TopoDS_Shape & newShape = BRepAlgoAPI_Cut(cutShape, mpInner);
                                if (!newShape.IsNull())
                                {
                                    cutShape = newShape;
                                }
                            }

                            shape = cutShape;
                        }

                        // 获取面体交线。
                        BRepAlgoAPI_Section intSec(shape, upToFace);
                        intSec.Build();
                        if (!intSec.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & sec = intSec.Shape();
                        if (sec.IsNull())
                        {
                            return false;
                        }

                        // 重组交线。
                        QList<TopoDS_Wire> wiresSec;
                        FITKOCCComandCommon::MakeWires(sec, wiresSec, 1e-5);

                        BRepBuilderAPI_MakeFace mkFaceTar;
                        mkFaceTar.Init(upToSurf, false, Precision::Confusion());

                        for (const TopoDS_Wire & wireSec : wiresSec)
                        {
                            if (wireSec.IsNull())
                            {
                                continue;
                            }

                            mkFaceTar.Add(wireSec);
                        }

                        mkFaceTar.Build();

                        if (!mkFaceTar.IsDone())
                        {
                            return false;
                        }

                        // 反向拉伸目标面并求交集。
                        const TopoDS_Face & tarFace = mkFaceTar.Face();
                        const TopoDS_Shape & reverseShape = BRepPrimAPI_MakePrism(tarFace, -dir);

                        ShapeFix_Shape fixReverse(reverseShape);
                        fixReverse.Perform();

                        builder.Add(compoundSolids, BRepAlgoAPI_Common(shape, fixReverse.Shape()));
                    }
                    //@}

                    // Old version.
                    //@{
                    //TopoDS_Builder builder;
                    //TopoDS_Compound compoundSolids;
                    //builder.MakeCompound(compoundSolids);

                    //FITKOCCComandCommon::MakeFaces(wiresList, faces);
                    //for (const TopoDS_Face & face : faces)
                    //{
                    //    if (face.IsNull())
                    //    {
                    //        continue;
                    //    }

                    //    const TopoDS_Shape & shape = BRepPrimAPI_MakePrism(face, dir);

                    //    // 获取面体交线。
                    //    BRepAlgoAPI_Section intSec(shape, upToFace);
                    //    intSec.Build();
                    //    if (!intSec.IsDone())
                    //    {
                    //        return false;
                    //    }

                    //    const TopoDS_Shape & sec = intSec.Shape();
                    //    if (sec.IsNull())
                    //    {
                    //        return false;
                    //    }

                    //    // 重组交线。
                    //    QList<TopoDS_Wire> wiresSec;
                    //    FITKOCCComandCommon::MakeWires(sec, wiresSec, 1e-5);

                    //    BRepBuilderAPI_MakeFace mkFaceTar;
                    //    mkFaceTar.Init(upToSurf, false, Precision::Confusion());

                    //    for (const TopoDS_Wire & wireSec : wiresSec)
                    //    {
                    //        if (wireSec.IsNull())
                    //        {
                    //            continue;
                    //        }

                    //        mkFaceTar.Add(wireSec);
                    //    }

                    //    mkFaceTar.Build();

                    //    if (!mkFaceTar.IsDone())
                    //    {
                    //        return false;
                    //    }

                    //    // 反向拉伸目标面并求交集。
                    //    const TopoDS_Face & tarFace = mkFaceTar.Face();
                    //    const TopoDS_Shape & reverseShape = BRepPrimAPI_MakePrism(tarFace, -dir);

                    //    ShapeFix_Shape fixReverse(reverseShape);
                    //    fixReverse.Perform();

                    //    builder.Add(compoundSolids, BRepAlgoAPI_Common(shape, fixReverse.Shape()));
                    //}
                    //@}

                    ShapeFix_Shape fix(compoundSolids);
                    fix.Perform();

                    topoShape = fix.Shape();
                }
                // 几何面。
                else
                {
                    auto shape = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface.VirtualTopoIndex);
                    if (shape == nullptr) return false;
                    const TopoDS_Shape & faceShape = shape->getTopoShape();

                    BRepBndLib::Add(faceShape, box, false);

                    // 使用包围盒对角线长度1.1倍距离进行拉伸。
                    double extrudeDist = sqrt(box.SquareExtent()) * 1.1;
                    dir = dir * extrudeDist / mag;

                    const TopoDS_Shape & extrudeShape = BRepPrimAPI_MakePrism(faceShape, dir);

                    // 获取面体交线。
                    BRepAlgoAPI_Section intSec(extrudeShape, upToFace);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & sec = intSec.Shape();
                    if (sec.IsNull())
                    {
                        return false;
                    }

                    // 重组交线。
                    QList<TopoDS_Wire> wiresSec;
                    FITKOCCComandCommon::MakeWires(sec, wiresSec, 1e-5);

                    // 覆盖边。
                    BRepBuilderAPI_MakeFace mkFace;
                    mkFace.Init(upToSurf, false, Precision::Confusion());

                    for (const TopoDS_Wire & wire : wiresSec)
                    {
                        if (wire.IsNull())
                        {
                            continue;
                        }

                        mkFace.Add(wire);
                    }

                    mkFace.Build();

                    if (!mkFace.IsDone())
                    {
                        return false;
                    }

                    // 反向拉伸目标面并求交集。
                    const TopoDS_Face & tarFace = mkFace.Face();
                    const TopoDS_Shape & reverseShape = BRepPrimAPI_MakePrism(tarFace, -dir);

                    ShapeFix_Shape fixReverse(reverseShape);
                    fixReverse.Perform();

                    topoShape = BRepAlgoAPI_Common(extrudeShape, fixReverse.Shape());
                }
            }
        }
        catch (...)
        {
            return false;
        }

        if (topoShape.IsNull())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        try
        {
            if (part)
            {
                TopoDS_Shape* partShape{ nullptr };
                FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
                if (!partAgent)
                {
                    return false;
                }

                partShape = partAgent->getShape();

                QList<TopoDS_Shape> partSubShapes, partSubShapes2D, partSubShapes3D;
                FITKOCCComandCommon::FlattenCompShape2D3D(*partShape, partSubShapes, partSubShapes2D, partSubShapes3D);

                // 合并布尔结果。
                BRep_Builder builder;
                TopoDS_Compound compound, compoundResult;
                builder.MakeCompound(compound);
                builder.MakeCompound(compoundResult);

                int nSolids = 0;
                for (const TopoDS_Shape & partSubShape : partSubShapes3D)
                {
                    builder.Add(compound, partSubShape);
                    nSolids++;
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes2D)
                {
                    BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & sec = intSec.Shape();
                    if (FITKOCCExtendTool::IsEmptyShape(sec))
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Cut faceCutter(partSubShape, sec);
                        if (!faceCutter.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & newShape = faceCutter.Shape();
                        if (newShape.IsNull())
                        {
                            return false;
                        }

                        builder.Add(compoundResult, newShape);
                    }
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes)
                {
                    // 处理一维模型与壳面相交。
                    if (partSubShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX)
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                        intSecLine.Build();
                        if (!intSecLine.IsDone())
                        {
                            return false;
                        }

                        BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                        intSec.Build();
                        if (!intSec.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & sec = intSec.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            builder.Add(compoundResult, partSubShape);
                        }
                        else
                        {
                            BRepAlgoAPI_Cut edgeCutter(partSubShape, sec);
                            if (!edgeCutter.IsDone())
                            {
                                return false;
                            }

                            const TopoDS_Shape & newEdge = edgeCutter.Shape();
                            if (newEdge.IsNull())
                            {
                                return false;
                            }

                            builder.Add(compoundResult, newEdge);
                        }
                    }
                }

                try
                {
                    // 没有体则直接进行拼接组合对象。
                    if (nSolids == 0)
                    {
                        builder.Add(compoundResult, topoShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Cut cut = BRepAlgoAPI_Cut(compound, topoShape);
                        if (!cut.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & solidsShape = cut.Shape();
                        builder.Add(compoundResult, solidsShape);
                    }

                    part->updatePartShape(compoundResult, false);
                    _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
                }
                catch (...)
                {
                    return false;
                }
            }
            else
            {
                _occShapeAgent->updateShape(topoShape);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to make solid!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelRevolCut::FITKOCCModelRevolCut() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelRevolCut::update()
    {
        gp_Pnt start(m_RotateAxisPoint1[0], m_RotateAxisPoint1[1], m_RotateAxisPoint1[2]);
        gp_Pnt end(m_RotateAxisPoint2[0], m_RotateAxisPoint2[1], m_RotateAxisPoint2[2]);
        if (start.Distance(end) <= Precision::Confusion()) {
            printLog(tr("The two ends of the axis of rotation can not coincide!"), 3);
            return false;
        }

        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto cmd = geoCmdList->getDataByID(m_SourceSurface.CmdId);
        if (cmd == nullptr) return false;

        TopoDS_Shape topoShape;

        //m_OriginId;
        gp_Ax1 ax1(start, gp_Dir(end.XYZ() - start.XYZ()));

        // 草图。
        if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
        {
            Interface::FITKAbsGeoSketch2D* sketch = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(cmd);
            if (!sketch)
            {
                return false;
            }

            double pos3[3], nor3[3], up3[3];
            sketch->getPlane(pos3, nor3, up3);
            gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));

            // 合并草图。
            cmd->update();

            FITKAbstractOCCModel* occModel = cmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            TopoDS_Shape* sketchShape = occModel->getShape();
            if (sketchShape->IsNull())
            {
                return false;
            }

            QList<EncloseWire> wiresList;
            QList<TopoDS_Face> faces;
            FITKOCCComandCommon::MakeWires(*sketchShape, wiresList, pln, 1e-5);

            // New version. 2025/10/24
            //@{
            QList<EncloseWire> wiresListFlat;
            FITKOCCComandCommon::FlattenWires(wiresList, wiresListFlat);

            TopoDS_Builder builder;
            TopoDS_Compound compoundSolids;
            builder.MakeCompound(compoundSolids);

            for (EncloseWire & ew : wiresListFlat)
            {
                const TopoDS_Face & fOut = BRepBuilderAPI_MakeFace(ew.Wire);
                if (fOut.IsNull())
                {
                    continue;
                }

                BRepPrimAPI_MakeRevol mr(fOut, ax1, m_Angle * M_PI / 180);
                if (!mr.IsDone())
                {
                    continue;
                }

                const TopoDS_Shape & shapeOut = mr.Shape();

                if (ew.Children.isEmpty())
                {
                    builder.Add(compoundSolids, shapeOut);
                }
                else
                {
                    TopoDS_Shape cutShape = shapeOut;
                    for (EncloseWire & ewHole : ew.Children)
                    {
                        const TopoDS_Face & fInner = BRepBuilderAPI_MakeFace(ewHole.Wire);
                        if (fInner.IsNull())
                        {
                            continue;
                        }

                        BRepPrimAPI_MakeRevol mrInner(fInner, ax1, m_Angle * M_PI / 180);
                        if (!mrInner.IsDone())
                        {
                            continue;
                        }

                        const TopoDS_Shape & newShape = BRepAlgoAPI_Cut(cutShape, mrInner);
                        if (!newShape.IsNull())
                        {
                            cutShape = newShape;
                        }
                    }

                    builder.Add(compoundSolids, cutShape);
                }
            }
            //@}

            // Old version.
            //@{
            //FITKOCCComandCommon::MakeFaces(wiresList, faces);

            //TopoDS_Builder builder;
            //TopoDS_Compound compoundSolids;
            //builder.MakeCompound(compoundSolids);

            //for (const TopoDS_Face & face : faces)
            //{
            //    if (face.IsNull())
            //    {
            //        continue;
            //    }

            //    try
            //    {
            //        const TopoDS_Shape & shape = BRepPrimAPI_MakeRevol(face, ax1, m_Angle * M_PI / 180);
            //        builder.Add(compoundSolids, shape);
            //    }
            //    catch (...)
            //    {
            //        return false;
            //    }
            //}
            //@}

            ShapeFix_Shape fix(compoundSolids);
            fix.Perform();

            topoShape = fix.Shape();
            // topoShape = BRepBuilderAPI_Copy(*sketchShape);
        }
        // 几何面。
        else
        {
            auto shape = cmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_SourceSurface.VirtualTopoIndex);
            if (shape == nullptr) return false;
            const TopoDS_Shape & faceShape = shape->getTopoShape();

            try
            {
                topoShape = BRepPrimAPI_MakeRevol(faceShape, ax1, m_Angle * M_PI / 180);
            }
            catch (...)
            {
                return false;
            }
        }

        if (topoShape.IsNull())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        try
        {
            if (part)
            {
                TopoDS_Shape* partShape{ nullptr };
                FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
                if (!partAgent)
                {
                    return false;
                }

                partShape = partAgent->getShape();

                QList<TopoDS_Shape> partSubShapes, partSubShapes2D, partSubShapes3D;
                FITKOCCComandCommon::FlattenCompShape2D3D(*partShape, partSubShapes, partSubShapes2D, partSubShapes3D);

                // 合并布尔结果。
                BRep_Builder builder;
                TopoDS_Compound compound, compoundResult;
                builder.MakeCompound(compound);
                builder.MakeCompound(compoundResult);

                int nSolids = 0;
                for (const TopoDS_Shape & partSubShape : partSubShapes3D)
                {
                    builder.Add(compound, partSubShape);
                    nSolids++;
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes2D)
                {
                    BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & sec = intSec.Shape();
                    if (FITKOCCExtendTool::IsEmptyShape(sec))
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Cut faceCutter(partSubShape, sec);
                        if (!faceCutter.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & newShape = faceCutter.Shape();
                        if (newShape.IsNull())
                        {
                            return false;
                        }

                        builder.Add(compoundResult, newShape);
                    }
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes)
                {
                    // 处理一维模型与壳面相交。
                    if (partSubShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX)
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                        intSecLine.Build();
                        if (!intSecLine.IsDone())
                        {
                            return false;
                        }

                        BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                        intSec.Build();
                        if (!intSec.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & sec = intSec.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            builder.Add(compoundResult, partSubShape);
                        }
                        else
                        {
                            BRepAlgoAPI_Cut edgeCutter(partSubShape, sec);
                            if (!edgeCutter.IsDone())
                            {
                                return false;
                            }

                            const TopoDS_Shape & newEdge = edgeCutter.Shape();
                            if (newEdge.IsNull())
                            {
                                return false;
                            }

                            builder.Add(compoundResult, newEdge);
                        }
                    }
                }

                try
                {
                    // 没有体则直接进行拼接组合对象。
                    if (nSolids == 0)
                    {
                        builder.Add(compoundResult, topoShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Cut cut = BRepAlgoAPI_Cut(compound, topoShape);
                        if (!cut.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & solidsShape = cut.Shape();
                        builder.Add(compoundResult, solidsShape);
                    }

                    part->updatePartShape(compoundResult, false);
                    _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
                }
                catch (...)
                {
                    return false;
                }
            }
            else
            {
                _occShapeAgent->updateShape(topoShape);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to make solid!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelSweepCut::FITKOCCModelSweepCut() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    bool FITKOCCModelSweepCut::update()
    {
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        auto profileCmd = geoCmdList->getDataByID(m_Profile.CmdId);
        auto curveCmd = geoCmdList->getDataByID(m_Curves.at(0).CmdId);
        if (profileCmd == nullptr || curveCmd == nullptr) return false;

        //Interface::FITKGeoEnum::VTopoShapeType curveType = curveCmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D ? Interface::FITKGeoEnum::VTopoShapeType::VSAssembly : Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
        //auto curveShape = curveCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(curveType, m_Curves.at(0).VirtualTopoIndex);
        //if (curveShape == nullptr) return false;

        //auto curveTopoShape = curveShape->getTopoShape();

        TopoDS_Shape curveTopoShape;

        // 草图。
        TopoDS_Builder builder;
        QList<TopoDS_Wire> wires;
        if (curveCmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
        {
            // 合并草图。
            curveCmd->update();

            FITKAbstractOCCModel* occModel = curveCmd->getTShapeAgent<FITKAbstractOCCModel>();
            if (!occModel)
            {
                return false;
            }

            TopoDS_Shape* sketchShape = occModel->getShape();
            if (sketchShape->IsNull())
            {
                return false;
            }

            FITKOCCComandCommon::MakeWires(*sketchShape, wires, 1e-5);
        }
        // 几何边。
        else
        {
            TopoDS_Compound compoundEdges;
            builder.MakeCompound(compoundEdges);

            for (const Interface::VirtualShape & shape : m_Curves)
            {
                auto occShape = curveCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSEdge, shape.VirtualTopoIndex);
                if (!occShape)
                {
                    return false;
                }

                builder.Add(compoundEdges, occShape->getTopoShape());
            }

            FITKOCCComandCommon::MakeWires(compoundEdges, wires, 1e-5);
        }

        if (wires.count() != 1)
        {
            return false;
        }

        curveTopoShape = wires.first();

        TopoDS_Wire sweepWire;

        try
        {
            auto type = curveTopoShape.ShapeType();
            if (type == TopAbs_ShapeEnum::TopAbs_WIRE)
            {
                sweepWire = TopoDS::Wire(curveTopoShape);
            }
            else if (type == TopAbs_ShapeEnum::TopAbs_EDGE) {
                sweepWire = BRepBuilderAPI_MakeWire(TopoDS::Edge(curveTopoShape));
            }
            else if (type == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
                TopoDS_Compound compound = TopoDS::Compound(curveTopoShape);
                TopExp_Explorer exp;
                BRepBuilderAPI_MakeWire makeWire;
                for (exp.Init(compound, TopAbs_EDGE); exp.More(); exp.Next())
                {
                    TopoDS_Edge edge = TopoDS::Edge(exp.Current());
                    if (edge.IsNull()) continue;
                    makeWire.Add(edge);
                }
                makeWire.Build();
                if (!makeWire.IsDone()) return false;
                sweepWire = TopoDS::Wire(makeWire.Shape());
            }
            else return false;
        }
        catch (...)
        {
            return false;
        }

        if (m_flipPath)
        {
            sweepWire.Reverse();
        }

        TopoDS_Shape topoShape;

        try
        {
            // 草图。
            if (profileCmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
            {
                Interface::FITKAbsGeoSketch2D* sketch = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(profileCmd);
                if (!sketch)
                {
                    return false;
                }

                double pos3[3], nor3[3], up3[3];
                sketch->getPlane(pos3, nor3, up3);
                gp_Pln pln(gp_Pnt(pos3[0], pos3[1], pos3[2]), gp_Dir(nor3[0], nor3[1], nor3[2]));

                // 合并草图。
                profileCmd->update();

                FITKAbstractOCCModel* occModel = profileCmd->getTShapeAgent<FITKAbstractOCCModel>();
                if (!occModel)
                {
                    return false;
                }

                TopoDS_Shape* sketchShape = occModel->getShape();
                if (sketchShape->IsNull())
                {
                    return false;
                }

                QList<EncloseWire> wiresList;
                FITKOCCComandCommon::MakeWires(*sketchShape, wiresList, pln, 1e-5);

                QList<EncloseWire> wiresListFlat;
                FITKOCCComandCommon::FlattenWires(wiresList, wiresListFlat);

                TopoDS_Builder builder;
                TopoDS_Compound compoundSolids;
                builder.MakeCompound(compoundSolids);

                for (EncloseWire & ew : wiresListFlat)
                {
                    BRepOffsetAPI_MakePipeShell mp(sweepWire);
                    mp.Add(ew.Wire);
                    mp.SetTransitionMode(BRepBuilderAPI_TransitionMode::BRepBuilderAPI_RightCorner);
                    mp.SetDiscreteMode();
                    mp.Build();
                    if (!mp.IsDone())
                    {
                        return false;
                    }

                    bool flag = mp.MakeSolid();
                    if (!flag)
                    {
                        return false;
                    }

                    if (ew.Children.isEmpty())
                    {
                        const TopoDS_Shape & shape = mp.Shape();
                        builder.Add(compoundSolids, shape);
                    }
                    else
                    {
                        TopoDS_Shape cutShape = mp;
                        for (EncloseWire & ewHole : ew.Children)
                        {
                            const TopoDS_Wire & w2 = ewHole.Wire;

                            BRepOffsetAPI_MakePipeShell mp2(sweepWire);
                            mp2.Add(w2);
                            mp2.SetTransitionMode(BRepBuilderAPI_TransitionMode::BRepBuilderAPI_RightCorner);
                            mp2.SetDiscreteMode();
                            mp2.Build();
                            if (!mp2.IsDone())
                            {
                                return false;
                            }

                            bool flag = mp2.MakeSolid();
                            if (!flag)
                            {
                                return false;
                            }

                            const TopoDS_Shape & newShape = BRepAlgoAPI_Cut(cutShape, mp2);
                            if (!newShape.IsNull())
                            {
                                cutShape = newShape;
                            }
                        }

                        builder.Add(compoundSolids, cutShape);
                    }
                }

                ShapeFix_Shape fix(compoundSolids);
                fix.Perform();

                topoShape = fix.Shape();
                // topoShape = BRepBuilderAPI_Copy(*sketchShape);
            }
            // 几何面。
            else
            {
                auto shape = profileCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSFace, m_Profile.VirtualTopoIndex);
                if (shape == nullptr) return false;
                const TopoDS_Shape & faceShape = shape->getTopoShape();

                //BRepOffsetAPI_MakePipeShell mp(sweepWire);
                //mp.SetTransitionMode(BRepBuilderAPI_TransitionMode::BRepBuilderAPI_RightCorner);
                //mp.SetDiscreteMode();
                //mp.Add(faceShape);
                //mp.Build();
                //bool flag = mp.MakeSolid();
                //if (!flag)
                //{
                //    return false;
                //}

                //topoShape = mp.Shape();

                // topoShape = BRepOffsetAPI_MakePipe(sweepWire, faceShape);

                gp_Pnt pos;
                TopExp_Explorer iterE;
                for (iterE.Init(sweepWire, TopAbs_ShapeEnum::TopAbs_EDGE); iterE.More(); iterE.Next())
                {
                    const TopoDS_Edge & ie = TopoDS::Edge(iterE.Value());
                    if (ie.IsNull())
                    {
                        continue;
                    }

                    Standard_Real first, last;
                    Handle(Geom_Curve) curve = BRep_Tool::Curve(ie, first, last);

                    if (ie.Orientation() == TopAbs_Orientation::TopAbs_FORWARD)
                    {
                        curve->D0(first, pos);
                    }
                    else
                    {
                        curve->D0(last, pos);
                    }
                }

                Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(faceShape));

                GeomAPI_ProjectPointOnSurf projSurf;
                projSurf.Init(pos, surface);

                if (projSurf.NbPoints() == 0)
                {
                    return false;
                }

                gp_Pnt ctOnSurf = projSurf.Point(1);

                gp_Trsf gpTrans;
                gpTrans.SetTranslationPart(gp_Vec(ctOnSurf.XYZ() - pos.XYZ()));

                BRepBuilderAPI_Transform trans(sweepWire, gpTrans, true);
                sweepWire = TopoDS::Wire(trans.Shape());
                topoShape = BRepOffsetAPI_MakePipe(sweepWire, faceShape);
            }
        }
        catch (...)
        {
            return false;
        }

        if (topoShape.IsNull())
        {
            return false;
        }

        FITKOCCModelPart* part = _occShapeAgent->getPart();

        try
        {
            if (part)
            {
                TopoDS_Shape* partShape{ nullptr };
                FITKAbstractOCCModel* partAgent = part->getTShapeAgent<FITKAbstractOCCModel>();
                if (!partAgent)
                {
                    return false;
                }

                partShape = partAgent->getShape();

                QList<TopoDS_Shape> partSubShapes, partSubShapes2D, partSubShapes3D;
                FITKOCCComandCommon::FlattenCompShape2D3D(*partShape, partSubShapes, partSubShapes2D, partSubShapes3D);

                // 合并布尔结果。
                BRep_Builder builder;
                TopoDS_Compound compound, compoundResult;
                builder.MakeCompound(compound);
                builder.MakeCompound(compoundResult);

                int nSolids = 0;
                for (const TopoDS_Shape & partSubShape : partSubShapes3D)
                {
                    builder.Add(compound, partSubShape);
                    nSolids++;
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes2D)
                {
                    BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                    intSec.Build();
                    if (!intSec.IsDone())
                    {
                        return false;
                    }

                    const TopoDS_Shape & sec = intSec.Shape();
                    if (FITKOCCExtendTool::IsEmptyShape(sec))
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Cut faceCutter(partSubShape, sec);
                        if (!faceCutter.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & newShape = faceCutter.Shape();
                        if (newShape.IsNull())
                        {
                            return false;
                        }

                        builder.Add(compoundResult, newShape);
                    }
                }

                for (const TopoDS_Shape & partSubShape : partSubShapes)
                {
                    // 处理一维模型与壳面相交。
                    if (partSubShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_VERTEX)
                    {
                        builder.Add(compoundResult, partSubShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Section intSecLine(topoShape, partSubShape);
                        intSecLine.Build();
                        if (!intSecLine.IsDone())
                        {
                            return false;
                        }

                        BRepAlgoAPI_Common intSec(topoShape, partSubShape);
                        intSec.Build();
                        if (!intSec.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & sec = intSec.Shape();
                        if (FITKOCCExtendTool::IsEmptyShape(sec))
                        {
                            builder.Add(compoundResult, partSubShape);
                        }
                        else
                        {
                            BRepAlgoAPI_Cut edgeCutter(partSubShape, sec);
                            if (!edgeCutter.IsDone())
                            {
                                return false;
                            }

                            const TopoDS_Shape & newEdge = edgeCutter.Shape();
                            if (newEdge.IsNull())
                            {
                                return false;
                            }

                            builder.Add(compoundResult, newEdge);
                        }
                    }
                }

                try
                {
                    // 没有体则直接进行拼接组合对象。
                    if (nSolids == 0)
                    {
                        builder.Add(compoundResult, topoShape);
                    }
                    else
                    {
                        BRepAlgoAPI_Cut cut = BRepAlgoAPI_Cut(compound, topoShape);
                        if (!cut.IsDone())
                        {
                            return false;
                        }

                        const TopoDS_Shape & solidsShape = cut.Shape();
                        builder.Add(compoundResult, solidsShape);
                    }

                    part->updatePartShape(compoundResult, false);
                    _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
                }
                catch (...)
                {
                    return false;
                }
            }
            else
            {
                _occShapeAgent->updateShape(topoShape);
            }
        }
        catch (...)
        {
            printLog(tr("Failed to make solid!"), 3);
            return false;
        }

        return true;
    }

    FITKOCCModelMultiSectionCut::FITKOCCModelMultiSectionCut() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelMultiSectionCut::update()
    {
        return false;
    }
}
