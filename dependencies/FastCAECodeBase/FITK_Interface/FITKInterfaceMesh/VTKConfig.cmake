# VTKConfig.cmake
include_guard(GLOBAL)

# 首先定义 VTK_ROOT，在多配置分支也需要这个变量
if(WIN32)
    set(VTK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../Tools/Win64/VTK942")
    set(VTK_INCLUDE_DIR "${VTK_ROOT}/include/vtk-9.4")
    
    # Windows VTK 库基础名称列表
    set(VTK_LIB_BASE_NAMES
        vtkcgns-9.4
        vtkChartsCore-9.4
        vtkCommonColor-9.4
        vtkCommonComputationalGeometry-9.4
        vtkCommonCore-9.4
        vtkCommonDataModel-9.4
        vtkCommonExecutionModel-9.4
        vtkCommonMath-9.4
        vtkCommonMisc-9.4
        vtkCommonSystem-9.4
        vtkCommonTransforms-9.4
        vtkDICOMParser-9.4
        vtkDomainsChemistry-9.4
        vtkDomainsChemistryOpenGL2-9.4
        vtkDomainsParallelChemistry-9.4
        vtkdoubleconversion-9.4
        vtkexodusII-9.4
        vtkexpat-9.4
        vtkFiltersAMR-9.4
        vtkFiltersCellGrid-9.4
        vtkFiltersCore-9.4
        vtkFiltersExtraction-9.4
        vtkFiltersFlowPaths-9.4
        vtkFiltersGeneral-9.4
        vtkFiltersGeneric-9.4
        vtkFiltersGeometry-9.4
        vtkFiltersGeometryPreview-9.4
        vtkFiltersHybrid-9.4
        vtkFiltersHyperTree-9.4
        vtkFiltersImaging-9.4
        vtkFiltersModeling-9.4
        vtkFiltersParallel-9.4
        vtkFiltersParallelGeometry-9.4
        vtkFiltersParallelImaging-9.4
        vtkFiltersParallelMPI-9.4
        vtkFiltersParallelVerdict-9.4
        vtkFiltersPoints-9.4
        vtkFiltersProgrammable-9.4
        vtkFiltersReduction-9.4
        vtkFiltersSelection-9.4
        vtkFiltersSMP-9.4
        vtkFiltersSources-9.4
        vtkFiltersStatistics-9.4
        vtkFiltersTemporal-9.4
        vtkFiltersTensor-9.4
        vtkFiltersTexture-9.4
        vtkFiltersTopology-9.4
        vtkFiltersVerdict-9.4
        vtkfmt-9.4
        vtkfreetype-9.4
        vtkGeovisCore-9.4
        vtkgl2ps-9.4
        vtkglad-9.4
        vtkGUISupportQt-9.4
        vtkGUISupportQtQuick-9.4
        vtkGUISupportQtSQL-9.4
        vtkhdf5-9.4
        vtkhdf5_hl-9.4
        vtkImagingColor-9.4
        vtkImagingCore-9.4
        vtkImagingFourier-9.4
        vtkImagingGeneral-9.4
        vtkImagingHybrid-9.4
        vtkImagingMath-9.4
        vtkImagingMorphological-9.4
        vtkImagingSources-9.4
        vtkImagingStatistics-9.4
        vtkImagingStencil-9.4
        vtkInfovisCore-9.4
        vtkInfovisLayout-9.4
        vtkInteractionImage-9.4
        vtkInteractionStyle-9.4
        vtkInteractionWidgets-9.4
        vtkIOAMR-9.4
        vtkIOAsynchronous-9.4
        vtkIOCellGrid-9.4
        vtkIOCesium3DTiles-9.4
        vtkIOCGNSReader-9.4
        vtkIOChemistry-9.4
        vtkIOCityGML-9.4
        vtkIOCONVERGECFD-9.4
        vtkIOCore-9.4
        vtkIOEngys-9.4
        vtkIOEnSight-9.4
        vtkIOERF-9.4
        vtkIOExodus-9.4
        vtkIOExport-9.4
        vtkIOExportGL2PS-9.4
        vtkIOExportPDF-9.4
        vtkIOFDS-9.4
        vtkIOFLUENTCFF-9.4
        vtkIOGeometry-9.4
        vtkIOHDF-9.4
        vtkIOImage-9.4
        vtkIOImport-9.4
        vtkIOInfovis-9.4
        vtkIOIOSS-9.4
        vtkIOLegacy-9.4
        vtkIOLSDyna-9.4
        vtkIOMINC-9.4
        vtkIOMotionFX-9.4
        vtkIOMovie-9.4
        vtkIOMPIImage-9.4
        vtkIONetCDF-9.4
        vtkIOOggTheora-9.4
        vtkIOParallel-9.4
        vtkIOParallelNetCDF-9.4
        vtkIOParallelXML-9.4
        vtkIOPLY-9.4
        vtkIOSegY-9.4
        vtkIOSQL-9.4
        vtkioss-9.4
        vtkIOTecplotTable-9.4
        vtkIOVeraOut-9.4
        vtkIOVideo-9.4
        vtkIOXML-9.4
        vtkIOXMLParser-9.4
        vtkjpeg-9.4
        vtkjsoncpp-9.4
        vtkkissfft-9.4
        vtklibharu-9.4
        vtklibproj-9.4
        vtklibxml2-9.4
        vtkloguru-9.4
        vtklz4-9.4
        vtklzma-9.4
        vtkmetaio-9.4
        vtknetcdf-9.4
        vtkogg-9.4
        vtkParallelCore-9.4
        vtkParallelDIY-9.4
        vtkParallelMPI-9.4
        vtkpng-9.4
        vtkpugixml-9.4
        vtkRenderingAnnotation-9.4
        vtkRenderingCellGrid-9.4
        vtkRenderingContext2D-9.4
        vtkRenderingContextOpenGL2-9.4
        vtkRenderingCore-9.4
        vtkRenderingFreeType-9.4
        vtkRenderingGL2PSOpenGL2-9.4
        vtkRenderingHyperTreeGrid-9.4
        vtkRenderingImage-9.4
        vtkRenderingLabel-9.4
        vtkRenderingLICOpenGL2-9.4
        vtkRenderingLOD-9.4
        vtkRenderingOpenGL2-9.4
        vtkRenderingQt-9.4
        vtkRenderingSceneGraph-9.4
        vtkRenderingUI-9.4
        vtkRenderingVolume-9.4
        vtkRenderingVolumeOpenGL2-9.4
        vtkRenderingVtkJS-9.4
        vtksqlite-9.4
        vtksys-9.4
        vtkTestingCore-9.4
        vtkTestingRendering-9.4
        vtktheora-9.4
        vtktiff-9.4
        vtktoken-9.4
        vtkverdict-9.4
        vtkViewsContext2D-9.4
        vtkViewsCore-9.4
        vtkViewsInfovis-9.4
        vtkViewsQt-9.4
        vtkWrappingTools-9.4
        vtkzlib-9.4
    )
    
else()
    # Linux 配置
    set(VTK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../Tools/Linux64/VTK942")
    set(VTK_INCLUDE_DIR "${VTK_ROOT}/include/vtk-9.4")
    set(VTK_LIB_DIR "${VTK_ROOT}/lib")
    
    # Linux VTK 库列表
    set(VTK_LIBRARIES
        vtkcgns-9.4
        vtkChartsCore-9.4
        vtkCommonColor-9.4
        vtkCommonComputationalGeometry-9.4
        vtkCommonCore-9.4
        vtkCommonDataModel-9.4
        vtkCommonExecutionModel-9.4
        vtkCommonMath-9.4
        vtkCommonMisc-9.4
        vtkCommonSystem-9.4
        vtkCommonTransforms-9.4
        vtkDICOMParser-9.4
        vtkDomainsChemistry-9.4
        vtkDomainsChemistryOpenGL2-9.4
        vtkDomainsParallelChemistry-9.4
        vtkdoubleconversion-9.4
        vtkexodusII-9.4
        vtkexpat-9.4
        vtkFiltersAMR-9.4
        vtkFiltersCellGrid-9.4
        vtkFiltersCore-9.4
        vtkFiltersExtraction-9.4
        vtkFiltersFlowPaths-9.4
        vtkFiltersGeneral-9.4
        vtkFiltersGeneric-9.4
        vtkFiltersGeometry-9.4
        vtkFiltersGeometryPreview-9.4
        vtkFiltersHybrid-9.4
        vtkFiltersHyperTree-9.4
        vtkFiltersImaging-9.4
        vtkFiltersModeling-9.4
        vtkFiltersParallel-9.4
        vtkFiltersParallelGeometry-9.4
        vtkFiltersParallelImaging-9.4
        vtkFiltersParallelMPI-9.4
        vtkFiltersParallelVerdict-9.4
        vtkFiltersPoints-9.4
        vtkFiltersProgrammable-9.4
        vtkFiltersReduction-9.4
        vtkFiltersSelection-9.4
        vtkFiltersSMP-9.4
        vtkFiltersSources-9.4
        vtkFiltersStatistics-9.4
        vtkFiltersTemporal-9.4
        vtkFiltersTensor-9.4
        vtkFiltersTexture-9.4
        vtkFiltersTopology-9.4
        vtkFiltersVerdict-9.4
        vtkfmt-9.4
        vtkfreetype-9.4
        vtkGeovisCore-9.4
        vtkgl2ps-9.4
        vtkglad-9.4
        vtkGUISupportQt-9.4
        vtkGUISupportQtSQL-9.4
        vtkhdf5-9.4
        vtkhdf5_hl-9.4
        vtkImagingColor-9.4
        vtkImagingCore-9.4
        vtkImagingFourier-9.4
        vtkImagingGeneral-9.4
        vtkImagingHybrid-9.4
        vtkImagingMath-9.4
        vtkImagingMorphological-9.4
        vtkImagingSources-9.4
        vtkImagingStatistics-9.4
        vtkImagingStencil-9.4
        vtkInfovisCore-9.4
        vtkInfovisLayout-9.4
        vtkInteractionImage-9.4
        vtkInteractionStyle-9.4
        vtkInteractionWidgets-9.4
        vtkIOAMR-9.4
        vtkIOAsynchronous-9.4
        vtkIOCellGrid-9.4
        vtkIOCesium3DTiles-9.4
        vtkIOCGNSReader-9.4
        vtkIOChemistry-9.4
        vtkIOCityGML-9.4
        vtkIOCONVERGECFD-9.4
        vtkIOCore-9.4
        vtkIOEngys-9.4
        vtkIOEnSight-9.4
        vtkIOERF-9.4
        vtkIOExodus-9.4
        vtkIOExport-9.4
        vtkIOExportGL2PS-9.4
        vtkIOExportPDF-9.4
        vtkIOFDS-9.4
        vtkIOFLUENTCFF-9.4
        vtkIOGeometry-9.4
        vtkIOHDF-9.4
        vtkIOImage-9.4
        vtkIOImport-9.4
        vtkIOInfovis-9.4
        vtkIOIOSS-9.4
        vtkIOLegacy-9.4
        vtkIOLSDyna-9.4
        vtkIOMINC-9.4
        vtkIOMotionFX-9.4
        vtkIOMovie-9.4
        vtkIOMPIImage-9.4
        vtkIONetCDF-9.4
        vtkIOOggTheora-9.4
        vtkIOParallel-9.4
        vtkIOParallelNetCDF-9.4
        vtkIOParallelXML-9.4
        vtkIOPLY-9.4
        vtkIOSegY-9.4
        vtkIOSQL-9.4
        vtkioss-9.4
        vtkIOTecplotTable-9.4
        vtkIOVeraOut-9.4
        vtkIOVideo-9.4
        vtkIOXML-9.4
        vtkIOXMLParser-9.4
        vtkjpeg-9.4
        vtkjsoncpp-9.4
        vtkkissfft-9.4
        vtklibharu-9.4
        vtklibproj-9.4
        vtklibxml2-9.4
        vtkloguru-9.4
        vtklz4-9.4
        vtklzma-9.4
        vtkmetaio-9.4
        vtknetcdf-9.4
        vtkogg-9.4
        vtkParallelCore-9.4
        vtkParallelDIY-9.4
        vtkParallelMPI-9.4
        vtkpng-9.4
        vtkpugixml-9.4
        vtkRenderingAnnotation-9.4
        vtkRenderingCellGrid-9.4
        vtkRenderingContext2D-9.4
        vtkRenderingContextOpenGL2-9.4
        vtkRenderingCore-9.4
        vtkRenderingFreeType-9.4
        vtkRenderingGL2PSOpenGL2-9.4
        vtkRenderingHyperTreeGrid-9.4
        vtkRenderingImage-9.4
        vtkRenderingLabel-9.4
        vtkRenderingLICOpenGL2-9.4
        vtkRenderingLOD-9.4
        vtkRenderingOpenGL2-9.4
        vtkRenderingQt-9.4
        vtkRenderingSceneGraph-9.4
        vtkRenderingUI-9.4
        vtkRenderingVolume-9.4
        vtkRenderingVolumeOpenGL2-9.4
        vtkRenderingVtkJS-9.4
        vtksqlite-9.4
        vtksys-9.4
        vtkTestingCore-9.4
        vtkTestingRendering-9.4
        vtktheora-9.4
        vtktiff-9.4
        vtktoken-9.4
        vtkverdict-9.4
        vtkViewsContext2D-9.4
        vtkViewsCore-9.4
        vtkViewsInfovis-9.4
        vtkViewsQt-9.4
        vtkWrappingTools-9.4
        vtkzlib-9.4
    )
endif()

# 处理单配置和多配置的差异
if(CMAKE_CONFIGURATION_TYPES)
    # 多配置生成器
    
    # 检查 VTK 目录结构
    if(WIN32)
        if(NOT EXISTS "${VTK_ROOT}/libd" OR NOT EXISTS "${VTK_ROOT}/lib")
            message(WARNING "VTK: Could not find both libd and lib directories")
        endif()
    endif()
    
    # 创建两个不同的目标
    add_library(VTK::VTK_Debug INTERFACE IMPORTED)
    add_library(VTK::VTK_Release INTERFACE IMPORTED)
    
    # Debug 目标
    target_include_directories(VTK::VTK_Debug INTERFACE "${VTK_INCLUDE_DIR}")
    
    # Release 目标
    target_include_directories(VTK::VTK_Release INTERFACE "${VTK_INCLUDE_DIR}")
    
    if(WIN32)
        # Windows
        target_link_directories(VTK::VTK_Debug INTERFACE "${VTK_ROOT}/libd")
        target_link_directories(VTK::VTK_Release INTERFACE "${VTK_ROOT}/lib")
        
        # 为 Debug 库添加 "d" 后缀
        set(DEBUG_LIBS)
        foreach(lib ${VTK_LIB_BASE_NAMES})
            list(APPEND DEBUG_LIBS "${lib}d")
        endforeach()
        target_link_libraries(VTK::VTK_Debug INTERFACE ${DEBUG_LIBS})
        
        # Release 库无后缀
        target_link_libraries(VTK::VTK_Release INTERFACE ${VTK_LIB_BASE_NAMES})
    else()
        # Linux
        target_link_directories(VTK::VTK_Debug INTERFACE "${VTK_ROOT}/lib")
        target_link_directories(VTK::VTK_Release INTERFACE "${VTK_ROOT}/lib")
        target_link_libraries(VTK::VTK_Debug INTERFACE ${VTK_LIBRARIES})
        target_link_libraries(VTK::VTK_Release INTERFACE ${VTK_LIBRARIES})
    endif()
    
    # 创建统一的 VTK::VTK 目标，使用生成器表达式
    add_library(VTK::VTK INTERFACE IMPORTED)
    target_link_libraries(VTK::VTK INTERFACE
        $<$<CONFIG:Debug>:VTK::VTK_Debug>
        $<$<CONFIG:Release>:VTK::VTK_Release>
        $<$<CONFIG:MinSizeRel>:VTK::VTK_Release>
        $<$<CONFIG:RelWithDebInfo>:VTK::VTK_Release>
    )
    
else()
    # 单配置生成器
    
    if(WIN32)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(VTK_LIB_DIR "${VTK_ROOT}/libd")
            set(VTK_LIB_SUFFIX "d")
        else()
            set(VTK_LIB_DIR "${VTK_ROOT}/lib")
            set(VTK_LIB_SUFFIX "")
        endif()
        
        # 生成带后缀的库列表
        set(VTK_LIBRARIES_WIN)
        foreach(lib_base ${VTK_LIB_BASE_NAMES})
            list(APPEND VTK_LIBRARIES_WIN "${lib_base}${VTK_LIB_SUFFIX}")
        endforeach()
        
        # 创建 VTK 目标
        add_library(VTK::VTK INTERFACE IMPORTED)
        target_include_directories(VTK::VTK INTERFACE ${VTK_INCLUDE_DIR})
        target_link_directories(VTK::VTK INTERFACE ${VTK_LIB_DIR})
        target_link_libraries(VTK::VTK INTERFACE ${VTK_LIBRARIES_WIN})
        
    else()
        # Linux：使用预定义的 VTK_LIBRARIES
        add_library(VTK::VTK INTERFACE IMPORTED)
        target_include_directories(VTK::VTK INTERFACE ${VTK_INCLUDE_DIR})
        target_link_directories(VTK::VTK INTERFACE ${VTK_LIB_DIR})
        target_link_libraries(VTK::VTK INTERFACE ${VTK_LIBRARIES})
    endif()
    
endif()

# 输出配置信息
message(STATUS "VTK: Configuration complete")
message(STATUS "VTK: Include directory: ${VTK_INCLUDE_DIR}")
if(WIN32)
    if(CMAKE_CONFIGURATION_TYPES)
        message(STATUS "VTK: Multi-configuration mode (Visual Studio)")
        message(STATUS "VTK: Debug libraries will use 'd' suffix")
        message(STATUS "VTK: Release libraries will not use suffix")
    else()
        message(STATUS "VTK: Single-configuration mode (Build type: ${CMAKE_BUILD_TYPE})")
    endif()
else()
    message(STATUS "VTK: Linux configuration")
endif()