/******************************************************************************/
/*!
\file       Serializer.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       01 September 2022
\brief

  Serializer class function implementations for both Serializing and Deserializing 
  using RTTR and nlohmann json libraries.
  
  Functions use a reflection system to determine the type for both 
  serialization and deserialization.
  
  Terms:
  Fundamental Types - Plain old data (POD), Enums, Strings
  variant - stores the value of property
  Sequential Container - array, vector, deque, forward_list, list
  Associative Container - set, map, multiset, multimap

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#include "include/Serialization/Serializer.hpp"
#include "include/ECS/Component/Transform.hpp"


#define SERIALIZE_COMPONENTS SERIALIZE_OBJECT(Transform, "1Transform")\
							 SERIALIZE_OBJECT(Script, "Script")\
							 SERIALIZE_OBJECT_VECTOR(std::vector<Transform>, "vTransform")


#define DESERIALIZE_COMPONENTS	DESERIALIZE_OBJECT_BASIC(Script, "Script")\
								DESERIALIZE_OBJECT_VECTOR(std::vector<Transform>, Transform, "vTransform")


#define SERIALIZE_OBJECT(type, oName)	if (coordinator->HasComponent<type>(entity))\
										{\
											type* ptr = coordinator->GetComponent<type>(entity);\
											instance t = ptr;\
											writer = Serializer::InstanceToJson(writer, ptr, oName);\
										}


#define SERIALIZE_OBJECT_VECTOR(vType, vName)	if (coordinator->HasComponent<vType>(entity))\
												{\
													vType* ptr = coordinator->GetComponent<vType>(entity);\
													int i = 0;\
													for (auto& t : *ptr)\
													{\
														VariantToWriter(t, writer[vName][i]);\
														i++;\
													}\
												}


#define DESERIALIZE_TRANSFORM	if (component.first == "1Transform")\
								{\
									Transform transform{};\
									json component_json_value = component.second.get<json::object_t>();\
									JsonToInstance(transform, component_json_value);\
									coordinator->AddComponent<Transform>(*entity);\
									Transform* ptr = coordinator->GetComponent<Transform>(*entity);\
									*ptr = transform;\
								}


#define DESERIALIZE_TRANSFORM_PREFAB	if (component.first == "1Transform")\
										{\
											Transform transform{};\
											json component_json_value = component.second.get<json::object_t>();\
											JsonToInstance(transform, component_json_value);\
											if (coordinator->HasComponent<Transform>(*entity))\
											{\
												Transform* ptr = coordinator->GetComponent<Transform>(*entity);\
												if (!ptr->isOverridePosition)\
												{\
													ptr->position = transform.position;\
												}\
												if (!ptr->isOverrideRotation)\
												{\
													ptr->rot_q = transform.rot_q;\
												}\
												if (!ptr->isOverrideScale)\
												{\
													ptr->scale = transform.scale;\
												}\
											}\
											else\
											{\
												coordinator->AddComponent<Transform>(*entity);\
												Transform* ptr = coordinator->GetComponent<Transform>(*entity);\
												*ptr = transform;\
											}\
										}


#define DESERIALIZE_OBJECT_COMBINE(type, oName)	else if (component.first == oName)\
												{\
													type t{};\
													json component_json_value = component.second.get<json::object_t>();\
													JsonToInstance(t, component_json_value);\
													if (coordinator->HasComponent<type>(*entity))\
													{\
														type* ptr = coordinator->GetComponent<type>(*entity);\
														*ptr += t;\
													}\
													else\
													{\
														coordinator->AddComponent<type>(*entity);\
														type* ptr = coordinator->GetComponent<type>(*entity);\
														*ptr = t;\
													}\
												}


#define DESERIALIZE_OBJECT_BASIC(type, oName)	else if (component.first == oName)\
												{\
													type t{};\
													json component_json_value = component.second.get<json::object_t>();\
													JsonToInstance(t, component_json_value);\
													if (coordinator->HasComponent<type>(*entity))\
													{\
														type* ptr = coordinator->GetComponent<type>(*entity);\
														*ptr = t;\
													}\
													else\
													{\
														coordinator->AddComponent<type>(*entity);\
														type* ptr = coordinator->GetComponent<type>(*entity);\
														*ptr = t;\
													}\
												}


#define DESERIALIZE_OBJECT_COLLIDER(type, oName)	else if (component.first == oName)\
													{\
														type t{};\
														json component_json_value = component.second.get<json::object_t>();\
														JsonToInstance(t, component_json_value);\
														if (coordinator->HasComponent<type>(*entity))\
														{\
															type* ptr = coordinator->GetComponent<type>(*entity);\
															ptr->RemoveActor();\
															*ptr = t;\
															ptr->CreateActor();\
														}\
														else\
														{\
															coordinator->AddComponent<type>(*entity, type(coordinator->GetComponent<Transform>(*entity)));\
															type* ptr = coordinator->GetComponent<type>(*entity);\
															*ptr = t;\
															ptr->CreateActor();\
														}\
													}


#define DESERIALIZE_OBJECT_VECTOR(vType, type, vName)	else if (component.first == vName)\
														{\
															vType v{};\
															type t{};\
															if (coordinator->HasComponent<vType>(*entity))\
															{\
																json component_json_value = component.second.get<json::array_t>();\
																for (auto& value : component_json_value)\
																{\
																	JsonToInstance(t, value);\
																	v.emplace_back(t);\
																}\
																vType* ptr = coordinator->GetComponent<vType>(*entity);\
																*ptr = v;\
															}\
															else\
															{\
																coordinator->AddComponent<vType>(*entity);\
																json component_json_value = component.second.get<json::array_t>();\
																for (auto& value : component_json_value)\
																{\
																	JsonToInstance(t, value);\
																	v.emplace_back(t);\
																}\
																vType* ptr = coordinator->GetComponent<vType>(*entity);\
																*ptr = v;\
															}\
														}


namespace Engine
{
	//using namespace Graphics::Addition;

	void Serializer::SerializeEntities(Coordinator* coordinator, std::string scenefile)
	{
		std::vector<std::string> vJsonStrings{};
		for (auto& entity : coordinator->GetEntities())
		{
			json writer;
			EntityID parentID = entity.GetParent();

			// Set parent ID according to coordinator's mEntities indexing, then set it back to normal after serialising
			if (entity.IsChild())
			{
				for (int i = 0; i < coordinator->GetEntities().size(); ++i)
				{
					if (parentID == coordinator->GetEntities()[i].GetEntityID())
					{
						entity.SetParentID(i);
					}
				}
			}

			writer = Serializer::InstanceToJson(writer, entity, "0Entity");
			SERIALIZE_COMPONENTS

			vJsonStrings.emplace_back(writer.dump(4));

			entity.SetParentID(parentID);
		}

		// Formatting json objects string
		std::string js{};
		for (int i = 0; i < vJsonStrings.size(); ++i)
		{
			if (i != vJsonStrings.size() - 1)
			{
				js += vJsonStrings[i] + ",\n";
			}
			else
			{
				js += vJsonStrings[i];
			}
		}
		Serializer::StringToJson(scenefile, js);
	}


	void Serializer::SerializePrefab(Coordinator* coordinator, EntityID id, std::string filename)
	{
		std::vector<std::string> vJsonStrings{};
		json writer;

		// Need to recursively get all the children and store into container
		std::vector<EntityID> ids{};
		coordinator->GetAllChildren(ids, id);

		// Serialise root entity first. Parent will be nothing -> Reset back afterwards
		Entity& entity = *coordinator->GetEntity(ids[0]);
		EntityID parentID = entity.GetParent();
		entity.SetParentID(MAX_ENTITIES + 1);
		entity.SetPrefab(filename);

		writer = Serializer::InstanceToJson(writer, entity, "0Entity");
		SERIALIZE_COMPONENTS
		vJsonStrings.emplace_back(writer.dump(4));

		// Reset parent ID back
		entity.SetParentID(parentID);


		// If entity has children, similar process, parent id is index in ids instead.
		for (int i = 1;  i < ids.size(); ++i)
		{
			Entity& entity = *coordinator->GetEntity(ids[i]);
			EntityID parentID = entity.GetParent();

			writer.clear();

			for (int j = 0; j < ids.size(); ++j)
			{
				if (parentID == ids[j])
				{
					entity.SetParentID(j);

					writer = Serializer::InstanceToJson(writer, entity, "0Entity");
					SERIALIZE_COMPONENTS
					vJsonStrings.emplace_back(writer.dump(4));

					// Reset the ID back to original
					entity.SetParentID(parentID);
				}
			}
		}

		// Formatting json objects string
		std::string js{};
		for (int i = 0; i < vJsonStrings.size(); ++i)
		{
			if (i != vJsonStrings.size() - 1)
			{
				js += vJsonStrings[i] + ",\n";
			}
			else
			{
				js += vJsonStrings[i];
			}
		}
		Serializer::StringToJson(filename, js);
	}


	void Serializer::DeserializeJson(Coordinator* coordinator, std::string filename)
	{
		DeserializeJsonInternal(coordinator, filename);
	}


	EntityID Serializer::CreateEntityPrefab(Coordinator* coordinator, std::string filename)
	{
		// Parse string to writer, check error
		std::ifstream ifs{ "Assets/" + filename };
		json writer;

		// Check for parse error
		try
		{
			writer = json::parse(ifs);
		}
		catch (json::parse_error& e)
		{
			LOG_WARNING("Parse Error: PREFAB file either does not exist or has formatting issues", e.what());
			return MAX_ENTITIES + 1;
		}

		std::vector<EntityID> ids{};

		for (auto& object : writer)
		{
			// Check if object has "Entity", false = invalid
			if (!(object.contains("0Entity")))
			{
				LOG_ERROR("Object does not have an Entity! Json file will NOT be deserialized!");
				return MAX_ENTITIES + 1;
			}

			// Always deserialize entity first, set its properties, then proceed.
			// Get correct naming before creating entity
			std::string entity_name = object["0Entity"]["name"];
			std::string duplicate_name{};

			int i = 1;
			for (int size = 0; size < coordinator->GetEntities().size(); ++size)
			{
				duplicate_name = entity_name + "(" + std::to_string(i) + ")";

				if (coordinator->GetEntities()[size].GetEntityName() == duplicate_name)
				{
					// Increament till highest value, reset back loop to check again.
					++i;
					size = 0;
				}
			}
			EntityID entity_id = coordinator->CreateEntity(duplicate_name);
			Entity* entity = coordinator->GetEntity(entity_id);
			entity->SetParentID(object["0Entity"]["parent"]);
			entity->SetPrefab(object["0Entity"]["prefab"]);

			// Push back to container to update to the correct parent id for coordinator's container
			ids.emplace_back(entity_id);

			// Get all component names, remove "Entity"
			auto componentList = object.get<json::object_t>();
			componentList.erase(componentList.begin());

			// Go through each component of an object
			for (auto& component : componentList)
			{
				// Get variant, type and add component to coordinator through component name string
				DESERIALIZE_TRANSFORM_PREFAB
				DESERIALIZE_COMPONENTS
			}
		}

		// Skip ids[0] as parent id will always be invalid 
		EntityID offset = ids[0];

		// Once prefab entities are deserialised, set parent ids accordingly.
		for (int i = 1; i < ids.size(); ++i)
		{
			EntityID id = ids[i];
			Entity* entity = coordinator->GetEntity(id);

			coordinator->ToChild(offset + entity->GetParent(), id);
		}

		// Return head entity id
		return offset;
	}


	void Serializer::ApplyUpdatedPrefab(Coordinator* coordinator, std::string filename)
	{
		for (auto& entity : coordinator->GetEntities())
		{
			if (entity.GetPrefab() == filename)
			{
				// Replace with prefab components
				DeserializePrefab(coordinator, &entity, filename);
			}
		}
	}


	json Serializer::InstanceToJson(json writer, instance obj, std::string cName)
	{
		// Check instance for errors
		if (!obj.is_valid())
		{
			LOG_WARNING("Instance Error.");
			return std::string{};
		}

		InstanceToWriter(obj, writer[cName.c_str()]);

		return writer;
	}


	void Serializer::DeserializeJsonInternal(Coordinator* coordinator, std::string filename)
	{
		// Parse string to writer, check error
		std::ifstream ifs{ "Assets/" + filename };
		json writer;

		// Check for parse error
		try
		{
			writer = json::parse(ifs);
		}
		catch (json::parse_error& e)
		{
			LOG_WARNING("Parse Error: SCENE file either does not exist or has formatting issues", e.what());
			return;
		}

		// Loop through each object in writer
		for (auto& object : writer)
		{
			// Check if object has "Entity", false = invalid
			if (!(object.contains("0Entity")))
			{
				LOG_ERROR("Object does not have an Entity! Json file will NOT be deserialized!");
				return;
			}

			// Always deserialize entity first, set its properties, then proceed
			EntityID entity_id = coordinator->CreateEntity(object["0Entity"]["name"]);
			Entity* entity = coordinator->GetEntity(entity_id);
			
			// Dont deserialise prefab as its properties should be serialised
			entity->SetPrefab(object["0Entity"]["prefab"]);
			entity->SetParentID(object["0Entity"]["parent"]); 

			// Get all component names, remove "Entity"
			auto componentList = object.get<json::object_t>();
			componentList.erase(componentList.begin());

			// Go through each component of an object
			for (auto& component : componentList)
			{
				// Get variant, type and add component to coordinator through component name string
				DESERIALIZE_TRANSFORM
				DESERIALIZE_COMPONENTS
			}
		}

		for (auto& entity : coordinator->GetEntities())
		{
			if (entity.GetParent() > MAX_ENTITIES)
			{
				continue;
			}

			coordinator->ToChild(entity.GetParent(), entity.GetEntityID());
		}
	}


	void Serializer::DeserializePrefab(Coordinator* coordinator, Entity* entity, std::string filename)
	{
		// Parse string to writer, check error
		std::ifstream ifs{ "Assets/" + filename };
		json writer;

		// Check for parse error
		try
		{
			writer = json::parse(ifs);
		}
		catch (json::parse_error& e)
		{
			LOG_WARNING("Parse Error: PREFAB file either does not exist or has formatting issues", e.what());
			return;
		}

		// Unable to deserialise parent-child prefab
		// For both Scene file and Reloading
		if (writer.size() > 1)
		{
			return;
		}

		entity->SetPrefab(filename);

		// Loop through each object in writer
		for (auto& object : writer)
		{
			// Get all component names
			auto componentList = object.get<json::object_t>();

			// Go through each component of an object
			for (auto& component : componentList)
			{
				// Get variant, type and add component to coordinator through component name string
				DESERIALIZE_TRANSFORM_PREFAB
				DESERIALIZE_COMPONENTS
			}
		}
	}


	std::string Serializer::CombineTwoJsonWriters(json j1, json j2)
	{
		return j1.dump(4) + ",\n" + j2.dump(4);
	}


	std::string Serializer::CombineStringJsonWriter(std::string j1, json j2)
	{
		return std::string(j1 + ",\n" + j2.dump(4));
	}


	void Serializer::StringToJson(const std::string& filename, const std::string& jsonString)
	{
		// Create JSON file and input string to file
		std::ofstream jsonFile{ "Assets/" + filename};
		jsonFile << "[\n" << jsonString << "\n]";

		// Check if successful
		if (jsonFile.good() == false)
		{
			LOG_WARNING("Cant write JSON string to file.");
			return;
		}
	}


	void Serializer::InstanceToWriter(const instance obj, json& writer)
	{
		instance rawObj = obj.get_type().get_raw_type().is_wrapper() ? obj.get_wrapped_instance() : obj;

		auto propertyList = rawObj.get_derived_type().get_properties();
		for (auto prop : propertyList)
		{
			variant prop_value = prop.get_value(obj);

			// Cannot serialize
			if (!prop_value)
			{
				continue;
			}

			std::string propertyName = prop.get_name().to_string();
			if (!VariantToWriter(prop_value, writer[propertyName.data()]))
			{
				LOG_ERROR("Failed to serialize: ", propertyName);
			}
		}
	}


	bool Serializer::VariantToWriter(const variant& var, json& writer)
	{
		type varType = var.get_type();
		variant localVar = var;

		if (varType.is_wrapper())
		{
			varType = varType.get_wrapped_type();
			localVar = localVar.extract_wrapped_value();
		}

		if (FundementalType(varType, localVar, writer))
		{
			return true;
		}
		else if (var.is_sequential_container())
		{
			Array(var.create_sequential_view(), writer);
		}
		else if (var.is_associative_container())
		{
			AssociativeContainer(var.create_associative_view(), writer);
		}
		// For specific glm types that should be stored as an array
		else if (varType == type::get<glm::vec2>() || varType == type::get<glm::vec3>() || varType == type::get<glm::vec4>() || varType == type::get<glm::uvec4>() || 
			varType == type::get<glm::mat3>() || varType == type::get<glm::mat4>() || varType == type::get<glm::quat>())
		{
			writer = json::array();
			if (varType == type::get<glm::vec2>())
			{
				for (int i = 0; i < 2; ++i)
				{
					VariantToWriter(localVar.get_value<glm::vec2>()[i], writer[i]);
				}
			}
			else if (varType == type::get<glm::vec3>())
			{
				for (int i = 0; i < 3; ++i)
				{
					VariantToWriter(localVar.get_value<glm::vec3>()[i], writer[i]);
				}
			}
			else if (varType == type::get<glm::vec4>())
			{
				for (int i = 0; i < 4; ++i)
				{
					VariantToWriter(localVar.get_value<glm::vec4>()[i], writer[i]);
				}
			}
			else if (varType == type::get<glm::uvec4>())
			{
				for (int i = 0; i < 4; ++i)
				{
					VariantToWriter(localVar.get_value<glm::uvec4>()[i], writer[i]);
				}
			}
			else if (varType == type::get<glm::mat3>())
			{
				int size = 0;
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						VariantToWriter(localVar.get_value<glm::mat3>()[i][j], writer[size++]);
					}
				}
			}
			else if (varType == type::get<glm::mat4>())
			{
				int size = 0;
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						VariantToWriter(localVar.get_value<glm::mat4>()[i][j], writer[size++]);
					}
				}
			}
			else if (varType == type::get<glm::quat>())
			{
				for (int i = 0; i < 4; ++i)
				{
					VariantToWriter(localVar.get_value<glm::quat>()[i], writer[i]);
				}
			}
		}
		// Object class type
		else
		{
			auto child_props = varType.get_properties();
			if (!child_props.empty())
			{
				// Recurve
				InstanceToWriter(var, writer);
			}
			else
			{
				// Type cannot be recognised, check registration or it could be pointers
				assert("Unknown RTTR serilization case/type.");

				return false;
			}
		}

		return true;
	}


	bool Serializer::FundementalType(const type& t, const variant& var, json& writer)
	{
		// POD
		if (t.is_arithmetic())
		{
			if (t == type::get<bool>())
			{
				writer = var.to_bool();
			}
			else if (t == type::get<char>())
			{
				writer = var.to_string();
			}
			else if (t == type::get<int>())
			{
				writer = var.to_int();
			}
			else if (t == type::get<EntityID>())
			{
				writer = var.to_uint32();
			}
			else if (t == type::get<uint64_t>())
			{
				writer = var.to_uint64();
			}
			else if (t == type::get<double>())
			{
				writer = var.to_double();
			}
			else if (t == type::get<float>())
			{
				writer = var.to_double();
			}

			return true;
		}
		// Enumeration
		else if (t.is_enumeration())
		{
			// To be used to check if it is said type
			bool flag = false;

			var.to_string(&flag);
			if (flag)
			{
				writer = var.to_string();
			}
			else
			{
				var.to_uint64(&flag);
				if (flag)
				{
					writer = var.to_uint64(&flag);
				}
				else
				{
					writer = nullptr;
				}
			}

			return true;
		}
		// String
		else if (t == type::get<std::string>())
		{
			writer = var.to_string();
			return true;
		}

		// Not a fundamental type
		return false;
	}


	void Serializer::Array(const variant_sequential_view& view, json& writer)
	{
		writer = json::array();
		int i = 0;

		for (auto& item : view)
		{
			VariantToWriter(item, writer[i]);
			i++;
		}
	}


	void Serializer::AssociativeContainer(const variant_associative_view& view, json& writer)
	{
		static const string_view key_name("key");
		static const string_view value_name("value");

		writer = json::array();
		int i = 0;

		// Dealing with keys = values containers like sets
		if (view.is_key_only_type())
		{
			for (auto& item : view)
			{
				VariantToWriter(item.first, writer);
				i++;
			}
		}
		else
		{
			for (auto& item : view)
			{
				VariantToWriter(item.first, writer[i][key_name.data()]);
				VariantToWriter(item.second, writer[i][value_name.data()]);
				i++;
			}
		}
	}


	void Serializer::JsonToInstance(instance obj, json& jsonValue)
	{
		instance rawObj = obj.get_type().get_raw_type().is_wrapper() ? obj.get_wrapped_instance() : obj;

		auto propertyList = rawObj.get_derived_type().get_properties();
		for (auto prop : propertyList)
		{
			// Get iterator of property in json
			auto iter = jsonValue.find(prop.get_name().data());
			if (iter == jsonValue.end())
			{
				continue;
			}
			const type value_type = prop.get_type();

			auto& value = iter.value();
			switch (value.type())
			{
			case json::value_t::array:
			{
				variant var;
				if (value_type.is_sequential_container())
				{
					var = prop.get_value(rawObj);
					auto view = var.create_sequential_view();

					SetArrayRecursive(view, value);
				}
				else if (value_type.is_associative_container())
				{
					var = prop.get_value(rawObj);
					auto associative_view = var.create_associative_view();
					
					SetAssociativeContainerRecursive(associative_view, value);
				}
				// Special cases that uses glm containers, fastest way but ugly
				else if (value_type == type::get<glm::vec2>() || value_type == type::get<glm::vec3>() || value_type == type::get<glm::vec4>() || value_type == type::get<glm::uvec4>()
					|| value_type == type::get<glm::mat3>() || value_type == type::get<glm::mat4>() || value_type == type::get<glm::quat>())
				{
					if (value_type == type::get<glm::vec2>())
					{
						glm::vec2 v = { value[0].get<float>(), value[1].get<float>() };
						prop.set_value(rawObj, v);
					}
					else if (value_type == type::get<glm::vec3>())
					{
						glm::vec3 v = { value[0].get<float>(), value[1].get<float>(), value[2].get<float>() };
						prop.set_value(rawObj, v);
					}
					else if (value_type == type::get<glm::vec4>())
					{
						glm::vec4 v = { value[0].get<float>(), value[1].get<float>(), value[2].get<float>(), value[3].get<float>() };
						prop.set_value(rawObj, v);
					}
					else if (value_type == type::get<glm::uvec4>())
					{
						glm::uvec4 v = { value[0].get<unsigned int>(), value[1].get<unsigned int>(), value[2].get<unsigned int>(), value[3].get<unsigned int>() };
						prop.set_value(rawObj, v);
					}
					else if (value_type == type::get<glm::mat3>())
					{
						glm::mat3 m = { value[0].get<float>(), value[1].get<float>(), value[2].get<float>(),
										value[3].get<float>(),  value[4].get<float>(), value[5].get<float>(),
										value[6].get<float>(), value[7].get<float>(), value[8].get<float>()
						};

						prop.set_value(rawObj, m);
					}
					else if (value_type == type::get<glm::mat4>())
					{
						glm::mat4 m = { value[0].get<float>(), value[1].get<float>(), value[2].get<float>(), value[3].get<float>(),
										value[4].get<float>(), value[5].get<float>(), value[6].get<float>(), value[7].get<float>(),
										value[8].get<float>(), value[9].get<float>(), value[10].get<float>(), value[11].get<float>(),
										value[12].get<float>(), value[13].get<float>(), value[14].get<float>(), value[15].get<float>(),
						};
						prop.set_value(rawObj, m);
					}
					else if (value_type == type::get<glm::quat>())
					{
						glm::quat v = { value[0].get<float>(), value[1].get<float>(), value[2].get<float>(), value[3].get<float>() };
						prop.set_value(rawObj, v);
					}

					break;
				}

				prop.set_value(rawObj, var);
				break;
			}
			case json::value_t::object:
			{
				variant var = prop.get_value(rawObj);
				JsonToInstance(var, value);
				prop.set_value(rawObj, var);
				break;
			}
			default:
			{
				variant extractedValue = GetBasicTypes(value);

				// Only works with const type, not sure if its right
				if (extractedValue.convert(value_type))
				{
					prop.set_value(rawObj, extractedValue);
				}
			}
			}
		}
	}


	variant Serializer::GetKeyValue(json::iterator& iter, const type& t)
	{
		auto& jsonValue = iter.value();
		variant value = GetBasicTypes(jsonValue);
		
		const bool could_convert = value.convert(t);
		if (!could_convert)
		{
			if (jsonValue.is_object())
			{
				constructor ctor = t.get_constructor();
				for (auto& item : t.get_constructors())
				{
					if (item.get_instantiated_type() == t)
					{
						ctor = item;
					}
				}
				value = ctor.invoke();
				JsonToInstance(value, jsonValue);
			}
		}

		return value;
	}


	variant Serializer::GetBasicTypes(json& jsonValue)
	{
		// Handle only POD types
		switch (jsonValue.type())
		{
		case json::value_t::string:
		{
			return std::string(jsonValue.get<std::string>());
			break;
		}
		case json::value_t::null:
		{
			break;
		}
		case json::value_t::boolean:
		{
			return jsonValue.get<bool>();
			break;
		}
		case json::value_t::number_integer:
		{
			if (jsonValue.is_number_integer())
			{
				return jsonValue.get<int>();
			}
			break;
		}
		case json::value_t::number_unsigned:
		{
			if (jsonValue.is_number_unsigned())
			{
				return jsonValue.get<unsigned int>();
			}
			break;
		}
		case json::value_t::number_float:
		{
			if (jsonValue.is_number_float())
			{
				return jsonValue.get<double>();
			}
			break;
		}
		case json::value_t::array:
		{
			// If vector has elements
			if (jsonValue.size() != 0)
			{
				// Find element type and get its value.
				switch (jsonValue.front().type())
				{
				case json::value_t::string:
				{
					return jsonValue.get<std::vector<std::string>>();
					break;
				}
				case json::value_t::null:
				{
					break;
				}
				case json::value_t::boolean:
				{
					return jsonValue.get<std::vector<bool>>();
					break;
				}
				case json::value_t::number_integer:
				{
					if (jsonValue.front().is_number_integer())
					{
						return jsonValue.get<std::vector<int>>();
					}
					break;
				}
				case json::value_t::number_unsigned:
				{
					if (jsonValue.front().is_number_unsigned())
					{
						return jsonValue.get<std::vector<unsigned int>>();
					}
					break;
				}
				case json::value_t::number_float:
				{
					if (jsonValue.front().is_number_float())
					{
						return jsonValue.get<std::vector<double>>();
					}
					break;
				}
				}
			}
			// Cant find specific map's value type (Array most probably)
			return variant();
		}
		case json::value_t::object:
		{
			// not handling map value objects currently
			return variant();
		}
		}

		return variant();
	}


	void Serializer::SetArrayRecursive(variant_sequential_view& view, json& variable)
	{
		// Allow size change, get container type
		view.set_size(variable.size());
		const type arrayType = view.get_rank_type(1);

		for (size_t i = 0; i < variable.size(); ++i)
		{
			auto& Indexvalue = variable[i];

			// Special cases that contains glm containers
			if (arrayType == type::get<glm::vec3>() || arrayType == type::get<glm::vec4>()
				|| arrayType == type::get<glm::mat3>() || arrayType == type::get<glm::mat4>())
			{
				if (arrayType == type::get<glm::vec3>())
				{
					glm::vec3 v = { Indexvalue[0].get<float>(), Indexvalue[1].get<float>(), Indexvalue[2].get<float>() };
					view.set_value(i, v);
				}
				else if (arrayType == type::get<glm::vec4>())
				{
					glm::vec4 v = { Indexvalue[0].get<float>(), Indexvalue[1].get<float>(), Indexvalue[2].get<float>(), Indexvalue[3].get<float>() };
					view.set_value(i, v);
				}
				else if (arrayType == type::get<glm::mat3>())
				{
					glm::mat3 m = { Indexvalue[0].get<float>(), Indexvalue[1].get<float>(), Indexvalue[2].get<float>(),
									Indexvalue[3].get<float>(),  Indexvalue[4].get<float>(), Indexvalue[5].get<float>(),
									Indexvalue[6].get<float>(), Indexvalue[7].get<float>(), Indexvalue[8].get<float>()
					};

					view.set_value(i, m);
				}
				else if (arrayType == type::get<glm::mat4>())
				{
					glm::mat4 m = { Indexvalue[0].get<float>(), Indexvalue[1].get<float>(), Indexvalue[2].get<float>(), Indexvalue[3].get<float>(),
									Indexvalue[4].get<float>(), Indexvalue[5].get<float>(), Indexvalue[6].get<float>(), Indexvalue[7].get<float>(),
									Indexvalue[8].get<float>(), Indexvalue[9].get<float>(), Indexvalue[10].get<float>(), Indexvalue[11].get<float>(),
									Indexvalue[12].get<float>(), Indexvalue[13].get<float>(), Indexvalue[14].get<float>(), Indexvalue[15].get<float>(),
					};

					view.set_value(i, m);
				}
			}
			// Array inside array
			else if (Indexvalue.is_array())
			{
				auto sub_array_view = view.get_value(i).create_sequential_view();
				SetArrayRecursive(sub_array_view, Indexvalue);
			}
			// Array of objects
			else if (Indexvalue.is_object())
			{
				variant tempVar = view.get_value(i);
				variant wrappedVar = tempVar.extract_wrapped_value();
				JsonToInstance(wrappedVar, Indexvalue);

				view.set_value(i, wrappedVar);
			}
			// Fundamental types
			else
			{
				// Get value and set it
				variant extractedValue = GetBasicTypes(Indexvalue);
				if (extractedValue.convert(arrayType))
				{
					view.set_value(i, extractedValue);
				}
			}
		}
	}


	void Serializer::SetAssociativeContainerRecursive(variant_associative_view& view, json& variable)
	{
		for (size_t i = 0; i < variable.size(); ++i)
		{
			auto& jsonIndexValue = variable[i];

			// Check if it is a key-value associative view
			if (jsonIndexValue.is_object())
			{
				auto keyIter = jsonIndexValue.find("key");
				auto valueIter = jsonIndexValue.find("value");

				if (keyIter != jsonIndexValue.end() && valueIter != jsonIndexValue.end())
				{
					auto keyVar = GetKeyValue(keyIter, view.get_key_type());
					auto valueVar = GetKeyValue(valueIter, view.get_value_type());

					if (keyVar && valueVar)
					{
						view.insert(keyVar, valueVar);
					}
				}
			}
			// Key-only associative view
			else
			{
				variant extractedValue = GetBasicTypes(jsonIndexValue);
				if (extractedValue && extractedValue.convert(view.get_key_type()))
				{
					view.insert(extractedValue);
				}
			}
		}
	}

} // end of namespace