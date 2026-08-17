#ifndef _OBJECTHAZARD_H
#define _OBJECTHAZARD_H

class OMO_OrbitHazard : public IO_OverMapObject
{
	public:
		OMO_OrbitHazard(gfxSprite *nspr, short x, short y, float radius, float vel, float angle, short iNumSpr, short aniSpeed, short iCollisionWidth, short iCollisionHeight, short iCollisionOffsetX, short iCollisionOffsetY, short iAnimationOffsetX, short iAnimationOffsetY, short iAnimationHeight, short iAnimationWidth);
		~OMO_OrbitHazard() {}

		void update() override;

		bool collide(CPlayer *) override;

	private:
		void CalculatePosition();

		float dAngle, dVel, dRadius;
		float dCenterX, dCenterY;
};

class OMO_StraightPathHazard : public IO_OverMapObject
{
	public:
		OMO_StraightPathHazard(gfxSprite *nspr, short x, short y, float angle, float vel, short iNumSpr, short aniSpeed, short iCollisionWidth, short iCollisionHeight, short iCollisionOffsetX, short iCollisionOffsetY, short iAnimationOffsetX, short iAnimationOffsetY, short iAnimationHeight, short iAnimationWidth);
		~OMO_StraightPathHazard() {}

		void update() override;

		bool collide(CPlayer *) override;

	private:

		float dAngle, dVel;
};

class MO_BulletBill : public IO_MovingObject
{
	public:
		MO_BulletBill(gfxSprite *nspr, gfxSprite *nsprdead, short x, short y, float nspeed, short playerID, bool isspawned);
		~MO_BulletBill(){};

		void update() override;
		void draw() override;
		void draw(short iOffsetX, short iOffsetY);
		bool collide(CPlayer * player) override;
		void collide(IO_MovingObject * object) override;

		bool hittop(CPlayer * player);
		bool hitother(CPlayer * player);
		
		void Die() override;
		void SetDirectionOffset();

	private:
		gfxSprite * spr_dead;

		short iColorID;
		
		short iColorOffsetY;
		short iDirectionOffsetY;

		bool fIsSpawned;
		short iHiddenDirection;
		short iHiddenPlane;

	friend class MO_Podobo;
};

class IO_BulletBillCannon : public CObject
{
	public:
		IO_BulletBillCannon(short x, short y, short freq, float vel, bool preview);
		~IO_BulletBillCannon() {}

		void draw() override {}
		void update() override;

		bool collide(CPlayer *) override {return false;}
		void collide(IO_MovingObject *) override {}
		
	private:
		void SetNewTimer();

		short iFreq, iTimer;
		float dVel;
		bool fPreview;
};


class MO_Explosion : public IO_MovingObject
{
	public:
		MO_Explosion(gfxSprite *nspr, short x, short y, short iNumSpr, short aniSpeed, short id, short iTeamID, killstyle style);
		~MO_Explosion(){};

		void update() override;
		bool collide(CPlayer * player) override;
		
	private:
		
		short timer;
		killstyle iStyle;
};


class IO_FlameCannon : public CObject
{
	public:
		IO_FlameCannon(short x, short y, short freq, short direction);
		~IO_FlameCannon() {}

		void draw() override;
		void draw(short iOffsetX, short iOffsetY);
		void update() override;

		bool collide(CPlayer * player) override;
		void collide(IO_MovingObject *) override {}
				
	private:
		void SetNewTimer();

		short iFreq, iTimer, iCycle;
		short iFrame;

		short iDirection;

	friend class CPlayerAI;
};


class MO_PirhanaPlant : public IO_MovingObject
{
	public:
		MO_PirhanaPlant(short x, short y, short type, short freq, short direction, bool preview);
		~MO_PirhanaPlant() {}

		void draw() override;
		void draw(short iOffsetX, short iOffsetY);
		void update() override;

		bool collide(CPlayer * player) override;
		void collide(IO_MovingObject *) override;
		
		void KillPlant();

	private:
		void SetNewTimer();
		
		float GetFireballAngle();

		short iType, iDirection;
		short iFreq, iTimer;
		//short iHiddenPlane, iHiddenDirection;
		//short iSrcX, iSrcY;
		short iAnimationTimer;
		short iFrame;
		short iActionTimer;

		bool fPreview;

	friend class CPlayerAI;
};

#endif //_OBJECTHAZARD_H

