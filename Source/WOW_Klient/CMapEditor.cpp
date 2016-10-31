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
	cam->setPosition(vector3df(0,0,0-100));
	cam->setTarget(vector3df(0, 0, 0));

	IGUIImage * previewImage = guienv->addImage(rect<s32>(gState->gConf->resolution.Width - SIZE_GUI_IMAGE_PREVIEW_WIDTH, gState->gConf->resolution.Height - SIZE_GUI_IMAGE_PREVIEW_HEIGHT, gState->gConf->resolution.Width, gState->gConf->resolution.Height), 0, ID_GUI_IMAGE_PREVIEW, L"Preview");

	IGUIButton * buttonAdd = guienv->addButton(rect<s32>(gState->gConf->resolution.Width - SIZE_GUI_IMAGE_PREVIEW_WIDTH, gState->gConf->resolution.Height - SIZE_GUI_IMAGE_PREVIEW_HEIGHT - SIZE_GUI_ADD_BUTTON_HEIGHT, gState->gConf->resolution.Width, gState->gConf->resolution.Height - SIZE_GUI_IMAGE_PREVIEW_HEIGHT), 0, ID_GUI_ADD_BUTTON, L"Add", L"ToolTip");

	IGUIListBox * listBoxModels = guienv->addListBox(rect<s32>(gState->gConf->resolution.Width - SIZE_GUI_IMAGE_PREVIEW_WIDTH, 0, gState->gConf->resolution.Width, gState->gConf->resolution.Height - SIZE_GUI_IMAGE_PREVIEW_HEIGHT - SIZE_GUI_ADD_BUTTON_HEIGHT), 0, ID_GUI_LISTBOX_MODELS, true);

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
			printf("\n pripona: %s", pripona.c_str());

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
			if (metaNode)
			metaNode->setVisible(true);
			driver->setRenderTarget(rtt, true, true, SColor(255, 128, 128, 128));
			smgr->drawAll();
			if (metaNode)
			metaNode->setVisible(false);
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