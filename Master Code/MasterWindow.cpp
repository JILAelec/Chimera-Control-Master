#include "stdafx.h"
#include "MasterWindow.h"
#include "Control.h"
//#include "newVariableDialogProcedure.h"
#include "constants.h"
#include "viewAndChangeTTL_NamesProcedure.h"
#include "viewAndChangeDAC_NamesProcedure.h"
#include "textPromptDialogProcedure.h"
#include "TTL_System.h"
#include "explorerOpen.h"

IMPLEMENT_DYNAMIC( MasterWindow, CDialog )

BEGIN_MESSAGE_MAP( MasterWindow, CDialog )
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND_RANGE( TTL_ID_BEGIN, TTL_ID_END, &MasterWindow::handleTTLPush )
	ON_COMMAND( TTL_HOLD, &MasterWindow::handlTTLHoldPush )
	ON_COMMAND( ID_TTLS_VIEW_OR_CHANGE_TTL_NAMES, &MasterWindow::ViewOrChangeTTLNames )
	ON_COMMAND( ID_DACS_VIEW_OR_CHANGE_DAC_NAMES, &MasterWindow::ViewOrChangeDACNames )
	ON_COMMAND( ID_MASTER_SAVEMASTERCONFIGURATION, MasterWindow::SaveMasterConfig )
	ON_COMMAND( IDM_EXIT, &MasterWindow::Exit )
	ON_COMMAND( ID_MASTERSCRIPT_SAVESCRIPT, &MasterWindow::SaveMasterScript )
	ON_COMMAND( ID_MASTERSCRIPT_SAVESCRIPTAS, &MasterWindow::SaveMasterScriptAs )
	ON_COMMAND( ID_MASTERSCRIPT_NEWSCRIPT, &MasterWindow::NewMasterScript )
	ON_COMMAND( ID_MASTERSCRIPT_OPENSCRIPT, &MasterWindow::OpenMasterScript )
	ON_COMMAND( ID_MASTERSCRIPT_SAVEFUNCTION, &MasterWindow::SaveMasterFunction )
	ON_COMMAND( ID_CONFIGURATION_DELETE_CURRENT_CONFIGURATION, &MasterWindow::DeleteConfiguration )
	ON_COMMAND( ID_CONFIGURATION_NEW_CONFIGURATION, &MasterWindow::NewConfiguration )
	ON_COMMAND( ID_CONFIGURATION_SAVECONFIGURATIONSETTINGS, &MasterWindow::SaveConfiguration )
	ON_COMMAND( ID_CONFIGURATION_SAVE_CONFIGURATION_AS, &MasterWindow::SaveConfigurationAs )
	ON_COMMAND( ID_CONFIGURATION_RENAME_CURRENT_CONFIGURATION, &MasterWindow::RenameConfiguration )
	ON_COMMAND( ID_CATEGORY_SAVECATEGORYSETTINGS, &MasterWindow::SaveCategory )
	ON_COMMAND( ID_CATEGORY_SAVECATEGORYSETTINGSAS, &MasterWindow::SaveCategoryAs )
	ON_COMMAND( ID_CATEGORY_RENAME_CURRENT_CATEGORY, &MasterWindow::RenameCategory )
	ON_COMMAND( ID_CATEGORY_DELETE_CURRENT_CATEGORY, &MasterWindow::DeleteCategory )
	ON_COMMAND( ID_CATEGORY_NEW_CATEGORY, &MasterWindow::NewCategory )
	ON_COMMAND( ID_EXPERIMENT_NEW_EXPERIMENT_TYPE, &MasterWindow::NewExperiment )
	ON_COMMAND( ID_EXPERIMENT_DELETE_CURRENT_EXPERIMENT, &MasterWindow::DeleteExperiment )
	ON_COMMAND( ID_EXPERIMENT_SAVEEXPERIMENTSETTINGS, &MasterWindow::SaveExperiment )
	ON_COMMAND( ID_EXPERIMENT_SAVEEXPERIMENTSETTINGSAS, &MasterWindow::SaveExperimentAs )
	ON_COMMAND( ID_SEQUENCE_ADD_TO_SEQUENCE, &MasterWindow::AddToSequence )
	ON_COMMAND( ID_SEQUENCE_DELETE_SEQUENCE, &MasterWindow::DeleteSequence )
	ON_COMMAND( ID_SEQUENCE_RESET_SEQUENCE, &MasterWindow::ResetSequence )
	ON_COMMAND( ID_SEQUENCE_RENAMESEQUENCE, &MasterWindow::RenameSequence )
	ON_COMMAND( ID_SEQUENCE_NEW_SEQUENCE, &MasterWindow::NewSequence )
	ON_COMMAND( SET_REPETITION_ID, &MasterWindow::SetRepetitionNumber )
	ON_COMMAND( ID_ERROR_CLEAR, &MasterWindow::ClearError )
	ON_COMMAND( ID_STATUS_CLEAR, &MasterWindow::ClearGeneral )
	ON_COMMAND( ID_DAC_SET_BUTTON, &MasterWindow::SetDacs )
	ON_COMMAND( ID_FILE_EXPORTSETTINGSLOG, &MasterWindow::LogSettings )
	ON_COMMAND( ID_FILE_RUN, &MasterWindow::StartExperiment )
	ON_COMMAND( ID_FILE_PAUSE, &MasterWindow::HandlePause )
	ON_COMMAND( ID_FILE_ABORT, &MasterWindow::HandleAbort )
	ON_COMMAND( ID_PROFILE_SAVE_PROFILE, &MasterWindow::SaveEntireProfile )
	ON_COMMAND( IDC_ZERO_TTLS, &MasterWindow::zeroTtls )
	ON_COMMAND( IDC_ZERO_DACS, &MasterWindow::zeroDacs )
	ON_COMMAND_RANGE( IDC_SHOW_TTLS, IDC_SHOW_TTLS, &MasterWindow::handleOptionsPress )
	ON_COMMAND_RANGE( IDC_SHOW_DACS, IDC_SHOW_DACS, &MasterWindow::handleOptionsPress )
	ON_COMMAND_RANGE(IDC_TOP_BOTTOM_CHANNEL1_BUTTON, IDC_FLASHING_AGILENT_COMBO, &MasterWindow::handleAgilentOptions)
	ON_CBN_SELENDOK( IDC_TOP_BOTTOM_AGILENT_COMBO, &MasterWindow::handleTopBottomAgilentCombo )
	ON_CBN_SELENDOK( IDC_AXIAL_UWAVE_AGILENT_COMBO, &MasterWindow::handleAxialUWaveAgilentCombo )
	ON_CBN_SELENDOK( IDC_FLASHING_AGILENT_COMBO, &MasterWindow::handleFlashingAgilentCombo )
	ON_CBN_SELENDOK( EXPERIMENT_COMBO_ID, &MasterWindow::OnExperimentChanged )
	ON_CBN_SELENDOK( CATEGORY_COMBO_ID, &MasterWindow::OnCategoryChanged )
	ON_CBN_SELENDOK( CONFIGURATION_COMBO_ID, &MasterWindow::OnConfigurationChanged )
	ON_CBN_SELENDOK( SEQUENCE_COMBO_ID, &MasterWindow::OnSequenceChanged )
	ON_CBN_SELENDOK( ORIENTATION_COMBO_ID, &MasterWindow::OnOrientationChanged )
	ON_CBN_SELENDOK( FUNCTION_COMBO_ID, &MasterWindow::HandleFunctionChange )

	

	ON_CONTROL_RANGE( EN_CHANGE, ID_DAC_FIRST_EDIT, (ID_DAC_FIRST_EDIT + 23), &MasterWindow::DAC_EditChange )

	ON_EN_CHANGE( MASTER_RICH_EDIT, &MasterWindow::EditChange )
	ON_EN_CHANGE( CONFIGURATION_NOTES_ID, &MasterWindow::ConfigurationNotesChange )
	ON_EN_CHANGE( CATEGORY_NOTES_ID, &MasterWindow::CategoryNotesChange )
	ON_EN_CHANGE( EXPERIMENT_NOTES_ID, &MasterWindow::ExperimentNotesChange )

	ON_NOTIFY( LVN_COLUMNCLICK, IDC_CONFIG_VARS_LISTVIEW, &MasterWindow::ConfigVarsColumnClick )
	ON_NOTIFY( NM_DBLCLK, IDC_CONFIG_VARS_LISTVIEW, &MasterWindow::ConfigVarsDblClick )
	ON_NOTIFY( NM_RCLICK, IDC_CONFIG_VARS_LISTVIEW, &MasterWindow::ConfigVarsRClick )

	ON_NOTIFY( NM_DBLCLK, IDC_GLOBAL_VARS_LISTVIEW, &MasterWindow::GlobalVarDblClick )
	ON_NOTIFY( NM_RCLICK, IDC_GLOBAL_VARS_LISTVIEW, &MasterWindow::GlobalVarRClick )

END_MESSAGE_MAP()

fontMap MasterWindow::getFonts()
{
	return masterFonts;
}

void MasterWindow::OnSize(UINT nType, int cx, int cy)
{
	topBottomAgilent.rearrange(cx, cy, getFonts());
	uWaveAxialAgilent.rearrange(cx, cy, getFonts());
	flashingAgilent.rearrange(cx, cy, getFonts());
	profile.rearrange(cx, cy, getFonts());
	notes.rearrange(cx, cy, getFonts());
	masterScript.rearrange(cx, cy, getFonts());

	errorStatus.rearrange(cx, cy, getFonts());
	generalStatus.rearrange(cx, cy, getFonts());

	RhodeSchwarzGenerator.rearrange(cx, cy, getFonts());

	configVariables.rearrange(cx, cy, getFonts());
	globalVariables.rearrange(cx, cy, getFonts());

	ttlBoard.rearrange(cx, cy, getFonts());
	dacBoards.rearrange(cx, cy, getFonts());

	repetitionControl.rearrange(cx, cy, getFonts());
	debugControl.rearrange(cx, cy, getFonts());

	/*
	ExperimentLogger logger;
	ExperimentManager manager;
	RunInfo systemRunningInfo;
	SocketWrapper niawgSocket;
	Gpib gpibHandler;
	KeyHandler masterKey;
	Debugger debugControl;
	MasterConfiguration masterConfig{ MASTER_CONFIGURATION_FILE_ADDRESS };
	*/
}

void MasterWindow::handleAgilentOptions( UINT id )
{
	// zero the id.
	id -= IDC_TOP_BOTTOM_CHANNEL1_BUTTON;
	int agilentNum = id / 8;
	// figure out which box it was.
	Agilent* agilent = NULL;
	if (agilentNum == 0)
	{
		agilent = &topBottomAgilent;
	}
	else if (agilentNum == 1)
	{
		agilent = &uWaveAxialAgilent;
	}
	else if (agilentNum == 2)
	{
		agilent = &flashingAgilent;
	}
	else
	{
		errBox( "4th agilent???" );
		return;
	}
	// call the correct function.
	if (id % 8 == 0)
	{
		// channel 1
		agilent->handleChannelPress( 1 );
	}
	else if (id % 8 == 1)
	{
		// channel 2
		agilent->handleChannelPress( 2 );
	}
	else if (id % 8 == 3)
	{
		// sync 
		//agilent->handleSync();
	}
	// else it's a boring combo that I don't care about at the moment.
}


void MasterWindow::handleTopBottomAgilentCombo()
{
	topBottomAgilent.handleCombo();
}


void MasterWindow::handleAxialUWaveAgilentCombo()
{
	uWaveAxialAgilent.handleCombo();
}


void MasterWindow::handleFlashingAgilentCombo()
{
	flashingAgilent.handleCombo();
}


void MasterWindow::GlobalVarDblClick( NMHDR * pNotifyStruct, LRESULT * result )
{
	std::vector<Script*> scriptList;
	scriptList.push_back( &masterScript );
	try
	{
		globalVariables.updateVariableInfo( scriptList, this );
	}
	catch (Error& exception)
	{
		errorStatus.addStatusText( "Global Variables Double Click Handler : " + exception.whatStr() + "\r\n", 0 );
	}
	masterConfig.save( &ttlBoard, &dacBoards, &globalVariables );
}


void MasterWindow::GlobalVarRClick( NMHDR * pNotifyStruct, LRESULT * result )
{
	try
	{
		globalVariables.deleteVariable();
	}
	catch (Error& exception)
	{
		errorStatus.addStatusText( "Global Variables Right Click Handler : " + exception.whatStr() + "\r\n", 0 );
	}
	masterConfig.save( &ttlBoard, &dacBoards, &globalVariables );
}


void MasterWindow::handleOptionsPress( UINT id )
{
	debugControl.handlePress( id );
	return;
}


void MasterWindow::zeroDacs()
{
	try
	{
		dacBoards.resetDACEvents();
		ttlBoard.resetTTLEvents();
		for (int dacInc = 0; dacInc < 24; dacInc++)
		{
			dacBoards.prepareDacForceChange( dacInc, 0, &ttlBoard );
		}
		dacBoards.analyzeDAC_Commands();
		dacBoards.makeFinalDataFormat();
		dacBoards.stopDacs(); 
		dacBoards.configureClocks();
		dacBoards.writeDacs();
		dacBoards.startDacs();
		ttlBoard.analyzeCommandList();
		ttlBoard.convertToFinalFormat();
		ttlBoard.writeData();
		ttlBoard.startBoard();
		ttlBoard.waitTillFinished();
		generalStatus.addStatusText( "Zero'd DACs.\r\n", 0 );
	}
	catch (Error& exception)
	{
		generalStatus.addStatusText( "Failed to Zero DACs!!!\r\n", 0 );
		errorStatus.addStatusText( exception.what(), 0 );
	}
}


void MasterWindow::zeroTtls()
{
	try
	{
		ttlBoard.zeroBoard();
		generalStatus.addStatusText( "Zero'd TTLs.\r\n", 0 );
	}
	catch (Error& exception)
	{
		this->generalStatus.addStatusText( "Failed to Zero TTLs!!!\r\n", 0 );
		this->errorStatus.addStatusText( exception.what(), 0 );
	}
}


void MasterWindow::HandleAbort()
{
	/// needs implementation...
	if ( manager.runningStatus() )
	{
		manager.abort();
		ttlBoard.unshadeTTLs();
		dacBoards.unshadeDacs();
	}
	else
	{
		generalStatus.addStatusText( "Can't abort, experiment was not running.\r\n", 0 );
	}
}


void MasterWindow::HandlePause()
{
	if ( manager.runningStatus() )
	{
		CMenu *menu = GetMenu();
		if ( menu == NULL )
		{
			errBox( "WTF no menu?" );
		}
		if ( manager.getIsPaused() )
		{
			// then it's currently paused, so unpause it.
			menu->CheckMenuItem( ID_FILE_PAUSE, MF_UNCHECKED | MF_BYCOMMAND );
			manager.unPause();
		}
		else
		{
			// then not paused so pause it.
			menu->CheckMenuItem( ID_FILE_PAUSE, MF_CHECKED | MF_BYCOMMAND );
			manager.pause();
		}
	}
	else
	{
		generalStatus.addStatusText( "Can't pause, experiment was not running.\r\n", 0 );
	}
}

void MasterWindow::loadMotSettings()
{
	try
	{
		generalStatus.addStatusText("Loading MOT Configuration...\r\n", 1);
		//
		manager.loadMotSettings(this);
	}
	catch (Error& exception)
	{
		generalStatus.addStatusText(": " + exception.whatStr() + " " + exception.whatStr() + "\r\n", 1);
	}
	/*	
	/// Set TTL values.
	// switch on MOT light, set power, set detuning
	// forceTtl(row, number, state), A = 0, B = 1, C = 2, D = 3
	// MOT AO RF
	ttlBoard.forceTtl( 0, 0, 1 );
	// MOT slave shutter
	ttlBoard.forceTtl( 3, 0, 1 );
	// RP shutter open
	ttlBoard.forceTtl( 1, 1, 1 );
	// F=2 shutter open (blocks side MOT beam)
	ttlBoard.forceTtl( 2, 13, 1);
	// switch on repump light
	ttlBoard.forceTtl( 0, 1, 1 );
	// Switch on side beam
	ttlBoard.forceTtl( 0, 5, 1 );
	// Make sure raman shutter is closed initially
	ttlBoard.forceTtl( 2, 4, 0 );
	// (value time line) in vb6
	dacBoards.prepareDacForceChange(22, 0.084, &ttlBoard);
	// trap light detuning 0 for MOT detuning (0 is 10 MHz detuning, -.3 is 17 MHz detuning)
	dacBoards.prepareDacForceChange( 20, 0.1, &ttlBoard );
	// trap light power (0.2 Full power)
	dacBoards.prepareDacForceChange( 9, 0.28, &ttlBoard );
	// trap light AO detuning
	dacBoards.prepareDacForceChange( 8, 1, &ttlBoard );
	// setting power (was 0.285)
	dacBoards.prepareDacForceChange( 11, 0.32, &ttlBoard );
	//setting frequency via offset lock
	dacBoards.prepareDacForceChange( 21, 0, &ttlBoard );
	//quad coils on// CR
	dacBoards.prepareDacForceChange( 6, 7, &ttlBoard );
	/// mot coils.
	// Right, 0.117 from the perspective of looking east to west across the table '0.64
	dacBoards.prepareDacForceChange( 0, 0.66, &ttlBoard );
	// Left, -0.64
	dacBoards.prepareDacForceChange( 1, -0.575, &ttlBoard );
	// Front, -1.272
	dacBoards.prepareDacForceChange( 2, -1.11, &ttlBoard );
	// Back, 1.272
	//dacBoards.prepareDacForceChange( 3, 0.5, &ttlBoard );
	dacBoards.prepareDacForceChange( 3, 1.11, &ttlBoard );
	// Bottom, -1.24
	dacBoards.prepareDacForceChange( 4, -1.38, &ttlBoard );
	// top
	dacBoards.prepareDacForceChange( 5, 1.31, &ttlBoard );
	// start the boards which actually sets the dac values.
	try
	{
		dacBoards.analyzeDAC_Commands();
		dacBoards.makeFinalDataFormat();
		dacBoards.configureClocks();
		dacBoards.stopDacs();
		dacBoards.writeDacs();
		dacBoards.startDacs();
		ttlBoard.analyzeCommandList();
		ttlBoard.convertToFinalFormat();
		ttlBoard.writeData();
		ttlBoard.startBoard();
		ttlBoard.waitTillFinished();
		generalStatus.appendText( "Finished Loading MOT Configuration!\r\n", 1 );
	}
	catch ( Error& exception )
	{
		errorStatus.appendText( exception.what(), 1 );
		generalStatus.appendText( ": " + exception.whatStr() + "\r\n", 1 );
	}
	*/
}

void MasterWindow::OnCancel()
{
	int answer = MessageBox( "Are you sure you'd like to exit?", "Exit", MB_OKCANCEL );
	if (answer == IDOK)
	{
		this->CDialog::OnCancel();
	}
}

void MasterWindow::HandleFunctionChange()
{
	this->masterScript.functionChangeHandler(this);
	return;
}

void MasterWindow::StartExperiment()
{	
	// check to make sure ready.
	try
	{
		generalStatus.addStatusText( "............................\r\n", 1 );
		generalStatus.addStatusText( "Checking if Ready...\n", 0 );
		profile.allSettingsReadyCheck( this );
		masterScript.checkSave( this );
		generalStatus.addStatusText( "Starting Experiment Thread...\r\n", 0 );
		manager.startExperimentThread( this );
	}
	catch (Error& exception)
	{
		generalStatus.addStatusText( ": " + exception.whatStr() + " " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::ConfigurationNotesChange()
{
	profile.updateConfigurationSavedStatus(false);
}


void MasterWindow::CategoryNotesChange()
{
	this->profile.updateCategorySavedStatus(false);
	return;
}


void MasterWindow::ExperimentNotesChange()
{
	this->profile.updateExperimentSavedStatus(false);
	return;
}


void MasterWindow::SaveEntireProfile()
{
	this->profile.saveEntireProfile(this);
	return;
}


void MasterWindow::LogSettings()
{
	this->logger.generateLog(this);
	this->logger.exportLog();
	return;
}


void MasterWindow::SetDacs()
{
	// have the dac values change
	try
	{
		dacBoards.resetDACEvents();
		ttlBoard.resetTTLEvents();

		generalStatus.addStatusText( "Starting Setting Dacs...\r\n", 0 );
		dacBoards.handleButtonPress( &this->ttlBoard );
		generalStatus.addStatusText( "Analyzing Dac Info...\r\n", 0 );
		dacBoards.analyzeDAC_Commands();
		generalStatus.addStatusText( "Finalizing Dacs Info...\r\n", 0 );
		dacBoards.makeFinalDataFormat();
		// start the boards which actually sets the dac values.
		generalStatus.addStatusText( "Stopping Dacs (if running)...\r\n", 0 );
		dacBoards.stopDacs();
		generalStatus.addStatusText( "Configuring Clocks...\r\n", 0 );
		dacBoards.configureClocks();
		generalStatus.addStatusText( "Writing New Dac Settings...\r\n", 0 );
		dacBoards.writeDacs();
		generalStatus.addStatusText( "Arming Dacs...\r\n", 0 );
		dacBoards.startDacs();
		generalStatus.addStatusText( "Analyzing DAC TTL Triggers...\r\n", 0 );
		ttlBoard.analyzeCommandList();
		generalStatus.addStatusText( "Finalizing DAC TTL Triggers...\r\n", 0 );
		ttlBoard.convertToFinalFormat();
		generalStatus.addStatusText( "Writing DAC TTL Triggers...\r\n", 0 );
		ttlBoard.writeData();
		generalStatus.addStatusText( "Starting DAC TTL Triggers...\r\n", 0 );
		ttlBoard.startBoard();
		generalStatus.addStatusText( "Waiting until TTLs are finished...\r\n", 0 );
		ttlBoard.waitTillFinished();
		generalStatus.addStatusText( "Finished Setting Dacs.\r\n", 0 );
	}
	catch (Error& exception)
	{
		errBox( exception.what() );
		generalStatus.addStatusText( ": " + exception.whatStr() + "\r\n", 0 );
		errorStatus.addStatusText( exception.what(), 0 );
	}
	profile.updateConfigurationSavedStatus(false);
}


void MasterWindow::DAC_EditChange(UINT id)
{
	dacBoards.handleEditChange(id - ID_DAC_FIRST_EDIT + 1);
}


void MasterWindow::ClearError()
{
	this->errorStatus.clear();
	return;
}

void MasterWindow::ClearGeneral()
{
	this->generalStatus.clear();
	return;
}

void MasterWindow::SetRepetitionNumber()
{
	try
	{
		repetitionControl.handleButtonPush();
	}
	catch (Error& exception)
	{
		errorStatus.addStatusText( "Set Repetition Number : " + exception.whatStr() + "\r\n", 0 );
	}
	profile.updateConfigurationSavedStatus(false);
}


void MasterWindow::ConfigVarsColumnClick(NMHDR * pNotifyStruct, LRESULT * result)
{
	try
	{
		this->configVariables.handleColumnClick( pNotifyStruct, result );
	}
	catch(Error& exception )
	{
		this->errorStatus.addStatusText("Handling config variable listview click : " + exception.whatStr() + "\r\n", 0);
	}
	this->profile.updateConfigurationSavedStatus(false);
	return;
}

void MasterWindow::OnExperimentChanged()
{
	try
	{
		this->profile.experimentChangeHandler( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Handling Experiment Selection Change : " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::OnCategoryChanged()
{
	try
	{
		this->profile.categoryChangeHandler( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Handling Category Selection Change : " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::OnConfigurationChanged()
{
	try
	{
		this->profile.configurationChangeHandler( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Handling Configuration Selection Change Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::OnSequenceChanged()
{
	try
	{
		this->profile.sequenceChangeHandler( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Handling Sequence Selection Change Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::OnOrientationChanged()
{
	try
	{
		this->profile.orientationChangeHandler( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Handling Orientation Selection Change Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::DeleteConfiguration()
{
	try
	{
		this->profile.deleteConfiguration();
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Deleting Configuration Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::NewConfiguration()
{
	try
	{
		this->profile.newConfiguration( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "New Configuration Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::SaveConfiguration()
{
	try
	{
		profile.saveConfiguration( this );
	}
	catch (Error& exception)
	{
		errorStatus.addStatusText( "Saving Configuration Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::SaveConfigurationAs()
{
	try
	{
		this->profile.saveConfigurationAs( this );
	}
	catch (Error& exception)
	{
		errorStatus.addStatusText( "Saving Configuration As Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::RenameConfiguration()
{
	try
	{
		this->profile.renameConfiguration( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Renaming Configuration Failed: " + exception.whatStr() + "\r\n", 0 );
	}
	return;
}


void MasterWindow::SaveCategory()
{
	try
	{
		profile.saveCategory( this );
	}
	catch (Error& exception)
	{
		errorStatus.addStatusText( "Saving Category Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::SaveCategoryAs()
{
	try
	{
		this->profile.saveCategoryAs( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Saving Category As Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::RenameCategory()
{
	try
	{
		this->profile.renameCategory();
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Renaming Category Failed: " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::DeleteCategory()
{
	try
	{
		this->profile.deleteCategory();
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("Deleting Category Failed: " + exception.whatStr() + "\r\n", 0);
	}
}


void MasterWindow::NewCategory()
{
	try
	{
		this->profile.newCategory( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("New Category Failed: " + exception.whatStr() + "\r\n", 0);
	}
}


void MasterWindow::NewExperiment()
{
	try
	{
		this->profile.newExperiment( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("New Experiment : " + exception.whatStr() + "\r\n", 0);
	}
}


void MasterWindow::DeleteExperiment()
{
	try
	{
		this->profile.deleteExperiment();
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("Delete Experiment : " + exception.whatStr() + "\r\n", 0);
	}
}


void MasterWindow::SaveExperiment()
{
	try
	{
		profile.saveExperiment( this );
	}
	catch (Error& exception)
	{
		errorStatus.addStatusText("ERROR: Save Experiment : " + exception.whatStr() + "\r\n", 0);
	}
}


void MasterWindow::SaveExperimentAs()
{
	try
	{
		this->profile.saveExperimentAs( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "Save Experiment As : " + exception.whatStr() + "\r\n", 0 );
	}
}


void MasterWindow::AddToSequence()
{
	try
	{
		this->profile.addToSequence( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("Add to Sequence : " + exception.whatStr() + "\r\n", 0);
	}
}


void MasterWindow::DeleteSequence()
{
	try
	{
		this->profile.deleteSequence( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("Delete Sequence : " + exception.whatStr() + "\r\n", 0);
	}
}


void MasterWindow::ResetSequence()
{
	//this->profile.???
}


void MasterWindow::RenameSequence()
{
	try
	{
		this->profile.renameSequence( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("Rename Sequence : " + exception.whatStr() + "\r\n", 0);
	}
}
void MasterWindow::NewSequence()
{
	try
	{
		this->profile.newSequence( this );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("New Sequence : " + exception.whatStr() + "\r\n", 0);
	}
	return;
}

//

void MasterWindow::SaveMasterScript()
{
	try
	{
		this->masterScript.saveScript( this );
	}
	catch ( Error& exception )
	{
		this->errorStatus.addStatusText("Save Master Script Failed: " + exception.whatStr() + "\r\n", 0);
	}
	return;
}

void MasterWindow::SaveMasterScriptAs()
{
	if (profile.getCurrentPathIncludingCategory() == "")
	{
		MessageBox("Please select a category before trying to save a the Master Script!", 0, 0);
	}
	std::string scriptName = (const char*)DialogBoxParam(this->programInstance, MAKEINTRESOURCE(IDD_TEXT_PROMPT_DIALOG), 0, (DLGPROC)textPromptDialogProcedure, (LPARAM)"Please enter new master script name.");
	if (scriptName == "")
	{
		return;
	}
	try
	{
		this->masterScript.saveScriptAs( this->profile.getCurrentPathIncludingCategory() + scriptName + MASTER_SCRIPT_EXTENSION, this );
	}
	catch(Error& exception )
	{
		this->errorStatus.addStatusText("Save Master Script As : " + exception.whatStr() + "\r\n", 0);
	}
	return;
}
void MasterWindow::NewMasterScript()
{
	try
	{
		this->masterScript.newScript( this );
	}
	catch ( Error& exception )
	{
		this->errorStatus.addStatusText( "New Master Script Failed: " + exception.whatStr() + "\r\n", 0 );
	}
	this->profile.updateConfigurationSavedStatus(false);
	return;
}
void MasterWindow::OpenMasterScript()
{
	// ???
	std::string address = explorerOpen(this, "*.mScript\0All\0 * .*", this->profile.getCurrentPathIncludingCategory());
	if (address == "")
	{
		return;
	}
	try
	{
		this->masterScript.openParentScript( address, this );
	}
	catch(Error& exception )
	{
		this->errorStatus.addStatusText("Open Master Script : " + exception.whatStr() + "" + exception.whatStr() + "\r\n", 0);
	}
	this->profile.updateConfigurationSavedStatus(false);
	return;
}

void MasterWindow::SaveMasterFunction()
{
	try
	{
		this->masterScript.saveAsFunction();
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("Save Master Script Function Failed: " + exception.whatStr() + "\r\n", 0);
	}
	return;
}

void MasterWindow::ConfigVarsDblClick(NMHDR * pNotifyStruct, LRESULT * result)
{	
	std::vector<Script*> scriptList;
	scriptList.push_back(&masterScript);
	try
	{
		this->configVariables.updateVariableInfo( scriptList, this );
	}
	catch(Error& exception )
	{
		this->errorStatus.addStatusText("Variables Double Click Handler : " + exception.whatStr() + "\r\n", 0);
	}
	this->profile.updateConfigurationSavedStatus(false);
	return;
}

void MasterWindow::ConfigVarsRClick(NMHDR * pNotifyStruct, LRESULT * result)
{
	try
	{
		this->configVariables.deleteVariable();
	}
	catch(Error& exception)
	{
		this->errorStatus.addStatusText("Variables Right Click Handler : " + exception.whatStr() + "\r\n", 0);
	}
	this->profile.updateConfigurationSavedStatus(false);
	return;
}

void MasterWindow::OnTimer(UINT TimerVal)
{
	try
	{
		this->masterScript.handleTimerCall( this );
	}
	catch (Error& exception )
	{
		this->errorStatus.addStatusText("Timer Call Handler Failed: " + exception.whatStr() + "\r\n", 0);
	}
	return;
}

void MasterWindow::EditChange()
{
	try
	{
		this->masterScript.handleEditChange( this );
	}
	catch ( Error& exception )
	{
		this->errorStatus.addStatusText( "Edit Change Handler Failed: " + exception.whatStr() + "\r\n", 0 );
	}
	return;
}

void MasterWindow::handleTTLPush(UINT id)
{
	try
	{
		this->ttlBoard.handleTTLPress( id );
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "TTL Press Handler Failed: " + exception.whatStr() + "\r\n", 0 );
	}
	this->profile.updateConfigurationSavedStatus(false);
	return;
}

void MasterWindow::handlTTLHoldPush()
{
	try
	{
		this->ttlBoard.handleHoldPress();
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText( "TTL Hold Handler Failed: " + exception.whatStr() + "\r\n", 0 );
	}
	this->profile.updateConfigurationSavedStatus(false);
	return;
}

void MasterWindow::ViewOrChangeTTLNames()
{
	ttlInputStruct input;
	input.ttls = &this->ttlBoard;
	input.toolTips = this->toolTips;
	DialogBoxParam(this->programInstance, MAKEINTRESOURCE(IDD_VIEW_AND_CHANGE_TTL_NAMES), 0, (DLGPROC)viewAndChangeTTL_NamesProcedure, (LPARAM)&input);
	return;
}

void MasterWindow::ViewOrChangeDACNames()
{
	dacInputStruct input;
	input.dacs = &this->dacBoards;
	input.toolTips = this->toolTips;
	DialogBoxParam(this->programInstance, MAKEINTRESOURCE(IDD_VIEW_AND_CHANGE_DAC_NAMES), 0, (DLGPROC)viewAndChangeDAC_NamesProcedure, (LPARAM)&input);
	return;
}

void MasterWindow::SaveMasterConfig()
{
	try
	{
		this->masterConfig.save( &this->ttlBoard, &this->dacBoards, &globalVariables);
	}
	catch (Error& exception)
	{
		this->errorStatus.addStatusText("Master Configuration Save Handler : " + exception.whatStr() + "\r\n", 0);
	}
	return;
}

void MasterWindow::Exit()
{
	this->EndDialog(0);
	return;
}

MasterWindow::~MasterWindow()
{
	if (masterWindowHandle)
	{
		DestroyWindow();
	}
}

HBRUSH MasterWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH result = this->ttlBoard.handleColorMessage(pWnd, this->masterBrushes, this->masterRGBs, pDC);
	if (result != NULL)
	{
		return result;
	}
	result = this->dacBoards.handleColorMessage(pWnd, this->masterBrushes, this->masterRGBs, pDC);
	if (result != NULL)
	{
		return result;
	}
	switch (nCtlColor)
	{

		case CTLCOLOR_STATIC:
		{
			pDC->SetTextColor(masterRGBs["Gold"]);
			pDC->SetBkColor(masterRGBs["Medium Grey"]);
			return masterBrushes["Medium Grey"];
		}
		case CTLCOLOR_EDIT:
		{
			pDC->SetTextColor(masterRGBs["White"]);
			pDC->SetBkColor(masterRGBs["Dark Grey"]);
			return masterBrushes["Dark Grey"];
		}
		case CTLCOLOR_LISTBOX:
		{
			pDC->SetTextColor(masterRGBs["White"]);
			pDC->SetBkColor(masterRGBs["Dark Grey"]);
			return masterBrushes["Dark Grey"];
		}
		default:
			return masterBrushes["Light Grey"];
	}
}

BOOL MasterWindow::PreTranslateMessage(MSG* pMsg)
{
	for (int toolTipInc = 0; toolTipInc < this->toolTips.size(); toolTipInc++)
	{
		this->toolTips[toolTipInc]->RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL MasterWindow::OnInitDialog()
{
	int id = 1000;
	POINT controlLocation{ 0, 0 };
	profile.initialize( controlLocation, this->toolTips, this, id );
	globalVariables.initialize( controlLocation, this->toolTips, this, id, "GLOBAL VARIABLES" );
	configVariables.initialize( controlLocation, this->toolTips, this, id, "CONFIGURATION VARIABLES" );
	repetitionControl.initialize( controlLocation, this->toolTips, this, id );
	ttlBoard.initialize( controlLocation, this->toolTipText, this->toolTips, this, id );
	dacBoards.initialize( controlLocation, this->toolTips, this, id );
	try
	{
		masterConfig.load( &ttlBoard, dacBoards, toolTips, this, &globalVariables );
	}
	catch (Error& exeption)
	{
		errBox( exeption.what() );
	}

	RECT controlArea = { 960, 0, 1320, 540 };
	POINT statusLoc = { 960, 0 };
	generalStatus.initialize(statusLoc, this, id, 1320-960, 540, "General Status", masterRGBs["Light Blue"],
							 getFonts(), toolTips);
	//generalStatus.initialize( controlArea, "Status", this->masterRGBs["Light Blue"], this, id);
	statusLoc = { 960, 540 };
	errorStatus.initialize(statusLoc, this, id, 1320 - 960, 540, "General Status", masterRGBs["Light Red"],
						   getFonts(), toolTips);
	//errorStatus.initialize( controlArea, "Errors", this->masterRGBs["Light Red"], this, id );
	controlLocation = POINT{ 480, 90 };
	notes.initialize( controlLocation, this, id );
	RhodeSchwarzGenerator.initialize( controlLocation, toolTips, this, id );
	debugControl.initialize( controlLocation, this, this->toolTips, id );
	topBottomAgilent.initialize( controlLocation, toolTips, this, id, "USB0::2391::11271::MY52801397::0::INSTR", "Top/Bottom Agilent" );
	uWaveAxialAgilent.initialize( controlLocation, toolTips, this, id, "STUFF...", "U-Wave / Axial Agilent" );
	flashingAgilent.initialize( controlLocation, toolTips, this, id, "STUFF...", "Flashing Agilent" );
	controlLocation = POINT{ 1320, 0 };
	masterScript.initialize( 600, 1080, controlLocation, this->toolTips, this, id );

	// controls are done. Report the initialization status...
	std::string msg;
	msg +=	"==========================================\n"
			"=============MASTER CONTROL===============\n"
			"==========================================\n"
			"==The Quantum Gas Assembly Control System=\n"
			"==========================================\n\n";
	
	msg += "<<Code Safemode Settings>>\n";
	msg += "TTL System... ";
	if ( !DIO_SAFEMODE )
	{
		msg += "ACTIVE!\n";
	}
	else
	{
		msg += "DISABLED!\n";
	}
	msg += "DAC System... ";
	if ( !DAQMX_SAFEMODE )
	{
		msg += "ACTIVE!\n";
	}
	else
	{
		msg += "DISABLED!\n";
	}
	msg += "GPIB System... ";
	if ( !GPIB_SAFEMODE )
	{
		msg += "ACTIVE!\n";
	}
	else
	{
		msg += "DISABLED!\n";
	}
	msg += "Agilent System... ";
	if ( !AGILENT_SAFEMODE )
	{
		msg += "ACTIVE!\n";
	}
	else
	{
		msg += "DISABLED!\n";
	}
	// 
	msg += "\n<<Device Connectivity>>\n";

	msg += "TTL Board: ";
	try
	{
		msg += ttlBoard.getSystemInfo() + "\n";
	}
	catch (Error& err)
	{
		msg += "Failed! " + err.whatStr() + "\n";
	}

	msg += dacBoards.getDacSystemInfo() + "\n";
	msg += "Top / Bottom Agilent: " + topBottomAgilent.getDeviceIdentity();
	msg += "U Wave / Axial Agilent: " + uWaveAxialAgilent.getDeviceIdentity();
	msg += "Flashing Agilent: " + flashingAgilent.getDeviceIdentity();
	msg += "Tektronics 1: " + gpibHandler.queryIdentity( TEKTRONICS_AFG_1_ADDRESS ) + "\n";
	msg += "Tektronics 2: " + gpibHandler.queryIdentity( TEKTRONICS_AFG_2_ADDRESS ) + "\n";
	msg += "RSG: " + gpibHandler.queryIdentity( RSG_ADDRESS ) + "\n";
	errBox( msg );
	
	menu.LoadMenu(IDC_MASTERCODE);
	this->SetMenu(&menu);
	this->ShowWindow(SW_MAXIMIZE);
	return TRUE;
}

std::unordered_map<std::string, COLORREF> MasterWindow::getRGBs()
{
	return this->masterRGBs;
}