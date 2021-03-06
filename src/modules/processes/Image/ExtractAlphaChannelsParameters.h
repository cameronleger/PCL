//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.07.0873
// ----------------------------------------------------------------------------
// Standard Image Process Module Version 01.02.09.0402
// ----------------------------------------------------------------------------
// ExtractAlphaChannelsParameters.h - Released 2017-08-01T14:26:58Z
// ----------------------------------------------------------------------------
// This file is part of the standard Image PixInsight module.
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

#ifndef __ExtractAlphaChannelsParameters_h
#define __ExtractAlphaChannelsParameters_h

#include <pcl/MetaParameter.h>

namespace pcl
{

PCL_BEGIN_LOCAL

// ----------------------------------------------------------------------------

class EAChannels : public MetaEnumeration
{
public:

   enum { AllAlphaChannels,   // All alpha channels
          ActiveAlphaChannel, // Active (first) alpha channel only
          ChannelList,        // Act on channels specified through a list
          NumberOfModes,
          Default = AllAlphaChannels };

   EAChannels( MetaProcess* );

   virtual IsoString Id() const;

   virtual size_type NumberOfElements() const;

   virtual IsoString ElementId( size_type ) const;
   virtual int ElementValue( size_type ) const;

   virtual size_type DefaultValueIndex() const;
};

extern EAChannels* TheEAChannelsParameter;

// ----------------------------------------------------------------------------

class EAChannelList : public MetaString
{
public:

   EAChannelList( MetaProcess* );

   virtual String DefaultValue() const;

   virtual IsoString Id() const;
   virtual String AllowedCharacters() const;
};

extern EAChannelList* TheEAChannelListParameter;

// ----------------------------------------------------------------------------

class EAExtract : public MetaBoolean
{
public:

   EAExtract( MetaProcess* );

   virtual IsoString Id() const;
   virtual bool DefaultValue() const;
};

extern EAExtract* TheEAExtractParameter;

// ----------------------------------------------------------------------------

class EADelete : public MetaBoolean
{
public:

   EADelete( MetaProcess* );

   virtual IsoString Id() const;
   virtual bool DefaultValue() const;
};

extern EADelete* TheEADeleteParameter;

// ----------------------------------------------------------------------------

PCL_END_LOCAL

} // pcl

#endif   // __ExtractAlphaChannelsParameters_h

// ----------------------------------------------------------------------------
// EOF ExtractAlphaChannelsParameters.h - Released 2017-08-01T14:26:58Z
