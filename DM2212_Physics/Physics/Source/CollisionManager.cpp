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
	case GameObject::GO_BALL:
	{
		Vector3 u1 = go->vel;
		Vector3 u2 = go2->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
		go->vel = u1 + 2.f * (u2N - u1N);
		go2->vel = u2 + 2.f * (u1N - u2N);		

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

		break;
	}
	case GameObject::GO_EXPLOSION:
	{
		Vector3 vel = (go->pos - go2->pos).Normalized();
		float magnitude_of_explosion = (1.f - go2->scale.x / go2->m_fMaxScale) * 30.f;
		vel *= magnitude_of_explosion;
		
		go->vel += vel;

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
		if (go2->toolproj == GameObject::TOOL_PROJ::DRILLPROJ)
			break;

		if (go->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
		{
			Vector3 vel = go2->vel;
			Vector3 normal = Vector3(0, 1, 0);
			Vector3 N = normal.Normalized();
			go2->vel.SetZero();
			go2->active = false;
			break;
		}

		Vector3 u1 = go->vel;
		Vector3 u2 = go2->vel;
		Vector3 N = (go2->pos - go->pos).Normalize();
		Vector3 u1N = u1.Dot(N) * N;
		Vector3 u2N = u2.Dot(N) * N;
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

		PositionalCorrection(go, go2);

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
