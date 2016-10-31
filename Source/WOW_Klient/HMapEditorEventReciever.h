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
		if (event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			switch (event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				IGUIElement * elm = SceneContext.device->getGUIEnvironment()->getRootGUIElement()->getElementFromPoint(SceneContext.device->getCursorControl()->getPosition());
				if (elm->getID() == ID_GUI_IMAGE_PREVIEW)
				{
					printf("\n Mys  na obrazku :-) \n");
				}
				break;
			}
		}
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

					std::basic_string<wchar_t> cesta;

					cesta.append(L"..\\..\\..\\Data\\model\\");
					cesta.append(SceneContext.listBoxModels->getListItem(SceneContext.listBoxModels->getSelected()));

					IAnimatedMesh* mesh = SceneContext.device->getSceneManager()->getMesh(cesta.c_str());
					
					*SceneContext.metaNode = SceneContext.device->getSceneManager()->addMeshSceneNode(mesh, 0, 0, vector3df(0, 0, -100));
					(*SceneContext.metaNode)->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
					(*SceneContext.metaNode)->setMaterialType(E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

					(*SceneContext.metaNode)->setTriangleSelector(SceneContext.device->getSceneManager()->createTriangleSelector((*SceneContext.metaNode)->getMesh(), (*SceneContext.metaNode)));

					float max = (*SceneContext.metaNode)->getBoundingBox().getExtent().X;
					if ((*SceneContext.metaNode)->getBoundingBox().getExtent().Y >  max) {
						max = (*SceneContext.metaNode)->getBoundingBox().getExtent().Y;
					}
					if ((*SceneContext.metaNode)->getBoundingBox().getExtent().Z >  max) {
						max = (*SceneContext.metaNode)->getBoundingBox().getExtent().Z;
					}

					core::vector3df c = (*SceneContext.metaNode)->getBoundingBox().getCenter();
					float scale = (100.0f / (SceneContext.device->getVideoDriver()->getScreenSize().Width / SceneContext.device->getVideoDriver()->getScreenSize().Height)) / max;

					c.X = -(c.X * scale);
					c.Y = -(c.Y * scale);
					c.Z = -(c.Z * scale);
					(*SceneContext.metaNode)->setPosition(c);

					(*SceneContext.metaNode)->setScale(core::vector3df(scale, scale, scale));
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