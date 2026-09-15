#include "FITKSectionAssign.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModelComponent.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITKAbstractSection.h"

namespace Interface
{
    FITKSectionAssign::FITKSectionAssign(int modelID, int sectionID)
        :_modelID(modelID), _sectionID(sectionID)
    {
    }


    int FITKSectionAssign::getSectionID()
    {
        return _sectionID;
    }

    int FITKSectionAssign::getModelID()
    {
        return _modelID;
    }

    void FITKSectionAssign::assign(int modelID, int sectionID, bool c)
    {
        _modelID = modelID;
        _sectionID = sectionID;
        //非法输入，不记录
        if (!c) return;
        if (!isValid())
            _modelID = _sectionID = -1;
    }

    void FITKSectionAssign::setSection(const int sectionID)
    {
        _sectionID = sectionID;
    }

    bool FITKSectionAssign::isValid()
    {
        //ID 错误
        if (_modelID < 0 || _sectionID < 0) return false;
        //获取为model或者component
        auto model = FITKDATAREPO->getTDataByID<FITKAbstractModel>(_modelID);
        auto comp = FITKDATAREPO->getTDataByID<FITKAbstractModelComponent>(_modelID);
        //是模型或者是部件即认为合法
        const bool isModel = model != nullptr || comp != nullptr;
        //获取截面
        auto sec = this->getSectionObj();
        return isModel && sec != nullptr;
    }

    bool FITKSectionAssign::isAssignComponent()
    {
        //尝试获取部件指针
        auto comp = FITKDATAREPO->getTDataByID<FITKAbstractModelComponent>(_modelID);
        return comp == nullptr;
    }

    Core::FITKAbstractDataObject* FITKSectionAssign::getSectionObj()
    {
        //获取截面
        return FITKDATAREPO->getTDataByID<FITKAbstractSection>(_sectionID);
    }

    Core::FITKAbstractDataObject* FITKSectionAssign::getModelObj()
    {
        //获取模型
        return FITKDATAREPO->getDataByID(_modelID);
    }

    QString FITKSectionAssign::getSectionName()
    {
        auto sec = this->getTSectionObj<Core::FITKAbstractNDataObject>();
        if (sec == nullptr) return QString();
        return sec->getDataObjectName();
    }

    QString FITKSectionAssign::getSectionType()
    {
        auto sec = this->getTSectionObj<FITKAbstractSection>();
        if (sec == nullptr) return QString();
        return sec->getTypeString();
    }

    bool FITKSectionAssign::copy(FITKAbstractDataObject* obj)
    {
        bool ok = Core::FITKAbstractDataObject::copy(obj);
        if (!ok)
            return false;
        FITKSectionAssign* sa = dynamic_cast<FITKSectionAssign*>(obj);
        if (sa == nullptr)
            return false;
        _modelID = sa->_modelID;
        _sectionID = sa->_sectionID;
        return true;
    }

    QString FITKSectionAssign::getModelName()
    {
        auto model = this->getTModelObj<Core::FITKAbstractNDataObject>();
        if (model == nullptr) return QString();
        return model->getDataObjectName();
    }

    bool FITKSectionAssign::isUsedDataObject(const QList<int> & dataIDs)
    {
        return dataIDs.contains(_modelID) || dataIDs.contains(_sectionID);
    }

    Interface::FITKSectionAssign* FITKSectionAssignManager::getMaterialAssignByModelSetID(const int modelsetID)
    {
        const int n = this->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            //遍历全部指派截面
            FITKSectionAssign* assign = this->getDataByIndex(i);
            if(assign == nullptr) continue;
            if (assign->getModelID() == modelsetID)
                return assign;
        }
        return nullptr;
    }

     
}


