## OpenFascae生态版平台

OpenFascae是FastCAE开源版。

在全球制造业加速向智能化转型的背景下，工业软件作为智能制造的核心支撑，其重要性日益凸显。OpenFascae作为一款开源的工业软件底座框架平台，凭借其高度的灵活性和可扩展性，为工业设计与仿真领域注入了新的活力。然而，面对日益复杂的工业应用场景和不断变化的用户需求，OpenFascae平台的功能与性能仍需持续优化与拓展。

OpenFascae为了规范CAE软件子系统集成，设计开发了基础集成底座，该集成底座中定义了基础的抽象数据结构，并提供了线程池等基础通用工具，同时定义了CAE软件的集成框架，规定了软件运行的顺序。基于该底座能够实现标准化的CAE软件集成，可有效提高CAE软件的集成效率与软件的鲁棒性。

OpenFascae基础集成底座基于Qt应用程序框架开发，分为Core、AppFramework、Adaptor、Python脚本驱动器四部分。Core模块是整个软件的核心模块，一方面定义了基础的抽象数据结构，并提供了数据管理与全局数据查找；另一方面，在Core模块中提供了软件开发过程中常用的工具与通用算法，例如线程池、第三方程序驱动、文件与路径创建删除等。AppFramework中定义了CAE软件的主要运行过程，在该模块中将CAE软件的运行过程分为了十四个部分，在提供基础功能的同时，提供了注册接口，能够实现个性化的软件集成。Adaptor模块中定义了适配器的抽象类及其管理器，该模块中将适配器分为文件IO适配器与渲染对象适配器两类。（详情见Document库中的“OpenFascae基础底座开发与使用说明书.doc”）

## 仓库目录介绍 
（因Atomgit 无法使用代码组 整合功能相同库，故有此目录介绍，便于理解所用平台组织结构）
   - FITK_Component组、FITK_Interface组、FITK_Kernel组内的子仓库是为了提供核心功能和基础模块支持，为FITK_Apps组内的应用仓库提供依赖。
   - 相关组内的代码库快捷显示方式-在精选仓库"FastCAECodeBase”中


1. **FITK_Apps组**  
   此组包含以下仓库组件：  
   - APPCADOCC        &emsp;&emsp;&nbsp;是基于OCC开发的几何建模软件 
   - APPFlow          &emsp;&emsp;&emsp;&emsp;是集成了OpenFoam的流体仿真软件  
   - APPCFDPost       &emsp;&emsp;&nbsp;是CFD的后处理软件 
   - AppFlowPHengLEI  &emsp;&emsp;&nbsp; AppFlow 集成风雷求解器
2. **FITK_Interface组**  
   快捷方式请查看FITK_Interface库
   此组包含以下仓库组件：  
   - FITKVTKAlgorithm
   - FITKInterfacePhysics
   - FITKInterfaceMesh
   - FITKInterfaceIOHDF5
   - FITKInterfaceFlowOF
   - FITKInterfaceMeshGen
   - FITKInterfaceGeoIOHDF5
   - FITKInterfaceGeometry
   - FITKInterfaceCFDPost
   - FITKInterfaceModel
   - FITKInterfaceIO
   - FITKInterfaceFlowPHengLEI
   
3. **FITK_Kernel组**  
   快捷方式请查看FITK_Kernel库
   此组包含以下仓库组件：  
   - FITKEasyParam
   - FITKCore
   - FITKPython
   - FITKAdaptor
   - FITKAppFramework

4. **FITK_Component组**  
   快捷方式请查看FITK_Component库
   此组包含以下仓库组件：   
   - FITKPlotWindow
   - FITKRenderWindowVTK
   - FITKOFDriver
   - FITKOFMeshIO
   - FITKOFDictWriter
   - FITKFlowOFIOHDF5
   - FITKCompMessageWidget
   - FITKMeshGenOF
   - FITKFluidVTKGraphAdaptor
   - FITKRenderWindowOCC
   - FITKGeoCompOCC
   - FITKGeoOCCIOHDF5
   - FITKOCCGraphAdaptor
   - FITKWidget

5. **Tools工具**
   此组包含以下仓库组件：  
   - Tools

## 项目相关文档在"Document"中 
- 1_OpenFascae基础底座开发与使用说明书
- 2_网格引擎集成与接口调用说明书
- 3_应用软件开发说明书
- 4_组件使用说明书
- 5_二维图表组件使用与接口调用
- 6_插件系统开发与使用说明
