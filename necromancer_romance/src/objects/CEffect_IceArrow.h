#ifndef _CEFFECT_ICEARROW_H_
#define _CEFFECT_ICEARROW_H_

#include "CEffect.h"

class CSound;

class CEffect_IceArrow : public CEffect
{
private:
	Qusy::Position m_effectPos;

	CBitmap* m_effectSprite;
	CTextureShader* m_effectShader;

	CObject_Num* m_num;

	float m_sustainmentTime;
	CTime* m_effectTimer;
	CTime* m_hitTimer;

	CSound* m_sound;

	int m_attackNum;
	float m_attackTiming;
	int m_damage;

	bool m_bText;
	bool m_bAttack;
public:
	CEffect_IceArrow();
	CEffect_IceArrow(const CEffect_IceArrow&);
	~CEffect_IceArrow();

	virtual void Initialize();
	virtual void Shutdown();

	virtual void Reset();
	virtual void SetDamage(int num, int totalDamage);
	virtual bool GetAlive();

	virtual void Update();
	virtual void Render();
};

#endif