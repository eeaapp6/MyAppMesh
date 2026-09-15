#include "FITKSectionAssignGroup.h"

namespace Interface
{
    void FITKSectionAssignGroup::setSection(const int sectionID)
    {
        const int n = this->getDataCount();
        //遍历全部Section assign
        for (int i =0;i<n; ++i)
        {
            FITKSectionAssign* ass = this->getDataByIndex(i);
            if (ass == nullptr) continue;
            ass->setSection(sectionID);
        }
    }


    int FITKSectionAssignGroup::getSectionID()
    {
        const int n = this->getDataCount();
        //没有asssign
        if (n == 0) return -1;
        //只有一个assign
        if (n == 1)
        {
            FITKSectionAssign* ass = this->getDataByIndex(0);
            if (ass == nullptr) return -1;
            return ass->getSectionID();
        }
        //有多个assign，如果Section不一致。则返回-1
        //首先记录第一个
        int sectionID = -1;
        FITKSectionAssign* ass = this->getDataByIndex(0);
        if (ass != nullptr)
            sectionID = ass->getSectionID();
        //遍历除了首个之外的assign
        for (int i = 1; i < n; ++i)
        {
            FITKSectionAssign* ass = this->getDataByIndex(i);
            if (ass == nullptr) return -1;
            int sec = ass->getSectionID();
            //与首个不一致，返回-1
            if (sec != sectionID) return -1;
        }
        //全部都一致
        return sectionID;
    }

}


