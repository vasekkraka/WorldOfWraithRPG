#include "HWOWKlient.h"

namespace WowKlient
{
	namespace MeshFunctions
	{
		int validPart(int id, int partsCount, int * parts)
		{
			int ret = -1;
			for (int i = 0; i < partsCount; i++)
			{
				if (parts[i] == id)
				{
					ret = i;
				}
			}
			return ret;
		}

		void parseJointParents(ISkinnedMesh * mesh, ISkinnedMesh::SJoint * Sjoint, ISkinnedMesh::SJoint * Pjoint)
		{
			ISkinnedMesh::SJoint * Njoint = mesh->addJoint(Pjoint);
			*Njoint = *Sjoint;
			Njoint->Weights.clear();
			Njoint->Children.clear();
			for (u32 i = 0; i < Sjoint->Children.size(); i++)
			{
				parseJointParents(mesh, Sjoint->Children[i], Njoint);
			}
		}

		// vytvori skinedmesh slozeny z vybranych dilu zdrojoveho meshe
		// source <- animated mesh bez zavolani metody finalize()
		// parts <- ukazatel na pole èástí
		// partsCount <- poèet èástí (velikost pole)
		ISkinnedMesh * makeMeshFromParts(WowKlient::Core::GameState * gState, IAnimatedMesh * source, int partsCount, int * parts)
		{
			ISkinnedMesh * skinned = (ISkinnedMesh *)source;

			ISkinnedMesh * clone = gState->irrDevice->getSceneManager()->createSkinnedMesh();

			for (int i = 0; i < partsCount; i++)
			{
				SSkinMeshBuffer * buf = clone->addMeshBuffer();

				buf->recalculateBoundingBox();
				buf->setDirty();

				for (u32 j = 0; j < ((SSkinMeshBuffer*)source->getMeshBuffer(parts[i]))->getVertexCount(); j++)
				{
					S3DVertex * v = new S3DVertex(*((SSkinMeshBuffer*)source->getMeshBuffer(parts[i]))->getVertex(j));
					buf->Vertices_Standard.push_back(*v);
				}

				for (u32 j = 0; j < ((SSkinMeshBuffer*)source->getMeshBuffer(parts[i]))->getIndexCount(); j++)
				{
					buf->Indices.push_back(u16(((SSkinMeshBuffer*)source->getMeshBuffer(parts[i]))->getIndices()[j]));
				}

				buf->Material = ((SSkinMeshBuffer*)source->getMeshBuffer(parts[i]))->getMaterial();


			}

			parseJointParents(clone, skinned->getAllJoints()[0], 0);

			for (u32 i = 0; i < skinned->getJointCount(); i++)
			{
				ISkinnedMesh::SJoint * joint;
				joint = clone->getAllJoints()[clone->getJointNumber(skinned->getAllJoints()[i]->Name.c_str())];

				for (u32 j = 0; j < skinned->getAllJoints()[i]->Weights.size(); j++)
				{
					s32 buffer = validPart(skinned->getAllJoints()[i]->Weights[j].buffer_id, partsCount, parts);
					if (buffer >= 0)
					{
						ISkinnedMesh::SWeight * w = clone->addWeight(joint);
						w->buffer_id = buffer;
						w->strength = skinned->getAllJoints()[i]->Weights[j].strength;
						w->vertex_id = skinned->getAllJoints()[i]->Weights[j].vertex_id;
					}
				}

			}

			clone->finalize();

			return clone;
		}
	}
}