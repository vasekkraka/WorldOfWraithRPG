#ifndef HLoginScreen
#define HLoginScreen

#include "HWOWKlient.h"

#define IDLOGINBUTTON 1
#define IDUSERNAMEBOX 2
#define IDPASSWORDBOX 3

#define loginBoxHeight 333
#define loginBoxWidth 254

#define loginButtonHeight 43
#define loginButtonWidth 151
#define loginButtonTop 250
#define loginButtonLeft 52

#define loginNameHeight 30
#define loginNameWidth 180
#define loginNameLeft 37
#define loginNameTop 77

#define loginPassHeight 30
#define loginPassWidth 180
#define loginPassLeft 37
#define loginPassTop 173
#define loginPassChar L"*"

#define campfirePositionNormal 446, 299, -150
#define campfirePositionWide 438, 312, -150

namespace WowKlient
{
	// ---- Event Reciever

	class LoginManagerEvent : public irr::IEventReceiver
	{
	public:
		LoginManagerEvent() 
		{ 
			login_click = false;
		}

		virtual bool OnEvent(const irr::SEvent& event);
		virtual bool isLoginClicked();
	private:
		bool login_click;
	};


	class LoginManager
	{
	public:
		LoginManager();
		LoginManager(WowKlient::Core::GameState * data);
		~LoginManager();
		void loginPrompt();
		irr::core::stringw getUserName();
		irr::core::string<char> getUserPassword();

	private:
		WowKlient::Core::GameState * gState;
	};
}

#endif