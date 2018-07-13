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

    private:
        Manager();
        virtual ~Manager();

    public:
        void SetMatrices();
        void SetPositionInTargetSpace(const D3DXVECTOR3& pos);
        void SetTarget(Transform* pTarget);

        const D3DXVECTOR3 GetPosition()         const;
        const D3DXMATRIX& GetViewMatrix()       const;
        const D3DXMATRIX& GetProjectionMatrix() const;

        friend Singleton<Manager>;
    };

    Manager* operator()();
};

