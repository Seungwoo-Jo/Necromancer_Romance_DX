#include "..\framework\CBitmap.h"
#include "..\framework\CTextureShader.h"
#include "..\framework\CTime.h"
#include "..\framework\CSound.h"

#include "CMonster.h"
#include "..\managers\CBattleManager.h"
#include "..\managers\CNero.h"

#include "CCard.h"
#include "CObject_Num.h"
#include "CEffect_Claw.h"

CEffect_Claw::CEffect_Claw()
{
	m_effectSprite		= nullptr;
	m_effectShader		= nullptr;
	m_num				= nullptr;
	m_sustainmentTime	= 0.0f;
	m_effectTimer		= nullptr;
	m_hitTimer			= nullptr;
	m_sound				= nullptr;
	m_attackNum			= 0;
	m_attackTiming		= 0.0f;
	m_target			= 0;
	m_bText				= false;
	m_bAttack			= false;
}

CEffect_Claw::CEffect_Claw(const CEffect_Claw& eft)
{
}

CEffect_Claw::~CEffect_Claw()
{
}

void CEffect_Claw::Initialize()
{
	m_effectPos.setPosition(0, 0, 0);
	m_sustainmentTime = 420;
	m_attackNum = 1;
	m_attackTiming = 0.10f;
	m_damage = 0;

	m_effectSprite = new CBitmap();
	m_effectSprite->Initialize(L"res/effect/claw.png", 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	m_effectSprite->SetSpriteAnimation(5, 2, m_sustainmentTime, false);

	m_effectShader = new CTextureShader();
	m_effectShader->Initialize(NULL, NULL);

	m_num = new CObject_Num();
	m_num->Initialize();

	m_effectTimer = new CTime();
	m_hitTimer = new CTime();

	m_sound = new CSound();
	m_sound->Initialize("res/sound/se/monster_attack.wav");
	m_sound->Loop(false);
}

void CEffect_Claw::Shutdown()
{
	if(m_effectSprite != nullptr) {
		m_effectSprite->Shutdown();
		SAFE_DELETE(m_effectSprite);
	}
	if(m_effectShader != nullptr) {
		m_effectShader->Shutdown();
		SAFE_DELETE(m_effectShader);
	}
	if(m_num != nullptr) {
		m_num->Shutdown();
		SAFE_DELETE(m_num);
	}
	if(m_effectTimer != nullptr) {
		SAFE_DELETE(m_effectTimer);
	}
	if(m_hitTimer != nullptr) {
		SAFE_DELETE(m_effectTimer);
	}
	if(m_sound != nullptr)
	{
		m_sound->Release();
		SAFE_DELETE(m_sound);
	}
}

void CEffect_Claw::Reset()
{
	m_attackNum = 1;
	m_effectSprite->AnimateReset();
	m_effectTimer->SetTimer(static_cast<float>(m_sustainmentTime)*0.001f, false);
	m_num->setPosition(548 + (rand()%30) -15, 175 + (rand()%10) -5, m_effectPos.posZ);

	CMonster* tempMonster = CBattleManager::getBattleManager()->GetMonster();
	tempMonster->setHit(false);
	m_bText = false;
}

void CEffect_Claw::SetDamage(int num, int totalDamage)
{
	m_target = num;
	m_attackNum = 1;
	m_damage = static_cast<int>(totalDamage);
}

bool CEffect_Claw::GetAlive()
{
	return !m_effectTimer->GetTimer();
}

void CEffect_Claw::Update()
{
	if((static_cast<float>(m_sustainmentTime)*0.001f - m_effectTimer->RemainTime()) > m_attackTiming)
	{
		if(m_attackNum >= 1) {
			m_bAttack = true;
			m_attackNum--;
		}
	}
	
	CMonster* tempMonster = CBattleManager::getBattleManager()->GetMonster();
	CCard* tempCard = CNero::getNero()->GetPartyCardWithSlot(m_target);

	m_effectPos.setPosition(261 + (200.5f*m_target), 470, 10);

	m_num->SetNum(m_damage);
	
	if(m_bAttack)
	{
		m_bText = true;
		m_bAttack = false;

		tempCard->setHp(tempCard->getStat().m_hp - m_damage);
		if(m_damage > 0)
		{
			m_hitTimer->SetTimer(0.35f, false);
		}
	}

	if(m_hitTimer->RemainTime() >= 0.28f) {
		m_sound->Play();
		if(tempCard != NULL) {
			tempCard->setHit(true);
		}
	}
	else {
		m_sound->Reset();
		if(tempCard != NULL) {
			tempCard->setHit(false);
		}
	}
}

void CEffect_Claw::Render()
{

	m_effectSprite->Render(m_effectPos.posX-18, m_effectPos.posY, m_effectPos.posZ, 1.0f, 1.0f);
	m_effectShader->Render(m_effectSprite->GetTexture(), 1.0f);

	if(m_bText)
	{
		float opacity = 1.0f;
		if(m_hitTimer->RemainTime() <= (m_hitTimer->GetSetTime() - 0.25f)) {
			opacity = m_hitTimer->RemainTime() / (m_hitTimer->GetSetTime() - 0.25f);
		}
		m_num->setPosition(m_effectPos.posX, m_effectPos.posY - 60, m_effectPos.posZ);
		m_num->setOpacity(opacity);
		m_num->Render();

		if(opacity <= 0.0f)
		{
			m_num->SetNum(0);
			m_bText = false;
		}
	}
}