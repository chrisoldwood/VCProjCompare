////////////////////////////////////////////////////////////////////////////////
//! \file   ToolSetting.hpp
//! \brief  The ToolSetting structure declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef TOOLSETTING_HPP
#define TOOLSETTING_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! A setting for a particular tool.

struct ToolSetting
{
	//
	// Members.
	//
	tstring	m_tool;		//!< The name of the tool.
	tstring	m_setting;	//!< The name of the setting.

	//! Default constructor.
	ToolSetting();

	//! Full constructor.
	ToolSetting(const tstring& tool, const tstring& setting);
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline ToolSetting::ToolSetting()
	: m_tool()
	, m_setting()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Full constructor.

inline ToolSetting::ToolSetting(const tstring& tool, const tstring& setting)
	: m_tool(tool)
	, m_setting(setting)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Global < operator for comparing two ToolSettings.

inline bool operator<(const ToolSetting& lhs, const ToolSetting& rhs)
{
	if (lhs.m_tool < rhs.m_tool)
		return true;

	if (lhs.m_tool == rhs.m_tool)
	{
		if (lhs.m_setting < rhs.m_setting)
			return true;
	}

	return false;
}

#endif // TOOLSETTING_HPP
