#include "roster.h"


Roster::Roster() {
    for (int i = 0; i < PLAYER_COUNT; i++) {
        offense[i] = nullptr;
        defense[i] = nullptr;
    }
    
    int lineOfScrimmage = convertToPixelYards(25);

    // OFFENSE

    // QB

    offense[0] = new Player(
        {(float)(lineOfScrimmage - convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)},
        true, Player::Position::QUARTERBACK,
        {
            .width        = 5,
            .height       = 8,
            .acceleration = 0.10f,
            .topSpeed     = 0.7f,
            .catchRadius  = 0.15f,
            .weight       = 200,
            .breakTackle  = 80,
            .tackle       = 40,
        }
    );
    offense[0]->setStatus(Player::Status::BALL_CARRIER);

    // WR

    offense[1] = new Player(
        {(float)lineOfScrimmage - 4, (float)(TOP + 20)},
        true, Player::Position::WIDE_RECEIVER,
        {
            .width        = 4,
            .height       = 9,
            .acceleration = 0.15f,
            .topSpeed     = 1.0f,
            .catchRadius  = 0.5f,
            .weight       = 180,
            .breakTackle  = 85,
            .tackle       = 50,
        }
    );
    offense[2] = new Player(
        {(float)lineOfScrimmage - 4, (float)(TOP + 40)},
        true, Player::Position::WIDE_RECEIVER,
        {
            .width        = 4,
            .height       = 9,
            .acceleration = 0.12f,
            .topSpeed     = 1.2f,
            .catchRadius  = 0.5f,
            .weight       = 210,
            .breakTackle  = 83,
            .tackle       = 50,
        }
    );
    offense[3] = new Player(
        {(float)lineOfScrimmage - 4, (float)(BOTTOM - 20)},
        true, Player::Position::WIDE_RECEIVER,
        {
            .width        = 4,
            .height       = 9,
            .acceleration = 0.15f,
            .topSpeed     = 1.1f,
            .catchRadius  = 0.5f,
            .weight       = 170,
            .breakTackle  = 93,
            .tackle       = 50,
        }
    );
    
    // DEFENSE

    defense[0] = new Player(
        {(float)(lineOfScrimmage + convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)},
        false, Player::Position::LINEBACKER,
        {
            .width        = 6,
            .height       = 7,
            .acceleration = 0.08f,
            .topSpeed     = 0.8f,
            .catchRadius  = 0.3f,
            .weight       = 230,
            .breakTackle  = 50,
            .tackle       = 85,
        }
    );

    // CBs lined up over WRs
    defense[1] = new Player(
        {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(TOP + 20)},
        false, Player::Position::CORNERBACK,
        {
            .width        = 4,
            .height       = 8,
            .acceleration = 0.14f,
            .topSpeed     = 1.0f,
            .catchRadius  = 0.3f,
            .weight       = 185,
            .breakTackle  = 55,
            .tackle       = 70,
        }
    );
    defense[2] = new Player(
        {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(TOP + 40)},
        false, Player::Position::CORNERBACK,
        {
            .width        = 4,
            .height       = 8,
            .acceleration = 0.13f,
            .topSpeed     = 1.0f,
            .catchRadius  = 0.3f,
            .weight       = 190,
            .breakTackle  = 55,
            .tackle       = 70,
        }
    );
    defense[3] = new Player(
        {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(BOTTOM - 20)},
        false, Player::Position::CORNERBACK,
        {
            .width        = 4,
            .height       = 8,
            .acceleration = 0.14f,
            .topSpeed     = 1.05f,
            .catchRadius  = 0.3f,
            .weight       = 180,
            .breakTackle  = 55,
            .tackle       = 70,
        }
    );

}

void Roster::endPlay(int lineOfScrimmage, const OffensivePlayContext& octx, const DefensivePlayContext& dctx)
{
    // TODO: where players line up needs to be defined by play context
    offense[0]->setStatus(Player::Status::BALL_CARRIER);
    offense[0]->pos = {(float)(lineOfScrimmage - convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)};
    defense[0]->pos = {(float)(lineOfScrimmage + convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)};
    defense[1]->pos = {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(TOP + 20)};
    defense[2]->pos = {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(TOP + 40)};
    defense[3]->pos = {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(BOTTOM - 20)};

    for (int i = 1; i < PLAYER_COUNT; i++) {
        if (offense[i] != nullptr) {
            if (i > 0) {
                offense[i]->pos.x = (float)(lineOfScrimmage - 4);
                offense[i]->clearStatus(Player::Status::BALL_CARRIER);
            }
            offense[i]->velocity = {0, 0};
        }
        if (defense[i] != nullptr) {
            defense[i]->clearStatus(Player::Status::BALL_CARRIER);
            defense[i]->velocity = {0, 0};
        }
    }
}