/******************************************************************************/
/*!
\file       Particle.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
  This file contains the implementation of Particle class.

  Fields can be adjusted freely, particles will be randomized
  from min to max variables.
  If you want it to be constant, set the min and max to be the same values.

  maxParticles is how many particles are "alive" at the same time, per frame.

  isCone, isSphere is the flags for emission shapes.
  Emission shape = spawn particles within that shape. NOT MOVE in that shape. (Unity)

  Init should be called BEFORE the setter function for isLooping.

  Pooling is for reusing the particles.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/

#include "include/ECS/Component/Particle.hpp"
#include "glm/ext/quaternion_common.hpp"
#include <random>
#include <iostream>

#define PI 3.14159265358979323846

std::random_device rd;  // Used to obtain a seed for the random number engine
std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0, 1.0);

template <typename T>
T lerp(T a, T b, float t) 
{
	return (a + t * (b - a));
}

namespace Engine
{
	Particle::Particle(bool isLooping, bool isRotate, int maxParticles, float gravityModifier, float radius, glm::vec3 rotationSpeed,
		glm::vec4 startColor, glm::vec4 endColor, glm::vec3 minSpeed, glm::vec3 maxSpeed, glm::vec3 minSize, glm::vec3 maxSize, 
		float minLifespan, float maxLifespan, bool isSphere, bool isCone) : minUV{ 0.f, 0.f }, maxUV{ 1.f, 1.f }
	{
		this->isLooping = isLooping; 
		this->isRotate = isRotate;
		this->maxParticles = maxParticles > particles.size() ? particles.size() : maxParticles;
		this->gravityModifier = gravityModifier; 
		this->radius = radius;
		this->rotationSpeed = rotationSpeed / 500.f;

		this->startColor = startColor; this->endColor = endColor;
		this->minSpeed = minSpeed; this->maxSpeed = maxSpeed;
		this->minSize = minSize; this->maxSize = maxSize;

		this->minLifespan = minLifespan; this->maxLifespan = maxLifespan;
		this->isSphere = isSphere; this->isCone = isCone;

		resetParticle();
	}

	
	void Particle::addParticle(glm::vec3 positionEntity, glm::vec3 rotationEntity)
	{
		if (!availableParticles.empty())
		{
			// Remove from pool
			int index = availableParticles.front();
			availableParticles.pop();

			ParticleProps& particle = particles[index];

			// Misc variables
			{
				particle.isActive = true;

				particle.lifespan = minLifespan + rand() / (static_cast<float>(RAND_MAX / (maxLifespan - minLifespan)));
				particle.lifeRemaining = particle.lifespan;

				particle.color = startColor;
				particle.angle = {};
			}

			// Randomized position
			{
				float randomAngle = dis(gen) * 2 * PI;
				float randomRadius = dis(gen) * radius;

				if (isSphere || isCone)
				{
					if (isSphere)
					{
						float zMinPosition = positionEntity.z - radius;
						float zMaxPosition = positionEntity.z + radius;
						particle.position.z = zMinPosition + rand() / (static_cast<float>(RAND_MAX / (zMaxPosition - zMinPosition)));
					}
					else
					{
						particle.position.z = positionEntity.z;
					}

					particle.position.x = positionEntity.x + randomRadius * cos(randomAngle);
					particle.position.y = positionEntity.y + randomRadius * sin(randomAngle);
				}
				else
				{
					particle.position.x = positionEntity.x;
					particle.position.y = positionEntity.y;
					particle.position.y = positionEntity.z;
				}
			}

			// Randomized velocity
			{
				particle.velocity.x = minSpeed.x + rand() / (static_cast<float>(RAND_MAX / (maxSpeed.x - minSpeed.x)));
				particle.velocity.z = minSpeed.z + rand() / (static_cast<float>(RAND_MAX / (maxSpeed.z - minSpeed.z)));

				if (gravityModifier > 0.f)
				{
					particle.velocity.y = -gravityModifier * (minSpeed.y + rand() / (static_cast<float>(RAND_MAX / (maxSpeed.y - minSpeed.y))));
				}
				else
				{
					particle.velocity.y = minSpeed.y + rand() / (static_cast<float>(RAND_MAX / (maxSpeed.y - minSpeed.y)));
				}
			}

			// Randomized size
			{
				particle.size.x = minSize.x + rand() / (static_cast<float>(RAND_MAX / (maxSize.x - minSize.x)));
				particle.size.y = minSize.y + rand() / (static_cast<float>(RAND_MAX / (maxSize.y - minSize.y)));
				particle.size.z = minSize.z + rand() / (static_cast<float>(RAND_MAX / (maxSize.z - minSize.z)));
			}
		}
	}


	void Particle::resetParticle()
	{
		availableParticles = {};

		for (int i = 0; i < particles.size(); ++i)
		{
			particles[i].isActive = false;
			availableParticles.push(i);
		}
	}


	void Particle::Init(glm::vec3 positionEntity, glm::vec3 rotationEntity)
	{
		resetParticle();

		for (int i = 0; i < maxParticles; ++i)
		{
			addParticle(positionEntity, rotationEntity);
		}
	}


	void Particle::Update(float deltaTime, glm::vec3 positionEntity, glm::vec3 rotationEntity)
	{
		for (int i = 0; i < particles.size(); ++i)
		{
			ParticleProps& particle = particles[i];
			if (particle.isActive == false)
			{
				continue;
			}

			if (particle.lifeRemaining > 0.f)
			{
				particle.position += particle.velocity * deltaTime;
				particle.lifeRemaining -= deltaTime;

				float life = particle.lifeRemaining / particle.lifespan;
				particle.color = lerp(endColor, startColor, life);

				if (isRotate)
				{
					particle.angle += rotationSpeed;
				}
			}
			else if (particle.lifeRemaining <= 0.f)
			{
				// Instead of putting it back to pool, just randomize the particle values again
				if (isLooping)
				{
					// Randomized lifespan, color, angle
					{
						particle.lifespan = minLifespan + rand() / (static_cast<float>(RAND_MAX / (maxLifespan - minLifespan)));
						particle.lifeRemaining = particle.lifespan;

						particle.color = startColor;
						particle.angle = {};
					}

					// Randomized position
					{
						if (!isSphere && !isCone)
						{
							particle.position = {};
						}
						else
						{
							float randomAngle = dis(gen) * 2 * PI;
							float randomRadius = dis(gen) * radius;
							particle.position.x = randomRadius * cos(randomAngle);
							particle.position.z = randomRadius * sin(randomAngle);

							if (isSphere)
							{
								float yMinPosition = positionEntity.y - radius;
								float yMaxPosition = positionEntity.y + radius;
								particle.position.y = yMinPosition + rand() / (static_cast<float>(RAND_MAX / (yMaxPosition - yMinPosition)));
							}
							else if (isCone)
							{
								particle.position.y = 0.f;
							}
						}
					}

					// Randomized velocity
					{
						particle.velocity.x = minSpeed.x + rand() / (static_cast<float>(RAND_MAX / (maxSpeed.x - minSpeed.x)));
						particle.velocity.z = minSpeed.z + rand() / (static_cast<float>(RAND_MAX / (maxSpeed.z - minSpeed.z)));

						if (gravityModifier > 0.f)
						{
							particle.velocity.y = -gravityModifier * (minSpeed.y + rand() / (static_cast<float>(RAND_MAX / (maxSpeed.y - minSpeed.y))));
						}
						else
						{
							particle.velocity.y = minSpeed.y + rand() / (static_cast<float>(RAND_MAX / (maxSpeed.y - minSpeed.y)));
						}
					}

					// Randomized size
					{
						particle.size.x = minSize.x + rand() / (static_cast<float>(RAND_MAX / (maxSize.x - minSize.x)));
						particle.size.y = minSize.y + rand() / (static_cast<float>(RAND_MAX / (maxSize.y - minSize.y)));
						particle.size.z = minSize.z + rand() / (static_cast<float>(RAND_MAX / (maxSize.z - minSize.z)));
					}
				}
				else
				{
					// Put it back into the pool as it is not in the loop anymore
					particle.isActive = false;
					availableParticles.push(i);
				}
			}
		}
	}
}