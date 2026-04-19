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
            .block        = 40,
            .blockShed    = 30,
            .throwStrength= 4.0f,
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
            .block        = 55,
            .blockShed    = 25,
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
            .block        = 65,
            .blockShed    = 30,
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
            .block        = 45,
            .blockShed    = 25,
        }
    );

    // OL — left tackle, center, right tackle lined up on the line of scrimmage
    offense[4] = new Player(
        {(float)(lineOfScrimmage - 7), (float)(DRAW_HEIGHT / 2 + TOP - 15)},
        true, Player::Position::OFFENSIVE_LINE,
        {
            .width        = 7,
            .height       = 8,
            .acceleration = 0.06f,
            .topSpeed     = 0.5f,
            .catchRadius  = 0.0f,
            .weight       = 310,
            .breakTackle  = 40,
            .tackle       = 60,
            .block        = 92,
            .blockShed    = 20,
        }
    );
    offense[5] = new Player(
        {(float)(lineOfScrimmage - 7), (float)(DRAW_HEIGHT / 2 + TOP)},
        true, Player::Position::OFFENSIVE_LINE,
        {
            .width        = 7,
            .height       = 8,
            .acceleration = 0.06f,
            .topSpeed     = 0.5f,
            .catchRadius  = 0.0f,
            .weight       = 320,
            .breakTackle  = 40,
            .tackle       = 60,
            .block        = 95,
            .blockShed    = 20,
        }
    );
    offense[6] = new Player(
        {(float)(lineOfScrimmage - 7), (float)(DRAW_HEIGHT / 2 + TOP + 15)},
        true, Player::Position::OFFENSIVE_LINE,
        {
            .width        = 7,
            .height       = 8,
            .acceleration = 0.06f,
            .topSpeed     = 0.5f,
            .catchRadius  = 0.0f,
            .weight       = 305,
            .breakTackle  = 40,
            .tackle       = 60,
            .block        = 90,
            .blockShed    = 20,
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
            .block        = 60,
            .blockShed    = 70,
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
            .block        = 40,
            .blockShed    = 55,
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
            .block        = 42,
            .blockShed    = 58,
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
            .block        = 38,
            .blockShed    = 52,
        }
    );

    // DL — left edge, left DT, right DT, right edge on the line of scrimmage
    defense[4] = new Player(
        {(float)lineOfScrimmage, (float)(DRAW_HEIGHT / 2 + TOP - 15)},
        false, Player::Position::DEFENSIVE_LINE,
        {
            .width        = 6,
            .height       = 8,
            .acceleration = 0.09f,
            .topSpeed     = 0.75f,
            .catchRadius  = 0.2f,
            .weight       = 255,
            .breakTackle  = 55,
            .tackle       = 75,
            .block        = 50,
            .blockShed    = 80,
        }
    );
    defense[5] = new Player(
        {(float)lineOfScrimmage, (float)(DRAW_HEIGHT / 2 + TOP + 15)},
        false, Player::Position::DEFENSIVE_LINE,
        {
            .width        = 6,
            .height       = 8,
            .acceleration = 0.09f,
            .topSpeed     = 0.75f,
            .catchRadius  = 0.2f,
            .weight       = 260,
            .breakTackle  = 55,
            .tackle       = 75,
            .block        = 50,
            .blockShed    = 82,
        }
    );
    // defense[6] = new Player(  // Left DT — commented out until guards are added
    //     {(float)lineOfScrimmage, (float)(DRAW_HEIGHT / 2 + TOP - 8)},
    //     false, Player::Position::DEFENSIVE_LINE,
    //     {
    //         .width        = 7,
    //         .height       = 8,
    //         .acceleration = 0.07f,
    //         .topSpeed     = 0.6f,
    //         .catchRadius  = 0.2f,
    //         .weight       = 290,
    //         .breakTackle  = 45,
    //         .tackle       = 80,
    //         .block        = 55,
    //         .blockShed    = 75,
    //     }
    // );
    // defense[7] = new Player(  // Right DT — commented out until guards are added
    //     {(float)lineOfScrimmage, (float)(DRAW_HEIGHT / 2 + TOP + 8)},
    //     false, Player::Position::DEFENSIVE_LINE,
    //     {
    //         .width        = 7,
    //         .height       = 8,
    //         .acceleration = 0.07f,
    //         .topSpeed     = 0.6f,
    //         .catchRadius  = 0.2f,
    //         .weight       = 285,
    //         .breakTackle  = 45,
    //         .tackle       = 80,
    //         .block        = 55,
    //         .blockShed    = 76,
    //     }
    // );

}

void Roster::endPlay(int lineOfScrimmage, const OffensivePlayContext& octx, const DefensivePlayContext& dctx)
{
    // TODO: where players line up needs to be defined by play context
    offense[0]->setStatus(Player::Status::BALL_CARRIER);
    offense[0]->pos = {(float)(lineOfScrimmage - convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)};
    defense[0]->pos = {(float)(lineOfScrimmage + convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)};
    if (offense[1] != nullptr) offense[1]->pos = {(float)(lineOfScrimmage - 4), (float)(TOP + 20)};
    if (offense[2] != nullptr) offense[2]->pos = {(float)(lineOfScrimmage - 4), (float)(TOP + 40)};
    if (offense[3] != nullptr) offense[3]->pos = {(float)(lineOfScrimmage - 4), (float)(BOTTOM - 20)};
    if (offense[4] != nullptr) offense[4]->pos = {(float)(lineOfScrimmage - 7), (float)(DRAW_HEIGHT / 2 + TOP - 15)};
    if (offense[5] != nullptr) offense[5]->pos = {(float)(lineOfScrimmage - 7), (float)(DRAW_HEIGHT / 2 + TOP)};
    if (offense[6] != nullptr) offense[6]->pos = {(float)(lineOfScrimmage - 7), (float)(DRAW_HEIGHT / 2 + TOP + 15)};
    defense[1]->pos = {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(TOP + 20)};
    defense[2]->pos = {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(TOP + 40)};
    defense[3]->pos = {(float)(lineOfScrimmage + convertToPixelYards(3)), (float)(BOTTOM - 20)};
    if (defense[4] != nullptr) defense[4]->pos = {(float)lineOfScrimmage, (float)(DRAW_HEIGHT / 2 + TOP - 15)};
    if (defense[5] != nullptr) defense[5]->pos = {(float)lineOfScrimmage, (float)(DRAW_HEIGHT / 2 + TOP + 15)};
    if (defense[6] != nullptr) defense[6]->pos = {(float)lineOfScrimmage, (float)(DRAW_HEIGHT / 2 + TOP - 8)};
    if (defense[7] != nullptr) defense[7]->pos = {(float)lineOfScrimmage, (float)(DRAW_HEIGHT / 2 + TOP + 8)};

    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (offense[i] != nullptr) {
            if (i > 0) {
                // offense[i]->pos.x = (float)(lineOfScrimmage - 4);
                offense[i]->clearStatus(Player::Status::BALL_CARRIER);
                offense[i]->clearStatus(Player::Status::STUMBLED);
                offense[i]->clearStatus(Player::Status::PANCAKED);
                offense[i]->clearStatus(Player::Status::BLOCKING);
            }
            offense[i]->velocity = {0, 0};
        }
        if (defense[i] != nullptr) {
            defense[i]->clearStatus(Player::Status::BALL_CARRIER);
            defense[i]->clearStatus(Player::Status::STUMBLED);
            defense[i]->clearStatus(Player::Status::PANCAKED);
            defense[i]->velocity = {0, 0};
        }
    }

}