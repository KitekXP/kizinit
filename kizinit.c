#include <sys/wait.h>
#include <sys/reboot.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void reap(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

int main() {
    struct sigaction sa = {0};
    sa.sa_handler = reap;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    // example service
    pid_t pid = fork();
    if (pid == 0) {
        execl("/kizinit/init", "init", NULL);
        _exit(1);
    }

    int status;
    waitpid(pid, &status, 0);

    sync();
    reboot(RB_POWER_OFF);
}
