// ****************************************************************************
// PixInsight Class Library - PCL 02.00.02.0584
// Standard PixInsightINDI Process Module Version 01.00.02.0092
// ****************************************************************************
// PixInsightINDIInterface.cpp - Released 2013/03/24 18:42:27 UTC
// ****************************************************************************
// This file is part of the standard PixInsightINDI PixInsight module.
//
// Copyright (c) 2003-2013, Pleiades Astrophoto S.L. All Rights Reserved.
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
// ****************************************************************************

#include "PixInsightINDIInterface.h"
#include "PixInsightINDIParameters.h"
#include "PixInsightINDIProcess.h"
#include "PixInsightINDIclient.h"
#include "basedevice.h"
#include "indibase.h"
#include "indiapi.h"
#include <pcl/Console.h>
#include <assert.h>

#include "PropertyNode.h"

namespace pcl
{

// ----------------------------------------------------------------------------

PixInsightINDIInterface* ThePixInsightINDIInterface = 0;

// ----------------------------------------------------------------------------

//#include "PixInsightINDIIcon.xpm"

// ----------------------------------------------------------------------------

PixInsightINDIInterface::PixInsightINDIInterface() :
ProcessInterface(), instance( ThePixInsightINDIProcess ), GUI( 0 ), m_treeBoxDeviceNodes(), m_serverMessage(""),m_numOfDevices(0), m_PropertyListNeedsUpdate(false)
{
   ThePixInsightINDIInterface = this;
}

PixInsightINDIInterface::~PixInsightINDIInterface()
{
   if ( GUI != 0 )
      delete GUI, GUI = 0;
}

IsoString PixInsightINDIInterface::Id() const
{
   return "PixInsightINDI";
}

MetaProcess* PixInsightINDIInterface::Process() const
{
   return ThePixInsightINDIProcess;
}

const char** PixInsightINDIInterface::IconImageXPM() const
{
   return 0; // PixInsightINDIIcon_XPM; ---> put a nice icon here
}

InterfaceFeatures PixInsightINDIInterface::Features() const
{
	return   InterfaceFeature::DragObject | InterfaceFeature::ApplyGlobalButton | InterfaceFeature::ApplyToViewButton | InterfaceFeature::BrowseDocumentationButton | InterfaceFeature::ResetButton;
}

void PixInsightINDIInterface::ApplyInstance() const
{
   instance.LaunchOnCurrentView();
}

void PixInsightINDIInterface::ApplyInstanceGlobal() const
{
   instance.LaunchGlobal();
}

void PixInsightINDIInterface::ResetInstance()
{
   PixInsightINDIInstance defaultInstance( ThePixInsightINDIProcess );
   ImportProcess( defaultInstance );
}

bool PixInsightINDIInterface::Launch( const MetaProcess& P, const ProcessImplementation*, bool& dynamic, unsigned& /*flags*/ )
{
   if ( GUI == 0 )
   {
      GUI = new GUIData( *this );
      SetWindowTitle( "PixInsightINDI" );
      UpdateControls();
   }

   dynamic = false;
   return &P == ThePixInsightINDIProcess;
}

ProcessImplementation* PixInsightINDIInterface::NewProcess() const
{
   return new PixInsightINDIInstance( instance );
}

bool PixInsightINDIInterface::ValidateProcess( const ProcessImplementation& p, String& whyNot ) const
{
   const PixInsightINDIInstance* r = dynamic_cast<const PixInsightINDIInstance*>( &p );
   if ( r == 0 )
   {
      whyNot = "Not a PixInsightINDI instance.";
      return false;
   }

   whyNot.Clear();
   return true;
}

bool PixInsightINDIInterface::RequiresInstanceValidation() const
{
   return true;
}

bool PixInsightINDIInterface::ImportProcess( const ProcessImplementation& p )
{
   instance.Assign( p );
   UpdateControls();
   return true;
}

// ----------------------------------------------------------------------------

void PixInsightINDIInterface::UpdateControls()
{
   GUI->ParameterPort_SpinBox.SetValue( instance.p_port );
   GUI->ParameterHost_Edit.SetText( instance.p_host );
}

// ----------------------------------------------------------------------------


void PixInsightINDIInterface::__IntegerValueUpdated( SpinBox& sender, int value )
{
   if ( sender == GUI->ParameterPort_SpinBox )
      instance.p_port = value;
}


void PixInsightINDIInterface::__EditGetFocus( Control& sender )
{
   if ( sender == GUI->ParameterHost_Edit )
   {
      // If you need to do something when sender gets focus, do it here.
   }
}

void PixInsightINDIInterface::__EditCompleted( Edit& sender )
{
   if ( sender == GUI->ParameterHost_Edit )
      instance.p_host = sender.Text();
}

void PixInsightINDIInterface::__CameraListButtons_Click( Button& sender, bool checked )
{
        
        if ( sender == GUI->ConnectServer_PushButton )
        {
            try
            {
				if (indiClient.get() == 0)
					indiClient.reset(new INDIClient(&instance));

				IsoString ASCIIHost(instance.p_host);
				indiClient->setServer(ASCIIHost.c_str() , instance.p_port);

				bool connected=false;

				connected = indiClient->connectServer();

				if (connected){
					GUI->UpdateDeviceList_Timer.Start();
					GUI->UpdateServerMessage_Timer.Start();
				}
            }
            ERROR_HANDLER
        }
		else if ( sender == GUI->DisconnectServer_PushButton )
		{
			try
            {
                if (indiClient.get() == 0)
					indiClient.reset(new INDIClient(&instance));

				if (indiClient->serverIsConnected())
					indiClient->disconnectServer();

				//if (!indiClient->serverIsConnected())
				//	Console() <<"Successfully disconnected from server \n";
				GUI->UpdateDeviceList_Timer.Stop();
			
				GUI->UpdateServerMessage_Timer.Stop();

				GUI->DeviceList_TreeBox.Clear();
				// clear property list
				GUI->PropertyList_TreeBox.Clear();
            }
            ERROR_HANDLER
		}
		else if ( sender == GUI->ConnectDevice_PushButton)
		{
			try
            {
				pcl::IndirectArray<pcl::TreeBox::Node> selectedNodes;
				GUI->DeviceList_TreeBox.GetSelectedNodes(selectedNodes);

				for (pcl::IndirectArray<pcl::TreeBox::Node>::iterator it=selectedNodes.Begin(); it!=selectedNodes.End();++it){
					IsoString deviceName((*it)->Text(1).To7BitASCII());
					
					INDI::BaseDevice* device = indiClient->getDevice(deviceName.c_str());

					if (device)
						if (!device->isConnected())
							indiClient->connectDevice(deviceName.c_str());	

					if (device->isConnected())
						(*it)->Check();
					
				}
				
			}
            ERROR_HANDLER
		}
		else if ( sender == GUI->DisconnectDevice_PushButton)
		{
			try
			{
				pcl::IndirectArray<pcl::TreeBox::Node> selectedNodes;
				GUI->DeviceList_TreeBox.GetSelectedNodes(selectedNodes);

				for (pcl::IndirectArray<pcl::TreeBox::Node>::iterator it=selectedNodes.Begin(); it!=selectedNodes.End();++it){
					IsoString deviceName((*it)->Text(1).To7BitASCII());

					INDI::BaseDevice* device = indiClient->getDevice(deviceName.c_str());

					if (device)
						if (device->isConnected())
							indiClient->disconnectDevice(deviceName.c_str());	

					// uncheck 
					if (!device->isConnected())
						(*it)->Check(false);
				}
			}
            ERROR_HANDLER
		}
		else if ( sender == GUI->RefreshDevice_PushButton)
		{
			try
            {
				UpdateDeviceList();
			}
            ERROR_HANDLER
		}
        
}


void PixInsightINDIInterface::UpdateDeviceList(){

	GUI->DeviceList_TreeBox.DisableUpdates();
	GUI->DeviceList_TreeBox.Clear();

	if (indiClient.get() == 0)
		return;

	if (instance.p_deviceList.Begin()==instance.p_deviceList.End())
		return;

	for (PixInsightINDIInstance::DeviceListType::iterator iter=instance.p_deviceList.Begin() ; iter!=instance.p_deviceList.End(); ++iter){

		PropertyNode* rootNode = new PropertyNode(GUI->DeviceList_TreeBox);
		PropertyNode* deviceNode = new PropertyNode(rootNode,IsoString(iter->DeviceName));
		deviceNode->getTreeBoxNode()->SetText( 1, iter->DeviceName );
		deviceNode->getTreeBoxNode()->SetAlignment( 1, TextAlign::Left );
		deviceNode->getTreeBoxNode()->Check(false);
		deviceNode->getTreeBoxNode()->SetAlignment( 0, TextAlign::Left );
	}
	GUI->UpdateDeviceList_Timer.Stop();
	GUI->DeviceList_TreeBox.EnableUpdates();
}
// ----------------------------------------------------------------------------

PixInsightINDIInterface::GUIData::GUIData( PixInsightINDIInterface& w )
{
   SetPropDlg.setInstance(&w.instance);
   SetPropDlg.setInterface(&w);
   pcl::Font fnt = w.Font();
   int labelWidth1 = fnt.Width( String( "Three:" ) ); // the longest label text
   int editWidth1 = fnt.Width( String( '0',14 ) );

   INDIServer_SectionBar.SetTitle("INDI Server Connection");
   INDIServer_SectionBar.SetSection(INDIServerConnection_Control);
   INDIServerConnection_Control.SetSizer(ParameterHost_Sizer);
   ParameterPort_Label.SetText( "Port:" );
   ParameterPort_Label.SetMinWidth( labelWidth1 );
   ParameterPort_Label.SetToolTip( "<p>INDI server port.</p>" );
   ParameterPort_Label.SetTextAlignment( TextAlign::Right|TextAlign::VertCenter );

   ParameterPort_SpinBox.SetRange( int( TheINDIServerPort->MinimumValue() ), int( TheINDIServerPort->MaximumValue() ) );
   ParameterPort_SpinBox.SetFixedWidth( editWidth1 );
   ParameterPort_SpinBox.SetToolTip( "<p>INDI server port.</p>" );
   ParameterPort_SpinBox.OnValueUpdated( (SpinBox::value_event_handler)&PixInsightINDIInterface::__IntegerValueUpdated, w );
   
   ParameterHost_Label.SetText( "Host:" );
   ParameterHost_Label.SetMinWidth( labelWidth1 );
   ParameterHost_Label.SetToolTip( "<p>INDI server host.</p>" );
   ParameterHost_Label.SetTextAlignment( TextAlign::Right|TextAlign::VertCenter );

   ParameterHost_Edit.SetMinWidth( editWidth1 );
   ParameterHost_Edit.OnGetFocus( (Control::event_handler)&PixInsightINDIInterface::__EditGetFocus, w );
   ParameterHost_Edit.OnEditCompleted( (Edit::edit_event_handler)&PixInsightINDIInterface::__EditCompleted, w );
   
   ParameterPort_Label.SetText( "Port:" );
   ParameterPort_Label.SetMinWidth( labelWidth1 );
   ParameterPort_Label.SetToolTip( "<p>INDI server port.</p>" );
   ParameterPort_Label.SetTextAlignment( TextAlign::Right|TextAlign::VertCenter );

   ParameterPort_SpinBox.SetRange( int( TheINDIServerPort->MinimumValue() ), int( TheINDIServerPort->MaximumValue() ) );
   ParameterPort_SpinBox.SetFixedWidth( editWidth1 );
   ParameterPort_SpinBox.SetToolTip( "<p>INDI server port.</p>" );
   ParameterPort_SpinBox.OnValueUpdated( (SpinBox::value_event_handler)&PixInsightINDIInterface::__IntegerValueUpdated, w );

   
   ParameterHost_Sizer.SetSpacing( 4 );
   ParameterHost_Sizer.Add( ParameterHost_Label );
   ParameterHost_Sizer.Add( ParameterHost_Edit, 100);
   ParameterHost_Sizer.Add( ParameterPort_Label );
   ParameterHost_Sizer.Add( ParameterPort_SpinBox );
   ParameterHost_Sizer.Add( ConnectionServer_Sizer );

   //pushbuttons
   ConnectionServer_Sizer.SetSpacing(4);
   ConnectServer_PushButton.SetText( "Connect" );
   ConnectServer_PushButton.OnClick( (Button::click_event_handler) &PixInsightINDIInterface::__CameraListButtons_Click, w );
   DisconnectServer_PushButton.SetText( "Disconnect" );
   DisconnectServer_PushButton.OnClick( (Button::click_event_handler) &PixInsightINDIInterface::__CameraListButtons_Click, w );

  
   ConnectionServer_Sizer.Add(ConnectServer_PushButton);
   ConnectionServer_Sizer.Add(DisconnectServer_PushButton);
   ConnectionServer_Sizer.AddStretch();

   // INDI devices
   INDIDevices_SectionBar.SetTitle("INDI Devices");
   INDIDevices_SectionBar.SetSection(INDIDevices_Control);
   INDIDevices_Control.SetSizer(INDIDevice_Sizer);
   DeviceList_TreeBox.SetMinHeight( 8*fnt.Height() +2 );
   DeviceList_TreeBox.EnableAlternateRowColor();
   DeviceList_TreeBox.EnableMultipleSelections();
   DeviceList_TreeBox.SetNumberOfColumns(2);
   DeviceList_TreeBox.SetHeaderText(0,"Status");
   DeviceList_TreeBox.SetHeaderText(1,"Device");



   DeviceAction_Sizer.SetSpacing(4);
   ConnectDevice_PushButton.SetText("Connect");
   ConnectDevice_PushButton.OnClick((Button::click_event_handler) &PixInsightINDIInterface::__CameraListButtons_Click, w );
   DisconnectDevice_PushButton.SetText("Disconnect");
   DisconnectDevice_PushButton.OnClick((Button::click_event_handler) &PixInsightINDIInterface::__CameraListButtons_Click, w );
   RefreshDevice_PushButton.SetText("Refresh");
   RefreshDevice_PushButton.OnClick((Button::click_event_handler) &PixInsightINDIInterface::__CameraListButtons_Click, w );

   DeviceAction_Sizer.Add(ConnectDevice_PushButton);
   DeviceAction_Sizer.Add(DisconnectDevice_PushButton);
   DeviceAction_Sizer.Add(RefreshDevice_PushButton);
   DeviceAction_Sizer.AddStretch();

   INDIDevice_Sizer.Add(DeviceList_TreeBox);
   INDIDevice_Sizer.Add(DeviceAction_Sizer);

   int width = fnt.Width( String( '0',70 ) );

   INDIProperties_SectionBar.SetTitle("INDI Device Properties");
   INDIProperties_SectionBar.SetSection(INDIProperties_Control);
   INDIProperties_Control.SetSizer(INDIDeviceProperty_Sizer);
   PropertyList_TreeBox.SetMinHeight( 26*fnt.Height() +2 );
   PropertyList_TreeBox.SetMinWidth(width);
   PropertyList_TreeBox.EnableAlternateRowColor();
   PropertyList_TreeBox.SetNumberOfColumns(3);
   PropertyList_TreeBox.HideColumn(2);
   PropertyList_TreeBox.SetColumnWidth(0,300);
   PropertyList_TreeBox.SetHeaderText(0,String("Property"));
   PropertyList_TreeBox.SetHeaderText(1,String("Value"));

   ServerMessage_Label.SetVariableWidth();
   ServerMessage_Label.SetTextAlignment( TextAlign::Left|TextAlign::VertCenter );
   ServerMessageLabel_Label.SetText("Last server message:");
   ServerMessageLabel_Label.SetTextAlignment( TextAlign::Left|TextAlign::VertCenter );

   ServerMessage_Sizer.Add(ServerMessageLabel_Label);
   ServerMessage_Sizer.Add(ServerMessage_Label);

   RefreshProperty_PushButton.SetText("Refresh");
   RefreshProperty_PushButton.OnClick((Button::click_event_handler) &PixInsightINDIInterface::PropertyButton_Click, w );
   EditProperty_PushButton.SetText("Edit");
   EditProperty_PushButton.OnClick((Button::click_event_handler) &PixInsightINDIInterface::PropertyButton_Click, w );

   INDIDeviceProperty_Sizer.Add(PropertyList_TreeBox);
   

   Buttons_Sizer.SetSpacing(4);
   Buttons_Sizer.AddSpacing(10);
   Buttons_Sizer.Add(RefreshProperty_PushButton);
   Buttons_Sizer.Add(EditProperty_PushButton);
   Buttons_Sizer.AddStretch();

   INDIDeviceProperty_Sizer.Add(Buttons_Sizer);
   //INDIDeviceProperty_Sizer.Add(DeviceMessage_Label);

   Global_Sizer.SetMargin( 8 );
   Global_Sizer.SetSpacing( 6 );
   Global_Sizer.Add(INDIServer_SectionBar);	
   Global_Sizer.Add(INDIServerConnection_Control);
   Global_Sizer.Add(INDIDevices_SectionBar);
   Global_Sizer.Add(INDIDevices_Control);
   Global_Sizer.Add(INDIProperties_SectionBar);
   Global_Sizer.Add(INDIProperties_Control);
   Global_Sizer.Add(ServerMessage_Sizer);

   UpdateDeviceList_Timer.SetInterval( 0.5 );
   UpdateDeviceList_Timer.SetPeriodic( true );
   UpdateDeviceList_Timer.OnTimer( (Timer::timer_event_handler)&PixInsightINDIInterface::__UpdateDeviceList_Timer, w );

   UpdateServerMessage_Timer.SetInterval( 0.5 );
   UpdateServerMessage_Timer.SetPeriodic( true );
   UpdateServerMessage_Timer.OnTimer( (Timer::timer_event_handler)&PixInsightINDIInterface::__UpdateServerMessage_Timer, w );

   

   w.SetSizer( Global_Sizer );
   w.AdjustToContents();
   w.SetFixedSize();
}


void PixInsightINDIInterface::__UpdateDeviceList_Timer( Timer &sender )
  {
	  
	  if( sender == GUI->UpdateDeviceList_Timer  ){
		  UpdateDeviceList();
	  }
	  
  }

void PixInsightINDIInterface::__UpdateServerMessage_Timer( Timer &sender )
  {

	  if( sender == GUI->UpdateServerMessage_Timer  ){
		  //GUI->ServerMessage_Label.SetText(instance.p_currentMessage);
	  }

  }

// ----------------------------------------------------------------------------

void SetPropertyDialog::EditCompleted( Edit& sender )
{
   if ( sender == Property_Edit ){
	   m_newPropertyListItem.NewPropertyValue=sender.Text();
   }
}

SetPropertyDialog::SetPropertyDialog():Dialog(),m_instance(NULL){
	pcl::Font fnt = Font();
	int labelWidth = fnt.Width( String( '0',30 ) );
	int editWidth = fnt.Width( String( '0',10 ) );

	SetWindowTitle(String("INDI property value"));

	Property_Label.SetMinWidth(labelWidth);
	Property_Label.SetTextAlignment( TextAlign::Left|TextAlign::VertCenter );

	Property_Edit.SetMinWidth(editWidth);
	Property_Edit.OnEditCompleted( (Edit::edit_event_handler)&SetPropertyDialog::EditCompleted, *this );
   
	OK_PushButton.SetText("OK");
	OK_PushButton.OnClick((Button::click_event_handler) &SetPropertyDialog::Button_Click, *this );
	Cancel_PushButton.SetText("Cancel");
	Cancel_PushButton.OnClick((Button::click_event_handler) &SetPropertyDialog::Button_Click, *this );

	Property_Sizer.SetMargin(10);
	Property_Sizer.SetSpacing(4);
	Property_Sizer.Add(Property_Label);
	Property_Sizer.Add(Property_Edit);
	Property_Sizer.AddStretch();
	
	
	Buttons_Sizer.SetSpacing(4);
	Buttons_Sizer.AddSpacing(10);
	Buttons_Sizer.AddStretch();
	Buttons_Sizer.Add(OK_PushButton);
	Buttons_Sizer.AddStretch();
	Buttons_Sizer.Add(Cancel_PushButton);
	Buttons_Sizer.AddStretch();

	Global_Sizer.Add(Property_Sizer);
	Global_Sizer.Add(Buttons_Sizer);

	SetSizer(Global_Sizer);

}



void PixInsightINDIInterface::UpdatePropertyList(){

	GUI->PropertyList_TreeBox.DisableUpdates();
	GUI->PropertyList_TreeBox.Clear();

	if (indiClient.get() == 0)
		return;

	GUI->ServerMessage_Label.SetText(m_serverMessage);

	if (instance.p_propertyList.Begin()==instance.p_propertyList.End())
		return;

	PropertyNodeMapType deviceNodeMap;
	PropertyNodeMapType propertyNodeMap;
	
	for (PixInsightINDIInstance::PropertyListType::iterator iter=instance.p_propertyList.Begin() ; iter!=instance.p_propertyList.End(); ++iter){

		// create device nodes
		PropertyNodeMapType::iterator deviceIter = deviceNodeMap.find(iter->Device);
		PropertyNode* deviceNode;
		if (deviceIter!= deviceNodeMap.end()){
			deviceNode=deviceIter->second;
		} else
		{	// create root node
			PropertyNode* rootNode=new PropertyNode(GUI->PropertyList_TreeBox);
			this->m_treeBoxDeviceNodes.push_back(rootNode);
			PropertyNode* child = new PropertyNode(rootNode,iter->Device);
			deviceNode = child;
			deviceNode->getTreeBoxNode()->SetText(0,iter->Device);
			deviceNode->getTreeBoxNode()->SetAlignment(0, TextAlign::Left);
			deviceNodeMap[iter->Device]=deviceNode;
		}
		// create property nodes
		PropertyNodeMapType::iterator propertyIter = propertyNodeMap.find(iter->Device+iter->Property);
		PropertyNode* propertyNode;
		if (propertyIter!= propertyNodeMap.end()){
			propertyNode=propertyIter->second;
		} else
		{
			PropertyNode* child = new PropertyNode(deviceNode,iter->Device,iter->Property);
			propertyNode = child;
			propertyNode->getTreeBoxNode()->SetText(0,iter->Property);
			propertyNode->getTreeBoxNode()->SetAlignment(0, TextAlign::Left);
			propertyNodeMap[iter->Device+iter->Property]=propertyNode;
		}
		// create element nodes
		PropertyNode* elementNode =  PropertyNode::create(propertyNode,iter->Device,iter->Property,iter->Element, iter->PropertyTypeStr);
		assert(elementNode!=NULL);
		elementNode->getTreeBoxNode()->SetText(0,iter->Element);
		elementNode->getTreeBoxNode()->SetAlignment(0, TextAlign::Left);
		elementNode->getTreeBoxNode()->SetText(1,iter->PropertyValue);
		elementNode->getTreeBoxNode()->SetAlignment(1, TextAlign::Left);
		elementNode->getTreeBoxNode()->SetText(2,iter->PropertyTypeStr);
	}
	
	GUI->PropertyList_TreeBox.EnableUpdates();
}

void SetPropertyDialog::Button_Click( Button& sender, bool checked ){

	if (indiClient.get() == 0)
		return;

	assert(m_instance!=NULL);

	if ( sender == OK_PushButton )
	{

		INDINewPropertyListItem newPropertyListItem=getNewPropertyListItem();
		m_instance->sendNewPropertyValue(newPropertyListItem);
		IPropertyVisitor* pVisitor = UpdateVisitor::create();
		std::vector<PropertyNode*>* rootNodes=m_interface->getPropertyTreeRootNodes();
		for (std::vector<PropertyNode*>::iterator it=rootNodes->begin(); it!=rootNodes->end(); ++it){
			(*it)->accept(pVisitor, newPropertyListItem.PropertyKey, newPropertyListItem.NewPropertyValue);
		}
		Ok();
	}
	else if (sender == Cancel_PushButton){
		Cancel();
	}
}

void PixInsightINDIInterface::PropertyButton_Click( Button& sender, bool checked ){
	
	if (sender==GUI->EditProperty_PushButton){
		pcl::IndirectArray<pcl::TreeBox::Node> selectedNodes;
		GUI->PropertyList_TreeBox.GetSelectedNodes(selectedNodes);

		for (pcl::IndirectArray<pcl::TreeBox::Node>::iterator it=selectedNodes.Begin(); it!=selectedNodes.End();++it){
			GUI->SetPropDlg.setPropertyLabelString((*it)->Text(0));
			GUI->SetPropDlg.setPropertyValueString((*it)->Text(1));
			INDINewPropertyListItem propItem;
			propItem.Device=(*it)->Parent()->Parent()->Text(0);
			propItem.Property=(*it)->Parent()->Text(0);
			propItem.Element=(*it)->Text(0);
			propItem.NewPropertyValue=(*it)->Text(1);
			propItem.PropertyType=(*it)->Text(2);
			propItem.PropertyKey=PropertyUtils::getKey(propItem.Device,propItem.Property,propItem.Element);
			GUI->SetPropDlg.setNewPropertyListItem(propItem);
		}
		GUI->SetPropDlg.Execute();
	}
	else if (sender==GUI->RefreshProperty_PushButton){
		UpdatePropertyList();
	}
}

} // pcl

// ****************************************************************************
// EOF PixInsightINDIInterface.cpp - Released 2013/03/24 18:42:27 UTC
