#ifndef HMapEditorEventReciever
#define HMapEditorEventReciever

#include "HMapLoader.h"
#include "HWOWKlient.h"

struct MapSceneContext
{
	IrrlichtDevice *device;
	s32             counter;
	IGUIListBox * listBoxModels;
	IGUIButton * buttonAdd;
	IAnimatedMeshSceneNode * previewNode;
	IMeshSceneNode ** metaNode;
};

class MapEditorEventReceiver : public IEventReceiver
{
public:
	MapEditorEventReceiver(MapSceneContext & context) : SceneContext(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = SceneContext.device->getGUIEnvironment();

			switch (event.GUIEvent.EventType)
			{
			case EGET_LISTBOX_CHANGED:
			case EGET_LISTBOX_SELECTED_AGAIN:
				if (id == ID_GUI_LISTBOX_MODELS)
				{
					wprintf(L"Vybran model: %s\n", SceneContext.listBoxModels->getListItem(SceneContext.listBoxModels->getSelected()));

					if (*SceneContext.metaNode != NULL)
					{
						(*SceneContext.metaNode)->remove();
					}

					std::basic_string<wchar_t> a;

					a.append(L"..\\..\\..\\Data");
					a.append(L"\\model\\");
					a.append(SceneContext.listBoxModels->getListItem(SceneContext.listBoxModels->getSelected()));

					IAnimatedMesh* mesh = SceneContext.device->getSceneManager()->getMesh(a.c_str());
					if (!mesh)
					{
						//SceneContext.device->drop();
						return 1;
					}
					
					*SceneContext.metaNode = SceneContext.device->getSceneManager()->addMeshSceneNode(mesh, 0, 0, vector3df(0, 0, -100));
					(*SceneContext.metaNode)->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
					(*SceneContext.metaNode)->setMaterialType(E_MATERIAL_TYPE::EMT_SOLID);

					(*SceneContext.metaNode)->setTriangleSelector(SceneContext.device->getSceneManager()->createTriangleSelector((*SceneContext.metaNode)->getMesh(), (*SceneContext.metaNode)));
				}
				break;
			}
		}
		return false;
	}

private:
	MapSceneContext & SceneContext;
};

#endif