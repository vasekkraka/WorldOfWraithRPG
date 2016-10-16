#include "HMapEditor.h"


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

	ICameraSceneNode * cam = smgr->addCameraSceneNodeMaya();
	cam->setPosition(vector3df(0,0,0));
	cam->setTarget(vector3df(0, 0, -100));

	smgr->addCubeSceneNode(10.0f, 0, 0, vector3df(0, 0, -100), vector3df(0, 0, 0));

	IGUIButton * button_add =  guienv->addButton(rect<s32>(gState->gConf->resolution.Width - 50, 500, gState->gConf->resolution.Width, 520), 0, ID_GUI_ADD_BUTTON, L"Add", L"ToolTip");

	IGUIListBox * listBoxModels = guienv->addListBox(rect<s32>(gState->gConf->resolution.Width - 300, 0, gState->gConf->resolution.Width, 330), 0, ID_GUI_LISTBOX_MODELS, true);

	//for (s32 i = 0; i<irr::gui::EGDC_COUNT; ++i)
	//{
	//	video::SColor col = guienv->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
	//	col.setAlpha(90);
	//	guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
	//}

	//gState->irrDevice->setEventReceiver();
	

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
				
		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
		u32 newtime = gState->irrDevice->getTimer()->getTime();
		if ((newtime - oldtime) < 1000 / gState->gConf->reqFps)
		{
			gState->irrDevice->sleep((1000 / gState->gConf->reqFps) - (newtime - oldtime));
		}
	}
}