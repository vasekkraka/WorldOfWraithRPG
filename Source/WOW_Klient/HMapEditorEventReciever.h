#ifndef HMapEditorEventReciever
#define HMapEditorEventReciever

#include "HMapLoader.h"
#include "HWOWKlient.h"
#include "HMapEditor.h"


class MapEditorEventReceiver : public IEventReceiver
{
public:
	MapEditorEventReceiver(mapSceneContext * context) : SceneContext(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		ISceneManager * smgr = SceneContext->device->getSceneManager();

		if (event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			IGUIElement * elm = SceneContext->device->getGUIEnvironment()->getRootGUIElement()->getElementFromPoint(SceneContext->device->getCursorControl()->getPosition());
			mousePos = SceneContext->device->getCursorControl()->getPosition();
			switch (event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				if (elm->getID() == ID_GUI_IMAGE_PREVIEW)
				{
					printf("\n Mys  na obrazku :-) \n");
					rotating = true;
					return false;
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
					if ((SceneContext->metaNode) != NULL)
					{
						SceneContext->metaNode->setVisible(!SceneContext->metaNode->isVisible());
						if (SceneContext->metaNode->isVisible())
						{
							SceneContext->device->getSceneManager()->setActiveCamera(SceneContext->cameraNodeModelPreview);
							irr::core::list<mapNode *>::Iterator it = SceneContext->mapNodes.begin();
							while (it != SceneContext->mapNodes.end())
							{
								(*it)->node->setVisible(false);
								it++;
							}
						}
						else
						{
							SceneContext->device->getSceneManager()->setActiveCamera(SceneContext->cameraNodeMaya);
							irr::core::list<mapNode *>::Iterator it = SceneContext->mapNodes.begin();
							while (it != SceneContext->mapNodes.end())
							{
								(*it)->node->setVisible(true);
								it++;
							}
						}
					}
					return true;
				}
				break;
			}

			if (isRotatingPreview() && SceneContext->metaNode != NULL)
			{
				vector3df rot = SceneContext->metaNode->getRotation();

				rot.Y += (SceneContext->device->getCursorControl()->getPosition().X - getLastMousePos().X);

				SceneContext->metaNode->setRotation(rot);
			}
		}
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = SceneContext->device->getGUIEnvironment();
			
			switch (event.GUIEvent.EventType)
			{
			case EGET_ELEMENT_CLOSED:
				switch (id)
				{
				case ID_TOOL_WINDOW:
					SceneContext->toolBox->setVisible(false);
					return true;
					break;
				}
				break;
			case EGET_BUTTON_CLICKED:
				switch (id)
				{
				case ID_GUI_TOOLBAR_BUTTON_NEW:
					smgr->clear();
					initializeCamera(SceneContext);
					SceneContext->metaNode = NULL;
					SceneContext->mapNodes.push_back(new mapNode());
					(*SceneContext->mapNodes.getLast())->node = smgr->addMeshSceneNode(SceneContext->device->getSceneManager()->getMesh(PATH_PREFIX "\\model\\editor\\new.obj"));
					(*SceneContext->mapNodes.getLast())->node->setPosition(vector3df(0,0,0));
					(*SceneContext->mapNodes.getLast())->node->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
					break;
				case ID_GUI_TOOLBAR_BUTTON_WMO:
					SceneContext->toolBox->setVisible(true);
					break;
				}
				break;
			case EGET_LISTBOX_CHANGED:
			case EGET_LISTBOX_SELECTED_AGAIN:
				if (id == ID_GUI_LISTBOX_MODELS)
				{
					//wprintf(L"Vybran model: %s\n", SceneContext->listBoxModels->getListItem(SceneContext->listBoxModels->getSelected()));

					if (SceneContext->metaNode != NULL)
					{
						SceneContext->metaNode->remove();
					}

					std::basic_string<wchar_t> cesta;

					cesta.append(L"..\\..\\..\\Data\\model\\");
					cesta.append(SceneContext->listBoxModels->getListItem(SceneContext->listBoxModels->getSelected()));

					IAnimatedMesh* mesh = SceneContext->device->getSceneManager()->getMesh(cesta.c_str());
					
					SceneContext->metaNode = SceneContext->device->getSceneManager()->addMeshSceneNode(mesh, 0, 0, vector3df(0, 0, -100));
					SceneContext->metaNode->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
					SceneContext->metaNode->setMaterialType(E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
					SceneContext->metaNode->setVisible(false);

					SceneContext->metaNode->setPosition(vector3df(0,0,0));

					SceneContext->metaNode->setTriangleSelector(SceneContext->device->getSceneManager()->createTriangleSelector(SceneContext->metaNode->getMesh(), SceneContext->metaNode));

					vector3df size = SceneContext->metaNode->getBoundingBox().getExtent();

					float max = size.X;
					if (size.Z >  max) {
						max = size.Z;
					}

					if (size.Y >  max) {
						max = size.Y;
					}

					core::vector3df c = SceneContext->metaNode->getBoundingBox().getCenter();
					float scale = (100.0f / max);

					c.X = -(c.X * scale);
					c.Y = -(c.Y * scale);
					c.Z = -(c.Z * scale);
					SceneContext->metaNode->setPosition(c);

					SceneContext->metaNode->setScale(core::vector3df(scale, scale, scale));
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
	mapSceneContext * SceneContext;
	bool rotating = false;
	vector2d<s32> mousePos;
};

#endif