////////////////////////////////////////////////////////////////////////////////
//! \file   OptionsDlg.hpp
//! \brief  The OptionsDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef OPTIONSDLG_HPP
#define OPTIONSDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "ProjectCompare.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The dialog used to configure the program settings.

class OptionsDlg : public CDialog
{
public:
	//! Default constructor.
	OptionsDlg();
	
private:
	//
	// Members.
	//
	ToolSettings	m_ignoreList;		//!< The settings to be ignored when comparing.
	ToolSettings	m_buildDepList;		//!< The settings that are build dependent.

	//
	// Controls.
	//
	CRadioBtn		m_ignoreButton;		//!< Button to select the Ignore settings.
	CRadioBtn		m_buildButton;		//!< Button to select the Build Dependent settings.
	CListView		m_settingsList;		//!< The view of the list of settings.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();

	//! Switch to the Ignored settings list.
	void onEditIgnoreList();

	//! Switch to the build settings list.
	void onEditBuildList();

	//! Remove the selected setting.
	void onRemoveSetting();

	//
	// Internal methods.
	//

	//! Populate the list of settings.
	void populateSettings(const ToolSettings& settings);
};

#endif // OPTIONSDLG_HPP
