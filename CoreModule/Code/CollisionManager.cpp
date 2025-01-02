#include "CollisionManager.h"

#include <unordered_set>

#include "Collider.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(GameEngine::CollisionManager)

static const float EPSILON = 1e-6f;

GameEngine::CollisionManager::~CollisionManager()
{
	Release();
}

void GameEngine::CollisionManager::Collider_Update()
{
	for (auto& col : m_Colliders)
	{
		if (col->Get_GameObject()->Is_Active() && col->Is_Enabled())
		{
			col->Update_Collider();
		}
	}

    std::vector<std::pair<BoxCollider*, BoxCollider*>> potentialPairs;

    broadPhase_Sap(potentialPairs);

    narrowPhase_OBB(potentialPairs);
}

void GameEngine::CollisionManager::Add_Collider(Collider* _collider)
{
	m_RegisterQueue.push_back(_collider);
}

void GameEngine::CollisionManager::Remove_Collider(Collider* _collider)
{
	for (auto& col : m_DestroyQueue)
	{
		if (col == _collider)
		{
			return;
		}
	}
	m_DestroyQueue.push_back(_collider);
}

void GameEngine::CollisionManager::Register_Collider()
{
	for (auto it = m_RegisterQueue.begin(); it != m_RegisterQueue.end();)
	{
		m_Colliders.push_back(*it);
		it = m_RegisterQueue.erase(it);
	}

	m_RegisterQueue.clear();
}

void GameEngine::CollisionManager::Destroy_Collider()
{
	for (auto& col : m_DestroyQueue)
	{
		GameObjectList* objList = Scene::GetInstance().Get_GameObjectList();
		for (auto& gameObject : *objList)
		{
			if (gameObject == col->Get_GameObject())
			{
				gameObject->Remove_Component(col);
				break;
			}
		}

		delete col;
		m_Colliders.erase(std::remove(m_Colliders.begin(), m_Colliders.end(), col), m_Colliders.end());

        for (auto pairIt = m_CollisionPairs.begin(); pairIt != m_CollisionPairs.end();)
        {
            auto& p = *pairIt;
            BoxCollider* a = p.first;
            BoxCollider* b = p.second;

            if (a == col || b == col)
            {
                pairIt = m_CollisionPairs.erase(pairIt);
            }

            else
            {
	            ++pairIt;
            }
        }

	}

}

void GameEngine::CollisionManager::Release()
{
	for (const auto& col : m_Colliders)
	{
		delete col;
	}

	for (const auto& col : m_RegisterQueue)
	{
		delete col;
	}

	m_Colliders.clear();
    m_CollisionPairs.clear();
	m_RegisterQueue.clear();
	m_DestroyQueue.clear();
}

bool GameEngine::CollisionManager::RayCast(const Ray& _ray, RayHit& _outHit, const float _maxDistance, int _layerMask)
{
    {
        _outHit.Distance = 0.f;
        _outHit.Collider = nullptr;

        float 	closestT = _maxDistance;
        bool	hasHit = false;
        RayHit tempHit;

        for (auto col : m_Colliders)
        {
            // ���̾� üũ
            // TODO : ������ �̱���, ����ȭ�� �ʿ��ϰų� ����� �ʿ��� �� ������ ��
            // if (!CheckLayerMask(col->Get_Layer(), layerMask))
            //		continue;

            // Ʈ���� üũ
            // TODO : ������ Collider�� Trigger�� ����� �ϴ��� Trigger ���� ���� �� ������ ��.
            // if (col->Is_Trigger())
            //		continue;

            BoxCollider* boxCol = dynamic_cast<BoxCollider*>(col);
            if (!boxCol)
            {
	            continue;
            }

            float t;
            Vector3 normal;
            bool hit = rayIntersectsOBB(_ray, boxCol->Get_OBB(), t, normal);

            if (!hit)
                continue;

            if (t < 0.f)
                continue;

            if (t < closestT && t <= _maxDistance)
            {
                closestT = t;
                hasHit = true;

                tempHit.Distance = t;
                tempHit.Collider = col;
                tempHit.Point = _ray.Get_Point(t);
                tempHit.Normal = normal;
            }
        }

        if (hasHit)
        {
            _outHit = tempHit;
            return true;
        }

        return false;
    }
}

void GameEngine::CollisionManager::broadPhase_Sap(std::vector<std::pair<BoxCollider*, BoxCollider*>>& _outPotentialPairs)
{
    // AABB X�� ������ ����
    struct Edge
    {
        float         	value;     // x�� ��ġ
        BoxCollider* 	collider;
        bool          	isMin;     // min���� max����
    };

    std::vector<Edge> edges;
	edges.reserve(m_Colliders.size() * 2);


    for (auto& col : m_Colliders)
    {
        BoxCollider* boxCol = dynamic_cast<BoxCollider*>(col);

        Edge eMin;
        eMin.value = boxCol->Get_WorldMin().x;
        eMin.collider = boxCol;
        eMin.isMin = true;
        edges.push_back(eMin);

        Edge eMax;
        eMax.value = boxCol->Get_WorldMax().x;
        eMax.collider = boxCol;
        eMax.isMin = false;
        edges.push_back(eMax);
    }

    // X�� �������� ����
    std::sort(edges.begin(), edges.end(), 
        [](const Edge& a, const Edge& b) { return a.value < b.value;});

    // Sweep
    std::vector<BoxCollider*> activeList;
    activeList.reserve(m_Colliders.size());

    for (auto& edge : edges)
    {
        if (edge.isMin)
        {
            // activeList�� �ִ� ��� collider�� ���� �浹 ��
            for (auto& active : activeList)
            {
                // �ߺ� ������: (A,B) / (B,A)�� ���� (�ּ� ��)
                BoxCollider* c1 = (active < edge.collider) ? active : edge.collider;
                BoxCollider* c2 = (active < edge.collider) ? edge.collider : active;

                // broad-phase ��, x�� ��ġ�� �ϴ� �ĺ�
                // ��� ���⼭ x�ุ ���� ��ġ�� push_back
                // y,z�� NarrowPhase���� ������ �Ǵ�
                _outPotentialPairs.emplace_back(c1, c2);
            }
            // �ڱ� �ڽŵ� active�� �߰�
            activeList.push_back(edge.collider);
        }
        else
        {
            // max ���� -> activeList���� ����
            auto it = std::find(activeList.begin(), activeList.end(), edge.collider);
            if (it != activeList.end())
            {
                activeList.erase(it);
            }
        }
    }

    // �ߺ� ���� ����: (A,B)�� ���� �� ���� ���ɼ� ����
	// ���� sort
    std::sort(_outPotentialPairs.begin(), _outPotentialPairs.end(),
        [](const std::pair<BoxCollider*, BoxCollider*>& lhs,
            const std::pair<BoxCollider*, BoxCollider*>& rhs)
        {
            // first ������ �ּ� �� -> second ������ �ּ� ��
            if (lhs.first < rhs.first) return true;
            if (lhs.first > rhs.first) return false;
            return (lhs.second < rhs.second);
        }
    );

    // unique�� ���� �ߺ� ����
    auto last = std::unique(_outPotentialPairs.begin(), _outPotentialPairs.end());
    _outPotentialPairs.erase(last, _outPotentialPairs.end());
}

void GameEngine::CollisionManager::narrowPhase_OBB(
	const std::vector<std::pair<BoxCollider*, BoxCollider*>>& _potentialPairs)
{
    // �̹� �����ӿ� �浹�� ��
    std::unordered_set<std::pair<BoxCollider*, BoxCollider*>, ColliderPairHash, ColliderPairEq> newCollisions;
    newCollisions.reserve(_potentialPairs.size());

    for (auto& pair : _potentialPairs)
    {
        BoxCollider* a = pair.first;
        BoxCollider* b = pair.second;

        Vector3 aMin = a->Get_WorldMin();
        Vector3 aMax = a->Get_WorldMax();
        Vector3 bMin = b->Get_WorldMin();
        Vector3 bMax = b->Get_WorldMax();

        // 1) AABB ���� üũ (x, y, z ��� ��ġ����)
        bool overlapX = (aMin.x <= bMax.x) && (aMax.x >= bMin.x);
        bool overlapY = (aMin.y <= bMax.y) && (aMax.y >= bMin.y);
        bool overlapZ = (aMin.z <= bMax.z) && (aMax.z >= bMin.z);
        if (!(overlapX && overlapY && overlapZ))
            continue;

        // 2) OBB �浹 �˻� (ȸ�� ���, ���⼱ ����/����)
        if (check_OBBCollision(a, b))
        {
            // �浹 �߻�
            newCollisions.insert({ a,b });
        }
    }

    process_CollisionResults(newCollisions);

    m_CollisionPairs = std::move(newCollisions);
}

float GameEngine::CollisionManager::get_OBBRadiusOnAxis(const BoxCollider* _box, const Vector3& _axis)
{
    float r = 0.0f;

    Vector3 obbAxes[3] = 
    {
    	_box->Get_OBB().AxisX,
    	_box->Get_OBB().AxisY,
    	_box->Get_OBB().AxisZ
    };

    for (int i = 0; i < 3; ++i)
    {
        float dotVal = fabs(D3DXVec3Dot(&_axis, &obbAxes[i]));

        float e = 0.0f;
        if (i == 0) e = _box->Get_OBB().Extents.x;
        else if (i == 1) e = _box->Get_OBB().Extents.y;
        else                e = _box->Get_OBB().Extents.z;

        r += dotVal * e;
    }
    return r;
}

bool GameEngine::CollisionManager::is_AxisSeparating(const BoxCollider* _a, const BoxCollider* _b, const Vector3& _axis)
{
    float len2 = D3DXVec3LengthSq(&_axis);
    if (len2 < EPSILON)
        return false;

    Vector3 normAxis;
    D3DXVec3Normalize(&normAxis, &_axis);

    // �� OBB�� �� ��(normAxis)�� ������ ���� �ݰ� ��
    float rA = get_OBBRadiusOnAxis(_a, normAxis);
    float rB = get_OBBRadiusOnAxis(_b, normAxis);
    float rSum = rA + rB;

    Vector3 T = _b->Get_OBB().Center - _a->Get_OBB().Center;
    float dist = fabs(D3DXVec3Dot(&T, &normAxis));

    return dist > rSum;
}

bool GameEngine::CollisionManager::check_OBBCollision(const BoxCollider* _a, const BoxCollider* _b)
{
    Vector3 aAxes[3] = 
    {
    	_a->Get_OBB().AxisX,
    	_a->Get_OBB().AxisY,
    	_a->Get_OBB().AxisZ
    };

    Vector3 bAxes[3] =
    {
        _b->Get_OBB().AxisX,
        _b->Get_OBB().AxisY,
        _b->Get_OBB().AxisZ
    };

    // A�� 3��
    for (auto aAxe : aAxes)
    {
        if (is_AxisSeparating(_a, _b, aAxe))
            return false; // �и�
    }

    // B�� 3��
    for (auto bAxe : bAxes)
    {
        if (is_AxisSeparating(_a, _b, bAxe))
            return false;
    }

    // ������ 3��3=9��
    for (auto& aAxe : aAxes)
    {
        for (auto& bAxe : bAxes)
        {
            // cross
            Vector3 axis;
            D3DXVec3Cross(&axis, &aAxe, &bAxe);

            if (is_AxisSeparating(_a, _b, axis))
                return false;
        }
    }

    // ������� �и����� �ϳ��� �߰ߵ��� �ʾҴٸ� -> �浹
    return true;
}

void GameEngine::CollisionManager::process_CollisionResults(
	const std::unordered_set<std::pair<BoxCollider*, BoxCollider*>, ColliderPairHash, ColliderPairEq>& newCollisions)
{
    for (auto& c : newCollisions)
    {
	    if (m_CollisionPairs.find(c) == m_CollisionPairs.end())
	    {
		    // enter
            invoke_CollisionEnter(c.first, c.second);
	    }
        else
        {
            invoke_CollisionStay(c.first, c.second);
        }
    }

    for (auto& c : m_CollisionPairs)
    {
	    if (newCollisions.find(c) == newCollisions.end())
	    {
            invoke_CollisionExit(c.first, c.second);
	    }
    }
}

void GameEngine::CollisionManager::invoke_CollisionEnter(BoxCollider* _a, BoxCollider* _b)
{
    GameObject* objA = _a->Get_GameObject();
    GameObject* objB = _b->Get_GameObject();

    objA->On_CollisionEnter(Collision(objB, _b));
    objB->On_CollisionEnter(Collision(objA, _a));
}

void GameEngine::CollisionManager::invoke_CollisionStay(BoxCollider* _a, BoxCollider* _b)
{
    GameObject* objA = _a->Get_GameObject();
    GameObject* objB = _b->Get_GameObject();

    objA->On_CollisionStay(Collision(objB, _b));
    objB->On_CollisionStay(Collision(objA, _a));
}

void GameEngine::CollisionManager::invoke_CollisionExit(BoxCollider* _a, BoxCollider* _b)
{
    GameObject* objA = _a->Get_GameObject();
    GameObject* objB = _b->Get_GameObject();

    objA->On_CollisionExit(Collision(objB, _b));
    objB->On_CollisionExit(Collision(objA, _a));
}

bool GameEngine::CollisionManager::rayIntersectsOBB(const Ray& _ray, const OBB& _obb, float& _outT, Vector3& outNormal)
{
    Vector3 diff = _ray.Origin - _obb.Center;

    float ox = Vector3::Dot(diff, _obb.AxisX);
    float oy = Vector3::Dot(diff, _obb.AxisY);
    float oz = Vector3::Dot(diff, _obb.AxisZ);

    float dx = Vector3::Dot(_ray.Direction, _obb.AxisX);
    float dy = Vector3::Dot(_ray.Direction, _obb.AxisY);
    float dz = Vector3::Dot(_ray.Direction, _obb.AxisZ);

    Vector3 boxMin(-_obb.Extents.x, -_obb.Extents.y, -_obb.Extents.z);
    Vector3 boxMax(_obb.Extents.x, _obb.Extents.y, _obb.Extents.z);

    float tmin = 0.0f;
    float tmax = FLT_MAX;

    auto checkAxis = [&](float originCoord, float dirCoord, float minB, float maxB)->bool
        {
            if (fabsf(dirCoord) < 1e-6f)
            {
                // ����
                if (originCoord < minB || originCoord > maxB)
                    return false;
            }
            else
            {
                float invD = 1.f / dirCoord;
                float t1 = (minB - originCoord) * invD;
                float t2 = (maxB - originCoord) * invD;
                if (t1 > t2) std::swap(t1, t2);

                if (t1 > tmin) tmin = t1;
                if (t2 < tmax) tmax = t2;
                if (tmin > tmax) return false;
            }
            return true;
        };

    if (!checkAxis(ox, dx, boxMin.x, boxMax.x)) return false;
    if (!checkAxis(oy, dy, boxMin.y, boxMax.y)) return false;
    if (!checkAxis(oz, dz, boxMin.z, boxMax.z)) return false;

    if (tmin < 0.f)
    {
        // �ڽ� ���ο��� ����
        if (tmax < 0.f) return false;
        _outT = tmax;
    }
    else
    {
        _outT = tmin;
    }

    outNormal = Vector3(0, 0, 0);

    return true;
}
