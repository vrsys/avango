/*
 * PCLPointCloud.h
 *
 *  Created on: Jun 10, 2013
 *      Author: ddangelo
 */

#ifndef PCLPOINTCLOUD_H_
#define PCLPOINTCLOUD_H_

/**
 * \file
 * \ingroup av_utils
 */

#include <avango/FieldContainer.h>
#include <avango/StandardFields.h>
#include <avango/osg/Fields.h>
#include "windows_specific_utils.h"

namespace av
{
  namespace utils
  {

    class AV_UTILS_DLL PCLPointCloud : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      PCLPointCloud();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~PCLPointCloud();

      /* virtual */ void fieldHasChanged(const av::Field& field);
      /* virtual */ void evaluate();

    public:

      av::SFString Filename;
      av::osg::MFVec3 Points;
      av::osg::MFVec4 Colors;

    private:

      void loadPCDFile(std::string filename);

    };

    using SFPCLPointCloud = SingleField<Link<PCLPointCloud> >;
    using MFPCLPointCloud = MultiField<Link<PCLPointCloud> >;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_UTILS_DLL SingleField<Link<utils::PCLPointCloud> >;
  template class AV_UTILS_DLL MultiField<Link<utils::PCLPointCloud> >;
#endif
}


#endif /* PCLPOINTCLOUD_H_ */
