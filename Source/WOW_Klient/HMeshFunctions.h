#ifndef HMeshFunctions
#define HMeshFunctions

#include "HWOWKlient.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace WowKlient
{
	namespace MeshFunctions
	{
		// vytvori skinedmesh slozeny z vybranych dilu zdrojoveho meshe
		// source <- animated mesh bez zavolani metody finalize()
		// parts <- ukazatel na pole èástí
		// partsCount <- poèet èástí (velikost pole)
		ISkinnedMesh * makeMeshFromParts(WowKlient::Core::GameState * gState, IAnimatedMesh * source, int partsCount, int * parts);
	}
}

#endif