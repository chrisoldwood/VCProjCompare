////////////////////////////////////////////////////////////////////////////////
//! \file   OptionsDlg.cpp
//! \brief  The OptionsDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "OptionsDlg.hpp"
#include "Resource.h"
#include "TheApp.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

OptionsDlg::OptionsDlg()
	: CDialog(IDD_OPTIONS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_IGNORE,	&m_ignoreButton)
		CTRL(IDC_BUILD,		&m_buildButton)
		CTRL(IDC_SETTINGS,	&m_settingsList)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_IGNORE,	BN_CLICKED,	&OptionsDlg::onEditIgnoreList)
		CMD_CTRLMSG(IDC_BUILD,	BN_CLICKED,	&OptionsDlg::onEditBuildList)
		CMD_CTRLMSG(IDC_REMOVE,	BN_CLICKED,	&OptionsDlg::onRemoveSetting)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void OptionsDlg::OnInitDialog()
{
	// Capture existing settings.
	m_ignoreList   = g_app.m_ignoreList;
	m_buildDepList = g_app.m_buildDepList;

	// Initialise controls.
	m_settingsList.InsertColumn(0, TXT("Tool"),    m_settingsList.StringWidth(30), LVCFMT_LEFT);
	m_settingsList.InsertColumn(1, TXT("Setting"), m_settingsList.StringWidth(30), LVCFMT_LEFT);
	m_settingsList.FullRowSelect();

	m_ignoreButton.Check(true);
	m_buildButton.Check(false);
	onEditIgnoreList();
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool OptionsDlg::OnOk()
{
	// Save new settings.
	g_app.m_ignoreList = m_ignoreList;
	g_app.m_modified  |= TheApp::IGNORE_LIST;

	g_app.m_buildDepList = m_buildDepList;
	g_app.m_modified    |= TheApp::BUILDDEP_LIST;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Switch to the Ignored settings list.

void OptionsDlg::onEditIgnoreList()
{
	populateSettings(m_ignoreList);
}

////////////////////////////////////////////////////////////////////////////////
//! Switch to the build settings list.

void OptionsDlg::onEditBuildList()
{
	populateSettings(m_buildDepList);
}

////////////////////////////////////////////////////////////////////////////////
//! Remove the selected setting.

void OptionsDlg::onRemoveSetting()
{
	// Ignore, if no selection.
	if (!m_settingsList.IsSelection())
		return;

	ToolSetting toolSetting;

	// Get the selected item.
	size_t selection      = m_settingsList.Selection();
	toolSetting.m_tool    = m_settingsList.ItemText(selection, 0);
	toolSetting.m_setting = m_settingsList.ItemText(selection, 1);

	// Remove from the list.
	if (m_ignoreButton.IsChecked())
		m_ignoreList.erase(m_ignoreList.find(toolSetting));
	else
		m_buildDepList.erase(m_buildDepList.find(toolSetting));

	// Remove from the view and fixup selection.
	m_settingsList.DeleteItem(selection);

	if (selection == m_settingsList.ItemCount())
		--selection;

	m_settingsList.Select(selection);
}

////////////////////////////////////////////////////////////////////////////////
//! Populate the list of settings.

void OptionsDlg::populateSettings(const ToolSettings& settings)
{
	// Clear existing data.
	m_settingsList.DeleteAllItems();

	size_t i = 0;

	// Load the list of settings.
	for (ToolSettings::const_iterator it = settings.begin(); it != settings.end(); ++it, ++i)
	{
		const ToolSetting& setting = *it;

		m_settingsList.insertItem (i,    setting.m_tool);
		m_settingsList.setItemText(i, 1, setting.m_setting);
	}
}
