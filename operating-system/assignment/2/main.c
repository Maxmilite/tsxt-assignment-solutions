#include "main.h"

int pipes[6][2];
// pipes[0]: main thread => thread 1
// pipes[1]: main thread <= thread 1
// pipes[2]: thread 1 => thread 2
// pipes[3]: thread 1 <= thread 2
// pipes[4]: thread 1 => thread 3
// pipes[5]: thread 1 <= thread 3
pthread_t threads[3];

int fxy();
int fx();
int fy();

int main(int argc, char* argv[]) {
    // x, y are passed into the program through args
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <integer x> <integer y>.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int x = atoi(argv[1]), y = atoi(argv[2]);
    assert(x > 0 && y > 0);

    for (int i = 0; i < 6; ++i) {
        if (pipe(pipes[i]) < 0) {
            perror("Error while creating pipes");
            exit(EXIT_FAILURE);
        }
    }

    int status[3];
    int result = pthread_create(threads + 0, NULL, (void *) fxy, (void *) status + 0);
    if (result) { perror("Error while creating thread 1"); exit(EXIT_FAILURE); }
    result = pthread_create(threads + 1, NULL, (void *) fx, (void *) status + 1);
    if (result) { perror("Error while creating thread 2"); exit(EXIT_FAILURE); }
    result = pthread_create(threads + 2, NULL, (void *) fy, (void *) status + 2);
    if (result) { perror("Error while creating thread 3"); exit(EXIT_FAILURE); }

    write(pipes[0][1], &x, sizeof(int));
    write(pipes[0][1], &y, sizeof(int));
    pthread_join(threads[0], NULL);
    int ans = 0;
    read(pipes[1][0], &ans, sizeof(int));
    printf("%d\n", ans);

    exit(EXIT_SUCCESS);
}

int fxy() {
    int x = 0, y = 0, ans = 0;
    read(pipes[0][0], &x, sizeof(int));
    read(pipes[0][0], &y, sizeof(int));

    do {
        int init_ans = 1;
        write(pipes[2][1], &x, sizeof(int));
        write(pipes[2][1], &init_ans, sizeof(int));
    } while (0);
    do {
        int should_send_to_main = 1;
        write(pipes[4][1], &y, sizeof(int));
        write(pipes[4][1], &should_send_to_main, sizeof(int));
    } while (0);


    int result = 0;
    pthread_join(threads[1], NULL);
    read(pipes[3][0], &result, sizeof(int));
    ans += result;
    pthread_join(threads[2], NULL);
    read(pipes[5][0], &result, sizeof(int));
    ans += result;

    write(pipes[1][1], &ans, sizeof(int));
    return EXIT_SUCCESS;
}

int fx() {
    int x, ans;
    read(pipes[2][0], &x, sizeof(int));
    read(pipes[2][0], &ans, sizeof(int));

    if (x <= 1) {
        write(pipes[3][1], &ans, sizeof(int));
        return EXIT_SUCCESS;
    }
    
    ans *= x; --x;
    write(pipes[2][1], &x, sizeof(int));
    write(pipes[2][1], &ans, sizeof(int));
    fx();
    return EXIT_SUCCESS;
}

int fy() {
    int y, ans = 0, should_send_to_main;
    int offset = 0;
    read(pipes[4][0], &y, sizeof(int));
    read(pipes[4][0], &should_send_to_main, sizeof(int));

    if (y <= 2) {
        if (should_send_to_main) {
            write(pipes[5][1], &y, sizeof(int));
        } else {
            write(pipes[4][1], &y, sizeof(int));
        }
        return EXIT_SUCCESS;
    }
    
    y--;
    write(pipes[4][1], &y, sizeof(int));
    write(pipes[4][1], &offset, sizeof(int));
    fy();
    read(pipes[4][0], &offset, sizeof(int));
    ans += offset; offset = 0;

    y--;
    write(pipes[4][1], &y, sizeof(int));
    write(pipes[4][1], &offset, sizeof(int));
    fy();
    read(pipes[4][0], &offset, sizeof(int));
    ans += offset; offset = 0;

    if (should_send_to_main) {
        write(pipes[5][1], &ans, sizeof(int));
    } else {
        write(pipes[4][1], &ans, sizeof(int));
    }
    return EXIT_SUCCESS;
}
