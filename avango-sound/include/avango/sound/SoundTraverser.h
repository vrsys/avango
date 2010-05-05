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

#ifndef AV_SOUND_SOUNDTRAVERSER_H
#define AV_SOUND_SOUNDTRAVERSER_H

#include <avango/FieldContainer.h>
#include <avango/osg/Fields.h>
#include <avango/osg/Node.h>
#include <avango/sound/SoundRenderer.h>
#include <avango/Application.h>
#include <vector>

/**
 * \file
 * \ingroup av_sound
 */

namespace av {
  namespace sound {
    /**
     * This class traverses the scenegraph under RootNode and accumulates the transformation
     * matrix. It then lets the SoundRenderers stored in Renderers update their LocalSources stored
     * in a SoundSource that is found in the tree.
     */
    class SoundTraverser : public FieldContainer {

      AV_FC_DECLARE();

      public:
        SoundTraverser();
      protected:
        /* virtual */~SoundTraverser();
      public:
        /**
         * The renderers that are used to render the SoundSources
         */
        MFSoundRenderer     Renderers;
        /**
         * The rootnode of the subgraph that should be traversed by the SoundTraverser
         */
        av::osg::SFNode     RootNode;
        /**
         * If true, traverses the node under RootNode every frame
         */
        av::SFBool          Traverse;

        /**
         * Recursively traverse the graph under rootNode and accumulate the transformation matrices
         * @param rootNode The root of the subgraph
         * @param absolutTransform The matrix transformation up to this point in the graph
         */
        void traverse(Link<av::osg::Node> rootNode, ::osg::Matrix absolutTransform) const;
        void fieldHasChanged(const Field& field) ;
        void evaluate();
      private:
        void render();
        bool mCheckTraversal;
        bool mHaveCallbackHandle;
        av::Application::CallbackHandle mCallbackHandle;
        typedef std::vector<Link<SoundRenderer> > SoundRenderers;
        typedef std::vector<Link<av::osg::Node> > ChildNodes;
    };
  }
}

#endif /*AV_SOUND_SOUNDTRAVERSER_H*/
