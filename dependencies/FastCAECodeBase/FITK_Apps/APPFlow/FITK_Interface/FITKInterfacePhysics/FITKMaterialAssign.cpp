#include "FITKMaterialAssign.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModelComponent.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITKAbstractMaterial.h"

namespace Interface
{
    FITKMaterialAssign::FITKMaterialAssign(int modelID, int materialID)
        :_modelID(modelID),_materialID(materialID)
    {
    }

    int FITKMaterialAssign::getMaterialID()
    {
        return _materialID;
    }

    int FITKMaterialAssign::getModelID()
    {
        return _modelID;
    }

    void FITKMaterialAssign::assign(int modelID, int materialID)
    {
        _modelID = modelID;
        _materialID = materialID;
        //非法输入，不记录
        if(!isValid())
            _modelID = _materialID = -1;
    }

    bool FITKMaterialAssign::isValid()
    {
        //ID 错误
        if(_modelID <0 || _materialID <0) return false;
        //获取为model或者component
         auto model = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_modelID);
         auto comp = FITKDATAREPO->getTDataByID<FITKAbstractModelComponent>(_modelID);
         //是模型或者是部件即认为合法
         const bool isModel = model!=nullptr || comp != nullptr;
         //获取材料
         auto mat = this->getMaterialObj();
         return isModel && mat != nullptr;
    }

    bool FITKMaterialAssign::isAssignComponent()
    {
        //尝试获取部件指针
        auto comp = FITKDATAREPO->getTDataByID<FITKAbstractModelComponent>(_modelID);
        return comp == nullptr;
    }

    Core::FITKAbstractDataObject* FITKMaterialAssign::getMaterialObj()
    {
        //获取材料
        return FITKDATAREPO->getTDataByID<FITKAbstractMaterial>(_materialID);
    }

}


