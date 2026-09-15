# OCCConfig.cmake – OpenCASCADE 库的 CMake 导入配置文件
include_guard(GLOBAL)

# 检查 TOOLS_DIR 是否定义
if(NOT TOOLS_DIR)
    message(FATAL_ERROR "OCCConfig.cmake: TOOLS_DIR is not defined. Please set it in top-level CMakeLists.txt")
endif()

# 根据平台设置 OCC 根目录
if(WIN32)
    set(OCC_BASE_DIR "${TOOLS_DIR}/Win64/OCC")
elseif(UNIX)
    set(OCC_BASE_DIR "${TOOLS_DIR}/Linux64/OCC")
else()
    message(FATAL_ERROR "OCCConfig.cmake: Unsupported platform")
endif()

# 头文件目录（平台相关）
if(WIN32)
    set(OCC_INCLUDE_DIR "${OCC_BASE_DIR}/include")
elseif(UNIX)
    set(OCC_INCLUDE_DIR "${OCC_BASE_DIR}/include/opencascade")
endif()

# 定义所有可能的 OCC 组件库名称（基于 qmake 文件）
set(OCC_COMPONENTS_ALL
    TKBin
    TKBinL
    TKBinTObj
    TKBinXCAF
    TKBO
    TKBool
    TKBRep
    TKCAF
    TKCDF
    TKDCAF
    TKDraw
    TKernel
    TKFeat
    TKFillet
    TKG2d
    TKG3d
    TKGeomAlgo
    TKGeomBase
    TKHLR
    TKIGES
    TKIVtk
    TKIVtkDraw
    TKLCAF
    TKMath
    TKMesh
    TKMeshVS
    TKOffset
    TKOpenGl
    TKPrim
    TKQADraw
    TKRWMesh
    TKService
    TKShHealing
    TKStd
    TKStdL
    TKSTEP
    TKSTEP209
    TKSTEPAttr
    TKSTEPBase
    TKSTL
    TKTObj
    TKTObjDRAW
    TKTopAlgo
    TKTopTest
    TKV3d
    TKVCAF
    TKViewerTest
    TKVRML
    TKXCAF
    TKXDEDRAW
    TKXDEIGES
    TKXDESTEP
    TKXMesh
    TKXml
    TKXmlL
    TKXmlTObj
    TKXmlXCAF
    TKXSBase
    TKXSDRAW
)

# 根据平台筛选实际存在的组件
if(WIN32)
    set(OCC_COMPONENTS ${OCC_COMPONENTS_ALL})
elseif(UNIX)
    set(OCC_COMPONENTS ${OCC_COMPONENTS_ALL})
    # Unix 下通常不包含 TKIVtk 和 TKIVtkDraw（如 qmake 中注释所示）
    list(REMOVE_ITEM OCC_COMPONENTS TKIVtk TKIVtkDraw)
endif()

# 为每个组件创建导入目标
foreach(comp IN LISTS OCC_COMPONENTS)
    if(NOT TARGET OCC::${comp})
        add_library(OCC::${comp} UNKNOWN IMPORTED)

        # 设置公共包含目录（所有组件共用）
        set_target_properties(OCC::${comp} PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${OCC_INCLUDE_DIR}"
        )

        # 根据平台和配置设置库文件位置
        if(WIN32)
            # Windows 区分 Debug 和 Release
            set_target_properties(OCC::${comp} PROPERTIES
                IMPORTED_LOCATION_DEBUG   "${OCC_BASE_DIR}/libd/${comp}.lib"
                IMPORTED_LOCATION_RELEASE "${OCC_BASE_DIR}/lib/${comp}.lib"
            )
        elseif(UNIX)
            # Linux 使用共享库 (.so)
            set_target_properties(OCC::${comp} PROPERTIES
                IMPORTED_LOCATION "${OCC_BASE_DIR}/lib/lib${comp}.so"
            )
        endif()
    endif()
endforeach()

# 创建聚合接口目标 OCC::OCC，链接所有组件并提供包含目录
add_library(OCC::OCC INTERFACE IMPORTED)

# 动态收集所有组件目标
set(_occ_targets "")
foreach(comp IN LISTS OCC_COMPONENTS)
    list(APPEND _occ_targets OCC::${comp})
endforeach()
target_link_libraries(OCC::OCC INTERFACE ${_occ_targets})

# 为聚合接口设置包含目录（方便直接使用 OCC::OCC 时也能获得头文件路径）
set_target_properties(OCC::OCC PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${OCC_INCLUDE_DIR}"
)

# 可选：根据实际需要添加编译定义，例如：
# target_compile_definitions(OCC::OCC INTERFACE ...)