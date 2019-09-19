/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2019 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2019 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2019 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#ifndef ELEMENT_REGION_BASE_H
#define ELEMENT_REGION_BASE_H

#include "CellElementSubRegion.hpp"
#include "FaceElementSubRegion.hpp"
#include "wells/WellElementSubRegion.hpp"
#include "managers/ObjectManagerBase.hpp"

namespace geosx
{

class StableTimeStep;

class FaceManager;

/**
 * Class to manage the data stored at the element level.
 */
class ElementRegionBase : public ObjectManagerBase
{
public:

  /**
   * @name Static Factory Catalog Functions
   */
  ///@{

//  static const string CatalogName()
//  { return "ElementRegionBase"; }
//
//  virtual const string getCatalogName() const override
//  { return ElementRegionBase::CatalogName(); }


  ///@}


  ElementRegionBase() = delete;

  ElementRegionBase( string const & name, Group * const parent );


  ElementRegionBase(const ElementRegionBase& init);

  virtual ~ElementRegionBase() override;

  virtual void GenerateMesh( Group const * const GEOSX_UNUSED_ARG( cellBlocks ) )
  {
    GEOS_ERROR( "ElementRegionBase::GenerateMesh() should be overriden if called.");
  }

//  void GenerateAggregates( FaceManager const * const faceManager, NodeManager const * const NodeManager )
//  {
//    GEOS_ERROR( "ElementRegionBase::GenerateAggregates() should be overriden if called.");
//  }

  subGroupMap & GetSubRegions()
  {
    return GetGroup(viewKeyStruct::elementSubRegions)->GetSubGroups();
  }

  subGroupMap const & GetSubRegions() const
  {
    return GetGroup(viewKeyStruct::elementSubRegions)->GetSubGroups();
  }

  template< typename SUBREGIONTYPE=ElementSubRegionBase >
  SUBREGIONTYPE const * GetSubRegion( string const & regionName ) const
  {
    return this->GetGroup(viewKeyStruct::elementSubRegions)->GetGroup<SUBREGIONTYPE>(regionName);
  }
  template< typename SUBREGIONTYPE=ElementSubRegionBase >
  SUBREGIONTYPE * GetSubRegion( string const & regionName )
  {
    return this->GetGroup(viewKeyStruct::elementSubRegions)->GetGroup<SUBREGIONTYPE>(regionName);
  }

  template< typename SUBREGIONTYPE=ElementSubRegionBase >
  SUBREGIONTYPE const * GetSubRegion( localIndex const & index ) const
  {
    return this->GetGroup(viewKeyStruct::elementSubRegions)->GetGroup<SUBREGIONTYPE>(index);
  }
  template< typename SUBREGIONTYPE=ElementSubRegionBase >
  SUBREGIONTYPE * GetSubRegion( localIndex const & index )
  {
    return this->GetGroup(viewKeyStruct::elementSubRegions)->GetGroup<SUBREGIONTYPE>(index);
  }

  localIndex numSubRegions() const
  {
    return this->GetGroup(viewKeyStruct::elementSubRegions)->GetSubGroups().size();
  }


  template< typename LAMBDA >
  void forElementSubRegions( LAMBDA && lambda ) const
  {
    forElementSubRegions<CellElementSubRegion, FaceElementSubRegion, WellElementSubRegion>( std::forward<LAMBDA>(lambda) );
  }

  template< typename LAMBDA >
  void forElementSubRegions( LAMBDA && lambda )
  {
    forElementSubRegions<CellElementSubRegion, FaceElementSubRegion, WellElementSubRegion>( std::forward<LAMBDA>(lambda) );
  }

  template< typename SUBREGIONTYPE, typename ... SUBREGIONTYPES, typename LAMBDA >
  void forElementSubRegions( LAMBDA && lambda ) const
  {
    Group const * const elementSubRegions = this->GetGroup(viewKeyStruct::elementSubRegions);
    elementSubRegions->forSubGroups< SUBREGIONTYPE, SUBREGIONTYPES...>( std::forward<LAMBDA>(lambda) );
  }

  template< typename SUBREGIONTYPE, typename ... SUBREGIONTYPES, typename LAMBDA >
  void forElementSubRegions( LAMBDA && lambda )
  {
    Group * const elementSubRegions = this->GetGroup(viewKeyStruct::elementSubRegions);
    elementSubRegions->forSubGroups< SUBREGIONTYPE, SUBREGIONTYPES...>( std::forward<LAMBDA>(lambda) );
  }


  template< typename LAMBDA >
  void forElementSubRegionsIndex( LAMBDA && lambda ) const
  {
    forElementSubRegionsIndex<CellElementSubRegion, FaceElementSubRegion, WellElementSubRegion>( std::forward<LAMBDA>(lambda) );
  }

  template< typename LAMBDA >
  void forElementSubRegionsIndex( LAMBDA && lambda )
  {
    forElementSubRegionsIndex<CellElementSubRegion, FaceElementSubRegion, WellElementSubRegion>( std::forward<LAMBDA>(lambda) );
  }

  template< typename SUBREGIONTYPE, typename ... SUBREGIONTYPES, typename LAMBDA >
  void forElementSubRegionsIndex( LAMBDA && lambda ) const
  {
    for( localIndex esr=0 ;  esr<this->numSubRegions() ; ++esr )
    {
      ElementSubRegionBase const * const subRegion = this->GetSubRegion(esr);
      applyLambdaToContainer<ElementSubRegionBase, SUBREGIONTYPE, SUBREGIONTYPES...>( subRegion, [&]( auto const * const castedSubRegion )
      {
        lambda( esr, castedSubRegion );
      });
    }
  }

  template< typename SUBREGIONTYPE, typename ... SUBREGIONTYPES, typename LAMBDA >
  void forElementSubRegionsIndex( LAMBDA && lambda )
  {
    for( localIndex esr=0 ;  esr<this->numSubRegions() ; ++esr )
    {
      ElementSubRegionBase * const subRegion = this->GetSubRegion(esr);
      applyLambdaToContainer<ElementSubRegionBase, SUBREGIONTYPE,SUBREGIONTYPES...>( subRegion, [&]( auto * const castedSubRegion )
      {
        lambda( esr, castedSubRegion );
      });
    }
  }


  struct viewKeyStruct : public ObjectManagerBase::viewKeyStruct
  {
    static constexpr auto materialListString = "materialList";
    static constexpr auto elementSubRegions = "elementSubRegions";
  };

  string_array & getMaterialList() {return m_materialList;}
  string_array const & getMaterialList() const {return m_materialList;}

protected:

private:

  ElementRegionBase& operator=(const ElementRegionBase& rhs);

  string_array m_materialList;
  string m_numericalMethod;

};



///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////



}



#endif /* ELEMENT_REGION_BASE_H_ */