/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKShellFeatureEdges.h
 * @brief       The feature edges and boundary edges filter.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-05-16
 *********************************************************************/

#ifndef FITKSHELLFEATUREEDGES_H
#define FITKSHELLFEATUREEDGES_H

#include "vtkPolyDataAlgorithm.h"

#include "FITKVTKAlgorithmAPI.h"

#include <map>
#include <vector>
#include <array>

#include <QMap>
#include <QVector>

class vtkMergePoints;

/**
 * @brief       Override vtkFeatureEdges.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-03-25
 */
class FITKVTKALGORITHMAPI FITKShellFeatureEdges : public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro(FITKShellFeatureEdges, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent) override;

    /**
     * Construct object with feature angle = 30; all types of edges extracted
     * and colored.
     */
    static FITKShellFeatureEdges* New();

    //@{
    /**
     * Turn on/off the extraction of 3D cells.
     */
    vtkSetMacro(KeepOnly2D, vtkTypeBool);
    vtkGetMacro(KeepOnly2D, vtkTypeBool);
    vtkBooleanMacro(KeepOnly2D, vtkTypeBool);
    //@}

    //@{
    /**
     * Turn on/off the extraction of boundary edges.
     */
    vtkSetMacro(BoundaryEdges, vtkTypeBool);
    vtkGetMacro(BoundaryEdges, vtkTypeBool);
    vtkBooleanMacro(BoundaryEdges, vtkTypeBool);
    //@}

    //@{
    /**
     * Turn on/off the extraction of feature edges.
     */
    vtkSetMacro(FeatureEdges, vtkTypeBool);
    vtkGetMacro(FeatureEdges, vtkTypeBool);
    vtkBooleanMacro(FeatureEdges, vtkTypeBool);
    //@}

    //@{
    /**
     * Specify the feature angle for extracting feature edges.
     */
    vtkSetClampMacro(FeatureAngle, double, 0.0, 180.0);
    vtkGetMacro(FeatureAngle, double);
    //@}

    //@{
    /**
     * Turn on/off the extraction of non-manifold edges.
     */
    vtkSetMacro(NonManifoldEdges, vtkTypeBool);
    vtkGetMacro(NonManifoldEdges, vtkTypeBool);
    vtkBooleanMacro(NonManifoldEdges, vtkTypeBool);
    //@}

    //@{
    /**
     * Turn on/off the extraction of manifold edges.
     */
    vtkSetMacro(ManifoldEdges, vtkTypeBool);
    vtkGetMacro(ManifoldEdges, vtkTypeBool);
    vtkBooleanMacro(ManifoldEdges, vtkTypeBool);
    //@}

    //@{
    /**
     * Turn on/off the coloring of edges by type.
     */
    vtkSetMacro(Coloring, vtkTypeBool);
    vtkGetMacro(Coloring, vtkTypeBool);
    vtkBooleanMacro(Coloring, vtkTypeBool);

    /**
     * 设置是否
     */
    vtkSetMacro(PassThroughIds, vtkTypeBool);
    vtkGetMacro(PassThroughIds, vtkTypeBool);
    vtkBooleanMacro(PassThroughIds, vtkTypeBool);
    //@}

    /**
     * Create default locator. Used to create one when none is specified.
     */
    void CreateDefaultLocator();

    /**
     * Return MTime also considering the locator.
     */
    vtkMTimeType GetMTime() override;

    //@{
    /**
     * Set/get the desired precision for the output types. See the documentation
     * for the vtkAlgorithm::DesiredOutputPrecision enum for an explanation of
     * the available precision settings.
     */
    vtkSetMacro(OutputPointsPrecision, int);
    vtkGetMacro(OutputPointsPrecision, int);
    //@}
    /**
     * @brief 通过点号，获取线单元
     * @return 线单元编号
     * @param ptID 点编号
     * @author xiewenxi (xwx14@qq.com)
     * @date 2023-09-14
     */
    const std::vector< int > getLinesByPt(int ptID) const;
    /**
     * @brief 由特征边单元号获得其表面单元
     * @return 表面单元id
     * @param 特征边单元号
     * @author xiewenxi (xwx14@qq.com)
     * @date 2023-10-30
     */
    const std::vector< int > getEdgeCells(int iEdgeCellID, bool* b) const;
    //const std::map< int, std::vector< int > >& getEdgeCellMap( ) const;
    /**
     * @brief 由表面单元获取特征线单元号
     * @return 特征线单元号
     * @param 表面单元号
     * @author xiewenxi (xwx14@qq.com)
     * @date 2023-10-31
     */
    const QVector< int > getSurCellEdge(int iCell, bool* b = nullptr) const;

    int getSurfacePointIDByEdgePointID(int edgePointID);
    int getEdgePointIDBySurfacePointID(int surfPointID);

    ///**
    // * @brief       根据线单元索引获取原始表面中该线的索引。
    // * @param[in]   edgeCellId：线单元索引
    // * @param[in]   surfaceCellId：原始表面单元索引[引用]
    // * @return      表面单元中该线的索引
    // * @author      ChengHaotian (yeguangbaozi@foxmail.com)
    // * @date        2024-04-23
    // */
    //int getEdgeIndex(int edgeCellId, int& surfaceCellId);

protected:
    FITKShellFeatureEdges();
    ~FITKShellFeatureEdges() override;

    // Usual data generation method
    int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
    // 特征角度，edge两侧的面单元大于此角度的，会被定义为特征edge
    double                      FeatureAngle;
    vtkTypeBool                 KeepOnly2D;
    vtkTypeBool                 BoundaryEdges;
    vtkTypeBool                 FeatureEdges;
    vtkTypeBool                 NonManifoldEdges;
    vtkTypeBool                 ManifoldEdges;
    vtkTypeBool                 Coloring;
    QMap< int, QVector< int > > _surEdgeIDMap;

    //// 线单元与原始表面单元与索引映射。
    //// Key为线单元索引
    //// Value 1 为线单元所属原始表面单元索引
    //// Value 2 为线单元在原始表面单元内索引（第几条边）
    //QHash<int, QPair<int, int>> m_edgeSurfaceHash;

    /// <记录特征节点ID与面节点ID映射
    QMap< int, int > _edgePtSurfPtMap;
    QMap< int, int > _surfPtEdgePtMap;

    // 是否需要记录原id,包括单元id与节点id
    vtkTypeBool                 PassThroughIds;
    int                         OutputPointsPrecision;
    vtkMergePoints* Locator;
    // 用于记录每个点对应的线（key为输入polyData的点id，value为输出polyData的线单元编号）
    std::map< int, std::vector< int > > _ptLines;
    std::vector< int >                  _nullVec = {};
    std::map< int, std::vector< int > > _edgeSurCell = {};

private:
    FITKShellFeatureEdges(const FITKShellFeatureEdges&) = delete;
    void operator=(const FITKShellFeatureEdges&) = delete;
};

#endif // FITKSHELLFEATUREEDGES_H
