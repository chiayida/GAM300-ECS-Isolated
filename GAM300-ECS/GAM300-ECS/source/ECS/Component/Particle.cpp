#include "include/ECS/Component/Particle.hpp"
#include <random>

#define PI 3.14159265358979323846


namespace Engine
{
	Particle::Particle(bool isLooping, int particlesPerEmission,
		float minSpeed, float maxSpeed, float minSize, float maxSize, float minLifespan, float maxLifespan,
		bool isCone, float coneRadius, float coneRadiusRange, float coneAngle, bool isSphere, float sphereRadius, bool isBox)
		: minUV{ 0.f, 0.f }, maxUV{ 1.f, 1.f }
	{
		this->isLooping = isLooping;
		this->particlesPerEmission = particlesPerEmission;

		this->minSpeed = minSpeed;
		this->maxSpeed = maxSpeed;
		this->minSize = minSize;
		this->maxSize = maxSize;
		this->minLifespan = minLifespan;
		this->maxLifespan = maxLifespan;

		this->isCone = isCone;
		this->coneRadius = coneRadius;
		this->coneRadiusRange = coneRadiusRange;
		this->coneAngle = coneAngle;

		this->isSphere = isSphere;
		this->sphereRadius = sphereRadius;

		this->isBox = isBox;

		// Initialise an array based on maxParticles
		for (int i = 0; i < particles.size(); ++i)
		{
			particles[i].lifespan = -1.f;
			availableParticles.push(i);
		}
	}

	
	void Particle::addParticle(glm::vec3 positionEntity)
	{
		if (!availableParticles.empty())
		{
			int index = availableParticles.front();
			availableParticles.pop();
			particles[index].position = positionEntity;

			// Velocity/
			if (isCone) 
			{
				// randomize angle between coneAngle and 2*coneAngle
				float angle = glm::radians(coneAngle + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 * coneAngle))));
				float radius = coneRadius + static_cast<float>(rand()) / (static_cast <float>(RAND_MAX / coneRadiusRange));

				particles[index].velocity.x = radius * cosf(angle);
				particles[index].velocity.y = radius * sinf(angle);
				particles[index].velocity.z = radius * tanf(angle);
			}
			else if (isSphere)
			{
				// Set velocity based on constant radius and randomized angle
				float angle = 2.f * PI * rand() / (RAND_MAX);
				float azimuth = PI * rand() / (RAND_MAX);
				// set velocity based on a constant sphere radius and angle
				particles[index].velocity.x = sphereRadius * sinf(azimuth) * cosf(angle);
				particles[index].velocity.y = sphereRadius * sinf(azimuth) * sinf(angle);
				particles[index].velocity.z = sphereRadius * cosf(azimuth);
			}
			else {
				// Randomized velocity
				particles[index].velocity.x = minSpeed + rand() / (static_cast<float>(RAND_MAX / (maxSpeed - minSpeed)));
				particles[index].velocity.y = minSpeed + rand() / (static_cast<float>(RAND_MAX / (maxSpeed - minSpeed)));
				particles[index].velocity.z = minSpeed + rand() / (static_cast<float>(RAND_MAX / (maxSpeed - minSpeed)));
			}

			// Randomized size
			particles[index].size.x = minSize + rand() / (static_cast<float>(RAND_MAX / (maxSize - minSize)));
			particles[index].size.y = minSize + rand() / (static_cast<float>(RAND_MAX / (maxSize - minSize)));
			particles[index].size.z = minSize + rand() / (static_cast<float>(RAND_MAX / (maxSize - minSize)));
			
			// Randomized lifespan
			particles[index].lifespan = minLifespan + rand() / (static_cast<float>(RAND_MAX / (maxLifespan - minLifespan)));
		}
	}


	void Particle::Update(float deltaTime, glm::vec3 positionEntity)
	{
		int particlesToEmit = particlesPerEmission;
		while (particlesToEmit > 0 && !availableParticles.empty())
		{
			addParticle(positionEntity);
			--particlesToEmit;
		}

		for (int i = 0; i < particles.size(); ++i)
		{
			if (particles[i].lifespan > 0)
			{
				particles[i].position += particles[i].velocity * deltaTime;
				particles[i].lifespan -= deltaTime;
			}
			else if (particles[i].lifespan <= 0)
			{
				if (isLooping) 
				{
					// Velocity
					if (isCone)
					{
						// randomize angle between coneAngle and 2*coneAngle
						float angle = glm::radians(coneAngle + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 * coneAngle))));
						float radius = coneRadius + static_cast<float>(rand()) / (static_cast <float>(RAND_MAX / coneRadiusRange));

						particles[i].velocity.x = radius * cosf(angle);
						particles[i].velocity.y = radius * sinf(angle);
						particles[i].velocity.z = radius * tanf(angle);
					}
					else if (isSphere)
					{
						// Set velocity based on constant radius and randomized angle
						float angle = 2.f * PI * rand() / (RAND_MAX);
						float azimuth = PI * rand() / (RAND_MAX);
						// set velocity based on a constant sphere radius and angle
						particles[i].velocity.x = sphereRadius * sinf(azimuth) * cosf(angle);
						particles[i].velocity.y = sphereRadius * sinf(azimuth) * sinf(angle);
						particles[i].velocity.z = sphereRadius * cosf(azimuth);
					}
					else {
						// Randomized velocity
						particles[i].velocity.x = minSpeed + rand() / (static_cast<float>(RAND_MAX / (maxSpeed - minSpeed)));
						particles[i].velocity.y = minSpeed + rand() / (static_cast<float>(RAND_MAX / (maxSpeed - minSpeed)));
						particles[i].velocity.z = minSpeed + rand() / (static_cast<float>(RAND_MAX / (maxSpeed - minSpeed)));
					}

					// Randomized size
					particles[i].size.x = minSize + rand() / (static_cast<float>(RAND_MAX / (maxSize - minSize)));
					particles[i].size.y = minSize + rand() / (static_cast<float>(RAND_MAX / (maxSize - minSize)));
					particles[i].size.z = minSize + rand() / (static_cast<float>(RAND_MAX / (maxSize - minSize)));

					// Randomized lifespan
					particles[i].lifespan = minLifespan + rand() / (static_cast<float>(RAND_MAX / (maxLifespan - minLifespan)));

					particles[i].position = positionEntity;
				}
				else
				{
					// Life span is <= 0, therefore it will not be in the loop anymore
					availableParticles.push(i);
				}
			}
		}
	}
}