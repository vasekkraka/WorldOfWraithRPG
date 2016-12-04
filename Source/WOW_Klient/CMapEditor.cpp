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

void MapEditor::runMapEditor()
{
	irr::gui::IGUIEnvironment* guienv = gState->irrDevice->getGUIEnvironment();
	irr::video::IVideoDriver * driver = gState->irrDevice->getVideoDriver();
	irr::scene::ISceneManager * smgr = gState->irrDevice->getSceneManager();

	IMeshSceneNode * metaNode = NULL;

	ICameraSceneNode * cam = smgr->addCameraSceneNode();
	cam->setPosition(vector3df(0, 0, -125));
	cam->setTarget(vector3df(0, 0, 0));

	IGUIToolBar * toolBar = guienv->addToolBar(guienv->getRootGUIElement(), ID_GUI_TOOLBAR);

	toolBar->setRelativePosition(position2di(0,0));
	toolBar->setMinSize(dimension2du(600,50));
	toolBar->setMaxSize(dimension2du(600, 50));

	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_ALWAYS_32_BIT);

	ITexture * buttonTexture;
	IGUIButton * toolbarButton;

	buttonTexture = driver->getTexture(PATH_PREFIX "\\icons\\new.png");
	driver->makeColorKeyTexture(buttonTexture, SColor(0, 0, 0, 0));
	toolbarButton = toolBar->addButton(ID_GUI_TOOLBAR_BUTTON_NEW, L"", L"Vytovøí novou prazdnou mapu", buttonTexture);
	toolbarButton->setUseAlphaChannel();

	buttonTexture = driver->getTexture(PATH_PREFIX "\\icons\\open.png");
	driver->makeColorKeyTexture(buttonTexture, SColor(0, 0, 0, 0));
	toolbarButton = toolBar->addButton(ID_GUI_TOOLBAR_BUTTON_OPEN, L"", L"Otevøe soubor s mapou", buttonTexture);
	toolbarButton->setUseAlphaChannel();

	IGUIWindow * toolWindow = guienv->addWindow(rect<s32>(gState->gConf->resolution.Width - SIZE_TOOL_WINDOW_WIDTH, 0, gState->gConf->resolution.Width, SIZE_TOOL_WINDOW_HEIGHT), false, L"Nástroje");

	IGUITabControl * tabControl = guienv->addTabControl(rect<s32>(4, 25, SIZE_TAB_CONTROL_WIDTH, SIZE_TAB_CONTROL_HEIGHT), toolWindow);

	IGUITab * tabMap = tabControl->addTab(L"WMO", 1);
	IGUITab * tabProperties = tabControl->addTab(L"Vlastnosti", 1);
	
	IGUIImage * previewImage = guienv->addImage(rect<s32>(SIZE_TAB_LEFT, SIZE_GUI_IMAGE_PREVIEW_TOP, SIZE_GUI_IMAGE_PREVIEW_WIDTH, SIZE_GUI_IMAGE_PREVIEW_TOP + SIZE_GUI_IMAGE_PREVIEW_HEIGHT), tabMap, ID_GUI_IMAGE_PREVIEW, L"Preview");

	IGUIButton * buttonAdd = guienv->addButton(rect<s32>(SIZE_TAB_LEFT, SIZE_GUI_ADD_BUTTON_TOP, SIZE_GUI_ADD_BUTTON_WIDTH, SIZE_GUI_ADD_BUTTON_TOP + SIZE_GUI_ADD_BUTTON_HEIGHT), tabMap, ID_GUI_ADD_BUTTON, L"Add", L"ToolTip");

	IGUIListBox * listBoxModels = guienv->addListBox(rect<s32>(SIZE_TAB_LEFT, 0, SIZE_TAB_RIGHT, SIZE_GUI_LISTBOX_MODELS_HEIGHT + SIZE_TAB_CONTROL_TOP), tabMap, ID_GUI_LISTBOX_MODELS, true);

	MapSceneContext SceneContext;

	SceneContext.device = gState->irrDevice;
	SceneContext.listBoxModels = listBoxModels;
	SceneContext.metaNode = &metaNode;

	MapEditorEventReceiver eventReciever(SceneContext);
	gState->irrDevice->setEventReceiver(&eventReciever);

	IGUISkin * skin = guienv->getSkin();
	IGUIFont * font = guienv->getFont(PATH_PREFIX "\\font\\Comic_10.xml");

	if (font)
		skin->setFont(font);

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
				listBoxModels->addItem(p.filename().c_str());
			}

		}
	}

	printf("\n\nTotal OBJ count: %i", mtl_count);

	ITexture* rtt = 0;

	rtt = driver->addRenderTargetTexture(dimension2d<u32>(SIZE_GUI_IMAGE_PREVIEW_WIDTH, SIZE_GUI_IMAGE_PREVIEW_HEIGHT), "RTT1");

	previewImage->setImage(rtt);

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
			if (metaNode)
			{
				visible = metaNode->isVisible();
				metaNode->setVisible(true);
			}
			driver->setRenderTarget(rtt, true, true, SColor(255, 128, 128, 128));
			smgr->drawAll();
			if (metaNode)
			metaNode->setVisible(visible);
			driver->setRenderTarget(0, true, true, SColor(255, 128, 128, 128));
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