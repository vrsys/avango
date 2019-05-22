// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#if !defined(AVANGO_OSG_LOADFILE_H)
#define AVANGO_OSG_LOADFILE_H

/**
 * \file
 * \ingroup av_osg
 */

#include <avango/StandardFields.h>

#include <avango/osg/MatrixTransform.h>

namespace av
{
namespace osg
{
/**
 * Convenience node supposed to root loaded geometry,
 * using the osgDB::readNodeFiles() mechanism
 *
 * \ingroup av_osg
 */
class AV_OSG_DLL LoadFile : public MatrixTransform
{
    AV_FC_DECLARE();

  public:
    /**
     * Constructor. Note that there is no counterpart in osg for an av::osg::LoadFile.
     */
    LoadFile();

  protected:
    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~LoadFile();

  public:
    /**
     * File name of the geometry file to load.
     */
    SFString Filename;

    /**
     * Outputs the file name of the geometry as soon as it is actually loaded.
     * You can connect to this field to get a notification when the loading of the file was done.
     */
    SFString FilenameLoaded;

    /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
    /* virtual */ void evaluateLocalSideEffect();

    /**
     * Get the loaded ::osg::Node.
     */
    ::osg::Node* getLoadedNode() const;

  private:
    ::osg::ref_ptr<::osg::Node> mLoadedNode;
    bool mFilenameChanged;
    std::string mFilename;
    std::string mOldFilename;
};

typedef SingleField<Link<LoadFile>> SFLoadFile;
typedef MultiField<Link<LoadFile>> MFLoadFile;

} // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
template class AV_OSG_DLL SingleField<Link<osg::LoadFile>>;
template class AV_OSG_DLL MultiField<Link<osg::LoadFile>>;
#endif

} // namespace av

#endif
