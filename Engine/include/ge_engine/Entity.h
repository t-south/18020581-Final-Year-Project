#pragma once
namespace geProject {
	enum entityTypes {
		player, enemy, environment
	};

	struct Entity	{
		int id;
		entityTypes type;
		unsigned int compMask;
	};

}