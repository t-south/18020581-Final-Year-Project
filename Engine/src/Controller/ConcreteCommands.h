#pragma once
#include "Command.h"

//using the command design pattern for the controller, this 
//allows for flexibility on implementing player or AI commands
namespace geProject {
	class DashCommand : public Command {
	public:
		virtual void execute(Controller& entity) override { entity.dash(); };
	};

	class AttackCommand : public Command {
	public:
		virtual void execute(Controller& entity) override { entity.attack(); };
	};

	class SpecialAttackCommand : public Command {
	public:
		virtual void execute(Controller& entity) override { entity.specialAttack(); };
	};

	class ShieldCommand : public Command {
	public:
		virtual void execute(Controller& entity) override { entity.shield(); };
	};

	class SwitchAbilityCommand : public Command {
		virtual void execute(Controller& entity) override { entity.switchAbility(); };
	};

	class MoveCommand : public Command {
	public:
		virtual void execute(Controller& entity) override { entity.moveTo(x, y); };	
		int x;
		int y;
	};

}