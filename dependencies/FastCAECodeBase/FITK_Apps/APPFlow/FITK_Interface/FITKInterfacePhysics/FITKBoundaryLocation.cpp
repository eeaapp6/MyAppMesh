#include "FITKBoundaryLocation.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModelComponent.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAssembly.h"


namespace Interface
{
    void FITKBoundaryLocation::setModel(int id)
    {
        //设置模型
        _modelID = id;
    }

    void FITKBoundaryLocation::setModel(FITKAbstractModel* model)
    {
        //错误判断
        if(model)
            _modelID = model->getDataObjectID();
    }

    void FITKBoundaryLocation::setComponent(int id)
    {
        //设置组件
        _componentID = id;
    }

    void FITKBoundaryLocation::setComponent(FITKAbstractModelComponent* comp)
    {
        //错误判断
        if (comp)
            _componentID = comp->getDataObjectID();
    }

    int FITKBoundaryLocation::getModelID() const
    {
        return _modelID;
    }

    int FITKBoundaryLocation::getComponentID() const
    {
        return _componentID;
    }

    Interface::FITKAbstractModel* FITKBoundaryLocation::getModelObj()
    {
        //从仓库查找
        return FITKDATAREPO->getTDataByID<FITKAbstractModel>(_modelID);
    }

    Interface::FITKAbstractModelComponent* FITKBoundaryLocation::getComponentObj()
    {
        //从仓库查找
        return FITKDATAREPO->getTDataByID<FITKAbstractModelComponent>(_componentID);
    }

    bool FITKBoundaryLocation::isValid()
    {
        bool c = this->getComponentObj() != nullptr;
        bool m = this->getModelObj() != nullptr;
        return m && c;
    }

    QString FITKBoundaryLocation::getLocationTextLabel()
    { 
        auto comp = this->getComponentObj();
        auto model = this->getModelObj();
        if (comp == nullptr || model == nullptr) return QString();
        //内部集合
        auto set = FITKSafedownCast<Interface::FITKModelSet>(comp);
        if (set && set->isInternal()) return QString(tr("(Picked)"));
        //内部表面
        auto surf = FITKSafedownCast<Interface::FITKMeshSurface>(comp);
        if (surf && surf->isInternal()) return QString(tr("(Picked)"));
        
        //集合在单一的instance上，存储在assembly
        if (model == comp->getAbstractModel())
            return comp->getDataObjectName();
        
        //在instance上
        if (model->getAbsModelType() == FITKModelEnum::AbsModelType::AssInstance)
            return QString("%1.%2").arg(model->getDataObjectName()).arg(comp->getDataObjectName());
        //集合或者表面名称
        return comp->getDataObjectName();
    }

    void FITKBoundaryLocation::operator=(const FITKBoundaryLocation& loc)
    {
        _modelID = loc.getModelID();
        _componentID = loc.getComponentID();
    }

}