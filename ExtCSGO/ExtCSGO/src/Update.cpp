#include "Update.h"

namespace ExtCSGO
{
	static void UpdateThread(Update* update);
	Update::Update() :
		m_Settings(new Settings()),
		m_Engine(new Engine(m_Settings)),
		m_Enabled(false)
	{
	}

	Update::~Update()
	{
		delete m_Engine;
		delete m_Settings;
	}

	Settings * Update::GetSettings() const
	{
		return m_Settings;
	}

	Engine * Update::GetEngine() const
	{
		return m_Engine;
	}

	bool Update::IsEnabled() const
	{
		return m_Enabled;
	}

	void Update::SetEnabled(const bool & v)
	{
		m_Enabled = v;
	}

	int Update::Run()
	{
		auto hUpdate = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UpdateThread, (Update*)this, 0, 0);
		while (true)
		{
			Sleep(1);
			if (this->IsEnabled())
			{
				if (m_Engine->IsValid())
				{
					Features::Aimbot(m_Engine, m_Settings);
					Features::Triggerbot(m_Engine, m_Settings);
				}
			}
		}
		CloseHandle(hUpdate);
		return 0;
	}

	static void UpdateThread(Update* update)
	{
		while (true)
		{
			Sleep(1);
			if (update->IsEnabled())
			{ 
				if (update->GetEngine()->IsValid())
				{
					update->GetEngine()->UpdateEvents();
				}
				else
				{
					#ifdef _DEBUG
					#else
					Sleep(5000);
					#endif
					update->GetEngine()->Update();
				}
			}
			if (GetAsyncKeyState(VK_INSERT) & 1)
			{
				update->SetEnabled(!update->IsEnabled());
			}
		}
		return ExitThread(0);
	}
}