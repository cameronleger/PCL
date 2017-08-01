//     ____   ______ __
//    / __ \ / ____// /
//   / /_/ // /    / /
//  / ____// /___ / /___   PixInsight Class Library
// /_/     \____//_____/   PCL 02.01.07.0873
// ----------------------------------------------------------------------------
// Standard INDIClient Process Module Version 01.00.15.0217
// ----------------------------------------------------------------------------
// indibase.h - Released 2017-08-01T14:26:58Z
// ----------------------------------------------------------------------------
// This file is part of the standard INDIClient PixInsight module.
//
// Copyright (c) 2014-2017 Klaus Kretzschmar
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

#ifndef INDIBASE_H
#define INDIBASE_H

#include "indiapi.h"
#include "indidevapi.h"

#define MAXRBUF 2048

/**
 * \namespace INDI
   \brief Namespace to encapsulate INDI client, drivers, and mediator classes.
   Developers can subclass the base devices class to implement device specific functionality. This ensures interoperability and consistency among devices within the same family
   and reduces code overhead.

   <ul>
   <li>BaseClient: Base class for INDI clients. By subclassing BaseClient, client can easily connect to INDI server
   and handle device communication, command, and notifcation.</li>
   <li>BaseMediator: Abstract class to provide interface for event notifications in INDI::BaseClient.</li>
   <li>BaseDriver: Base class for all INDI virtual driver as handled and stored in INDI::BaseClient.</li>
   <li>DefaultDriver: INDI::BaseDriver with extended functionality such as debug, simulation, and configuration support.
       It is \e only used by drivers directly, it cannot be used by clients.</li>
   <li>FilterInterface: Basic interface for filter wheels functions.</li>
   <li>GuiderInterface: Basic interface for guider (ST4) port functions.</li>
   <li>CCD: Base class for CCD drivers. Provides basic support for single chip CCD and CCDs with a guide head as well.</li>
   <li>Telescope: Base class for telescope drivers.</li>
   <li>FilterWheel: Base class for Filter Wheels. It implements the FilterInterface.</li>
   <li>Focuser: Base class for focusers.</li>
   <li>USBDevice: Base class for USB devices for direct read/write/control over USB.</li>
   </ul>
\author Jasem Mutlaq
\author Gerry Rozema
 */
namespace INDI
{
    class BaseMediator;
    class BaseClientImpl;
    class BaseClient;
    class BaseDevice;
    class DefaultDevice;
    class FilterInterface;
    class GuiderInterface;
    class CCD;
    class Telescope;
    class FilterWheel;
    class Focuser;
    class USBDevice;
    class Property;
}

/*! INDI property type */
typedef enum
{
    INDI_NUMBER, /*!< INumberVectorProperty. */
    INDI_SWITCH, /*!< ISwitchVectorProperty. */
    INDI_TEXT,   /*!< ITextVectorProperty. */
    INDI_LIGHT,  /*!< ILightVectorProperty. */
    INDI_BLOB,    /*!< IBLOBVectorProperty. */
    INDI_UNKNOWN
} INDI_TYPE;


/**
 * \class INDI::BaseMediator
   \brief Meditates event notification as generated by driver and passed to clients.
*/
class INDI::BaseMediator
{
public:

    /** \brief Emmited when a new device is created from INDI server.
        \param dp Pointer to the base device instance
    */
    virtual void newDevice(INDI::BaseDevice *dp)  =0;

    virtual void deleteDevice(INDI::BaseDevice *dp)  =0;

    /** \brief Emmited when a new property is created for an INDI driver.
        \param property Pointer to the Property Container

    */
    virtual void newProperty(INDI::Property *property) =0;


    /** \brief Emmited when a property is deleted for an INDI driver.
        \param property Pointer to the Property Container to remove.

    */
    virtual void removeProperty(INDI::Property *property) =0;


    /** \brief Emmited when a new BLOB value arrives from INDI server.
        \param bp Pointer to filled and process BLOB.
    */
    virtual void newBLOB(IBLOB *bp)=0;

    /** \brief Emmited when a new switch value arrives from INDI server.
        \param svp Pointer to a switch vector property.
    */
    virtual void newSwitch(ISwitchVectorProperty *svp) =0;

    /** \brief Emmited when a new number value arrives from INDI server.
        \param nvp Pointer to a number vector property.
    */
    virtual void newNumber(INumberVectorProperty *nvp) =0;

    /** \brief Emmited when a new text value arrives from INDI server.
        \param tvp Pointer to a text vector property.
    */
    virtual void newText(ITextVectorProperty *tvp) =0;

    /** \brief Emmited when a new light value arrives from INDI server.
        \param lvp Pointer to a light vector property.
    */
    virtual void newLight(ILightVectorProperty *lvp) =0;

    /** \brief Emmited when a new message arrives from INDI server.
        \param dp pointer to the INDI device the message is sent to.
        \param messageID ID of the message that can be used to retrieve the message from the device's messageQueue() function.
    */
    virtual void newMessage(INDI::BaseDevice *dp, int messageID) =0;

    /** \brief Emmited when the server is connected.
    */
    virtual void serverConnected() =0;

    /** \brief Emmited when the server gets disconnected.
        \param exit_code 0 if client was requested to disconnect from server. -1 if connection to server is terminated due to remote server disconnection.
    */
    virtual void serverDisconnected(int exit_code) =0;

    virtual ~BaseMediator() {}

};

#endif // INDIBASE_H

// ----------------------------------------------------------------------------
// EOF indibase.h - Released 2017-08-01T14:26:58Z
