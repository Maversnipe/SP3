#include "CollisionManager.h"

CollisionManager* CollisionManager::cm;

CollisionManager * CollisionManager::getCManager()
{
	if (!cm)
	{
		cm = new CollisionManager;
	}

	return cm;
}

void CollisionManager::SetWorldSize(int height, int width)
{
	m_iworld_height = height;
	m_iworld_width = width;
	colltimecheck = 0;
}

/*void CollisionManager::Check(std::vector<GameObject*> &Objs, std::vector<Block *> &Blks, double dt)
{
//std::cout << colltimecheck << std::endl;
//colltimecheck += dt;

/*for (std::vector<GameObject *>::iterator it = Objs.begin(); it != Objs.end(); ++it)
{
GameObject *go = (GameObject *)*it;

if (go->mass == 0)
go->invmass = 0;
else
go->invmass = 1 / go->mass;

if (go->active && go->type == GameObject::GO_BALL)
{
go->pos += go->vel * static_cast<float>(dt);

//Exercise 2a: Rebound game object at screen edges
// Rebound on X
//if (go->pos.x < 0 + go->scale.x && go->vel.x < 0)
//	go->vel.x = -go->vel.x;
//else if (go->pos.x > m_worldWidth - go->scale.x && go->vel.x > 0)
//	go->vel.x = -go->vel.x;
//// Rebound on Y
//if (go->pos.y < 0 + go->scale.y && go->vel.y < 0)
//	go->vel.y = -go->vel.y;
//else if (go->pos.y > m_worldHeight - go->scale.y && go->vel.y > 0)
//	go->vel.y = -go->vel.y;
////Exercise 2b: Unspawn if it really leave the screen
//if (go->pos.x > m_worldWidth || go->pos.x < 0 || go->pos.y > m_worldHeight || go->pos.y < 0)
//{
//	go->active = false;
//	--m_objectCount;
//	continue;
//}
}

for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != Objs.end(); ++it2)
{
GameObject *go2 = (GameObject *)(*it2);

if (!go2->active)
continue;
if (go->type != GameObject::GO_BALL && go2->type != GameObject::GO_BALL)
continue;

GameObject *goA, *goB;

if (go->type == GameObject::GO_BALL)
{
goA = go;
goB = go2;
}
else
{
goA = go2;
goB = go;
}


if (CheckCollisionC(goA, goB, dt))
{
CollisionResponseC(goA, goB, dt);
}
}
//Set AABBs
go->aabb.SetAABB(go->pos, go->scale);
}

for (std::vector<Block *>::iterator it = Blks.begin(); it != Blks.end(); ++it)
{
Block *go = (Block *)*it;

if (go->mass == 0)
go->invmass = 0;
else
go->invmass = 1 / go->mass;

//Blocks
if (go->active && go->type == GameObject::GO_BLOCK)
{

if (go->isonAir && go->Btype != GameObject::BLOCK_TYPE::GO_GRASS)
{
go->pos += (go->vel + m_vec3Gravity) * static_cast<float>(dt);
}
else
go->pos += go->vel* static_cast<float>(dt);



if (go->vel.y != 0 || go->vel.x != 0)
go->isonAir = true;
else
go->isonAir = false;


//X
if (go->pos.x < 0 + go->scale.x && go->vel.x < 0 || go->pos.x > m_iworld_width - go->scale.x && go->vel.x > 0)
{
go->vel.x = 0;
}
//Y
if (go->isonAir && go->pos.y < 0 + go->scale.y && go->vel.y < 0 || go->pos.y > m_iworld_height - go->scale.y && go->vel.y > 0)
{
go->vel.SetZero();
go->isonAir = false;
go->iscolliding = true;
}

for (std::vector<Block *>::iterator it2 = it + 1; it2 != Blks.end(); ++it2)
{
Block *go2 = (Block *)(*it2);

if (!go2->active)
continue;
if (go->type != GameObject::GO_BLOCK && go2->type != GameObject::GO_BLOCK)
continue;

if (go->Btype == GameObject::BLOCK_TYPE::GO_GRASS && go2->Btype == GameObject::BLOCK_TYPE::GO_GRASS && !go->isonAir)
continue;

Block *goA, *goB;

if (go->type == GameObject::GO_BLOCK)
{
goA = go;
goB = go2;
}
else
{
goA = go2;
goB = go;
}

if (CheckCollisionB(goA, goB, dt))
{
CollisionResponseB(goA, goB);
}
}
}

for (std::vector<GameObject *>::iterator it2 = Objs.begin(); it2 != Objs.end(); ++it2)
{
GameObject *go2 = (GameObject *)(*it2);

if (!go2->active)
continue;
if (go2->type != GameObject::GO_BALL)
continue;

GameObject *goA, *goB;

if (go2->type == GameObject::GO_BALL)
{
goA = go2;
goB = go;
}
else
{
goA = go;
goB = go2;
}


if (CheckCollisionC(goA, goB, dt))
{
CollisionResponseC(goA, goB, dt);
}
}
//Set AABBs
go->aabb.SetAABB(go->pos, go->scale);
}
}*/

bool CollisionManager::CheckCollisionC(GameObject * go1, GameObject * go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		m->A = go1;
		m->B = go2;
		return CirclevsCircle(m);
	}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->dir;
		Vector3 NP = N.Cross(Vector3(0, 0, 1));
		float l = go2->scale.y;
		float r = go1->scale.x;
		float h = go2->scale.x;

		return (abs((w0 - b1).Dot(N)) < r + h * 0.5) &&
			(abs((w0 - b1).Dot(NP)) < r + l * 0.5);
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go1->pos;
		Vector3 p2 = go2->pos;
		float r1 = go1->scale.x;
		float r2 = go2->scale.x;

		return ((p2 - p1).LengthSquared() < ((r1 + r2) * (r1 * r2))) &&
			((p2 - p1).LengthSquared() > 0.f) &&
			((p2 - p1).Dot(go1->vel) > 0.f);
	}
	case GameObject::GO_BLOCK:
	{
		/*Block* go3 = static_cast <Block*>(go2);

		Vector3 w0 = go2->pos;
		Vector3 b1 = go->pos;
		Vector3 N = go2->dir;
		go3->setdir(Vector3(1, 0, 0));
		Vector3 N1 = go3->getdir();
		Vector3 NP = N.Cross(Vector3(0, 0, 1));
		Vector3 NP1 = N1.Cross(Vector3(0, 0, 1));
		float r = go->scale.x;
		float h = go2->scale.x;
		float l = go2->scale.y;
		float h1 = go->scale.y;

		bool check = false;

		if ((abs((w0 - b1).Dot(N)) < r + h * 0.5) &&
		(abs((w0 - b1).Dot(NP)) < r + l * 0.5))
		{
		go2->dir = Vector3(0, 1, 0);
		check = true;
		return true;
		}

		if (((abs((w0 - b1).Dot(N1)) < r + h * 0.5) &&
		(abs((w0 - b1).Dot(NP1)) < r + l * 0.5)))
		{
		go2->dir = go3->getdir();
		check = true;
		return true;
		}


		return check;*/
		//Manifold *m = new Manifold;
		m->A = go2;
		m->B = go1;


		return AABBvsCircle(m);
	}
	case GameObject::GO_EXPLOSION:
	{
		m->A = go1;
		m->B = go2;
		return CirclevsCircle(m);
	}
	default:
		break;
	}
	return false;
}

bool CollisionManager::CheckCollisionB(GameObject * go1, GameObject * go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		m->A = go1;
		m->B = go2;
		return AABBvsCircle(m);
	}
	case GameObject::GO_WALL:
	{
		m->A = go1;
		m->B = go2;
		return AABBvsAABB(m);
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go1->pos;
		Vector3 p2 = go2->pos;
		float r1 = go1->scale.x;
		float r2 = go2->scale.x;

		return ((p2 - p1).LengthSquared() < ((r1 + r2) * (r1 * r2))) &&
			((p2 - p1).LengthSquared() > 0.f) &&
			((p2 - p1).Dot(go1->vel) > 0.f);
	}
	case GameObject::GO_BLOCK:
	{
		m->A = go1;
		m->B = go2;

		return AABBvsAABB(m);
	}
	case GameObject::GO_EXPLOSION:
	{
		m->A = go1;
		m->B = go2;

		return AABBvsCircle(m);
	}
	default:
		break;
	}

	return false;
}

void CollisionManager::CollisionResponseC(GameObject * go, GameObject * go2)
{
	switch (go2->type)
	{
	case GameObject::GO_CANNONT:
		break;
	case GameObject::GO_BALL:
	{
		Vector3 u1 = go->vel;
		Vector3 u2 = go2->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
		go->vel = u1 + 2.f * (u2N - u1N);
		go2->vel = u2 + 2.f * (u1N - u2N);
		//Vector3 temp = go->vel;
		//go->vel -= go2->vel;
		//go2->vel -= temp;
		

		if (go->vel.x > 20)
		{
			go->vel.x = 20;
		}
		else if (go->vel.x < -20)
		{
			go->vel.x = -20;
		}
		if (go->vel.y > 20)
		{
			go->vel.y = 20;
		}
		else if (go->vel.y < -20)
		{
			go->vel.y = -20;
		}

		if (go2->vel.x > 20)
		{
			go2->vel.x = 20;
		}
		else if (go2->vel.x < -20)
		{
			go2->vel.x = -20;
		}
		if (go2->vel.y > 20)
		{
			go2->vel.y = 20;
		}
		else if (go2->vel.y < -20)
		{
			go2->vel.y = -20;
		}

		PositionalCorrection(go, go2);

		break;
	}
	case GameObject::GO_WALL:
	{
		Vector3 vel = go->vel;
		Vector3 N = go2->dir;
		go->vel = vel - (2.f * vel.Dot(N)) * N;

		PositionalCorrection(go, go2);

		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 vel = go->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		go->vel = vel - (2.f * vel.Dot(N)) * N;

		PositionalCorrection(go, go2);

		break;
	}
	case GameObject::GO_BLOCK:
	{
		if (go2->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
		{
			Vector3 vel = go->vel;
			Vector3 N = m->normal.Normalized();
			//go->vel = vel - (2.f * vel.Dot(N)) * N;
			go->vel.SetZero();
			break;
		}

		//relative velocity
		Vector3 rv = go2->vel - go->vel;

		//Velocity along normal
		float velAlongNormal = 0;
		if (m->normal != Vector3(0, 0, 0))
			velAlongNormal = rv.Dot(m->normal.Normalized());

		if (velAlongNormal > 0)
			return;

		//Calculate magnitude/bounciness
		float e = std::min(go->restitution, go2->restitution);

		float j = -(1 + e) * velAlongNormal;
		j /= go->invmass + go2->invmass;

		//Prevent Overlap
		PositionalCorrection(go, go2);

		//Applying Impulses
		if (velAlongNormal < 0.f)
		{
			//Impulse
			Vector3 Impulse = j * m->normal.Normalized();

			float masstotal = go->mass + go2->mass;

			float ratio = go->mass / masstotal;
			go->vel -= ratio * Impulse;

			ratio = go2->mass / masstotal;
			go2->vel += ratio * Impulse;

			//Friction
			Vector3 rv = go2->vel - go->vel;
			Vector3 t = rv - (m->normal.Normalized() * rv.Dot(m->normal.Normalized()));

			//j tangent magnitude
			float jt = -rv.Dot(t);
			jt /= (go->invmass + go2->invmass);

			//Calculate staticFric && dynamicFric
			float sf = std::sqrt(go->staticFric * go2->staticFric);
			float df = std::sqrt(go->dynamicFric * go2->dynamicFric);

			//Coulumb law
			Vector3 tangentImpulse;
			if (std::abs(jt) < j * sf)
				tangentImpulse = t * jt;
			else
				tangentImpulse = t * -j * df;

			go->vel -= go->invmass * tangentImpulse;
			go2->vel += go2->invmass * tangentImpulse;
		}

		if (go->vel.x > 5)
		{
			go->vel.x = 5;
		}
		else if (go->vel.x < -5)
		{
			go->vel.x = -5;
		}
		if (go->vel.y > 5)
		{
			go->vel.y = 5;
		}
		else if (go->vel.y < -5)
		{
			go->vel.y = -5;
		}

		if (go2->vel.x > 5)
		{
			go2->vel.x = 5;
		}
		else if (go2->vel.x < -5)
		{
			go2->vel.x = -5;
		}
		if (go2->vel.y > 5)
		{
			go2->vel.y = 5;
		}
		else if (go2->vel.y < -5)
		{
			go2->vel.y = -5;
		}

		//Vector3 rv = go2->vel - go->vel;

		//float velAlongNormal = 0;
		//if(m->normal != Vector3(0, 0, 0))
		//	velAlongNormal = rv.Dot(m->normal.Normalized());

		////std::cout << " Type: " << typeid(go2).name() << std::endl;

		///*if (velAlongNormal > 0)
		//	return;*/

		////Calculate magnitude/bounciness
		//float e = std::min(go->restitution, go2->restitution);

		//float j = -(1 + e) * velAlongNormal;
		//j /= 1 / go->mass + 1 / go2->mass;

		//Vector3 Impulse = j * m->normal.Normalized();
		///*go->vel += 1/go->mass * Impulse;
		//go2->vel -= 1 / go2->mass * Impulse;*/

		//float masstotal = go->mass + go2->mass;

		//float ratio = go->mass / masstotal;
		//go->vel -= ratio * Impulse;

		//ratio = go2->mass / masstotal;
		//go2->vel += ratio * Impulse;


		//PositionalCorrection(go, go2);

		//Vector3 rotation = 10 * m->normal + go2->vel;
		//go2->rotation += Math::RadianToDegree(atan2(rotation.y, rotation.x));
		//
		//go2->torque += m->normal.Cross(Vector3(0, 5, 0));

		//go2->iscolliding = true;

		break;
	}
	case GameObject::GO_EXPLOSION:
	{
		Vector3 vel = go->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();

		go->vel = vel - (2.f * vel.Dot(N)) * N;

		PositionalCorrection(go, go2);

		break;
	}
	default:
		break;
	}
}

void CollisionManager::CollisionResponseB(GameObject * go, GameObject * go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		if (go->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
		{
			Vector3 vel = go2->vel;
			Vector3 normal = Vector3(0, 1, 0);
			Vector3 N = normal.Normalized();
			//go2->vel = vel - (2.f * vel.Dot(N)) * N;
			go2->vel.SetZero();
			go2->active = false;
			break;
		}

		Vector3 u1 = go->vel;
		Vector3 u2 = go2->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
		//go->vel = u1 + 2.f * (u2N - u1N);
		go2->vel = u2 + 2.f * (u1N - u2N);
		go2->vel -= go->vel;
		go->vel.x -= go2->mass * m->normal.Normalized().x;
		go->torque += m->normal.Cross(Vector3(0, 1, 0));

		PositionalCorrection(go, go2);

		go->iscolliding = true;
		go2->iscolliding = true;
		break;
	}
	case GameObject::GO_WALL:
	{
		Vector3 vel = go->vel;
		Vector3 N = go2->dir;
		go->vel = vel - (2.f * vel.Dot(N)) * N;
		PositionalCorrection(go, go2);
		go->iscolliding = true;
		go2->iscolliding = true;
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 vel = go->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		go->vel = vel - (2.f * vel.Dot(N)) * N;
		PositionalCorrection(go, go2);
		go->iscolliding = true;
		go2->iscolliding = true;
		break;
	}
	case GameObject::GO_BLOCK:
	{
		//relative velocity
		Vector3 rv = go2->vel - go->vel;

		//Velocity along normal
		float velAlongNormal = 0;
		if (m->normal != Vector3(0, 0, 0))
			velAlongNormal = rv.Dot(m->normal.Normalized());

		if (velAlongNormal > 0)
			return;

		//Calculate magnitude/bounciness
		float e = std::min(go->restitution, go2->restitution);

		float j = -(1 + e) * velAlongNormal;
		j /= (go->invmass + go2->invmass);

		//Prevent Overlap
		PositionalCorrection(go, go2);

		//Applying Impulses
		if (velAlongNormal < 0.f)
		{
			//Impulse
			Vector3 Impulse = j * m->normal.Normalized();

			float masstotal = go->mass + go2->mass;

			float ratio = go->mass / masstotal;
			go->vel -= ratio * Impulse;

			ratio = go2->mass / masstotal;
			go2->vel += ratio * Impulse;

			//Friction
			Vector3 rv = go2->vel - go->vel;
			Vector3 t = rv - (m->normal.Normalized() * rv.Dot(m->normal.Normalized()));

			//j tangent magnitude
			float jt = -rv.Dot(t);
			jt /= (go->invmass + go2->invmass);

			//Calculate staticFric && dynamicFric
			float sf = std::sqrt((go->staticFric * go->staticFric) + (go2->staticFric * go2->staticFric));
			float df = std::sqrt((go->dynamicFric * go->dynamicFric) + (go2->dynamicFric * go2->dynamicFric));

			//Coulumb law
			Vector3 tangentImpulse;
			if (std::abs(jt) < j * sf)
				tangentImpulse = t * jt;
			else
				tangentImpulse = t * -j * df;

			go->vel -= go->invmass * tangentImpulse;
			go2->vel += go2->invmass * tangentImpulse;

			if (go->vel.x > 5)
			{
				go->vel.x = 5;
			}
			else if (go->vel.x < -5)
			{
				go->vel.x = -5;
			}
			if (go->vel.y > 5)
			{
				go->vel.y = 5;
			}
			else if (go->vel.y < -5)
			{
				go->vel.y = -5;
			}

			if (go2->vel.x > 5)
			{
				go2->vel.x = 5;
			}
			else if (go2->vel.x < -5)
			{
				go2->vel.x = -5;
			}
			if (go2->vel.y > 5)
			{
				go2->vel.y = 5;
			}
			else if (go2->vel.y < -5)
			{
				go2->vel.y = -5;
			}
		}

		/*go->iscolliding = true;
		go->isonAir = false;
		go2->iscolliding = true;*/

		break;
	}
	case GameObject::GO_EXPLOSION:
	{
		Vector3 u1 = go->vel;
		Vector3 u2 = go2->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
		go->vel = u1 + 2.f * (u2N - u1N);

		PositionalCorrection(go, go2);

		go->torque += m->normal.Cross(Vector3(0, 5, 0));

		go->iscolliding = true;
		break;
	}
	default:
		break;
	}
}

bool CollisionManager::PtVsAABB(Vector3 pos, GameObject * go)
{
	if ((pos.x > go->aabb.GetMinAABB().x && pos.x < go->aabb.GetMaxAABB().x)
	&& (pos.y > go->aabb.GetMinAABB().y && pos.y < go->aabb.GetMaxAABB().y))
		return true;
	else
		return false;
}

void CollisionManager::PositionalCorrection(GameObject * go, GameObject * go2)
{
	const float percent = 0.8; //Penetration percentage to correct(usually 20-80%)
	const float slop = 0.1; // Penetration allowance(usually 0.01 to 0.1)
	Vector3 correction = (std::max((m->penetration - slop), 0.0f) / (go->invmass + go2->invmass)) * percent * m->normal.Normalized();

	if (m->penetration > slop)
	{
		if (go->Btype != GameObject::BLOCK_TYPE::GO_GRASS)
			go->pos -= go->invmass * correction;
		if (go2->Btype != GameObject::BLOCK_TYPE::GO_GRASS)
			go2->pos += go2->invmass * correction;
	}
}

CollisionManager::~CollisionManager()
{
}

CollisionManager::CollisionManager()
{
}

bool CollisionManager::AABBvsAABB(Manifold * m)
{
	GameObject *A = m->A;
	GameObject *B = m->B;

	// Vector from A to B
	Vector3 n = B->pos - A->pos;

	AABB abox = A->aabb;
	AABB bbox = B->aabb;

	// Calculate half extents along x axis for each object
	float a_extent = (abox.GetMaxAABB().x - abox.GetMinAABB().x) / 2;
	float b_extent = (bbox.GetMaxAABB().x - bbox.GetMinAABB().x) / 2;

	// Calculate overlap on x axis
	float x_overlap = a_extent + b_extent - abs(n.x);

	// SAT test on x axis
	if (x_overlap > 0)
	{
		// Calculate half extents along y axis for each object
		float a_extent = (abox.GetMaxAABB().y - abox.GetMinAABB().y) / 2;
		float b_extent = (bbox.GetMaxAABB().y - bbox.GetMinAABB().y) / 2;

		// Calculate overlap on y axis
		float y_overlap = a_extent + b_extent - abs(n.y);

		// SAT test on y axis
		if (y_overlap > 0)
		{
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap)
			{

				// Point towards B knowing that n points from A to B
				if (n.x < 0)
					m->normal = Vector3(-1, 0, 0);
				else
					m->normal = Vector3(1, 0, 0);

				m->penetration = x_overlap;
				return true;
			}

			else
			{
				// Point toward B knowing that n points from A to B
				if (n.y < 0)
					m->normal = Vector3(0, -1, 0);
				else
					m->normal = Vector3(0, 1, 0);

				m->penetration = y_overlap;
				return true;
			}
		}
	}

	return false;
}

bool CollisionManager::CirclevsCircle(Manifold * m)
{
	Vector3 r = m->A->aabb.GetPos() - m->B->aabb.GetPos();

	return  r.LengthSquared() < (m->A->aabb.GetRadius() + m->B->aabb.GetRadius()) * (m->A->aabb.GetRadius() + m->B->aabb.GetRadius());
}

bool CollisionManager::AABBvsCircle(Manifold * m)
{
	GameObject*A = m->A;
	GameObject*B = m->B;

	//Vec from A to B
	Vector3 n = B->pos - A->pos;

	//Closest pt on A to Center of B
	Vector3 closest = n;

	//Calculate half extennts along each axis
	float x_extent = (A->aabb.GetMaxAABB().x - A->aabb.GetMinAABB().x) / 2;
	float y_extent = (A->aabb.GetMaxAABB().y - A->aabb.GetMinAABB().y) / 2;

	//Clamp point
	closest.x = Math::Clamp(closest.x, -x_extent, x_extent);
	closest.y = Math::Clamp(closest.y, -y_extent, y_extent);

	bool inside = false;

	if (n == closest)
	{
		inside = true;

		//Using SAT
		if (abs(n.x) > abs(n.y))
		{
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}
		else
		{
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}
	}

	Vector3 normal = n - closest;
	float d = normal.Length();
	B->aabb.SetRadius(B->scale.x);
	float r = B->aabb.GetRadius();

	//the d is not inside
	if (d > r && !inside)
		return false;

	if (inside)
	{
		m->normal = -n;
		m->penetration = r - d;
	}
	else
	{
		m->normal = n;
		m->penetration = r - d;
	}
	return true;
}
