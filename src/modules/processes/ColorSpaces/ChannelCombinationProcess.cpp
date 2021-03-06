//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.07.0873
// ----------------------------------------------------------------------------
// Standard ColorSpaces Process Module Version 01.01.00.0348
// ----------------------------------------------------------------------------
// ChannelCombinationProcess.cpp - Released 2017-08-01T14:26:57Z
// ----------------------------------------------------------------------------
// This file is part of the standard ColorSpaces PixInsight module.
//
// Copyright (c) 2003-2017 Pleiades Astrophoto S.L. All Rights Reserved.
//
// Redistribution and use in both source and binary forms, with or without
// modification, is permitted provided that the following conditions are met:
//
// 1. All redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. All redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. Neither the names "PixInsight" and "Pleiades Astrophoto", nor the names
//    of their contributors, may be used to endorse or promote products derived
//    from this software without specific prior written permission. For written
//    permission, please contact info@pixinsight.com.
//
// 4. All products derived from this software, in any form whatsoever, must
//    reproduce the following acknowledgment in the end-user documentation
//    and/or other materials provided with the product:
//
//    "This product is based on software from the PixInsight project, developed
//    by Pleiades Astrophoto and its contributors (http://pixinsight.com/)."
//
//    Alternatively, if that is where third-party acknowledgments normally
//    appear, this acknowledgment must be reproduced in the product itself.
//
// THIS SOFTWARE IS PROVIDED BY PLEIADES ASTROPHOTO AND ITS CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL PLEIADES ASTROPHOTO OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, BUSINESS
// INTERRUPTION; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; AND LOSS OF USE,
// DATA OR PROFITS) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------

#include "ChannelCombinationProcess.h"
#include "ChannelCombinationParameters.h"
#include "ChannelCombinationInstance.h"
#include "ChannelCombinationInterface.h"

namespace pcl
{

// ----------------------------------------------------------------------------

ChannelCombinationProcess* TheChannelCombinationProcess = 0;

// ----------------------------------------------------------------------------

#include "ChannelCombinationIcon.xpm"

// ----------------------------------------------------------------------------

ChannelCombinationProcess::ChannelCombinationProcess() : MetaProcess()
{
   TheChannelCombinationProcess = this;

   // Instantiate process parameters
   TheColorSpaceIdCombinationParameter = new ColorSpaceId( this );
   TheChannelTableCombinationParameter = new ChannelTable( this );
   TheChannelEnabledCombinationParameter = new ChannelEnabled( TheChannelTableCombinationParameter );
   TheChannelIdCombinationParameter = new ChannelId( TheChannelTableCombinationParameter );
}

// ----------------------------------------------------------------------------

IsoString ChannelCombinationProcess::Id() const
{
   return "ChannelCombination";
}

// ----------------------------------------------------------------------------

IsoString ChannelCombinationProcess::Category() const
{
   return "ColorSpaces,ChannelManagement";
}

// ----------------------------------------------------------------------------

uint32 ChannelCombinationProcess::Version() const
{
   return 0x100;
}

// ----------------------------------------------------------------------------

String ChannelCombinationProcess::Description() const
{
   return "";
}

// ----------------------------------------------------------------------------

const char** ChannelCombinationProcess::IconImageXPM() const
{
   return ChannelCombinationIcon_XPM;
}

// ----------------------------------------------------------------------------

ProcessInterface* ChannelCombinationProcess::DefaultInterface() const
{
   return TheChannelCombinationInterface;
}

// -------------------------------------------------------------------------

ProcessImplementation* ChannelCombinationProcess::Create() const
{
   return new ChannelCombinationInstance( this );
}

// ----------------------------------------------------------------------------

ProcessImplementation* ChannelCombinationProcess::Clone( const ProcessImplementation& p ) const
{
   const ChannelCombinationInstance* instPtr = dynamic_cast<const ChannelCombinationInstance*>( &p );
   return (instPtr != 0) ? new ChannelCombinationInstance( *instPtr ) : 0;
}

// ----------------------------------------------------------------------------

bool ChannelCombinationProcess::NeedsValidation() const
{
   return true;
}

// ----------------------------------------------------------------------------

} // pcl

// ----------------------------------------------------------------------------
// EOF ChannelCombinationProcess.cpp - Released 2017-08-01T14:26:57Z
