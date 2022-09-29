#include "memory.h"

#include<thread>

using namespace std;

namespace offset
{
	constexpr auto dwLocalPlayer = 0xDC14CC;
	constexpr auto dwForceAttack = 0x320DE5C;
	constexpr auto dwEntityList = 0x4DDD93C;
	constexpr auto m_iHealth = 0x100;
	constexpr auto m_iTeamNum = 0xF4;
	constexpr auto m_iCrosshairId = 0x11838;
}

int main()
{
	auto mem = Memory("csgo.exe");

	const auto cli = mem.GetModuleAddress("client.dll");

	cout << hex << "client.dll -> 0x" << cli << dec << endl;

	while (true)
	{
		this_thread::sleep_for(chrono::milliseconds(1));

		if (!GetAsyncKeyState(VK_SHIFT))
			continue;

		const auto localPlayer = mem.Read<uintptr_t>(cli + offset::dwLocalPlayer);
		const auto localHealth = mem.Read<int32_t>(localPlayer + offset::m_iHealth);

		if (!localPlayer)
			continue;

		const auto crosshairId = mem.Read<int32_t>(localPlayer + offset::m_iCrosshairId);

		if (!crosshairId || crosshairId > 64)
			continue;

		const auto player = mem.Read<uintptr_t>(cli + offset::dwEntityList + (crosshairId - 1) * 0x10);

		if (!mem.Read<int32_t>(player + offset::m_iHealth))
			continue;

		if (mem.Read<int32_t>(player + offset::m_iTeamNum) == mem.Read<int32_t>(localPlayer + offset::m_iTeamNum))
			continue;

		mem.Write<uintptr_t>(cli + offset::dwForceAttack, 6);
		this_thread::sleep_for(chrono::milliseconds(20));
		mem.Write<uintptr_t>(cli + offset::dwForceAttack, 4);
		
	}

	return 0;
}