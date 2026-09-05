#include "flappy.h"

#include "../font.h"
#include "../helpers/rng.h"
#include "../helpers/wait.h"
#include "../keyboard.h"
#include "../screen.h"
#include "cstdbool.h"
#include "cstring.h"

static int prev_space = 0;

static int rect_overlap(int l1, int r1, int l2, int r2) {
    return (r1 > l2 && l1 < r2);
}

void state_init(State *state) {
    state->player.isAlive = true;
    state->player.x = SCREEN_WIDTH / 2;
    state->player.y = SCREEN_HEIGHT / 2;

    state->points = 0;
    state->max_points = 0;
    state->started = false;

    for (int i = 0; i < PIPE_N; ++i) {
        state->pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / PIPE_N);
        state->pipes[i].y = rand_range(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);
    }

    prev_space = 0;
}

static void death_screen_render(State *s) {
    clear_buffer();

    char score_str[16];
    utoa(s->points, score_str);

    char full_text[64];
    strcpy(full_text, "Points: ");
    strcat(full_text, score_str);

    int text_length = strlen(full_text);
    int text_width = text_length * FONT_SIZE;

    int x_pos = (SCREEN_WIDTH - text_width) / 2;
    int y_pos = (SCREEN_HEIGHT - FONT_SIZE) / 2;

    print_string(full_text, point_new(x_pos, y_pos), WHITE);

    if (s->points >= s->max_points && s->points > 0) {
        const char *hs_text = "New Highscore!";
        int hs_width = strlen(hs_text) * FONT_SIZE;
        print_string(
            hs_text,
            point_new((SCREEN_WIDTH - hs_width) / 2, y_pos + FONT_SIZE * 2),
            YELLOW);
    } else {
        const char *restart_text = "Press Space to Restart";
        int r_width = strlen(restart_text) * FONT_SIZE;
        print_string(
            restart_text,
            point_new((SCREEN_WIDTH - r_width) / 2, y_pos + FONT_SIZE * 2),
            WHITE);
    }

    swap_buffers();
}

static void start_screen_render(State *s) {
    clear_buffer();

    const char *title = "FLAPPY BIRD";
    const char *prompt = "Press Space to Start";

    int title_width = strlen(title) * FONT_SIZE;
    int prompt_width = strlen(prompt) * FONT_SIZE;

    print_string(title,
                 point_new((SCREEN_WIDTH - title_width) / 2,
                           SCREEN_HEIGHT / 2 - FONT_SIZE),
                 YELLOW);
    print_string(prompt,
                 point_new((SCREEN_WIDTH - prompt_width) / 2,
                           SCREEN_HEIGHT / 2 + FONT_SIZE),
                 WHITE);

    swap_buffers();
}

void flappy_render(State *s) {
    if (!s->started) {
        start_screen_render(s);
        return;
    }

    if (!s->player.isAlive) {
        death_screen_render(s);
        return;
    }

    clear_buffer();

    fill_rect(point_new(s->player.x - PLAYER_W / 2, s->player.y - PLAYER_H / 2),
              point_new(s->player.x + PLAYER_W / 2, s->player.y + PLAYER_H / 2),
              YELLOW);

    for (int i = 0; i < PIPE_N; ++i) {
        int x = s->pipes[i].x;
        int y = s->pipes[i].y;

        fill_rect(point_new(x - PIPE_W / 2, 0),
                  point_new(x + PIPE_W / 2, y - PIPE_GAP / 2), GREEN);

        fill_rect(point_new(x - PIPE_W / 2, y + PIPE_GAP / 2),
                  point_new(x + PIPE_W / 2, SCREEN_HEIGHT), GREEN);
    }

    char buf[32];
    print_string(utoa(s->points, buf), point_new(0, 0), WHITE);
    print_string("MAX:", point_new(0, FONT_SIZE), WHITE);
    print_string(utoa(s->max_points, buf), point_new(5 * FONT_SIZE, FONT_SIZE),
                 WHITE);

    swap_buffers();
}

void flappy_update(State *s) {
    if (!s->started) {
        if (key_down(KEY_SPACE)) {
            s->started = true;
            prev_space = 0; // jump instantly on frame 1
        }
        return;
    }

    const int gravity = 1;
    const int jump_a = -6;

    int pipe_v = -5;
    static int player_v = 0;

    if (!s->player.isAlive) {
        player_v = 0;
        pipe_v = 0;

        if (s->max_points < s->points) {
            s->max_points = s->points;
        }

        death_screen_render(s);
        wait_s(1);

        while (!key_down(KEY_SPACE)) {
            death_screen_render(s);
        }

        pipe_v = -5;
        for (int i = 0; i < PIPE_N; ++i) {
            s->pipes[i].x = SCREEN_WIDTH + i * (SCREEN_WIDTH / PIPE_N);
            s->pipes[i].y = rand_range(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);
        }

        s->player.y = SCREEN_HEIGHT / 2;
        s->player.isAlive = true;
        s->points = 0;
        prev_space = 0; // jump instantly on restart frame

        return;
    }

    int space = key_down(KEY_SPACE);

    if (space && !prev_space) {
        player_v = jump_a;
    }

    prev_space = space;

    s->player.y += player_v;
    player_v += gravity;

    if (player_v > 8) {
        player_v = 8;
    }

    if (s->player.y + PLAYER_H / 2 >= SCREEN_HEIGHT) {
        s->player.y = SCREEN_HEIGHT - PLAYER_H / 2;
        s->player.isAlive = false;
    }

    if (s->player.y - PLAYER_H / 2 <= 0) {
        s->player.y = PLAYER_H / 2;
        s->player.isAlive = false;
    }

    for (int i = 0; i < PIPE_N; ++i) {
        int prev_pipe_right = s->pipes[i].x + PIPE_W / 2;
        s->pipes[i].x += pipe_v;
        int curr_pipe_right = s->pipes[i].x + PIPE_W / 2;

        if (prev_pipe_right >= s->player.x && curr_pipe_right < s->player.x) {
            s->points++;
        }

        int x = s->pipes[i].x;
        int y = s->pipes[i].y;

        int pipeLeft = x - PIPE_W / 2;
        int pipeRight = x + PIPE_W / 2;

        int playerLeft = s->player.x - PLAYER_W / 2;
        int playerRight = s->player.x + PLAYER_W / 2;
        int playerTop = s->player.y - PLAYER_H / 2;
        int playerBottom = s->player.y + PLAYER_H / 2;

        int gapTop = y - PIPE_GAP / 2;
        int gapBottom = y + PIPE_GAP / 2;

        if (rect_overlap(playerLeft, playerRight, pipeLeft, pipeRight)) {
            if (playerTop <= gapTop || playerBottom >= gapBottom) {
                s->player.isAlive = false;
            }
        }

        if (s->pipes[i].x < -PIPE_W) {
            s->pipes[i].x = SCREEN_WIDTH;
            s->pipes[i].y = rand_range(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);
        }
    }
}
