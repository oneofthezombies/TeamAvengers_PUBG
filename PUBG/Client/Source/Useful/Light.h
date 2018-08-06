#pragma once

class Transform;

struct Light
{
    class Manager : public Singleton<Manager>
    {
    private:
        D3DXVECTOR3 m_positionInTargetSpace;
        D3DXMATRIX  m_viewMatrix;
        D3DXMATRIX  m_projectionMatrix;
        Transform*  pTarget;
        std::vector<D3DXVECTOR3> m_projectVertices;
        D3DXVECTOR3 m_positionFactor;

    private:
        Manager();
        virtual ~Manager();

    public:
        void SetMatrices();
        void SetPositionInTargetSpace(const D3DXVECTOR3& pos);
        void SetTarget(Transform* pTarget);
        void Render();

        const D3DXVECTOR3 GetPosition()         const;
        const D3DXMATRIX& GetViewMatrix()       const;
        const D3DXMATRIX& GetProjectionMatrix() const;

        friend Singleton<Manager>;
    };

    Manager* operator()();
};

