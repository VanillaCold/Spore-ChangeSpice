#include "stdafx.h"
#include "ChangeSpice.h"

ChangeSpice::ChangeSpice()
{
}


ChangeSpice::~ChangeSpice()
{
}


void ChangeSpice::ParseLine(const ArgScript::Line& line)
{
	auto args = line.GetArguments(1); //get the specified spice
	uint32_t spiceID = id(args[0]); //convert the string to an FNV hash
	if (line.HasFlag("useHash")) //check if the name should be instead treated as a hash
	{
		mpFormatParser->ParseUInt(args[0]); //parse name as a hash
	}

	if (!line.HasFlag("noAliases")) //do all of the aliases
	{
		if (spiceID == id("red")) { spiceID = id("spice1"); }
		if (spiceID == id("blue")) { spiceID = id("spice2"); }
		if (spiceID == id("yellow")) { spiceID = id("spice3"); }
		if (spiceID == id("green")) { spiceID = id("spice4"); }
		if (spiceID == id("pink")) { spiceID = id("spice5"); }
		if (spiceID == id("purple")) { spiceID = id("spice6"); }
	}


	PropertyListPtr propList;
	if (PropManager.GetPropertyList(spiceID, GroupIDs::SpaceTrading, propList)) //check if the spice even exists
	{
		uint32_t spicecolour;
		if (App::Property::GetUInt32(propList.get(), 0x058CBB75, spicecolour)) //check if specified file is actually a spice and not anything else
		{
			Simulator::GetActivePlanetRecord()->mSpiceGen.instanceID = spiceID; //change spice ID
			Simulator::GetActivePlanet()->field_1C4 = spicecolour; //change spice geyser colour
		}
	}
}

const char* ChangeSpice::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Changes the spice of the current planet.";
	}
	else {
		return "This cheat changes the spice of the current planet to the specified file.\n"
			"For obvious reasons, there are aliases for the 6 vanilla spices, as follows:\n"
			"'red', 'blue', 'yellow', 'green', 'pink' and 'purple'. All of these aliases are case-sensitive.\n"
			"In case a mod adds spices that use names identical to the six vanilla aliases, you can use the '-noAliases' flag to disable the aliases.\n"
			"Syntax: 'changeSpice [spice filename/alias] [flags].";
	}
}
