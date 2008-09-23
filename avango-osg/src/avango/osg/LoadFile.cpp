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

#include <avango/osg/LoadFile.h>

#include <osgDB/ReadFile>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::LoadFile"));
}

AV_FC_DEFINE(av::osg::LoadFile);

AV_FIELD_DEFINE(av::osg::SFLoadFile);
AV_FIELD_DEFINE(av::osg::MFLoadFile);

av::osg::LoadFile::LoadFile() :
  MatrixTransform(),
  mFilenameChanged(false),
  mFilename(),
  mOldFilename()
{
  AV_FC_ADD_FIELD(Filename, mFilename);
}

av::osg::LoadFile::~LoadFile()
{}

void
av::osg::LoadFile::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::MatrixTransform::initClass();

    AV_FC_INIT(av::osg::MatrixTransform, av::osg::LoadFile, true);

    SFLoadFile::initClass("av::osg::SFLoadFile", "av::Field");
    MFLoadFile::initClass("av::osg::MFLoadFile", "av::Field");

    sClassTypeId.setDistributable(true);

  }
}

::osg::Node*
av::osg::LoadFile::getLoadedNode() const
{
  return mLoadedNode.get();
}

/* virtual */ void
av::osg::LoadFile::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  MatrixTransform::fieldHasChangedLocalSideEffect(field);
  LOG_TRACE(logger) << "fieldHasChangedLocalSideEffect()";

  if (&field == &Filename)
  {
    mFilename = Filename.getValue();
    logger.debug() << "fieldHadChangedLocalSideEffect(): Filename changed to " << mFilename;

    if (mFilename != mOldFilename)
    {
      logger.debug() << "fieldHadChangedLocalSideEffect(): Filename changed";
      mFilenameChanged = true;
      mOldFilename = mFilename;
    }
  }
}

/* virtual */ void
av::osg::LoadFile::evaluateLocalSideEffect()
{
  MatrixTransform::evaluateLocalSideEffect();
  LOG_TRACE(logger) << "evaluateLocalSideEffect()";

  if (mFilenameChanged)
  {
    logger.debug() << "evaluateLocalSideEffect(): got new filename " << mFilename;

    if (mLoadedNode.valid())
    {
      // remove old geometry from scene graph
      getOsgMatrixTransform()->removeChild(mLoadedNode.get());
    }

    // there are much more option to readNodeFile*(). This is just the first try.
    mLoadedNode = ::osgDB::readNodeFile(mFilename);
    logger.debug() << "evaluateLocalSideEffect(): loaded scene graph " << mFilename;

    if (mLoadedNode.valid())
    {
      // add new geometry
      getOsgMatrixTransform()->addChild(mLoadedNode.get());
    }

    mFilenameChanged = false;
  }
}
