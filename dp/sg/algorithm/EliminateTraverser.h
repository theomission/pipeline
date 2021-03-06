// Copyright NVIDIA Corporation 2002-2011
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#pragma once
/** \file */

#include <dp/sg/algorithm/Config.h>
#include <dp/sg/algorithm/OptimizeTraverser.h>

namespace dp
{
  namespace sg
  {
    namespace algorithm
    {

      /*! \brief Traverser that eliminates a number of Objects, if appropriate.
       *  \remarks There are a number of different object types that can be eliminated:\n
       *  - A Group, that is a child of a Group or a Transform, and that does not hold clip planes or
       *  lights, can be replaced by its children.
       *  - A Group, that has exactly one child, and no clip planes or lights, can be replaced by its child
       *  (this is a subset compared to the above)
       *  - An LOD, that holds exactly one child (and no ranges) can be replaced by that child.
       *  - A StateSet that holds no StateAttributes can be removed.
       *  \sa */
      class EliminateTraverser : public OptimizeTraverser
      {
        public:
          enum
          {
            ET_GROUP                  = BIT0,   //!< EliminateTarget Group: replace redundant Groups by their child/children.
            ET_GROUP_SINGLE_CHILD     = BIT1,   //!< EliminateTarget Group: replace single-child Groups by their chid
            ET_INDEX_SET              = BIT2,   //!< EliminateTarget IndexSet: remove IndexSet that just enumerates from 0 to n-1.
            ET_LOD                    = BIT3,   //!< EliminateTarget LOD: replace redundant LODs by its child.
            ET_ALL_TARGETS_MASK   = ( ET_GROUP | ET_INDEX_SET | ET_LOD )
          } EliminateTarget;

        public:
          //! Constructor
          DP_SG_ALGORITHM_API EliminateTraverser( void );

          //! Destructor
          DP_SG_ALGORITHM_API virtual ~EliminateTraverser( void );

          /*! \brief Get the bitmask describing the targets to eliminate.
           *  \return A bitmask describing the targets to eliminate. */
          DP_SG_ALGORITHM_API unsigned int getEliminateTargets() const;

          /*! \brief Set the bitmask describing the targets to eliminate.
           *  \param mask The bitmask describing the targets to eliminate. */
          DP_SG_ALGORITHM_API void setEliminateTargets( unsigned int mask );

          REFLECTION_INFO_API( DP_SG_ALGORITHM_API, EliminateTraverser );
          BEGIN_DECLARE_STATIC_PROPERTIES
              DP_SG_ALGORITHM_API DECLARE_STATIC_PROPERTY( EliminateTargets );
          END_DECLARE_STATIC_PROPERTIES

        protected:
          //! If the root node is a Group with a single child, it is removed.
          DP_SG_ALGORITHM_API virtual void doApply( const dp::sg::core::NodeSharedPtr & root );

          //! If the Billboard holds Groups without lights or clip planes, they are replaced by their children.
          DP_SG_ALGORITHM_API virtual void handleBillboard( dp::sg::core::Billboard *p );

          //! If the Group holds other Groups without lights or clip planes, they are replaced by their children.
          DP_SG_ALGORITHM_API virtual void handleGroup( dp::sg::core::Group * p );

          //! If the LOD holds Groups with one child without lights or clip planes, they are replaced by their respective child.
          DP_SG_ALGORITHM_API virtual void handleLOD( dp::sg::core::LOD *p );

          DP_SG_ALGORITHM_API virtual void handlePrimitive( dp::sg::core::Primitive * p );

          //! If the Switch holds Groups with one child without lights or clip planes, they are replaced by their respective child.
          DP_SG_ALGORITHM_API virtual void handleSwitch( dp::sg::core::Switch *p );

          //! If the Transform holds Groups without lights or clip planes, they are replaced by their children.
          DP_SG_ALGORITHM_API virtual void handleTransform( dp::sg::core::Transform *p );

        private:
          void eliminateGroups( dp::sg::core::Group *p );
          void eliminateSingleChildChildren( dp::sg::core::Group *p, unsigned int objectCode );
          bool isOneChildCandidate( dp::sg::core::GroupSharedPtr const& p );

        private:
          unsigned int            m_eliminateTargets;
          std::set<const void *>  m_objects;      //!< A set of pointers to hold all objects already encountered.
      };

      inline unsigned int EliminateTraverser::getEliminateTargets() const
      {
        return( m_eliminateTargets );
      }

      inline void EliminateTraverser::setEliminateTargets( unsigned int mask )
      {
        if ( mask != m_eliminateTargets )
        {
          m_eliminateTargets = mask;
          notify( PropertyEvent( this, PID_EliminateTargets ) );
        }
      }

    } // namespace algorithm
  } // namespace sp
} // namespace dp
