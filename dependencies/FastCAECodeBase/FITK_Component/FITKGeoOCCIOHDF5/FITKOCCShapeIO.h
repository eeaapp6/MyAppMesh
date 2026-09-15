/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKOCCShapeIO.h
 * @brief   IO tool for reading and writing TopoDS_Shape document.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-09
 *********************************************************************/

#ifndef __FITKOCCSHAPEIO_H__
#define __FITKOCCSHAPEIO_H__

#include <QString>
#include <QObject>

#include "FITKGeoOCCIOHDF5API.h"

// Forward declaration
class TopoDS_Shape;
class TDF_Label;

namespace Interface
{
    class FITKAbsGeoCommand;
    class FITKShapeVirtualTopoManager;
}

namespace IO
{
    /**
     * @brief   Base class for reading and writing TopoDS_Shape document.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-09
     */
    class FITKOCCShapeIO : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        explicit FITKOCCShapeIO();

        /**
         * @brief   Destructor.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        virtual ~FITKOCCShapeIO();

        /**
         * @brief   Save the file name for IO.
         * @param   fileName: The file name
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        void setFileName(QString fileName);

        /**
         * @brief   Get the file name for IO.
         * @return  The file name
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        QString getFileName();

        /**
         * @brief   Set the shape data as input or output.
         * @param   shape: The OCC shape data pointer
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        void setShape(TopoDS_Shape* shape);

        /**
         * @brief   Get the shape data of the IO input or output.
         * @return  The OCC shape data pointer
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        TopoDS_Shape* getShape();

        /**
         * @brief   The error information text.
         * @return  The error text
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        QString errorInfo();

        /**
         * @brief   Excute the IO( read or write ).[pure virtual]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        virtual bool update() = 0;

    protected:
        /**
         * @brief   Check the file name and the input shape data.
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        bool checkInputData();

        /**
         * @brief   Get the shape label in XCAF document.
         * @param   root: The root label of the document[quote]
         * @param   label: The shape label[quote]
         * @return  Has found shape root label
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        bool getShapeLabel(TDF_Label & root, TDF_Label & label);

    protected:
        /**
         * @brief   The file name.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        QString m_fileName;

        /**
         * @brief   The OCC shape data poiner.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        TopoDS_Shape* m_topoShape{ nullptr };

        /**
         * @brief   The error information text.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        QString m_errorText;

    };

    /**
     * @brief   IO tool for reading TopoDS_Shape document.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-09
     */
    class FITKOCCShapeReader : public FITKOCCShapeIO
    {
    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        explicit FITKOCCShapeReader() = default;

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        ~FITKOCCShapeReader() = default;

        /**
         * @brief   Read the XCAF bin file.
         * @return  Is valid
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        bool read();

        /**
         * @brief   Excute the IO read.[override]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        bool update() override;

    };

    /**
     * @brief   IO tool for writing TopoDS_Shape document.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-09
     */
    class FITKOCCShapeWriter : public FITKOCCShapeIO
    {
    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        explicit FITKOCCShapeWriter() = default;

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        ~FITKOCCShapeWriter() = default;

        /**
         * @brief   Write the XCAF bin file.
         * @return  Is valid
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        bool write();

        /**
         * @brief   Excute the IO write.[override]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        bool update() override;

    };

    /**
     * @brief   Static functions tool class for shape bin data IO.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-09
     */
    class FITKGEOOCCIOHDF5API FITKOCCIOTool
    {
    public:
        /**
         * @brief   Convert the shape data to the char array.
         * @param   shape: The shape need to be convert to raw data
         * @param   len: The length of the data array[quote]
         * @param   data: The raw data of the shape[quote]
         * @param   fileName: The temp file name[default]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        static bool ShapeToData(TopoDS_Shape* shape, uint & len, char* & data, QString fileName = "");

        /**
         * @brief   Convert the char array to the shape data.
         * @param   shape: The shape converted from the raw data
         * @param   len: The length of the data array
         * @param   data: The raw data of the shape
         * @param   fileName: The temp file name[default]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        static bool DataToShape(TopoDS_Shape* shape, uint len, char* data, QString fileName = "");

        /**
         * @brief   Convert the char array to the shape data, and set the shape to the agent.
         * @param   shape: The shape converted from the raw data
         * @param   len: The length of the data array
         * @param   data: The raw data of the shape
         * @param   fileName: The temp file name[default]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        static bool SetDataToCommand(Interface::FITKAbsGeoCommand* cmd, uint len, char* data, QString fileName = "");

    private:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-23
         */
        FITKOCCIOTool();

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-23
         */
        ~FITKOCCIOTool();

        /**
         * @brief   Internal function - Split the shape and get all assembly shape, initialize the virtual topos.
         * @param   subShape: The shape need to split[quote]
         * @param   assemblyTopoMgr: The assembly topo manager
         * @param   tool: The iterator counter of the assembly
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-23
         */
        static void recurSplitAssembly(const TopoDS_Shape & shape, Interface::FITKShapeVirtualTopoManager* assemblyTopoMgr, FITKOCCIOTool* tool);

    private:
        /**
         * @brief   Internal data, for splitting the shape.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-23
         */
        int m_indexOfAss = 0;

    };

}   // namespace IO

#endif // __FITKOCCSHAPEIO_H__