#include "ApplicationClass.h"

void ApplicationClass::GenerateBoundingSphere(matrix4 a_mModelToWorld, String a_sInstanceName)
{
	m_pMeshMngr->SetModelMatrix(a_mModelToWorld, a_sInstanceName);//doesnt need changes

	if(m_pMeshMngr->IsInstanceCreated(a_sInstanceName))//Ask if the model has been initialize first (just for safety purposes)
	{
		static bool bInitialized = false;//for setting a initialized flag
		static vector3 vCenterPoint;//for holding the centroid point (in model space AKA local space)
		static float fRadius;//for holding the radius
		matrix4 mCentroidSpace = matrix4(IDENTITY); //for holding the matrix that translates from the centroidspace to global space
		if(!bInitialized)
		{
			std::vector<vector3> lVertices = m_pMeshMngr->m_pModelMngr->GetVertices(a_sInstanceName); //Does not need changes, this will give you the list of vertex of the model

			//Find the radius center point (I need you to find the right center point, partial credit will be given if you use the average method)

			unsigned int nVertices = lVertices.size();
			vCenterPoint = lVertices[0];
			vector3 v3Max(lVertices[0]);
			vector3 v3Min(lVertices[0]);
			for(unsigned int nVertex = 1; nVertex < nVertices; nVertex++)
			{
				if(v3Min.x > lVertices[nVertex].x)
					v3Min.x = lVertices[nVertex].x;
				else if(v3Max.x < lVertices[nVertex].x)
					v3Max.x = lVertices[nVertex].x;
			
				if(v3Min.y > lVertices[nVertex].y)
					v3Min.y = lVertices[nVertex].y;
				else if(v3Max.y < lVertices[nVertex].y)
					v3Max.y = lVertices[nVertex].y;

				if(v3Min.z > lVertices[nVertex].z)
					v3Min.z = lVertices[nVertex].z;
				else if(v3Max.z < lVertices[nVertex].z)
					v3Max.z = lVertices[nVertex].z;
			}
			vCenterPoint = (v3Min + v3Max) / 2.0f;
			
			fRadius = glm::distance(vCenterPoint, lVertices[0]);
			for(unsigned int nVertex = 1; nVertex < nVertices; nVertex++)
			{
				float fDistance = glm::distance(vCenterPoint, lVertices[nVertex]);
				if(fRadius < fDistance)
					fRadius = fDistance;
			}

			bInitialized = true;//does not need changes
		}
		
		mCentroidSpace = a_mModelToWorld * glm::translate(vCenterPoint); //Calculate the real CentroidSpace

		m_pMeshMngr->AddAxisToQueue(a_mModelToWorld * glm::translate(vCenterPoint));//Does not need changes, renders an axis in the CenterPointSpace
		m_pMeshMngr->AddSphereToQueue( mCentroidSpace * glm::scale(vector3(fRadius * 2.0f)), MERED, MERENDER::WIRE);//Does not need changes, renders the sphere
	}
}
void ApplicationClass::CalculateSpherePosition(void)
{
	static float fTotalTime = 0.0f;
	float fLapDifference = m_pSystem->StopClock();
	fTotalTime += fLapDifference;

	matrix4 m4Orbit = glm::rotate(matrix4(IDENTITY), fTotalTime * 15.0f, vector3(0.0f, 0.0f, -1.0f));
	matrix4 m4Translation = glm::translate(vector3(0.0f, 3.0, 0.0f));
	matrix4 m4Rotation = glm::rotate(matrix4(IDENTITY), fTotalTime * 15.0f, vector3(0.0f, 0.0f, 1.0f));
	m_m4Creeper = m4Orbit * m4Translation * m4Rotation;
}

