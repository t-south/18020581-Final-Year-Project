#pragma once


namespace geProject {

	enum entityTypes {
		player, enemy, environment, playerprojectile, enemyprojectile, noType
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