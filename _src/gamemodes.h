#ifndef _GAMEMODES_H
#define _GAMEMODES_H

#define GAMEMODE_NUM_OPTIONS 21

//Enum for each game mode
enum GameModeType{game_mode_classic, game_mode_frag, game_mode_timelimit, game_mode_jail, game_mode_coins, game_mode_stomp, game_mode_eggs, game_mode_ctf, game_mode_chicken, game_mode_tag, game_mode_star, game_mode_domination, game_mode_koth, game_mode_race, game_mode_owned, game_mode_frenzy, game_mode_survival, game_mode_greed, game_mode_health, game_mode_collection, game_mode_chase, game_mode_shyguytag, GAMEMODE_LAST, game_mode_bonus = 999, game_mode_pipe_minigame = 1000, game_mode_boss_minigame = 1001, game_mode_boxes_minigame = 1002};
enum PlayerKillType{player_kill_none, player_kill_normal, player_kill_removed, player_kill_nonkill};

struct SModeOption
{
	char szName[64];
	short iValue;
};

struct TourStop;
class CO_PhantoKey;

class GameTimerDisplay
{
	public:
		GameTimerDisplay();
		~GameTimerDisplay() {}

		void Init(short iTime, bool fCountDown);
		short RunClock();
		void Draw();
		void SetTime(short iTime);
		void AddTime(short iTime);

	protected:
		void SetDigitCounters();

		short timeleft;
		bool countdown;

		short framesleft_persecond;
		short iDigitLeftSrcX;
		short iDigitMiddleSrcX;
		short iDigitRightSrcX;
		short iDigitLeftDstX;
		short iDigitMiddleDstX;
		short iDigitRightDstX;
		short iScoreOffsetX;

		short iFramesPerSecond;
};

//gamemode base class
class CGameMode
{
	public:
		
		CGameMode();
        virtual ~CGameMode();
		
		virtual void init();  //called once when the game is started
		virtual void think();	//called once a frame
		virtual void draw_background() {}
		virtual void draw_foreground() {}
		//called when a player stomps another player, after the p2p logic has run
		//returns true if the other was deleted
		virtual short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style);
		virtual short playerkilledself(CPlayer &player, killstyle style);
		virtual void playerextraguy(CPlayer &player, short iType);

		virtual short CheckWinner(CPlayer *) {return player_kill_normal;}

		virtual bool HasStoredPowerups() const {return true;}

		void transferbobombifneeded(CPlayer &inflictor, CPlayer &other);
		void displayplayertext();
		void playwarningsound();

		short winningteam;
		bool gameover;

		CPlayer *chicken;
		CPlayer *tagged;
		CPlayer *frenzyowner;

		GameModeType getgamemode() const {return gamemode;}
		GameModeType gamemode;

		char * GetModeName() {return szModeName;}
		char * GetGoalName() {return szGoalName;}
		SModeOption * GetOptions() {return modeOptions;}
		
		bool playedwarningsound;
		short goal;

		short GetClosestGoal(short iGoal) const;
		bool GetReverseScoring() const {return fReverseScoring;}

#ifdef _DEBUG
		virtual void setdebuggoal() {};
#endif
		
	protected:
		char szModeName[64];
		char szGoalName[64];
		SModeOption modeOptions[GAMEMODE_NUM_OPTIONS];

		void SetupModeStrings(const char * szMode, const char * szGoal, short iGoalSpacing);

		CPlayer * GetHighestScorePlayer(bool fGetHighest);
		short GetScoreRankedPlayerList(CPlayer * players[4], bool fGetHighest);

		bool fReverseScoring;
};



//Fraglimit
class CGM_Frag : public CGameMode
{
	public:
        CGM_Frag();
		virtual ~CGM_Frag() {}

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		short CheckWinner(CPlayer * player) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 5;}
#endif
		
};


//Timelimit
class CGM_TimeLimit : public CGameMode
{
	public:
        CGM_TimeLimit();
		virtual ~CGM_TimeLimit() {}

		void init() override;
		void think() override;
		void draw_foreground() override;
		void drawtime();
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;

		void addtime(short iTime);

#ifdef _DEBUG
		void setdebuggoal() override {goal = 30;}
#endif

	protected:
		GameTimerDisplay gameClock;

};


//mariowar classic
class CGM_Classic : public CGameMode
{
	public:
        CGM_Classic();
		virtual ~CGM_Classic() {}

		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 5;}
#endif

};


//capture the chicken
class CGM_Chicken : public CGameMode
{
	public:
        CGM_Chicken();
		virtual ~CGM_Chicken() {}

		void think() override;
		void draw_foreground() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;
		short CheckWinner(CPlayer * player) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 100;}
#endif

};

class CGM_Tag : public CGameMode
{
	public:
        CGM_Tag();
		virtual ~CGM_Tag() {}

		void init() override;
		void think() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 100;}
#endif

};

class CGM_ShyGuyTag : public CGameMode
{
	public:
        CGM_ShyGuyTag();
		virtual ~CGM_ShyGuyTag() {}

		void init() override;
		void think() override;
		void draw_foreground() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		void SetShyGuy(short iTeam);

#ifdef _DEBUG
		void setdebuggoal() override {goal = 100;}
#endif

	private:

		short CheckWinner(CPlayer * player) override;
		short CountShyGuys();
		void FreeShyGuys();

		GameTimerDisplay gameClock;
		bool fRunClock;
		short scorecounter;
};

class CGM_Coins : public CGameMode
{
		public:
        CGM_Coins();
		virtual ~CGM_Coins() {}

		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		short CheckWinner(CPlayer * player) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 5;}
#endif

};

class CGM_Eggs : public CGameMode
{
	public:
        CGM_Eggs();
		virtual ~CGM_Eggs() {}

		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;
		short CheckWinner(CPlayer * player) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 5;}
#endif

};

//Just like frag limit, but firepower cards appear randomly
class CGM_Frenzy : public CGM_Frag
{
	public:
        CGM_Frenzy();
		virtual ~CGM_Frenzy() {}

		void init() override;
		void think() override;
		char *getMenuString(char *buffer64);

	private:
		short timer;
		short iSelectedPowerup;
		short iItemWeightCount;
};

class CGM_Survival : public CGM_Classic
{
	public:
        CGM_Survival();
		virtual ~CGM_Survival() {}

		void init() override;
		void think() override;
		char *getMenuString(char *buffer64);

	protected:
		short timer;
		short ratetimer;
		short rate;
		short iSelectedEnemy;
		short iEnemyWeightCount;
};


//Domination (capture the area blocks)
class CGM_Domination : public CGameMode
{
	public:
        CGM_Domination();
		virtual ~CGM_Domination() {}

		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;
		short CheckWinner(CPlayer * player) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 100;}
#endif
};

//Similar to frag limit, but players get bonus frags for the number of players they have "owned"
class CGM_Owned : public CGameMode
{
	public:
        CGM_Owned();
		virtual ~CGM_Owned() {}

		void think() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 100;}
#endif

	private:
		short CheckWinner(CPlayer * player) override;

};

//Similar to frag limit but:
//When a player is killed by another player, they get a "jail" marker
//Jailed players move slowly
//Jailed players can be freed by teammates
//If all players on a team are jailed, bonus kill goes to other team
//Similar to frag limit, but players get bonus frags for the number of players they have "owned"
class CGM_Jail : public CGM_Frag
{
	public:
        CGM_Jail();
		virtual ~CGM_Jail() {}

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;
		char *getMenuString(char *buffer64);
};

//Similar to coin mode but you have to smash the most goombas/cheeps/koopas
class CGM_Stomp : public CGameMode
{
	public:
        CGM_Stomp();
		virtual ~CGM_Stomp() {}

		void init() override;
		void think() override;

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		char *getMenuString(char *buffer64);

	private:
		void ResetSpawnTimer();
		short CheckWinner(CPlayer * player) override;

		short spawntimer;
		short iSelectedEnemy;
		short iEnemyWeightCount;
};

//Players tag flying targets in order to complete laps
class CGM_Race : public CGameMode
{
	public:
        virtual ~CGM_Race() {}
		CGM_Race();
		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		short getNextGoal(short teamID) const {return nextGoal[teamID];}
		void setNextGoal(short teamID);

#ifdef _DEBUG
		void setdebuggoal() override {goal = 5;}
#endif

	protected:
		void PenalizeRaceGoals(CPlayer &player);
		short nextGoal[4];
		short quantity;
		short penalty;
};


//Star mode - shared timer ticks down and the players must pass around
//a hot potato.  When the timer hits zero, the player with the star
//loses a point.
class CGM_Star : public CGM_TimeLimit
{
	public:
        CGM_Star();
		virtual ~CGM_Star() {}

		void init() override;
		void think() override;
		void draw_foreground() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		bool isplayerstar(CPlayer * player);
		CPlayer * swapplayer(short id, CPlayer * player);
		CPlayer * getstarplayer(short id) {return starPlayer[id];}

		short getcurrentmodetype() const {return iCurrentModeType;}

#ifdef _DEBUG
		void setdebuggoal() override {goal = 1;}
#endif

	private:
		void SetupMode();

		CO_Star * starItem[3];
		CPlayer * starPlayer[3];
		short iCurrentModeType;
		bool fDisplayTimer;
};

//Capture The Flag mode - each team has a base and a flag
//Protect your colored flag from being taken and score a point
//for stealing another teams flag and returning it to your base
class CGM_CaptureTheFlag : public CGameMode
{
	public:
        CGM_CaptureTheFlag();
		virtual ~CGM_CaptureTheFlag() {}

		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;
		short CheckWinner(CPlayer * player) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 5;}
#endif
};


//Domination (capture the area blocks)
class CGM_KingOfTheHill : public CGM_Domination
{
	public:
        CGM_KingOfTheHill();
		virtual ~CGM_KingOfTheHill() {}

		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 100;}
#endif
};

//Greed mode (players try to steal each other's coins)
class CGM_Greed : public CGM_Classic
{
	public:
        CGM_Greed();
		virtual ~CGM_Greed() {}

		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		short ReleaseCoins(CPlayer &player, killstyle style);

#ifdef _DEBUG
		void setdebuggoal() override {goal = 10;}
#endif

};

//health mode
class CGM_Health : public CGM_Classic
{
	public:
        CGM_Health();
		virtual ~CGM_Health() {}

		void init() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 5;}
#endif

};

//Collection (Collect cards for points)
class CGM_Collection : public CGameMode
{
	public:
        CGM_Collection();
		virtual ~CGM_Collection() {}

		void init() override;
		void think() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		void ReleaseCard(CPlayer &player);

		short CheckWinner(CPlayer * player) override;

#ifdef _DEBUG
		void setdebuggoal() override {goal = 5;}
#endif

	private:
		short timer;
};


//Chase (player is chased by phanto)
class CGM_Chase : public CGameMode
{
	public:
        CGM_Chase();
		virtual ~CGM_Chase() {}

		void init() override;
		void think() override;
		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		short CheckWinner(CPlayer * player) override;

		CPlayer * GetKeyHolder();

#ifdef _DEBUG
		void setdebuggoal() override {goal = 50;}
#endif

	private:
		CO_PhantoKey * key;
};


//Special mode where players try to kill a boss
class CGM_Boss_MiniGame : public CGameMode
{
	public:
        CGM_Boss_MiniGame();
		virtual ~CGM_Boss_MiniGame() {}

		void init() override;
		void think() override;
		void draw_foreground() override;

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		char *getMenuString(char *buffer64);

		bool SetWinner(CPlayer * player);
		void SetBossType(short bosstype);
		short GetBossType() const {return iBossType;}

		bool HasStoredPowerups() const override {return false;}

	private:
		
		short enemytimer, poweruptimer;
		short iBossType;
};


//Special mode where players can collect a bonus item
class CGM_Bonus : public CGameMode
{
	public:
        CGM_Bonus();
		virtual ~CGM_Bonus() {}

		void init() override;
		void draw_background() override;

		//Override so it doesn't display winner text after you choose a powerup
		void think() override {}

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override {return false;}
		short playerkilledself(CPlayer &player, killstyle style) override {return false;}
		void playerextraguy(CPlayer &player, short iType) override {}

		bool HasStoredPowerups() const override {return false;}

	private:

		TourStop * tsTourStop;
};

//Special mode where players collect coins from a pipe
class CGM_Pipe_MiniGame : public CGameMode
{
	public:
        CGM_Pipe_MiniGame();
		virtual ~CGM_Pipe_MiniGame() {}

		void init() override;
		void think() override;

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;
		short CheckWinner(CPlayer * player) override;

		void SetBonus(short iType, short iTimer, short iTeamID);
		bool IsSlowdown() const {return fSlowdown;}

		bool HasStoredPowerups() const override {return false;}

	private:

		short iNextItemTimer;
		short iBonusTimer, iBonusType, iBonusTeam;

		bool fSlowdown;
};


//Special mode where players break boxes
class CGM_Boxes_MiniGame : public CGameMode
{
	public:
        CGM_Boxes_MiniGame();
		virtual ~CGM_Boxes_MiniGame() {}

		void init() override;
		void think() override;

		short playerkilledplayer(CPlayer &inflictor, CPlayer &other, killstyle style) override;
		short playerkilledself(CPlayer &player, killstyle style) override;
		void playerextraguy(CPlayer &player, short iType) override;

		char *getMenuString(char *buffer64);

		short CheckWinner(CPlayer * player) override;
		bool SetWinner(CPlayer * player);

		bool HasStoredPowerups() const override {return false;}

	private:
		
		void ReleaseCoin(CPlayer &player);
		void ReleaseAllCoinsFromTeam(CPlayer &player);
};

#endif

