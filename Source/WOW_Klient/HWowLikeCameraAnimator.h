#ifndef HWoWLikeCameraAnimator
#define HWoWLikeCameraAnimator

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class  wwAnim : public ISceneNodeAnimator
{
public:
	wwAnim(ISceneNode* MetaNode, float StartUhelY, float StartUhelX, gui::ICursorControl * CursorControl);
	virtual void animateNode(ISceneNode* node, u32 timeMs);
	virtual bool OnEvent(const SEvent& event);
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0);
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const;
	virtual ESCENE_NODE_ANIMATOR_TYPE getType() const;
	virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0);
	virtual bool isEventReceiverEnabled() const;

	bool isClicked(int buttonNumber);
	bool isPresed(int keyNumber);
	f32 wheelPos();
	vector3df wwAnim::caulculatePosition(ISceneCollisionManager * col, vector3df centr, f32 xUhel, f32 yUhel, f32 radius);

private:
	ISceneNode * metaNode;
	core::vector3df Rotation, Position, nodeLastPosition;
	f32 rotX, rotY;
	f32 wheel, okruh;
	bool buttonStat[3];
	vector2d<s32> mousePos;
	vector2d<s32> rotStart;
	vector2d<s32> lookStart;
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool rotating;
	bool looking;
	gui::ICursorControl * cursorControl;
	bool moving;
	IMeshSceneNode* terrain;
	int ci;
	u32 last_anim_time;
	int anim_cycle, move_cycle;
};

#endif