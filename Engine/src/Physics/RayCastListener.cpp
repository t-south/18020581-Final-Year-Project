#include "RayCastListener.h"

geProject::RayCastListener::RayCastListener(int entity): entityId(entity), rHit(false), targetEntityId(-1), rPoint(b2Vec2()), rNormal(b2Vec2()), rFraction(0), rFixture(NULL) { }

geProject::RayCastListener::~RayCastListener(){}

float geProject::RayCastListener::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction){
    rFixture = fixture;
    rPoint = point;
    rNormal = normal;
    rFraction = fraction;
    Entity* entity = (Entity*)fixture->GetBody()->GetUserData().pointer;
    if (fraction > 0) {
        rHit = true;
    }
    targetEntityId = entity->id;
    return fraction;
}
