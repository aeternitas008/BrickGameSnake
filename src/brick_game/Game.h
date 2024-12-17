#ifndef GAME_H
#define GAME_H

#include "defines.h"

class Game {
protected:
    static inline GameInfo_t* game_info_;

    virtual void StatsInit() = 0;

    int Offset(struct timespec last_time, struct timespec current_time) {
        return (current_time.tv_sec - last_time.tv_sec) * 1000 +
               (current_time.tv_nsec - last_time.tv_nsec) / 1000000;
    }


public:
    Game() {
        if (!game_info_) {
            game_info_ = new GameInfo_t;
        }
    }

    virtual void userInput(UserAction_t signal, bool hold) = 0;

    virtual GameInfo_t updateCurrentState() {
        return *game_info_;
    }

    void ReadHighScore(const char* file_name) {
        int N = 256;
        char buffer[N];
        FILE *fp = fopen(file_name, "r");
        if (fp != NULL) {
            fgets(buffer, N, fp);
            game_info_->high_score = atoi(buffer);
            fclose(fp);
        } else {
            FILE *file = fopen(file_name, "w");
            fprintf(file, "0");
            fclose(file);
            game_info_->high_score = 0;
        }
    }

    void StatsSave(const char* file_name) {
        if (game_info_->score > game_info_->high_score) {
            game_info_->high_score = game_info_->score;
            FILE *file = fopen(file_name, "w");
            
            fprintf(file, "%d", game_info_->high_score);
            fclose(file);
        }
    }
};

#endif // GAME_H