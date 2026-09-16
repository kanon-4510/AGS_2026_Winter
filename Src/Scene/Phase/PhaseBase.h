#pragma once
class PhaseBase
{
public:
	PhaseBase() = default;
	virtual ~PhaseBase() = default;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
};

