#ifndef HMapEditorEventReciever
#define HMapEditorEventReciever

#include "HMapLoader.h"
#include "HWOWKlient.h"
#include "HMapEditor.h"

struct MapSceneContext
{
	IrrlichtDevice *device;
	s32             counter;
	WowKlient::Core::GameState * gState;
	IGUIWindow * toolBox;
	IGUIListBox * listBoxModels;
	IGUIButton * buttonAdd;
	IAnimatedMeshSceneNode * previewNode;
	IMeshSceneNode ** metaNode;
	IGUIImage * previewImage;
};

class MapEditorEventReceiver : public IEventReceiver
{
public:
	MapEditorEventReceiver(MapSceneContext & context) : SceneContext(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			IGUIElement * elm = SceneContext.device->getGUIEnvironment()->getRootGUIElement()->getElementFromPoint(SceneContext.device->getCursorControl()->getPosition());
			mousePos = SceneContext.device->getCursorControl()->getPosition();
			switch (event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				if (elm->getID() == ID_GUI_IMAGE_PREVIEW)
				{
					printf("\n Mys  na obrazku :-) \n");
					rotating = true;
				}
				break;
			case EMIE_LMOUSE_LEFT_UP:
				if (rotating)
				{
					rotating = false;
				}
				break;
			case EMIE_RMOUSE_PRESSED_DOWN:
				if (elm->getID() == ID_GUI_IMAGE_PREVIEW)
				{
					if ((*SceneContext.metaNode) != NULL)
					(*SceneContext.metaNode)->setVisible(!(*SceneContext.metaNode)->isVisible());
				}
				break;
			}

			if (isRotatingPreview() && (*SceneContext.metaNode) != NULL)
			{
				vector3df rot = (*SceneContext.metaNode)->getRotation();

				rot.Y += (SceneContext.device->getCursorControl()->getPosition().X - getLastMousePos().X);

				(*SceneContext.metaNode)->setRotation(rot);
			}
		}
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = SceneContext.device->getGUIEnvironment();
			
			switch (event.GUIEvent.EventType)
			{
			case EGET_ELEMENT_CLOSED:
				switch (id)
				{
				case ID_TOOL_WINDOW:
					SceneContext.toolBox->setVisible(false);
					return true;
					break;
				}
				break;
			case EGET_BUTTON_CLICKED:
				switch (id)
				{
				case ID_GUI_TOOLBAR_BUTTON_NEW:
					printf("\nNew :-) \n");
					break;
				case ID_GUI_TOOLBAR_BUTTON_WMO:
					SceneContext.toolBox->setVisible(true);
					break;
				}
				break;
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
					(*SceneContext.metaNode)->setVisible(false);

					(*SceneContext.metaNode)->setPosition(vector3df(0,0,0));

					(*SceneContext.metaNode)->setTriangleSelector(SceneContext.device->getSceneManager()->createTriangleSelector((*SceneContext.metaNode)->getMesh(), (*SceneContext.metaNode)));

					vector3df size = (*SceneContext.metaNode)->getBoundingBox().getExtent();

					float max = size.X;
					if (size.Z >  max) {
						max = size.Z;
					}

					if (size.Y >  max) {
						max = size.Y;
					}

					core::vector3df c = (*SceneContext.metaNode)->getBoundingBox().getCenter();
					float scale = (100.0f / max);

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

	bool isRotatingPreview()
	{
		return rotating;
	}

	vector2d<s32> getLastMousePos()
	{
		return mousePos;
	}

private:
	MapSceneContext & SceneContext;
	bool rotating = false;
	vector2d<s32> mousePos;
};

#endif