////////////////////////////////////////////////////////////////////////////////
//! \file   ProjectCompare.cpp
//! \brief  The functions used to perform the project comparison..
//! \author Chris Oldwood

#include "Common.hpp"
#include "ProjectCompare.hpp"
#include <WCL/FolderIterator.hpp>
#include <WCL/File.hpp>
#include <XML/Reader.hpp>
#include <XML/XPathIterator.hpp>
#include <Core/ParseException.hpp>
#include <Core/StringUtils.hpp>
#include "TheApp.hpp"

////////////////////////////////////////////////////////////////////////////////
// Local type aliases.

typedef std::map<tstring, uint> ValueCounts;
typedef std::map<tstring, ValueCounts> ConfigValueCounts;

////////////////////////////////////////////////////////////////////////////////
// Local constants.

static const tstring DEFAULT = TXT("(default)");

////////////////////////////////////////////////////////////////////////////////
//! The recursive function to find the files.

void findProjectFilesImpl(const CPath& folder, FileList& files)
{
	WCL::FolderIterator end;

	for (WCL::FolderIterator it(tstring(folder), TXT("*.vcproj")); it != end; ++it)
		files.push_back(tstring(folder / *it));

	for (WCL::FolderIterator it(tstring(folder), TXT("*.*"), WCL::FolderIterator::FIND_FOLDERS); it != end; ++it)
		findProjectFilesImpl(folder / *it, files);
}

////////////////////////////////////////////////////////////////////////////////
//! Find project files in a folder and its children.

bool findProjectFiles(const tstring& folder, FileList& files)
{
	try
	{
		findProjectFilesImpl(CPath(folder.c_str()), files);
	}
	catch (const Core::Exception& e)
	{
		g_app.FatalMsg(TXT("Failed searching for project files under:-\n\n%s\n\n%s"), folder.c_str(), e.What());
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Read in all the project files.

bool readProjectFiles(Projects& projects)
{
	// Read in the project files.
	for (Projects::iterator it = projects.begin(); it != projects.end(); ++it)
	{
		ProjectFile& project = *it;

		try
		{
			tstring xml      = CFile::ReadTextFile(project.m_pathName.c_str());
			project.m_xmlDoc = XML::Reader().ReadDocument(xml);
		}
		catch (const Core::Exception& e)
		{
			g_app.FatalMsg(TXT("Failed to read project file:-\n\n%s\n\n%s"), project.m_pathName.c_str(), e.What());
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Parse the project files into a single structure.

bool parseProjectFiles(const Projects& projects, ProjectSettings& settings)
{
	// Type aliases.
	typedef std::set<size_t> Indexes;

	settings.clear();

	Indexes libProjects;

	// For each project...
	for (size_t p = 0; p < projects.size(); ++p)
	{
		const ProjectFile& project = projects[p];

		try
		{
			XML::ElementNodePtr projectNode = project.m_xmlDoc->GetRootElement();

			if ((projectNode.Get() == nullptr) || (projectNode->Name() != TXT("VisualStudioProject")))
				throw Core::ParseException(TXT("Failed to locate the <VisualStudioProject> node"));

			// Extract key.
			tstring projectName = projectNode->GetAttributes().Get(TXT("Name"))->Value();

			// For each configuration...
			XML::XPathIterator configIter(TXT("Configurations/Configuration"), projectNode);
			XML::XPathIterator configEnd;

			for (;configIter != configEnd; ++configIter)
			{
				XML::ElementNodePtr configNode = Core::dynamic_ptr_cast<XML::ElementNode>(*configIter);

				ASSERT(configNode.Get() != nullptr);

				// Extract key.
				tstring configName = configNode->GetAttributes().Get(TXT("Name"))->Value();
				tstring configType = configNode->GetAttributes().Get(TXT("ConfigurationType"))->Value();

				if (configType == TXT("4"))
					libProjects.insert(p);

				// For each tool...
				XML::XPathIterator toolIter(TXT("Tool"), configNode);
				XML::XPathIterator toolEnd;

				for (;toolIter != toolEnd; ++toolIter)
				{
					XML::ElementNodePtr toolNode = Core::dynamic_ptr_cast<XML::ElementNode>(*toolIter);

					ASSERT(toolNode.Get() != nullptr);

					// Find/Create tool subkey.
					tstring         toolName       = toolNode->GetAttributes().Get(TXT("Name"))->Value();
					ConfigSettings& configSettings = settings[toolName];

					// For each tool setting...
					XML::Attributes::const_iterator settingIter = toolNode->GetAttributes().Begin();
					XML::Attributes::const_iterator settingEnd  = toolNode->GetAttributes().End();

					for (;settingIter != settingEnd; ++settingIter)
					{
						const XML::AttributePtr& attribute = *settingIter;

						// Skip tool name.
						if (attribute->Name() == TXT("Name"))
							continue;

						tstring setting = attribute->Name();
						tstring value   = attribute->Value();

						// Find/Create setting subkeys.
						ConfigValues& configValues = configSettings[setting];
						Values&       values       = configValues[configName];

						// Default all project setting values.
						if (values.empty())
						{
							for (size_t i = 0; i != projects.size(); ++i)
								values.push_back(ValuePtr(new tstring(DEFAULT)));
						}

						*values[p] = value;
					}
				}
			}	
		}
		catch (const Core::Exception& e)
		{
			g_app.FatalMsg(TXT("Failed to parse project file:-\n\n%s\n\n%s"), project.m_fileName.c_str(), e.What());
			return false;
		}
	}

	if (!libProjects.empty())
	{
		// Remove settings not applicable to static library projects.
		for (Indexes::const_iterator it = libProjects.begin(); it != libProjects.end(); ++it)
		{
			const size_t& index = *it;

			// For each tool...
			for (ProjectSettings::iterator toolIter = settings.begin(); toolIter != settings.end(); ++toolIter)
			{
				const tstring&  tool           = toolIter->first;
				ConfigSettings& configSettings = toolIter->second;

				// Tool not valid for static libraries?
				if ( (tstricmp(tool.c_str(), TXT("VCLinkerTool")) == 0)
				  || (tstricmp(tool.c_str(), TXT("VCResourceCompilerTool")) == 0)
				  || (tstricmp(tool.c_str(), TXT("VCMIDLTool")) == 0) )
				{
					// For each setting...
					for (ConfigSettings::iterator settingIter = configSettings.begin(); settingIter != configSettings.end(); ++settingIter)
					{
//						const tstring& setting      = settingIter->first;
						ConfigValues&  configValues = settingIter->second;

						// For each build configuration...
						for (ConfigValues::iterator configIter = configValues.begin(); configIter != configValues.end(); ++configIter)
						{
//							const tstring& config = configIter->first;
							Values&        values = configIter->second;

							ASSERT(*values[index] == DEFAULT);

							values[index].Reset();
						}
					}
				}
			}
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Convert the raw project settings data to the table output format.

void listSettings(const Projects& projects, const ProjectSettings& settings, Table& results)
{
	// For each tool...
	for (ProjectSettings::const_iterator toolIter = settings.begin(); toolIter != settings.end(); ++toolIter)
	{
		const tstring&        tool           = toolIter->first;
		const ConfigSettings& configSettings = toolIter->second;

		// For each setting...
		for (ConfigSettings::const_iterator settingIter = configSettings.begin(); settingIter != configSettings.end(); ++settingIter)
		{
			const tstring&      setting      = settingIter->first;
			const ConfigValues& configValues = settingIter->second;

			// For each build configuration...
			for (ConfigValues::const_iterator configIter = configValues.begin(); configIter != configValues.end(); ++configIter)
			{
				const tstring& config = configIter->first;
				const Values&  values = configIter->second;

				// For each project...
				for (size_t i = 0; i < projects.size(); ++i)
				{
					// Ignore settings that are not applicable.
					if (values[i].Get() == nullptr)
						continue;

					const tstring& project = projects[i].m_fileName;
					const tstring& path    = projects[i].m_pathName;
					const tstring& value   = *values[i];

					RowPtr row(new Row);

					row->push_back(tool);
					row->push_back(setting);
					row->push_back(config);
					row->push_back(project);
					row->push_back(tstring(CPath(path.c_str()).Directory()));
					row->push_back(value);

					results.push_back(row);
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Find the most common value, if it exists.

bool findCommonValue(const ValueCounts& valueCounts, tstring& commonValue)
{ 
	// Find the most common value.
	uint commonCount = 0;
	bool unique = false;

	for (ValueCounts::const_iterator it = valueCounts.begin(); it != valueCounts.end(); ++it)
	{
		const tstring& valueName  = it->first;
		const uint&    valueCount = it->second;

		// More popular?
		if (valueCount > commonCount)
		{
			commonValue = valueName;
			commonCount = valueCount;
			unique = true;
		}
		// Also as popular?
		else if (valueCount == commonCount)
		{
			commonValue.clear();
			unique = false;
		}
	}

	return unique;
}

////////////////////////////////////////////////////////////////////////////////
//! Compare the project settings data and output a table of the differences.

void compareSettings(const Projects& projects, const ProjectSettings& settings, Table& results)
{
	// For each tool...
	for (ProjectSettings::const_iterator toolIter = settings.begin(); toolIter != settings.end(); ++toolIter)
	{
		const tstring&        tool           = toolIter->first;
		const ConfigSettings& configSettings = toolIter->second;

		// For each setting...
		for (ConfigSettings::const_iterator settingIter = configSettings.begin(); settingIter != configSettings.end(); ++settingIter)
		{
			const tstring&      setting      = settingIter->first;
			const ConfigValues& configValues = settingIter->second;

			// Ignore this setting?
			if (g_app.m_projectDepList.find(ToolSetting(tool, setting)) != g_app.m_projectDepList.end())
				continue;

			//
			// Compare the different values.
			//

			// Compare this setting according to the build configuration?
			if (g_app.m_buildDepList.find(ToolSetting(tool, setting)) != g_app.m_buildDepList.end())
			{
				ConfigValueCounts configValueCounts;

				// Build a map of the different values and their frequency.
				for (ConfigValues::const_iterator configIter = configValues.begin(); configIter != configValues.end(); ++configIter)
				{
					const tstring& config = configIter->first;
					const Values&  values = configIter->second;

					for (Values::const_iterator itValue = values.begin(); itValue != values.end(); ++itValue)
					{
						const ValuePtr& value = *itValue;

						if (value.Get() != nullptr)
							configValueCounts[config][*value]++;
					}
				}

				// For each build configuration...
				for (ConfigValueCounts::const_iterator configIter = configValueCounts.begin(); configIter != configValueCounts.end(); ++configIter)
				{
					const tstring&     config      = configIter->first;
					const ValueCounts& valueCounts = configIter->second;

					// Find the most common value.
					tstring commonValue;

					bool unique = findCommonValue(valueCounts, commonValue);

					// Add an entry to show the common value.
					if ((valueCounts.size() > 1) && unique)
					{
						RowPtr row(new Row);

						row->push_back(tool);
						row->push_back(setting);
						row->push_back(config);
						row->push_back(TXT("*"));
						row->push_back(TXT("*"));
						row->push_back(commonValue);

						results.push_back(row);
					}

					// Add comparison results to the table.
					const Values& values = configValues.find(config)->second;

					for (size_t i = 0; i < projects.size(); ++i)
					{
						// Ignore settings that are not applicable.
						if (values[i].Get() == nullptr)
							continue;

						const tstring& project = projects[i].m_fileName;
						const tstring& path    = projects[i].m_pathName;
						const tstring& value   = *values[i];

						if (!unique || (value != commonValue))
						{
							RowPtr row(new Row);

							row->push_back(tool);
							row->push_back(setting);
							row->push_back(config);
							row->push_back(project);
							row->push_back(tstring(CPath(path.c_str()).Directory()));
							row->push_back(value);

							results.push_back(row);
						}
					}
				}
			}
			else
			{
				ValueCounts valueCounts;

				// Build a map of the different values and their frequency.
				for (ConfigValues::const_iterator configIter = configValues.begin(); configIter != configValues.end(); ++configIter)
				{
//					const tstring& configName = configIter->first;
					const Values&  values     = configIter->second;

					for (Values::const_iterator valueIter = values.begin(); valueIter != values.end(); ++valueIter)
					{
						const ValuePtr& value = *valueIter;

						if (value.Get() != nullptr)
							valueCounts[*value]++;
					}
				}

				// Find the most common value.
				tstring commonValue;

				bool unique = findCommonValue(valueCounts, commonValue);

				// Add an entry to show the common value.
				if ((valueCounts.size() > 1) && unique)
				{
					RowPtr row(new Row);

					row->push_back(tool);
					row->push_back(setting);
					row->push_back(TXT("*"));
					row->push_back(TXT("*"));
					row->push_back(TXT("*"));
					row->push_back(commonValue);

					results.push_back(row);
				}

				// Add comparison results to the table.
				for (ConfigValues::const_iterator configIter = configValues.begin(); configIter != configValues.end(); ++configIter)
				{
					const tstring& config = configIter->first;
					const Values&  values = configIter->second;

					for (size_t i = 0; i < projects.size(); ++i)
					{
						// Ignore settings that are not applicable.
						if (values[i].Get() == nullptr)
							continue;

						const tstring& project = projects[i].m_fileName;
						const tstring& path    = projects[i].m_pathName;
						const tstring& value   = *values[i];

						if (!unique || (value != commonValue))
						{
							RowPtr row(new Row);

							row->push_back(tool);
							row->push_back(setting);
							row->push_back(config);
							row->push_back(project);
							row->push_back(tstring(CPath(path.c_str()).Directory()));
							row->push_back(value);

							results.push_back(row);
						}
					}
				}
			}
		}
	}
}
