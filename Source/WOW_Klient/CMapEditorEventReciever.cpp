#include <irrlicht.h>
#include "HMapEditorEventReciever.h"

bool MapEditorEventReceiver::parseMouse(const SEvent& event)
{
	IGUIElement * cursorElement = SceneContext->device->getGUIEnvironment()->getRootGUIElement()->getElementFromPoint(SceneContext->device->getCursorControl()->getPosition());
	switch (event.MouseInput.Event)
	{
	case EMIE_LMOUSE_PRESSED_DOWN:
		if (cursorElement)
		{
			switch (cursorElement->getID())
			{
			case ID_GUI_IMAGE_PREVIEW:
				break;
			default:
				if (cursorElement->getID())
				{
					// kliknuti na objekt s ID > 0 ale nevyuziva se
					printf("\n==> Jine UI !");
				}
				else
				{
					// kliknuti mimo UI
					printf("\n==> Mimo UI !");
					if ((SceneContext->metaNode) != NULL && SceneContext->metaNode->isVisible())
					{
						rotating = true;
						mousePos = SceneContext->device->getCursorControl()->getPosition();
						metaRotationOld = SceneContext->metaNode->getRotation();
					}
				}
				break;
			}
		}
		break;
	case EMIE_LMOUSE_LEFT_UP:
		if (rotating)
		{
			rotating = false;
		}

		break;
	case EMIE_RMOUSE_PRESSED_DOWN:
		if (cursorElement)
		{
			switch (cursorElement->getID())
			{
			case ID_GUI_IMAGE_PREVIEW:
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
				break;
			}
		default:
			if (cursorElement->getID())
			{
				// kliknuti na objekt s ID > 0 ale nevyuziva se
			}
			else
			{
				// kliknuti mimo UI
				printf("\n==> Mimo UI !");
			}
			break;
		}
		break;
	case EMIE_MOUSE_MOVED:
		if (cursorElement)
		{
			switch (cursorElement->getID())
			{
			case ID_GUI_IMAGE_PREVIEW:
				return true;
				break;
			}
		}
		break;
	}

	if (isRotatingPreview() && SceneContext->metaNode != NULL)
	{
		vector3df rot = SceneContext->metaNode->getRotation();

		rot.Y = metaRotationOld.Y - (SceneContext->device->getCursorControl()->getPosition().X - getLastMousePos().X);

		SceneContext->metaNode->setRotation(rot);
		return true;
	}
	return false;
}

bool MapEditorEventReceiver::parseGUI(const SEvent& event)
{
	s32 id = event.GUIEvent.Caller->getID();
	IGUIEnvironment* env = SceneContext->device->getGUIEnvironment();
	ISceneManager * smgr = SceneContext->device->getSceneManager();

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
			(*SceneContext->mapNodes.getLast())->node = smgr->addMeshSceneNode(SceneContext->device->getSceneManager()->getMesh(PATH_PREFIX "\\wmomodel\\editor\\new.obj"));
			(*SceneContext->mapNodes.getLast())->node->setPosition(vector3df(0, 0, 0));
			(*SceneContext->mapNodes.getLast())->node->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
			break;
		case ID_GUI_TOOLBAR_BUTTON_WMO:
			SceneContext->toolBox->setVisible(true);
			break;
		}
		break;
	case EGET_LISTBOX_CHANGED:
	case EGET_LISTBOX_SELECTED_AGAIN:
		switch (id)
		{
		case ID_GUI_LISTBOX_MODELS:
			if (SceneContext->metaNode != NULL)
			{
				SceneContext->metaNode->remove();
			}

			std::basic_string<wchar_t> cesta;

			cesta.append(L"..\\..\\..\\Data\\wmomodel\\");
			cesta.append(SceneContext->listBoxModels->getListItem(SceneContext->listBoxModels->getSelected()));

			IAnimatedMesh* mesh = SceneContext->device->getSceneManager()->getMesh(cesta.c_str());

			SceneContext->metaNode = SceneContext->device->getSceneManager()->addMeshSceneNode(mesh, 0, 0, vector3df(0, 0, -100));
			SceneContext->metaNode->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
			SceneContext->metaNode->setMaterialType(E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
			SceneContext->metaNode->setVisible(false);

			SceneContext->metaNode->setPosition(vector3df(0, 0, 0));

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
			break;
		}
		break;
	}
	return false;
}

bool MapEditorEventReceiver::OnEvent(const SEvent& event)
{
	switch (event.EventType)
	{
	case EET_MOUSE_INPUT_EVENT:
		if (parseMouse(event))
			return true;
		break;
	case EET_GUI_EVENT:
		if (parseGUI(event))
			return true;
		break;
	}
	return false;
}