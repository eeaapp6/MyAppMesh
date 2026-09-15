#include "FITKAbstractSection.h"
#include "FITKAbstractMaterial.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

namespace Interface
{

    FITKAbstractSection::FITKAbstractSection()
    {
        static int ABSSECTIONID = 0;
        _sectionID = ++ABSSECTIONID;
    }

    FITKAbstractSection::~FITKAbstractSection()
    {
        if (_sectionInfo) delete _sectionInfo;
    }

    void FITKAbstractSection::setMaterial(int id, int index /*= 0*/)
    {
        //首先填充数组，保证索引有效
        this->setSize(index + 1);
        _materialIDs[index] = id;
    }

    void FITKAbstractSection::setMaterial(Core::FITKAbstractDataObject* mat, int index /*= 0*/)
    {
        if (mat == nullptr) return;
        //首先填充数组，保证索引有效
        this->setSize(index + 1);
        _materialIDs[index] = mat->getDataObjectID();
    }

    int FITKAbstractSection::getMaterialID(int index /*= 0*/) const
    {
        //错误值-1
        if (index <0 || index >= _materialIDs.size()) return -1;
        return _materialIDs[index];
    }

    FITKAbstractMaterial* FITKAbstractSection::getMaterial(int index /*= 0*/) const
    {
        const int id = this->getMaterialID(index);
        if (id < 0) return nullptr;
        //仓库查找
        return Core::FITKDataRepo::getInstance()->getTDataByID<FITKAbstractMaterial>(id);
    }

    QString FITKAbstractSection::getMaterialName(int index /*= 0*/) const
    {
        auto ma = this->getMaterial(index);
        if (ma)
            return ma->getDataObjectName();
        //错误值
        return "";
    }

    int FITKAbstractSection::getMaterialCount()
    {
        return _materialIDs.size();
    }

    void FITKAbstractSection::removeMaterial(int index)
    {
        //获取索引
        if (index < 0 || index >= _materialIDs.size()) return;
        _materialIDs.removeAt(index);
    }

    QString FITKAbstractSection::getTypeString()
    {
        return QString();
    }

    int FITKAbstractSection::getSectionID() const
    {
        return _sectionID;
    }

    Interface::FITKAbstractSectionInfo* FITKAbstractSection::getSectionInfo()
    {
        return _sectionInfo;
    }

    void FITKAbstractSection::setSectionInfo(FITKAbstractSectionInfo* info)
    {
        _sectionInfo = info;
    }

    void FITKAbstractSection::setSize(int count)
    {
        //不断追加 -1
        while (_materialIDs.size() < count)
        {
            _materialIDs.append(-1);
        }

    }

    FITKAbstractSectionInfo::~FITKAbstractSectionInfo()
    {

    }

    Interface::FITKAbstractSectionInfo::SectionType FITKAbstractSectionInfo::getSectionType()
    {
        return Interface::FITKAbstractSectionInfo::SectionType::STNone;
    }

}

