/******************************************************************************/
/*!
\file       Serializer.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       01 September 2022
\brief

Serializer class declaration for both Serializing and Deserializing using 
RTTR and nlohmann json libraries.

Functions use a reflection system to determine the type for both
serialization and deserialization.

Whenever new components are added: 
- UPDATE SerializeEntities function
- UPDATE DeserializeJson function
- UPDATE RTTR_REGISTRATION macro

SerializeEntities, DeserializeJson functions: 
- I can't think of a way to 'automate' getting of types putting it into AddComponent and GetComponent for coordinator.

Serialization:
- Use reflection system (rttr library) to generate JSON object.
- Use JSON object and JSON library (rapidjason) to write to file.

Deserialization:
- Read JSON object from file.
- Use JSON object for components/prefabs.


HOW TO REGISTER TO RTTR REFLECTION SYSTEM: 

RTTR_ENABLE(...)
- Only use for componenets that are to be serialised
- Base and derived classes (components).
- Enums are not required to have

RTTR_REGISTRATION 
{
// FOR COMPONENT CLASSES

rttr::registration::class_<ComponentName>("ComponentName")
.constructor<float, std::string, int, ...>()	// If class has a constructor
.property("variableName", &ComponentName::variableName) // public variable
.property("variableName", &ComponentName::GetterFunction, &ComponentName::SetterFunction) // private variable
;

// FOR ENUMS
rttr::registration::enumeration<EnumName>("EnumName")
(
rttr::value("First", EnumName::First),
rttr::value("Second", EnumName::Second),
rttr::value("Third", EnumName::Third)
)
;
}
*/
/******************************************************************************/
#pragma once

#include "include/ECS/Architecture/ECS.hpp"
#include "include/Logging.hpp"

#include "include/ECS/Architecture/Coordinator.hpp"

#include "lib/rttr/registration.h"
#include "lib/nlohmann/json.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 

#include "lib/glm/glm.hpp"

namespace Engine 
{
	// For reflection and serialization of json files
	using namespace rttr;
	using namespace nlohmann;

	class Serializer
	{
	public:
		static void SerializeEntities(Coordinator* coordinator, std::string scenefile);
		static void DeserializeJson(Coordinator* coordinator, std::string filename);
		
	private:
		// For Serialization
		static json InstanceToJson(json writer, instance obj, std::string name);

		static std::string CombineTwoJsonWriters(json j1, json j2);
		static std::string CombineStringJsonWriter(std::string j1, json j2);
		static void StringToJson(const std::string& filename, const std::string& jsonString);
		 
		static void InstanceToWriter(const instance obj, json& writer);
		static bool VariantToWriter(const variant& var, json& writer);
		 
		// Serializing, determine the type of the variant and insert it to the writer
		static bool FundementalType(const type& t, const variant& var, json& writer);
		static void Array(const variant_sequential_view& view, json& writer);
		static void AssociativeContainer(const variant_associative_view& view, json& writer);
		 
		// For Deserialization
		static void JsonToInstance(instance obj, json& jsonValue);
		 
		// Deserializing, determine the type and value of json and 'insert' it to instance
		static variant GetKeyValue(json::iterator& iter, const type& t);
		static variant GetBasicTypes(json& jsonValue);
		static void SetArrayRecursive(variant_sequential_view& view, json& variable);
		static void SetAssociativeContainerRecursive(variant_associative_view& view, json& variable);
	};

} // end of namespace