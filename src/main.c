#include "game.h"


int main(void)
{
    // NOTE: Disable stdout buffering. In a macOS GUI app, stdout is often fully
    // buffered and only flushes on exit, so printfs etc don’t appear until the
    // program quits.
    // TODO: Assign to some global debug variable
    setvbuf(stdout, NULL, _IONBF, 0);  // make stdout unbuffered

    bool exit_status = EXIT_FAILURE;

    struct Game *game = NULL;

    if (game_new(&game))
    {
        game_run(game);
        exit_status = EXIT_SUCCESS;
    }

    game_free(&game);
    return exit_status;
}