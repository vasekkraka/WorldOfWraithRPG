#include "HMapEditor.h"
#include "HMapEditorEventReciever.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp> 

namespace fs = boost::filesystem;
using namespace std;

MapEditor::MapEditor()
{

}

MapEditor::MapEditor(WowKlient::Core::GameState * state)
{
	gState = state;
}

MapEditor::~MapEditor()
{
	
}

void MapEditor::setGState(WowKlient::Core::GameState * state)
{
	gState = state;
}



void MapEditor::prepairModelsList(mapSceneContext * sceneContext)
{
	ifstream  mtl_file;
	ofstream  output_file;
	fs::path targetDir(PATH_PREFIX "\\model");

	fs::directory_iterator it(targetDir), eod;

	int mtl_count = 0;

	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (is_regular_file(p))
		{
			std::string pripona = p.filename().string().substr(p.filename().string().length() - 4, 4);

			if (!pripona.compare(".obj"))
			{
				mtl_count++;
				sceneContext->listBoxModels->addItem(p.filename().c_str());
			}

		}
	}
	printf("\n\nTotal OBJ count: %i", mtl_count);
}

IGUIToolBar * MapEditor::createToolbar(mapSceneContext * sceneContext)
{
	irr::gui::IGUIEnvironment* guienv = gState->irrDevice->getGUIEnvironment();
	irr::video::IVideoDriver * driver = gState->irrDevice->getVideoDriver();
	irr::scene::ISceneManager * smgr = gState->irrDevice->getSceneManager();

	IGUIToolBar * toolBar = guienv->addToolBar(guienv->getRootGUIElement(), ID_GUI_TOOLBAR);

	toolBar->setRelativePosition(position2di(0, 0));
	toolBar->setMinSize(dimension2du(600, 60));
	toolBar->setMaxSize(dimension2du(600, 60));

	ITexture * buttonTexture;
	IGUIButton * toolbarButton;

	buttonTexture = driver->getTexture(PATH_PREFIX "\\icons\\new.png");
	driver->makeColorKeyTexture(buttonTexture, SColor(0, 0, 0, 0));
	toolbarButton = toolBar->addButton(ID_GUI_TOOLBAR_BUTTON_NEW, L"", L"Vytvoøí novou prazdnou mapu", buttonTexture);
	toolbarButton->setUseAlphaChannel();

	buttonTexture = driver->getTexture(PATH_PREFIX "\\icons\\open.png");
	driver->makeColorKeyTexture(buttonTexture, SColor(0, 0, 0, 0));
	toolbarButton = toolBar->addButton(ID_GUI_TOOLBAR_BUTTON_OPEN, L"", L"Otevøe soubor s mapou", buttonTexture);
	toolbarButton->setUseAlphaChannel();

	buttonTexture = driver->getTexture(PATH_PREFIX "\\icons\\save.png");
	driver->makeColorKeyTexture(buttonTexture, SColor(0, 0, 0, 0));
	toolbarButton = toolBar->addButton(ID_GUI_TOOLBAR_BUTTON_SAVE, L"", L"Uloží mapu", buttonTexture);
	toolbarButton->setUseAlphaChannel();

	buttonTexture = driver->getTexture(PATH_PREFIX "\\icons\\wmo.png");
	driver->makeColorKeyTexture(buttonTexture, SColor(0, 0, 0, 0));
	toolbarButton = toolBar->addButton(ID_GUI_TOOLBAR_BUTTON_WMO, L"", L"Práce s WMO", buttonTexture);
	toolbarButton->setUseAlphaChannel();

	buttonTexture = driver->getTexture(PATH_PREFIX "\\icons\\npc.png");
	driver->makeColorKeyTexture(buttonTexture, SColor(0, 0, 0, 0));
	toolbarButton = toolBar->addButton(ID_GUI_TOOLBAR_BUTTON_NPC, L"", L"Práce s NPC", buttonTexture);
	toolbarButton->setUseAlphaChannel();

	buttonTexture = driver->getTexture(PATH_PREFIX "\\icons\\route.png");
	driver->makeColorKeyTexture(buttonTexture, SColor(0, 0, 0, 0));
	toolbarButton = toolBar->addButton(ID_GUI_TOOLBAR_BUTTON_ROUTE, L"", L"Práce s cestami", buttonTexture);
	toolbarButton->setUseAlphaChannel();

	return toolBar;
}

void initializeCamera(mapSceneContext * sceneContext)
{
	sceneContext->cameraNodeMaya = sceneContext->device->getSceneManager()->addCameraSceneNodeMaya();
	sceneContext->cameraNodeMaya->setTarget(vector3df(0, 5, 0));
	irr::core::list<ISceneNodeAnimator *>::ConstIterator anim = sceneContext->cameraNodeMaya->getAnimators().begin();
	if ((*anim)->getType() == ESCENE_NODE_ANIMATOR_TYPE::ESNAT_CAMERA_MAYA)
	{
		((ISceneNodeAnimatorCameraMaya*)(*anim))->setDistance(1);
	}

	sceneContext->cameraNodeModelPreview = sceneContext->device->getSceneManager()->addCameraSceneNode();
	sceneContext->cameraNodeModelPreview->setPosition(vector3df(0, 0, -125));
	sceneContext->cameraNodeModelPreview->setTarget(vector3df(0, 0, 0));

	sceneContext->device->getSceneManager()->setActiveCamera(sceneContext->cameraNodeMaya);
}

void MapEditor::runMapEditor()
{
	irr::gui::IGUIEnvironment* guienv = gState->irrDevice->getGUIEnvironment();
	irr::video::IVideoDriver * driver = gState->irrDevice->getVideoDriver();
	irr::scene::ISceneManager * smgr = gState->irrDevice->getSceneManager();

	mapSceneContext SceneContext;
	SceneContext.gState = gState;
	SceneContext.device = gState->irrDevice;
	//IMeshSceneNode * metaNode = NULL;
	SceneContext.metaNode = NULL;
	ITexture* rtt = 0;

	MapEditorEventReceiver eventReciever(&SceneContext);
	gState->irrDevice->setEventReceiver(&eventReciever);

	IGUISkin * skin = guienv->getSkin();
	IGUIFont * font = guienv->getFont(PATH_PREFIX "\\font\\Comic_10.xml");

	if (font)
		skin->setFont(font);

	createToolbar(&SceneContext);
	initializeCamera(&SceneContext);

	SceneContext.toolBox = guienv->addWindow(rect<s32>(gState->gConf->resolution.Width - SIZE_TOOL_WINDOW_WIDTH, 0, gState->gConf->resolution.Width, SIZE_TOOL_WINDOW_HEIGHT), false, L"Nástroje", 0,ID_TOOL_WINDOW);
	SceneContext.toolBox->setVisible(false);

	IGUITabControl * tabControl = guienv->addTabControl(rect<s32>(4, 25, SIZE_TAB_CONTROL_WIDTH, SIZE_TAB_CONTROL_HEIGHT), SceneContext.toolBox);

	IGUITab * tabMap = tabControl->addTab(L"WMO", 1);
	IGUITab * tabProperties = tabControl->addTab(L"Vlastnosti", 1);

	SceneContext.previewImage = guienv->addImage(rect<s32>(SIZE_TAB_LEFT, SIZE_GUI_IMAGE_PREVIEW_TOP, SIZE_GUI_IMAGE_PREVIEW_WIDTH, SIZE_GUI_IMAGE_PREVIEW_TOP + SIZE_GUI_IMAGE_PREVIEW_HEIGHT), tabMap, ID_GUI_IMAGE_PREVIEW, L"Preview");

	guienv->addButton(rect<s32>(SIZE_TAB_LEFT, SIZE_GUI_ADD_BUTTON_TOP, SIZE_GUI_ADD_BUTTON_WIDTH, SIZE_GUI_ADD_BUTTON_TOP + SIZE_GUI_ADD_BUTTON_HEIGHT), tabMap, ID_GUI_ADD_BUTTON, L"Add", L"ToolTip");

	SceneContext.listBoxModels = guienv->addListBox(rect<s32>(SIZE_TAB_LEFT, 0, SIZE_TAB_RIGHT, SIZE_GUI_LISTBOX_MODELS_HEIGHT + SIZE_TAB_CONTROL_TOP), tabMap, ID_GUI_LISTBOX_MODELS, true);

	rtt = driver->addRenderTargetTexture(dimension2d<u32>(SIZE_GUI_IMAGE_PREVIEW_WIDTH, SIZE_GUI_IMAGE_PREVIEW_HEIGHT), "RTT1");
	SceneContext.previewImage->setImage(rtt);

	prepairModelsList(&SceneContext);

	while (gState->irrDevice->run())
	{

		u32 oldtime = gState->irrDevice->getTimer()->getTime();
		int fps = driver->getFPS();

		core::stringw str = L"World of Wraith - Story Begin MAP EDITOR [";
		str += driver->getName();
		str += "] FPS:";
		str += fps;

		gState->irrDevice->setWindowCaption(str.c_str());
		

		driver->beginScene(true, true, SColor(255, 128, 128, 128)); // zacnu, s cernym pozadim

		if (rtt)
		{
			bool visible;
			ICameraSceneNode * activeCam = smgr->getActiveCamera();
			smgr->setActiveCamera(SceneContext.cameraNodeModelPreview);
			if (SceneContext.metaNode != NULL)
			{
				visible = SceneContext.metaNode->isVisible();
				SceneContext.metaNode->setVisible(true);

				SceneContext.device->getSceneManager()->setActiveCamera(SceneContext.cameraNodeModelPreview);
				irr::core::list<mapNode *>::Iterator it = SceneContext.mapNodes.begin();
				while (it != SceneContext.mapNodes.end())
				{
					(*it)->node->setVisible(false);
					it++;
				}
			}
			driver->setRenderTarget(rtt, true, true, SColor(255, 128, 128, 128));
			smgr->drawAll();
			if (SceneContext.metaNode)
			{
				SceneContext.metaNode->setVisible(visible);

				SceneContext.device->getSceneManager()->setActiveCamera(SceneContext.cameraNodeModelPreview);
				irr::core::list<mapNode *>::Iterator it = SceneContext.mapNodes.begin();
				while (it != SceneContext.mapNodes.end())
				{
					(*it)->node->setVisible(true);
					it++;
				}
			}

			driver->setRenderTarget(0, true, true, SColor(255, 128, 128, 128));
			smgr->setActiveCamera(activeCam);
		}
				
		smgr->drawAll();
		guienv->drawAll();

		vector3df point;
		triangle3df triangle;

		smgr->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(gState->irrDevice->getCursorControl()->getPosition()), point, triangle);

		video::SMaterial material;
		material.Lighting = false;
		material.Wireframe = true;
		driver->setMaterial(material);
		driver->setTransform(video::ETS_WORLD, core::matrix4());
		driver->draw3DTriangle(triangle, SColor(200, 255, 0, 0));

		driver->endScene();
		u32 newtime = gState->irrDevice->getTimer()->getTime();
		if ((newtime - oldtime) < 1000 / gState->gConf->reqFps)
		{
			gState->irrDevice->sleep((1000 / gState->gConf->reqFps) - (newtime - oldtime));
		}
	}
}