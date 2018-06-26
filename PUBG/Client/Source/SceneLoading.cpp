#include "stdafx.h"
#include "SceneLoading.h"
#include "Church.h"
#include "ResPathFileName.h"

SceneLoading::SceneLoading()
    : IScene()
{
}

SceneLoading::~SceneLoading()
{
}

void SceneLoading::OnInit()
{
    //Resource()()->LoadAll();

    pair<string, string> p = ResPathFileName::Get(TAG_RES_STATIC::Church);
    cout << p.first << ", " << p.second << endl;
    //const string churchPath = "./Resource_Temp/Church/";
    //const string churchFilename = "Church.x";
    const string churchPath = p.first;
    const string churchFilename = p.second;

    //ResourceContainer* result = OnLoadEffectMeshAsync(
    //    churchPath, churchFilename);
    //if (result == nullptr)
    //{
    //    cout << "no...\n";
    //}
    //else
    //{
    //    cout << "yes\n";
    //}
    m_start = std::chrono::system_clock::now();
    m_isFinished = false;
    m_future = std::async(std::launch::async, &OnLoadEffectMeshAsync, 
        churchPath, churchFilename);

    //std::future_status futureStatus;
    //do
    //{
    //    futureStatus = future.wait_for(std::chrono::milliseconds(1000));
    //    if (futureStatus == std::future_status::deferred)
    //    {
    //        cout << "deferred\n";
    //    }
    //    else if (futureStatus == std::future_status::timeout)
    //    {
    //        cout << "timeout\n";
    //    }
    //    else if (futureStatus == std::future_status::ready)
    //    {
    //        cout << "ready\n";
    //    }
    //} while (futureStatus != std::future_status::ready);
}

void SceneLoading::OnUpdate()
{
    // repeat sound
    // loading guage (percentage)

    Debug << "load async... ";

    if (!m_isFinished)
    {
        std::future_status futureStatus = 
            m_future.wait_for(std::chrono::milliseconds(1));
        if (futureStatus == std::future_status::deferred)
        {
            Debug << "deferred\n";
        }
        else if (futureStatus == std::future_status::timeout)
        {
            Debug << "timeout\n";
        }
        else if (futureStatus == std::future_status::ready)
        {
            Debug << "ready\n";
            m_finish = std::chrono::system_clock::now();
            m_isFinished = true;
            m_elapsed = m_finish - m_start;
            Resource()()->AddResource(m_future.get());
            AddObject(new Church);
        }
    }

    if (m_isFinished)
    {
        Debug << "elapsed time : " << m_elapsed.count();
    }
}
