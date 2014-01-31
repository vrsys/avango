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

#if !defined(AVANGO_OSG_FRAGMENTGROUP_H)
#define AVANGO_OSG_FRAGMENTGROUP_H

/**
 * \file
 * \ingroup av_gua
 */

#include <avango/gua/scenegraph/TransformNode.hpp>

namespace av
{
  namespace gua
  {
    class NetMatrixTransform;

    /**
     * Wrapper for ::gua::TransformNode
     *
     * \ingroup av_gua
     */
    class FragmentGroup : public av::gua::TransformNode
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TransformNode is created.
       * Otherwise, the given ::gua::TransformNode is used.
       */
      FragmentGroup();

      void fieldHasChangedLocalSideEffect(const Field&);
      void evaluateLocalSideEffect();

      void registerNetMatrixTransform(NetMatrixTransform* netMatrixTransform);

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~FragmentGroup();

    private:

      NetMatrixTransform* mNetMatrixTransform;
      bool      mChildrenChanged;

    };

    typedef SingleField<Link<FragmentGroup> > SFFragmentGroup;
    typedef MultiField<Link<FragmentGroup> > MFFragmentGroup;

  } // namespace gua

} // namespace av

#endif

