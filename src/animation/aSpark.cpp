// Spark.cpp: implementation of the ASpark class.
//
//////////////////////////////////////////////////////////////////////

#include "aSpark.h"
#include <math.h>

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ASpark::ASpark()
{
	//coefficients of restitution equals 0.25
	m_COR = 0.25f;
	m_mass = 1.0;
}

ASpark::ASpark(vec3 color): AParticle()
{
	for (int i = 0; i < 3; i++)
		m_color[i] = color[i];
 
	//coefficients of restitution equals 0.25
	m_COR = 0.25f;
}

ASpark::~ASpark()
{

}

//Set attractor position
void ASpark::setAttractor(vec3 position)
{
	m_attractorPos = position;
}

//Set repeller position
void ASpark::setRepeller(vec3 position)
{
	m_repellerPos = position;
}

void ASpark::setWind(vec3 wind)
{
	m_windForce = wind;
}


void ASpark::update(float deltaT, int extForceMode)
{
	m_deltaT = deltaT;
	if (m_state[10] <= 0.0)
	{
		m_alive = false;
		return;
	}

	if (!(extForceMode & EXT_SPARKFORCES_ACTIVE))
		extForceMode = 0;
	
	computeForces(extForceMode);
	
	updateState(deltaT, EULER);

	resolveCollisions();
	
	
}

void ASpark::computeForces(int extForceMode)
{

	// zero out all forces
	m_state[6] = 0.0;
	m_state[7] = 0.0;
	m_state[8] = 0.0;

	// gravity force
	addForce(m_mass*m_gravity);
	
	// wind force
	if (extForceMode & WIND_ACTIVE)
	{
		//TODO: Add your code here
		addForce(m_windForce);
	}

	if (extForceMode & DRAG_ACTIVE)
	{
		//TODO: Add your code here
		vec3 dragForce = 0.5 * 1.225 * m_Vel.SqrLength() * 0.47 * M_PI * 0.001 * 0.001 * m_Vel.Normalize();
		addForce(-dragForce);
	}

	// attractor force
	if (extForceMode & ATTRACTOR_ACTIVE)
	{
		//TODO: Add your code here
		vec3 attract = m_attractorPos - m_Pos;
		vec3 attractForce = 6.673 * pow(10, -11) * 0.001 * 1 / attract.SqrLength() * attract.Normalize();
		addForce(attractForce);
	}

	// repeller force
	if (extForceMode & REPELLER_ACTIVE)
	{
		//TODO: Add your code here
		vec3 repel = m_Pos - m_repellerPos;
		vec3 repelForce = 6.673 * pow(10, -11) * 0.001 * 1 / repel.SqrLength() * repel.Normalize();
		addForce(repelForce);
	}

	// random force
	if (extForceMode & RANDOM_ACTIVE)
	{
		//TODO: Add your code here
		float randomX = ((float)rand()) / (RAND_MAX / 200.f) + -100.f;
		float randomY = ((float)rand()) / (RAND_MAX / 200.f) + -100.f;
		float randomZ = ((float)rand()) / (RAND_MAX / 200.f) + -100.f;
		vec3 randomForce = vec3(randomX, randomY, randomZ);
		addForce(randomForce);
	}

}

void ASpark::resolveCollisions()
{
	//TODO: Add your code here that reverses the y value of the spark velocity vector 
	// when the y position value of the spark is < 0
	if (m_state[1] <= 0 && m_state[4] < 0)
	{
		m_state[4] = -(m_state[4] * m_COR);
	}
}
