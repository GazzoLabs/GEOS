/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 TotalEnergies
 * Copyright (c) 2019-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */


/**
 * @file AcousticFirstOrderWaveEquationSEM.hpp
 */

#ifndef GEOSX_PHYSICSSOLVERS_WAVEPROPAGATION__HPP_WAVESOLVERBASEFIELDS
#define GEOSX_PHYSICSSOLVERS_WAVEPROPAGATION__HPP_WAVESOLVERBASEFIELDS

#include "common/DataLayouts.hpp"
#include "mesh/MeshFields.hpp"

namespace geosx
{

namespace fields
{

namespace wavesolverfields
{

DECLARE_FIELD( Pressure_np1,
               "pressure_np1",
               array1d< real32 >,
               0,
               LEVEL_0,
               WRITE_AND_READ,
               "Scalar pressure at time n+1." );

DECLARE_FIELD( Velocity_x,
               "velocity_x",
               array2d< real32 >,
               0,
               LEVEL_0,
               WRITE_AND_READ,
               "Velocity in the x-direction." );

DECLARE_FIELD( Velocity_y,
               "velocity_y",
               array2d< real32 >,
               0,
               LEVEL_0,
               WRITE_AND_READ,
               "Velocity in the y-direction." );

DECLARE_FIELD( Velocity_z,
               "velocity_z",
               array2d< real32 >,
               0,
               LEVEL_0,
               WRITE_AND_READ,
               "Velocity in the z-direction." );

DECLARE_FIELD( ForcingRHS,
               "rhs",
               array1d< real32 >,
               0,
               NOPLOT,
               WRITE_AND_READ,
               "RHS" );

DECLARE_FIELD( MassVector,
               "massVector",
               array1d< real32 >,
               0,
               NOPLOT,
               WRITE_AND_READ,
               "Diagonal of the Mass Matrix." );

DECLARE_FIELD( DampingVector,
               "dampingVector",
               array1d< real32 >,
               0,
               NOPLOT,
               WRITE_AND_READ,
               "Diagonal of the Damping Matrix." );

DECLARE_FIELD( MediumVelocity,
               "mediumVelocity",
               array1d< real32 >,
               0,
               NOPLOT,
               WRITE_AND_READ,
               "Medium velocity of the cell" );
DECLARE_FIELD( MediumDensity,
               "mediumDensity",
               array1d< real32 >,
               0,
               NOPLOT,
               WRITE_AND_READ,
               "Medium density of the cell" );

DECLARE_FIELD( FreeSurfaceFaceIndicator,
               "freeSurfaceFaceIndicator",
               array1d< localIndex >,
               0,
               NOPLOT,
               WRITE_AND_READ,
               "Free surface indicator, 1 if a face is on free surface 0 otherwise." );

DECLARE_FIELD( FreeSurfaceNodeIndicator,
               "freeSurfaceNodeIndicator",
               array1d< localIndex >,
               0,
               NOPLOT,
               WRITE_AND_READ,
               "Free surface indicator, 1 if a node is on free surface 0 otherwise." );


}

}

} /* namespace geosx */

#endif /* GEOSX_PHYSICSSOLVERS_WAVEPROPAGATION_AcousticFirstOrderWaveEquationSEM_HPP_ */
