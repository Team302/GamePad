
//====================================================================================================================================================
// Copyright 2020 Lake Orion Robotics FIRST Team 302 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//====================================================================================================================================================


// C++ Includes

// FRC includes

// Team 302 includes

// Third Party Includes
#include <string>
#include <frc/GenericHID.h>
#include <team302/gamepad/IDragonGamePad.h>
#include <team302/gamepad/DragonXBox.h>
#include <team302/gamepad/DragonGamePad.h>
#include <team302/gamepad/TeleopControl.h>
#include <frc/DriverStation.h>
#include <utils/Logger.h>

using namespace frc;
using namespace std;
using namespace team302::gamepad;

//----------------------------------------------------------------------------------
// Method:      GetInstance
// Description: If there isn't an instance of this class, it will create one.  The
//              single class instance will be returned.
// Returns:     OperatorInterface*  instance of this class
//----------------------------------------------------------------------------------
TeleopControl* TeleopControl::m_instance = nullptr; // initialize the instance variable to nullptr
TeleopControl* TeleopControl::GetInstance()
{
    if ( TeleopControl::m_instance == nullptr )
    {
        TeleopControl::m_instance = new TeleopControl();
    }
    return TeleopControl::m_instance;
}
//----------------------------------------------------------------------------------
// Method:      OperatorInterface <<constructor>>
// Description: This will construct and initialize the object.
//              It maps the functions to the buttons/axis.
//---------------------------------------------------------------------------------
TeleopControl::TeleopControl() : m_axisIDs(),
								 m_buttonIDs(),
								 m_controllerIndex(),
								 m_controllers(),
								 m_count( 0 )
{
	DriverStation* ds = &DriverStation::GetInstance();
	for ( int inx=0; inx<DriverStation::kJoystickPorts; ++inx )
	{
		m_controllers[inx] = nullptr;
		if ( ds->GetJoystickIsXbox( inx ) )
		{
            auto xbox = new DragonXBox( inx );
			m_controllers[inx] = xbox;
		}
		else if ( ds->GetJoystickType( inx ) == GenericHID::kHID1stPerson )
		{
            auto gamepad = new DragonGamepad( inx );
			m_controllers[inx] = gamepad;
		}
	}


    // Initialize the items to not defined
	m_axisIDs.resize(FUNCTION_IDENTIFIER::MAX_FUNCTIONS);
	m_buttonIDs.resize(FUNCTION_IDENTIFIER::MAX_FUNCTIONS);
	m_controllerIndex.resize(FUNCTION_IDENTIFIER::MAX_FUNCTIONS);
    for ( int inx=0; inx<FUNCTION_IDENTIFIER::MAX_FUNCTIONS; ++inx )
    {
        m_axisIDs[inx]    		= IDragonGamePad::UNDEFINED_AXIS;
        m_buttonIDs[inx]  		= IDragonGamePad::UNDEFINED_BUTTON;
        m_controllerIndex[inx]  = -1;
    }

    auto ctrlNo = 0;
    if ( m_controllers[ctrlNo] != nullptr && ds->GetJoystickIsXbox(ctrlNo) )
    {
		m_controllerIndex[ SWERVE_DRIVE_DRIVE]			= ctrlNo;
		m_axisIDs[ SWERVE_DRIVE_DRIVE]					= IDragonGamePad::LEFT_JOYSTICK_Y;
		m_controllerIndex[ SWERVE_DRIVE_STEER]			= ctrlNo;
		m_axisIDs[ SWERVE_DRIVE_STEER]					= IDragonGamePad::LEFT_JOYSTICK_X;
		m_controllerIndex[ SWERVE_DRIVE_ROTATE]			= ctrlNo;
		m_axisIDs[ SWERVE_DRIVE_ROTATE]					= IDragonGamePad::RIGHT_JOYSTICK_X;
		m_controllerIndex[ REZERO_PIGEON ]				= ctrlNo;
		m_buttonIDs[ REZERO_PIGEON ]					= IDragonGamePad::X_BUTTON;

		m_controllerIndex[ AUTO_DRIVE_TO_YELLOW ]		= ctrlNo;
		m_buttonIDs[ AUTO_DRIVE_TO_YELLOW ]				= IDragonGamePad::Y_BUTTON;
		m_controllerIndex[ AUTO_DRIVE_TO_LOADING_ZONE ]	= ctrlNo;
		m_buttonIDs[ AUTO_DRIVE_TO_LOADING_ZONE ]		= IDragonGamePad::B_BUTTON;

		m_controllerIndex[ TURN_AROUND_FRONT_RIGHT ] 	= ctrlNo;
		m_buttonIDs[ TURN_AROUND_FRONT_RIGHT ] 			= IDragonGamePad::A_BUTTON;

		m_controllerIndex[DRIVE_FULL] 		= ctrlNo;
		m_buttonIDs[DRIVE_FULL] 			= IDragonGamePad::POV_0;	
		m_controllerIndex[DRIVE_75PERCENT] 	= ctrlNo;
		m_buttonIDs[DRIVE_75PERCENT] 		= IDragonGamePad::POV_90;	
		m_controllerIndex[DRIVE_50PERCENT] 	= ctrlNo;
		m_buttonIDs[DRIVE_50PERCENT] 		= IDragonGamePad::POV_270;	
		m_controllerIndex[DRIVE_25PERCENT] 	= ctrlNo;
		m_buttonIDs[DRIVE_25PERCENT] 		= IDragonGamePad::POV_180;	
		m_controllerIndex[DRIVE_SHIFT_UP] 	= ctrlNo;
		m_buttonIDs[DRIVE_SHIFT_UP] 		= IDragonGamePad::RIGHT_BUMPER;	
		m_controllerIndex[DRIVE_SHIFT_DOWN] = ctrlNo;
		m_buttonIDs[DRIVE_SHIFT_DOWN] 		= IDragonGamePad::LEFT_BUMPER;	
		m_controllerIndex[DRIVE_TURBO] 		= ctrlNo;
		m_axisIDs[ DRIVE_TURBO]	     		= IDragonGamePad::RIGHT_TRIGGER;
		m_controllerIndex[DRIVE_BRAKE] 		= ctrlNo;
		m_axisIDs[ DRIVE_BRAKE]	     		= IDragonGamePad::LEFT_TRIGGER;
    }
    else
    {
        Logger::GetLogger()->LogError( string("TeleopControl::TeleopControl"), string("No controller plugged into port 0"));
    }

    ctrlNo = 1;
    if ( m_controllers[ctrlNo] != nullptr && ds->GetJoystickIsXbox(ctrlNo) )
    {
		m_controllerIndex[ SHOOTER_PREPARE_TO_SHOOT_GREEN ] = ctrlNo;
		m_buttonIDs[ SHOOTER_PREPARE_TO_SHOOT_GREEN ] = IDragonGamePad::A_BUTTON;

		m_controllerIndex[ SHOOTER_PREPARE_TO_SHOOT_RED] = ctrlNo;
		m_buttonIDs[ SHOOTER_PREPARE_TO_SHOOT_RED ] = IDragonGamePad::B_BUTTON;

		m_controllerIndex[ SHOOTER_PREPARE_TO_SHOOT_BLUE ] = ctrlNo;
		m_buttonIDs[ SHOOTER_PREPARE_TO_SHOOT_BLUE ] = IDragonGamePad::X_BUTTON;
		
		m_controllerIndex[ SHOOTER_PREPARE_TO_SHOOT_YELLOW] = ctrlNo;
		m_buttonIDs[ SHOOTER_PREPARE_TO_SHOOT_YELLOW ] = IDragonGamePad::Y_BUTTON;
	
		m_controllerIndex[SHOOTER_SHOOT] = ctrlNo;
		m_buttonIDs[SHOOTER_SHOOT] = IDragonGamePad::RIGHT_BUMPER;	
	
		m_controllerIndex[TURRET_LIMELIGHT_AIM] = ctrlNo;
		m_buttonIDs[TURRET_LIMELIGHT_AIM] = IDragonGamePad::LEFT_BUMPER;
	}
    else if ( m_controllers[ctrlNo] != nullptr )
    {
	}
	else
	{
        Logger::GetLogger()->LogError( string("TeleopControl::TeleopControl"), string("Controller 1 not handled"));
    }

	ctrlNo = 2;
    if ( m_controllers[ctrlNo] != nullptr && ds->GetJoystickIsXbox(ctrlNo) )
    {
	}
    else if ( m_controllers[ctrlNo] != nullptr )
    {
	}
	else
	{
        Logger::GetLogger()->LogError( string("TeleopControl::TeleopControl"), string("Controller 2 not handled"));
    }

    ctrlNo = 3;
    if ( m_controllers[ctrlNo] != nullptr && ds->GetJoystickIsXbox(ctrlNo) )
    {
	}
    else if ( m_controllers[ctrlNo] != nullptr )
    {
	}
	else
	{
		Logger::GetLogger()->LogError( string("TeleopControl::TeleopControl"), string("Controller 3 not handled"));

	}

    ctrlNo = 4;
    if ( m_controllers[ctrlNo] != nullptr && ds->GetJoystickIsXbox(ctrlNo) )
    {
	}
    else if ( m_controllers[ctrlNo] != nullptr )
    {
	}
	else
	{
        Logger::GetLogger()->LogError( string("TeleopControl::TeleopControl"), string("Controller 4 not handled"));
    }

    ctrlNo = 5;
    if ( m_controllers[ctrlNo] != nullptr && ds->GetJoystickIsXbox(ctrlNo) )
    {
	}
    else if ( m_controllers[ctrlNo] != nullptr )
    {
	}
	else
	{
        Logger::GetLogger()->LogError( string("TeleopControl::TeleopControl"), string("Controller 5 not handled"));
    }
}


//------------------------------------------------------------------
// Method:      SetAxisScaleFactor
// Description: Allow the range of values to be set smaller than
//              -1.0 to 1.0.  By providing a scale factor between 0.0
//              and 1.0, the range can be made smaller.  If a value
//              outside the range is provided, then the value will
//              be set to the closest bounding value (e.g. 1.5 will
//              become 1.0)
// Returns:     void
//------------------------------------------------------------------
void TeleopControl::SetAxisScaleFactor
(
    TeleopControl::FUNCTION_IDENTIFIER  	function,      // <I> - function that will update an axis
    double                                  scaleFactor    // <I> - scale factor used to limit the range
)
{
	int ctlIndex = m_controllerIndex[ function];
	IDragonGamePad::AXIS_IDENTIFIER axis = m_axisIDs[ function ];
    if ( ctlIndex > -1 && axis != IDragonGamePad::AXIS_IDENTIFIER::UNDEFINED_AXIS  )
    {
    	if (m_controllers[ ctlIndex ] != nullptr)
    	{
    		m_controllers[ ctlIndex ]->SetAxisScale( axis,scaleFactor);
    	}
    }
}

void TeleopControl::SetDeadBand
(
	TeleopControl::FUNCTION_IDENTIFIER		function,
	IDragonGamePad::AXIS_DEADBAND			deadband    
)
{
	int ctlIndex = m_controllerIndex[ function];
	IDragonGamePad::AXIS_IDENTIFIER axis = m_axisIDs[ function ];
    if ( ctlIndex > -1 && axis != IDragonGamePad::AXIS_IDENTIFIER::UNDEFINED_AXIS  )
    {
    	if (m_controllers[ ctlIndex ] != nullptr)
    	{
    		m_controllers[ ctlIndex ]->SetAxisDeadband( axis,deadband);
    	}
    }}

void TeleopControl::SetSlewRateLimiter
(
	TeleopControl::FUNCTION_IDENTIFIER		function,
	double 									slewRateFactor
)
{
	int ctlIndex = m_controllerIndex[ function ];
	IDragonGamePad::AXIS_IDENTIFIER axis = m_axisIDs [ function ];
	if ( ctlIndex > -1 && axis != IDragonGamePad::AXIS_IDENTIFIER::UNDEFINED_AXIS )
		{
			if ( m_controllers [ ctlIndex ] != nullptr )
				{
					m_controllers[ ctlIndex ]->SetSlewLimit( axis, slewRateFactor);
				}
		}
}

//------------------------------------------------------------------
// Method:      SetAxisProfile
// Description: Sets the axis profile for the specifed axis
// Returns:     void
//------------------------------------------------------------------
void TeleopControl::SetAxisProfile
(
    TeleopControl::FUNCTION_IDENTIFIER  function,       // <I> - function that will update an axis
    IDragonGamePad::AXIS_PROFILE        profile         // <I> - profile to use
)
{
	int ctlIndex = m_controllerIndex[ function];
	IDragonGamePad::AXIS_IDENTIFIER axis = m_axisIDs[ function ];
    if ( ctlIndex > -1 && axis != IDragonGamePad::AXIS_IDENTIFIER::UNDEFINED_AXIS  )
    {
    	if (m_controllers[ ctlIndex ] != nullptr)
    	{
    		m_controllers[ ctlIndex ]->SetAxisProfile( axis,profile);
    	}
    }
}
 
//------------------------------------------------------------------
// Method:      GetAxisValue
// Description: Reads the joystick axis, removes any deadband (small
//              value) and then scales as requested.
// Returns:     double   -  scaled axis value
//------------------------------------------------------------------
double TeleopControl::GetAxisValue
(
    TeleopControl::FUNCTION_IDENTIFIER  function    // <I> - function that whose axis will be read
) const
{
    double value = 0.0;
	int ctlIndex = m_controllerIndex[ function];
	IDragonGamePad::AXIS_IDENTIFIER axis = m_axisIDs[ function ];
    if ( ctlIndex > -1 && axis != IDragonGamePad::AXIS_IDENTIFIER::UNDEFINED_AXIS  )
    {
    	if (m_controllers[ ctlIndex ] != nullptr)
    	{
    		value = m_controllers[ ctlIndex ]->GetAxisValue( axis );
    	}
    }
    return value;
}

//------------------------------------------------------------------
// Method:      IsButtonPressed
// Description: Reads the button value.  Also allows POV, bumpers,
//              and triggers to be treated as buttons.
// Returns:     bool   -  scaled axis value
//------------------------------------------------------------------
bool TeleopControl::IsButtonPressed
(
    TeleopControl::FUNCTION_IDENTIFIER  function    // <I> - function that whose button will be read
) const
{
    bool isSelected = false;
	int ctlIndex = m_controllerIndex[ function];
	IDragonGamePad::BUTTON_IDENTIFIER btn = m_buttonIDs[ function ];
    if ( ctlIndex > -1 && btn != IDragonGamePad::BUTTON_IDENTIFIER::UNDEFINED_BUTTON  )
    {
    	if (m_controllers[ ctlIndex ] != nullptr)
    	{
    		isSelected = m_controllers[ ctlIndex ]->IsButtonPressed( btn );
    	}
    }
    return isSelected;
}


