#ifndef HLoginScreen
#define HLoginScreen

#include "HWOWKlient.h"

#define loginBoxHeight 333
#define loginBoxWidth 254


#define loginNameHeight 30
#define loginNameWidth 180
#define loginNameLeft 37
#define loginNameTop 77

#define loginPassHeight 30
#define loginPassWidth 180
#define loginPassLeft 37
#define loginPassTop 173
#define loginPassChar L"*"

namespace WowKlient
{
	class LoginManager
	{
	public:
		LoginManager();
		LoginManager(WowKlient::Core::GameState * data);
		~LoginManager();
		void loginPrompt();
		irr::core::string<char> getUserName();
		irr::core::string<char> getUserPassword();

	private:
		WowKlient::Core::GameState * gState;
	};
}

#endif