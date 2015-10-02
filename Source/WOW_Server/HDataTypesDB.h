/*
|--------------------------------------------------------------------|
|                                                                    |
|    Václav Macura   (c) 2015      World of Wraith - Story Begin     |
|                                                                    |
|    ===> CWOWServer.cpp - hlavicka datovych typu pro data z DB      |
|                                                                    |
|--------------------------------------------------------------------|
*/

#ifndef HDataTypesDB
#define HDataTypesDB


#include "HWOWServer.h"

// 

struct dbWorld
{
	int id;
	sql::SQLString name;
};

struct dbCountry
{
	int id;
	sql::SQLString name;
	int worldId;
	int levelMin;
	int levelMax;
};

struct dbInstance
{
	int id;
	sql::SQLString name;
	int worldId;
	int levelMin;
	int playerReq;
};

struct dbNpc
{
	int id;
	sql::SQLString name;
	sql::SQLString groupName;
	int respawnTime;
	bool spawnRestart;
	int spawnCount;
	int level;
	int hp;
	int energy;
	int attackerType;
	int gearSkinId;
	int modelId;
	int aiType;
	int countryId;
	int instanceId;
};

struct dbItem
{
	int id;
	sql::SQLString name;
	sql::SQLString description;
	int level;
	bool usable;
	int usableFuncType;
	int stat[4];
	int statType[4];
	int baseType;
	int rare;
	int priceGold;
	int priceSilver;
	bool soulbound;
	bool logofDissapear;
};

struct dbNpcAbility
{
	int id;
	int damage;
	int type;
	int minRange;
	int maxRange;
	int cooldown;
	int npcId;
};

struct dbSpell
{
	int id;
	int classId;
	int baseDamage;
	int cooldown;
	int minRange;
	int maxRange;
	int levelReq;
	int requiredTalentId;
};

struct dbAccount
{
	int id;
	sql::SQLString login;
	sql::SQLString password;
	sql::SQLString email;
	sql::SQLString name;
	sql::SQLString surname;
};

struct dbProfession
{
	int id;
	sql::SQLString name;
	int maxSkill;
};

struct dbClass
{
	int id;
	sql::SQLString name;
	int bonusHp;
	int bonusEnergy;
	int baseHp;
	int baseEnergy;
	int energyType;
};

struct dbPlayerCharacter 
{
	int id;
	sql::SQLString name;
	int classId;
	int firstTalentId;
	int secondTalentId;

	int firstProfessionId;
	int secondProfessionId;
	int bagsId;
	int golds;
	int silver;
	int accountId;
	int level;
	int xpInLevel;
};

struct dbProfessionItems
{
	int id;
	int profesionId;
	int createdItemId;
	int createdItemCount;
	int requiredSkill;
	int middleSkill;
	int primitiveSkill;
	int codTime;
};

struct dbBags
{
	int id;
	int bagId[4];
	int itemId;
};

struct dbStockProfession
{
	int id;
	int itemId;
	int count;
	int profesionItemId;
};

struct dbTalentTree
{
	int id;
	int talentId[15];
};

struct dbTalent
{
	int id;
	sql::SQLString name;
	sql::SQLString description;
	int functionType;
	int value[5];
	sql::SQLString luaFunc;
};

struct dbBag
{
	int id;
	int size;
	int itemId;
};

struct dbBagItem
{
	int id;
	int bagId;
	int position;
	int itemId;
};



#endif