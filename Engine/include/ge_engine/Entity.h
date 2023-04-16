#pragma once


namespace geProject {

	enum entityTypes {
		player			= 0x0001,
		enemy			= 0x0002,
		environment     = 0x0004,
		playerprojectile= 0x0008,
		enemyprojectile = 0x0010,
		completion      = 0x0020,
		viewSensor      = 0x0040,
		noType          = 0x0080
	};

	struct Entity	{
		int id;
		int parentId{ -1 };
		int lifeTime{-1};
		entityTypes type{ noType };
		bool targetVisible{ false };
		unsigned int compMask;
	};

}