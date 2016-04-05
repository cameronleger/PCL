//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.01.0784
// ----------------------------------------------------------------------------
// Standard INDIClient Process Module Version 01.00.03.0102
// ----------------------------------------------------------------------------
// INDICCDFrameParameters.h - Released 2016/03/18 13:15:37 UTC
// ----------------------------------------------------------------------------
// This file is part of the standard INDIClient PixInsight module.
//
// Copyright (c) 2014-2016 Klaus Kretzschmar
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

#ifndef __INDICCDFrameParameters_h
#define __INDICCDFrameParameters_h

#include <pcl/MetaParameter.h>

namespace pcl
{

PCL_BEGIN_LOCAL

// ----------------------------------------------------------------------------

class ICFDeviceName : public MetaString
{
public:

   ICFDeviceName( MetaProcess* );

   virtual IsoString Id() const;
};

extern ICFDeviceName* TheICFDeviceNameParameter;

// ----------------------------------------------------------------------------

class ICFUploadMode : public MetaEnumeration
{
public:

   enum { UploadClient,
          UploadServer,
          UploadServerAndClient,
          NumberOfUploadModes,
          Default = UploadClient };

   ICFUploadMode( MetaProcess* );

   virtual IsoString Id() const;
   virtual size_type NumberOfElements() const;
   virtual IsoString ElementId( size_type ) const;
   virtual int ElementValue( size_type ) const;
   virtual size_type DefaultValueIndex() const;
};

extern ICFUploadMode* TheICFUploadModeParameter;

// ----------------------------------------------------------------------------

class ICFServerUploadDirectory : public MetaString
{
public:

   ICFServerUploadDirectory( MetaProcess* );

   virtual IsoString Id() const;
};

extern ICFServerUploadDirectory* TheICFServerUploadDirectoryParameter;

// ----------------------------------------------------------------------------

class ICFServerFileNameTemplate : public MetaString
{
public:

   ICFServerFileNameTemplate( MetaProcess* );

   virtual IsoString Id() const;
};

extern ICFServerFileNameTemplate* TheICFServerFileNameTemplateParameter;

// ----------------------------------------------------------------------------

class ICFFrameType : public MetaEnumeration
{
public:

   enum { LightFrame,
          BiasFrame,
          DarkFrame,
          FlatFrame,
          NumberOfFrameTypes,
          Default = LightFrame };

   ICFFrameType( MetaProcess* );

   virtual IsoString Id() const;
   virtual size_type NumberOfElements() const;
   virtual IsoString ElementId( size_type ) const;
   virtual int ElementValue( size_type ) const;
   virtual size_type DefaultValueIndex() const;
};

extern ICFFrameType* TheICFFrameTypeParameter;

// ----------------------------------------------------------------------------

class ICFBinningX : public MetaInt32
{
public:

   ICFBinningX( MetaProcess* );

   virtual IsoString Id() const;
   virtual double DefaultValue() const;
   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
};

extern ICFBinningX* TheICFBinningXParameter;

// ----------------------------------------------------------------------------

class ICFBinningY : public MetaInt32
{
public:

   ICFBinningY( MetaProcess* );

   virtual IsoString Id() const;
   virtual double DefaultValue() const;
   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
};

extern ICFBinningY* TheICFBinningYParameter;

// ----------------------------------------------------------------------------

class ICFExposureTime : public MetaDouble
{
public:

   ICFExposureTime( MetaProcess* );

   virtual IsoString Id() const;
   virtual int Precision() const;
   virtual double DefaultValue() const;
   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
};

extern ICFExposureTime* TheICFExposureTimeParameter;

// ----------------------------------------------------------------------------

class ICFExposureDelay : public MetaDouble
{
public:

   ICFExposureDelay( MetaProcess* );

   virtual IsoString Id() const;
   virtual int Precision() const;
   virtual double DefaultValue() const;
   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
};

extern ICFExposureDelay* TheICFExposureDelayParameter;

// ----------------------------------------------------------------------------

class ICFExposureCount : public MetaInt32
{
public:

   ICFExposureCount( MetaProcess* );

   virtual IsoString Id() const;
   virtual double DefaultValue() const;
   virtual double MinimumValue() const;
   virtual double MaximumValue() const;
};

extern ICFExposureCount* TheICFExposureCountParameter;

// ----------------------------------------------------------------------------

class ICFNewImageIdTemplate : public MetaString
{
public:

   ICFNewImageIdTemplate( MetaProcess* );

   virtual IsoString Id() const;
   virtual String DefaultValue() const;
};

extern ICFNewImageIdTemplate* TheICFNewImageIdTemplateParameter;

// ----------------------------------------------------------------------------

PCL_END_LOCAL

} // pcl

#endif   // __INDICCDFrameParameters_h

// ----------------------------------------------------------------------------
// EOF INDICCDFrameParameters.h - Released 2016/03/18 13:15:37 UTC