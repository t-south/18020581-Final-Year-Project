#pragma once


namespace geProject {

	enum entityTypes {
		player, enemy, environment, projectile
	};

	struct Entity	{
		int id;
		int parentId{ -1 };
		int lifeTime{-1};
		entityTypes type;
		unsigned int compMask;
	};

}