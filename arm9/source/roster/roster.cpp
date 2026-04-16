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
        {5, 8, 0.7f, 0.15f}
    );
    offense[0]->setStatus(Player::Status::BALL_CARRIER);

    // WR

    offense[1] = new Player(
        {(float)lineOfScrimmage - 4, (float)(TOP + 20)},
        true, Player::Position::WIDE_RECEIVER,
        {4, 9, 1.0f, 0.5f}
    );
    offense[2] = new Player(
        {(float)lineOfScrimmage - 4, (float)(TOP + 40)},
        true, Player::Position::WIDE_RECEIVER,
        {4, 9, 1.2f, 0.5f}
    );
    offense[3] = new Player(
        {(float)lineOfScrimmage - 4, (float)(BOTTOM - 20)},
        true, Player::Position::WIDE_RECEIVER,
        {4, 9, 1.1f, 0.5f}
    );
    // DEFENSE

    defense[0] = new Player(
        {(float)(lineOfScrimmage + convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)},
        false, Player::Position::LINEBACKER,
        {6, 7, 0.8f, 0.3f}
    );

}

void Roster::endPlay(int lineOfScrimmage, const OffensivePlayContext& octx, const DefensivePlayContext& dctx)
{
    // TODO: where players line up needs to be defined by play context
    offense[0]->setStatus(Player::Status::BALL_CARRIER);
    offense[0]->pos = {(float)(lineOfScrimmage - convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)};
    defense[0]->pos = {(float)(lineOfScrimmage + convertToPixelYards(5)), (float)(DRAW_HEIGHT / 2 + TOP)};

    for (int i = 1; i < PLAYER_COUNT; i++) {
        if (offense[i] != nullptr) {
            offense[i]->pos.x = (float)(lineOfScrimmage - 4);
            offense[i]->clearStatus(Player::Status::BALL_CARRIER);
        }
    }
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if (defense[i] != nullptr) {
            defense[i]->clearStatus(Player::Status::BALL_CARRIER);
        }
    }
}